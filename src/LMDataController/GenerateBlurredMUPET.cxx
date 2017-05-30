#include <iostream>
#include <string>
#include <cstring>  // strcpy
#include <sstream>
#include <fstream>
#include "../include/LMDataController.h"
#include "../include/JPETHitEvent.h"
#include "../include/CListEventMUPET_simple.h"
#include "../include/CrystalMapJPET.h"
#include "../include/ConvertBlurredToSAFIR.inl"

// TODO - multiple files!!!

//void convert_to_safir(const std::string& PM_type,
//                       const std::string& input_file,
//                       std::unordered_map<std::string, float>& parameters_map){
//    // Variable for various stringstream
//    std::ostringstream text_to_concatenate;

//    // Create stream for data import
//    text_to_concatenate << PM_type << "_" << input_file;
//    std::cout << "--- "<< PM_type << " photomultiplier ---\n";
//    std::ifstream myfile(text_to_concatenate.str());

//    // SAFIR file name
//    text_to_concatenate.str(""); // clear the stream
//    text_to_concatenate.clear();
//    text_to_concatenate << PM_type << "_data.clm.safir";
//    std::cout << "Writing to "<< text_to_concatenate.str() << std::endl;
//    std::ofstream outfile;
//    outfile.open(text_to_concatenate.str());

//    // Add header (Important!)
//    const char* file_sig = "MUPET CListModeData\0";
//    outfile.write( file_sig, 32 ); // 32 bytes are reserved for file signature

//    std::string data; // one row from file
//    float x1,y1,z1,t1,
//          x2,y2,z2,t2;

//    // Begin conversion
//    int i = 0;
//    std::string event_no;
//    std::stringstream s_event_no;
//    std::cout << "Number of events:" << std::endl;  // for counter

//    while(true){
//        std::getline(myfile, data);
//        i += 1;
//        if(!myfile) break; //eof check

//        std::istringstream buffer(data);
//        buffer >> x1 >> y1 >> z1 >> t1
//               >> x2 >> y2 >> z2 >> t2;  // a tricky way to extract data sequentially
//        // Counter for display
//        s_event_no.str("");
//        s_event_no << i;
//        event_no = s_event_no.str();

//        JPETEvent event({x1*10,y1*10,(z1*10) + parameters_map["LENGTH"]/2},
//                {x2*10,y2*10,(z2*10) + parameters_map["LENGTH"]/2});
//        // This is alternative way to create Event fron two Hits
//        // JPETHit hit1(x1*10,y1*10,(z1*10) + params_hash["LENGTH"]/2),
//        //         hit2(x2*10,y2*10,(z2*10) + params_hash["LENGTH"]/2);
//        // JPETEvent event(hit1, hit2);
//        event.calibrate(parameters_map);

//        CListRecordMUPET_simple tmprecord = event.to_MUPET();
//        // Save one event to .safir file
//        outfile.write(reinterpret_cast<char*>(&tmprecord), sizeof(tmprecord));

//        // Counter for events
//        std::cout << event_no << std::flush << "\r";
//    }
//    std::cout << "Total number of events = " << i
//              << " (last line could have been empty)" << std::endl;
//    std::cout << "Conversion finished.\n" << std::endl;

//    outfile.close();
//    myfile.close();
//}


int main( int argc, char** argv ) {
    // Input file
    char* input_file = new char[2];

    // Default parameters file
    std::string* params_string = new std::string;
    *params_string = "JPETGeometry.par";
    char* params_file = new char[params_string->length()+1];
    std::strcpy(params_file, params_string->c_str());
    delete params_string;

    for (int j = 1; j < argc; j++){
        if (argv[j] == std::string("-p")) {
            params_file = argv[j+1];
        }
        if (argv[j] == std::string("-i")) {
            input_file = argv[j+1];
        }
    }

    if (std::string(input_file) == "") {
        std::cout << "Usage:\n" << argv[0]
          << " -i input_file"
          << " [-p parameters_file.par]" << std::endl;
        return 0;
    }

    std::cout << "\n--- PARAMETERS ---" << std::endl;
    std::cout << "Input file: " << input_file << std::endl;
    std::cout << "Parameters file: " << params_file << std::endl;
    std::cout << "------------------" << std::endl;

    // Generate controller
    LMDataController my_controller(input_file);
    // Create parameters file
    my_controller.create_parameters_file(params_file);

    // TODO - validate if successfully created!

    // Blur by all three PM's
    my_controller.export_blured(LMDataController::PMT);
    my_controller.export_blured(LMDataController::WLS);
    my_controller.export_blured(LMDataController::SI);

    // Import parameters from created file for generating Crystal Map
    JPETParameters JPET_params;
    JPET_params.import_parameters(params_file);
    std::unordered_map<std::string, float> params_hash = JPET_params.params_map();

    CrystalMapJPET my_JPET(params_hash["RADIUS"],
                           params_hash["LENGTH"],
                           params_hash["DETECTORS"],
                           params_hash["RINGS"],
                           true);
    // Print out parameters
    std::cout << "\nParameters of J-PET:" << std::endl;
    std::cout << "\tRadius (mm): " << params_hash["RADIUS"] << std::endl;
    std::cout << "\tAxial length (mm): " << params_hash["LENGTH"] << std::endl;
    std::cout << "\tNumber of strips: " << params_hash["DETECTORS"] << std::endl;
    std::cout << "\tNumber of strip cuts : " << params_hash["RINGS"]<< std::endl;
    std::cout << "Writing crystal map to file: " << "crystal_map_jpet.txt\n" << std::endl;
    my_JPET.save_to_file("crystal_map_jpet.txt");

    // Create MUPET files for 3 photomultipliers
    convert_to_safir("WLS", my_controller.get_file_name(), params_file);
    convert_to_safir("SI", my_controller.get_file_name(), params_file);
    convert_to_safir("PMT", my_controller.get_file_name(), params_file);

    // Create file with DOI parameters
    my_controller.create_DOI_file("DOI_parameters.par");

    // Display DOI parameters
    std::cout << "--- Depth of interaction (averages in mm) ---" << std::endl;
    std::cout << "WLS\n\tMean: "<< my_controller.DOI_averages("WLS")[0] << "\n"
              << "\tMedian: "<< my_controller.DOI_averages("WLS")[1] << "\n";
    std::cout << "SI\n\tMean: "<< my_controller.DOI_averages("SI")[0] << "\n"
              << "\tMedian: "<< my_controller.DOI_averages("SI")[1] << "\n";
    std::cout << "PMT\n\tMean: "<< my_controller.DOI_averages("PMT")[0] << "\n"
              << "\tMedian: "<< my_controller.DOI_averages("PMT")[1] << "\n";
}
