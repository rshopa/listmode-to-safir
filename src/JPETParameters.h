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

// TODO json outer file/interfile as input???

class JPETParameters {
public:
    //! Constructors for given file name or empty
    JPETParameters()
        : m_params({0.,0.,0.,0.}), m_empty(true) {}
    JPETParameters(char* file_name)
        : m_params({0.,0.,0.,0.}), m_empty(true) {
        m_import(file_name);
        m_empty = false; // flag to false (filled in)
    }
    ~JPETParameters() {}

    //! Duplicates one of the constructors
    void import_parameters(char* file_name) {
        m_import(file_name);
        m_empty = false; // flag to false (filled in)
    }

    //! Returns output in the form of unordered map
    //! Keys: "RADIUS", "LENGTH", "RINGS" and "DETECTORS"
    std::unordered_map<std::string, float> params_map(){
        if(m_empty) std::cout << "WARNING: an empty map!" << std::endl;
        std::unordered_map<std::string, float> params = {
            {"RADIUS", m_params[0]},
            {"LENGTH", m_params[1]},
            {"RINGS", m_params[2]},
            {"DETECTORS", m_params[3]}
        };
        return params;
    }

private:
    std::vector<float> m_params;
    bool m_empty;

    //! Imports data from parameters file
    void m_import(char* file_name){
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
            m_params.push_back(std::stof(value)); // all to floats
        }
    }
};
