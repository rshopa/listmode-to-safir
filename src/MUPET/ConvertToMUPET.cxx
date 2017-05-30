/*! Executable for generating MUPET binary file from listmode
* (comparable to SAFIR - see CListEventMUPET_simple.h)
* Note! Since hit coordinates in PSF* files are in cm units, they are 
* multiplied by 10 manually when creating JPETEvent, 
* as all algorithms in STIR operate in mm
* author: Roman Shopa
* Roman.Shopa@ncbj.gov.pl */
#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/CListEventMUPET_simple.h"
#include "../include/JPETHitEvent.h"
#include "../include/JPETParameters.h"

int main( int argc, char** argv ) {
    char* input_file;
    char* output_file;
    char* params_file;
    if (argc != 7) {
        std::cout << "Usage: " << argv[0] 
  		  << " -i input_file" 
 		  << " -p parameters_file.par"
 		  << " -o output_file.clm.safir" << std::endl;
    	return 0;
    }
    for (int j = 1; j < argc; j++){
	if (argv[j] == std::string("-i")) {
	    input_file = argv[j+1];
	} else if (argv[j] == std::string("-o")) {
	    output_file = argv[j+1];
	} else if (argv[j] == std::string("-p")) {
	    params_file = argv[j+1];
	}
    }
    // First, import parameters from outer file
    JPETParameters JPET_params;
    JPET_params.import_parameters(params_file);
    std::unordered_map<std::string, float> params_hash = JPET_params.params_map();
    // Import data
    std::ifstream myfile(input_file);
    std::ofstream outfile;
    outfile.open(output_file);

    std::cout << "\nFiles to operate:" << std::endl;
    std::cout << "\tGeometry of PET: " << params_file << std::endl;
    std::cout << "\tInput data: " << input_file << std::endl;
    std::cout << "\tSAFIR output: " << output_file << std::endl;
    std::cout << "\nConverting to MUPET. Please wait..." << std::endl;

    // Add header (Important!)
    const char* file_sig = "MUPET CListModeData\0";
    outfile.write( file_sig, 32 ); // 32 bytes are reserved for file signature

    std::string data; // one row from file
    float x1,y1,z1,t1,
          x2,y2,z2,t2,
          the_rest;   // a tricky way to cut out the rest of line

    // Begin conversion
    int i = 0;
    std::string event_no;
    std::stringstream s_event_no;
    std::cout << "Number of events:" << std::endl;  // for counter

    while(true){
        std::getline(myfile, data);
        i += 1;
        if(!myfile) break; //eof check

        std::istringstream buffer(data);
        buffer >> x1 >> y1 >> z1 >> t1
               >> x2 >> y2 >> z2 >> t2 >> the_rest;  // a tricky way to extract data sequentially
        // Counter for display
        s_event_no.str("");
        s_event_no << i;
        event_no = s_event_no.str();

         JPETEvent event({x1*10,y1*10,(z1*10) + params_hash["LENGTH"]/2},
                        {x2*10,y2*10,(z2*10) + params_hash["LENGTH"]/2});
//        This is alternative way to create Event fron two Hits
//        JPETHit hit1(x1*10,y1*10,(z1*10) + params_hash["LENGTH"]/2),
//                hit2(x2*10,y2*10,(z2*10) + params_hash["LENGTH"]/2);
//        JPETEvent event(hit1, hit2);

        event.calibrate(params_hash);
        CListRecordMUPET_simple tmprecord = event.to_MUPET();

        // Save one event to .safir file
        outfile.write(reinterpret_cast<char*>(&tmprecord), sizeof(tmprecord));
        std::cout << event_no << std::flush << "\r";
    }
    std::cout << "Total number of events = " << i << std::endl;
    std::cout << "Conversion finished.\n" << std::endl;

    outfile.close();
    myfile.close();

    return 0;
}
