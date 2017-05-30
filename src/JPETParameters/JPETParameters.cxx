/* Class for importing geometry parameters from outer file
* Originally for json, but there are no standard library in g++ for json.
* Hence the file comprises rows in the Interfile-like form: "parameter_name := value"
* author: Roman Shopa
* Roman.Shopa@ncbj.gov.pl */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "../include/JPETParameters.h"

// Constructors and destructor
JPETParameters::JPETParameters()
    : m_params({0.,0.,0.,0.,0.,0.,0.}), m_empty(true) {}
JPETParameters::JPETParameters(const char* file_name)
    : m_params({0.,0.,0.,0.,0.,0.,0.}), m_empty(true) {
    m_import(file_name);
    m_empty = false; // flag to false (filled in)
}
JPETParameters::~JPETParameters(){}

// Duplicates one of the constructors
void JPETParameters::import_parameters(const char* file_name) {
    m_import(file_name);
    m_empty = false; // flag to false (filled in)
}

// Returns output in the form of unordered map
std::unordered_map<std::string, float> JPETParameters::params_map(){
    if(m_empty) std::cout << "WARNING: an empty map!" << std::endl;
    std::unordered_map<std::string, float> params = {
        {"RADIUS", m_params[1]},
        {"LENGTH", m_params[2]},
        {"RINGS", m_params[3]},
        {"DETECTORS", m_params[4]},
        {"STRIP_WIDTH", m_params[5]},
        {"STRIP_HEIGHT", m_params[6]},
    };
    return params;
}

/* Imports data from parameters file (the example is below)
* 0 Scanner parameters :=
* 1    Radius (mm)                    :=      XXX.X
* 2    Length (mm)                    :=      XXX.X
* 3    Rings                          :=      XXX
* 4    Detectors                      :=      XXX
* 5    Strip width, tangential (mm)   :=      XXX.X
* 6    Strip width, radial (mm)       :=      XXX.X */
void JPETParameters::m_import(const char* file_name){
    // first empty STL vector of values
    m_params.clear();
    std::ifstream param_file(file_name);
    std::string row, value;
    while(true){
        std::getline(param_file, row); // read one line
        if(!param_file) break;
        std::istringstream buffer(row);
        // reverse string and reverse first element of it:
        std::istringstream( { row.rbegin(), row.rend() } ) >> value ;
        std::reverse(value.begin(), value.end());
        // add to vector of values
        try {
            m_params.push_back(std::stof(value)); // all to floats
        } catch(std::exception &err) {
            m_params.push_back(.0); // if non-float (first line)
        }
    }
}
