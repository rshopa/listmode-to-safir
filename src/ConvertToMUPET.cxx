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
#include "CListEventMUPET_simple.h"
#include "JPETHitEvent.h"
#include "JPETParameters.h"

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

	std::cout << "Files to operate:" << std::endl;
    std::cout << "\tGeometry of PET: " << params_file << std::endl;
    std::cout << "\tInput data: " << input_file << std::endl;
    std::cout << "\tSAFIR output: " << output_file << std::endl;

    // Add header (Important!)
    const char* file_sig = "MUPET CListModeData\0";
    outfile.write( file_sig, 32 ); // 32 bytes are reserved for file signature

    std::string data;
    float x1,y1,z1,t1,
          x2,y2,z2,t2,
          the_rest;
    int i = 0;

    while(true){
        std::getline(myfile, data);
        i += 1;
        if(!myfile) break; //eof check

        std::istringstream buffer(data);
        buffer >> x1 >> y1 >> z1 >> t1
               >> x2 >> y2 >> z2 >> t2 >> the_rest;  // a tricky way to extract data sequentially

// This is alternative way to create Event fron two Hits
//        JPETHit hit1(x1*10,y1*10,(z1*10) + params_hash["LENGTH"]/2),
//                hit2(x2*10,y2*10,(z2*10) + params_hash["LENGTH"]/2);
//        JPETEvent event(hit1, hit2);

	JPETEvent event({x1*10,y1*10,(z1*10) + params_hash["LENGTH"]/2},
		        {x2*10,y2*10,(z2*10) + params_hash["LENGTH"]/2});
        event.calibrate(params_hash);

        CListRecordMUPET_simple tmprecord = event.to_MUPET();

        outfile.write(reinterpret_cast<char*>(&tmprecord), sizeof(tmprecord));
		// OPTIONAL: print out every line
        std::cout << "RingA: " << tmprecord.event_data.ringA << "\t";
        std::cout << "RingB: " << tmprecord.event_data.ringB << "\t";
        std::cout << "DetA: " << tmprecord.event_data.detA << "\t";
        std::cout << "DetB: " << tmprecord.event_data.detB << "\t";
        std::cout << "Raw Hash: " << tmprecord.raw << std::endl;
    }

    outfile.close();
    myfile.close();

    return 0;
}
