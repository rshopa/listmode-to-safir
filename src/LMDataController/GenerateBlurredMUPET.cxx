/*! Executable for generating smeared MUPET binary files from listmode
* For PMT, SI and WLS (tube, Si and WLS photomultipliers)
* Also creates DOI output with mean and median estimated values
* author: Roman Shopa
* Roman.Shopa@ncbj.gov.pl */
#include <iostream>
#include <string>
#include <cstring>  // strcpy
#include <sstream>
#include <fstream>
#include <unordered_map>
#include "../include/JPETParameters.h"
#include "../include/JPETHitEvent.h"
#include "../include/LMDataController.h"
#include "../include/CrystalMapJPET.h"
//#include "../include/CListEventMUPET_simple.h"
#include "../include/ConvertToSAFIR.cxx"

int main( int argc, char** argv ) {

    // Input file
    char* input_file = new char[2]; // [2]?

    // Default parameters file
    std::string* params_string = new std::string;
    *params_string = "JPETGeometry.par";
    char* params_file = new char[params_string->length()+1];

    // Assign parameters (input file [and geometry])
    for (int j = 1; j < argc; j++){
        if (argv[j] == std::string("-i")) {
            input_file = argv[j+1];
        }
        if (argv[j] == std::string("-p")) {
            params_file = argv[j+1];
        }
    }

    // Usage help
    if (std::string(input_file) == "") {
        std::cout << "Usage:\n" << argv[0]
                  << " -i input_file"
                  << " [-p parameters_file.par]" << std::endl;
        return 0;
    }

    // Create controller
    LMDataController my_controller(input_file);

    // If no geometry (params_file) given, try to parse from filename
    // or look for the default file 'JPETGeometry.par'
    if (std::string(params_file) == "") {
        std::strcpy(params_file, params_string->c_str()); // sets to default
        // if parseable, create geometry, otherwise import from default
        if(my_controller.is_geometry_parsed_from_filename())
            my_controller.create_parameters_file(params_file);
        else my_controller.import_geometry(params_file);
    } else
        my_controller.import_geometry(params_file);
    delete params_string; // delete temporary string "JPETGeometry.par"

    std::cout << "\n--- FILES TO OPERATE ---" << std::endl;
    std::cout << "Input file: " << input_file << std::endl;
    std::cout << "Parameters file: " << params_file << std::endl;
    std::cout << "------------------" << std::endl;

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

    return 0;
}

