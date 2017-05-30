/* Class for importing geometry parameters from outer file
* Originally for json, but there are no standard library in g++ for json.
* Hence the file comprises rows in the Interfile-like form: "parameter_name := value" 
* author: Roman Shopa
* Roman.Shopa@ncbj.gov.pl */

#include <vector>
#include <unordered_map>

#ifndef JPETParameters_H
#define JPETParameters_H

// TODO json outer file/interfile as input???

class JPETParameters {
public:
    //! Constructors for given file name or empty
    JPETParameters();
    JPETParameters(const char* file_name);
    ~JPETParameters();

    //! Duplicates one of the constructors
    void import_parameters(const char* file_name);

    //! Returns output in the form of unordered map
    //! Keys: "RADIUS", "LENGTH", "RINGS", "DETECTORS",
    //! "STRIP_WIDTH" and "STRIP_HEIGHT"
    std::unordered_map<std::string, float> params_map();

private:
    std::vector<float> m_params;
    bool m_empty;

    //! Imports data from parameters file (the example is below)
    //! 0 Scanner parameters :=
    //! 1    Radius (mm)                    :=      XXX.X
    //! 2    Length (mm)                    :=      XXX.X
    //! 3    Rings                          :=      XXX
    //! 4    Detectors                      :=      XXX
    //! 5    Strip width, tangential (mm)   :=      XXX.X
    //! 6    Strip width, radial (mm)       :=      XXX.X
    void m_import(const char* file_name);
};

#endif
