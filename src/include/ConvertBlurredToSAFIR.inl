/* ConvertBlurredToSAFIR.inl
* separate function for conversion with PM type chosen
* author: Roman Shopa
* Roman.Shopa@ncbj.gov.pl */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "JPETParameters.h"
#include "JPETHitEvent.h"
#include "CListEventMUPET_simple.h"

void convert_to_safir(const std::string& PM_type,
                       const std::string& input_file,
			const char* geometry_file){
    // Variable for various stringstream
    std::ostringstream text_to_concatenate;

    // Create stream for data import
    text_to_concatenate << PM_type << "_" << input_file;
    std::cout << "--- "<< PM_type << " photomultiplier ---\n";
    std::ifstream myfile(text_to_concatenate.str());

    // SAFIR file name
    text_to_concatenate.str(""); // clear the stream
    text_to_concatenate.clear();
    text_to_concatenate << PM_type << "_data.clm.safir";
    std::cout << "Writing to "<< text_to_concatenate.str() << std::endl;
    std::ofstream outfile;
    outfile.open(text_to_concatenate.str());

    // Add header (Important!)
    const char* file_sig = "MUPET CListModeData\0";
    outfile.write( file_sig, 32 ); // 32 bytes are reserved for file signature

    std::string data; // one row from file
    float x1,y1,z1,t1,
          x2,y2,z2,t2;

    // Import parameters from geometry file
    JPETParameters JPET_params;
    JPET_params.import_parameters(geometry_file);
    std::unordered_map<std::string, float> params_hash = JPET_params.params_map();

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
               >> x2 >> y2 >> z2 >> t2;  // a tricky way to extract data sequentially
        // Counter for display
        s_event_no.str("");
        s_event_no << i;
        event_no = s_event_no.str();

        JPETEvent event({x1*10,y1*10,(z1*10) + params_hash["LENGTH"]/2},
                {x2*10,y2*10,(z2*10) + params_hash["LENGTH"]/2});
        // This is alternative way to create Event fron two Hits
        // JPETHit hit1(x1*10,y1*10,(z1*10) + params_hash["LENGTH"]/2),
        //         hit2(x2*10,y2*10,(z2*10) + params_hash["LENGTH"]/2);
        // JPETEvent event(hit1, hit2);
        event.calibrate(params_hash);

        CListRecordMUPET_simple tmprecord = event.to_MUPET();
        // Save one event to .safir file
        outfile.write(reinterpret_cast<char*>(&tmprecord), sizeof(tmprecord));

        // Counter for events
        std::cout << event_no << std::flush << "\r";
    }
    std::cout << "Total number of events = " << i
              << " (last line could have been empty)" << std::endl;
    std::cout << "Conversion finished.\n" << std::endl;

    outfile.close();
    myfile.close();
}
