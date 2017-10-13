/* ConvertToSAFIR.cxx
* (moved to include directory though .cxx extensions)
* separate functions for conversion, with PM type chosen
* Note! Since hit coordinates in GATE files are in cm units, they are
* multiplied by 10 manually when creating JPETEvent,
* as all algorithms in STIR operate in mm
* author: Roman Shopa
* Roman.Shopa@ncbj.gov.pl */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "JPETParameters.h"
#include "JPETHitEvent.h"
#include "CListEventMUPET_simple.h"

//! Exports one event to .safir file converted to <string> singleRow
//! using getline() if calibrated successfully (otherwise passes).
//! params - parameters hash,
//! coerceToEdges - whether adjust Z coordinates out of scanner to edges
//! WARNING! output file should be opened! And it leaves opened after return!
bool export_event_to_safir_file(const std::string& singleRow,
                                std::ofstream& openedSafirFile,
                                std::unordered_map<std::string,float> params,
                                const bool& coerceToEdges = false) {
    float x1,y1,z1,t1,x2,y2,z2,t2;   // pair of 2 hits
    std::istringstream buffer(singleRow);
    buffer >> x1 >> y1 >> z1 >> t1
           >> x2 >> y2 >> z2 >> t2;  // a tricky way to extract data sequentially
    JPETEvent event({x1*10,y1*10,(z1*10) + params["LENGTH"]/2},
            {x2*10,y2*10,(z2*10) + params["LENGTH"]/2});
    event.calibrate(params,coerceToEdges);
    // pass if something's wrong
    if(!event.is_calibrated()) return false;
    CListRecordMUPET_simple tmprecord = event.to_MUPET();
    // Save one event to .safir file (as in the example from Jannis Fischer)
    openedSafirFile.write(reinterpret_cast<char*>(&tmprecord),
                          sizeof(tmprecord));
    return true;
}

//! Converts to PM type chosen (full file)
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

    // Import parameters from geometry file
    JPETParameters JPET_params;
    JPET_params.import_parameters(geometry_file);
    std::unordered_map<std::string, float> params_hash = JPET_params.params_map();

    // Begin conversion
    int i(0), totalEvents(0);
    std::string event_no,
                OneRowListMode; // one row from file
    std::stringstream s_event_no;

    std::cout << "Number of events:" << std::endl;  // for counter
    while(true){
        std::getline(myfile, OneRowListMode);
        i += 1;
        if(!myfile) break; //eof check
        if(export_event_to_safir_file(OneRowListMode,
                                      outfile,
                                      params_hash,
                                      false)) totalEvents += 1;
        // Counter for display
        s_event_no.str("");
        s_event_no << i;
        event_no = s_event_no.str();
        // Counter for events
        std::cout << event_no << std::flush << "\r";
    }

    std::cout << std::endl; // to prevent flush in log file
    std::cout << "Events exported: " << totalEvents << std::endl;
    std::cout << "Total number of lines in file: " << i
              << " (last line could have been empty)" << std::endl;
    std::cout << "Conversion finished.\n" << std::endl;

    outfile.close();
    myfile.close();
}
