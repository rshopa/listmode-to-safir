/*! Executable for generating crystal map for SAFIR module 
* author: Roman Shopa
* Roman.Shopa@ncbj.gov.pl */
#include "../include/CrystalMapJPET.h"
#include "../include/JPETParameters.h"

//! Geometry of J-PET (if fixed)
// #define RADIUS 437.3
// #define AXIAL_LENGTH 500.0
// #define NUMBER_OF_STRIPS 384
// #define STRIP_CUTS 64

int main( int argc, char** argv ) {
    char* params_file;
    char* output_file;
    if (argc != 5) {
        std::cout << "Usage: " << argv[0] 
                  << " -p parameters_file.par"
                  << " -o crystal_map_file.txt" << std::endl;
        return 0;
    }
    for (int j = 1; j < argc; j++) {
        if (argv[j] == std::string("-p")) {
            params_file = argv[j+1];
        } else if (argv[j] == std::string("-o")) {
            output_file = argv[j+1];
        }
    }
    // Import parameters from .par file
    JPETParameters JPET_params;
    JPET_params.import_parameters(params_file);
    std::unordered_map<std::string, float> params_hash = JPET_params.params_map();
    // Create CrystalMapJPET object
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
    std::cout << "Writing crystal map to file: " << output_file << std::endl;
    my_JPET.save_to_file(output_file);

    return 0;
}
