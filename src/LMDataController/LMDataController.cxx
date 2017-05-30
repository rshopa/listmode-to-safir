/*! LMDataController.h
 * List mode data controller for import, export and transform data.
 * A function for generating the .par file with JPET geometry
 * is implemented
 * author: Roman Shopa
 * Roman.Shopa@ncbj.gov.pl */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <random>
#include <math.h>
#include <algorithm>
//#include <numeric>
#include "../include/LMDataController.h"
#include "../include/JPETParameters.h"

#define PI 3.14159265

// List mode data controller for import, export and transform data.
LMDataController::LMDataController(const char* path_to_file)
    : m_DOI_averages({
                        {"PMT",{.0,.0}},
                        {"SI",{.0,.0}},
                        {"WLS",{.0,.0}},
                    }),
      m_full_path_to_file(path_to_file) {
    // read name of the file,
    // cut pre-path and get number of rows
    std::istringstream path(path_to_file);
    path >> m_file_name;
    cut_pre_path();
    get_number_of_rows();

    // parameters from the name of file, which
    // don't throw out an error even for exceptions
    m_pos_D = m_file_name.find("D");
    m_pos_L = m_file_name.find("L");
    m_pos_mm = m_file_name.find("mm");
    m_params_parsed = (m_pos_D != std::string::npos) &&
                      (m_pos_L != std::string::npos) &&
                      (m_pos_mm != std::string::npos);
    // boolean - if params file could be created by parsing filename
    if(m_params_parsed){
        m_diameter_key = m_file_name.substr(m_pos_D+1,2);
        m_length_key = m_file_name.substr(m_pos_L+1,3);
        m_width_key = m_file_name.substr(m_pos_mm-1,1);
    } else {
        std::cout << "WARNING: could not parse parameters "
                  << "from file name." << std::endl;
    }
}

// Destructor
LMDataController::~LMDataController(){}

// Getter for file name (not the full path)
std::string LMDataController::get_file_name(){
    return m_file_name;
};

// Saves file with geometry parameters
void LMDataController::create_parameters_file
                       (const char *output_file_name){
    // Geometry parameters
    float radius,  // temporary variables
          length,
          width,
          rings,
          no_of_strips;
    if(!is_formatted()){
        std::cout << "Sorry! The format of file name "
                  << "and/or geometry parameters "
                  << "is(are) not supported. Please, create "
                  << ".par file manually." <<std::endl;
    } else {
        width = std::stof(m_width_key);
        radius = PMBGeometries()["DIAMETER"][DIAMETER]["WIDTH"][width]["RADIUS"];
        length = std::stof(m_length_key)*10.;
        rings = PMBGeometries()["DIAMETER"][DIAMETER]["LENGTH"][length]["RINGS"];
        no_of_strips = PMBGeometries()["DIAMETER"][DIAMETER]["WIDTH"][width]["NO_OF_STRIPS"];

        std::ofstream outfile;
        outfile.open(output_file_name);
        outfile << "Scanner parameters "
                << ":=" << std::endl;
        if(std::stof(m_diameter_key) == 85) {
            outfile << "\t" << "Radius (mm) := ";
            outfile << radius << std::endl;
            outfile << "\t" << "Length (mm) := ";
            outfile << length << std::endl;
            outfile << "\t" << "Rings := ";
            outfile << rings << std::endl;
            outfile << "\t" << "Detectors := ";
            outfile << no_of_strips << std::endl;
            outfile << "\t" << "Strip width, tangential (mm) := ";
            outfile << width << std::endl;
            outfile << "\t" << "Strip width, radial (mm) := ";
            outfile << HEIGHT << std::endl;

            std::cout << "File \"" << output_file_name << "\" created.\n"
                      << "Please note that the strip width in radial direction "
                      << "is set to 20 mm. You could change these values "
                      << "if needed." << std::endl;
        } else {
            std::cout << "Sorry! Unsupported geometry. Please, create "
                      << ".par file manually." << std::endl;
        }
        outfile << "END Scanner parameters "
                << ":=" << std::endl;
        outfile.close();
    }
}

// Imports geometry from the geometry parameters file
void LMDataController::import_geometry(const char* geometry_file){
    // First, import parameters from outer file
    JPETParameters JPET_params;
    JPET_params.import_parameters(geometry_file);
    std::unordered_map<std::string, float> params_hash =
                                    JPET_params.params_map();
    // Create m_<>_keys as strings from floats
    std::ostringstream s_length,
                       s_diameter,
                       s_width;
    s_length << params_hash["LENGTH"]/10.;
    // Complex formula (distinct radius to simple key, i.e. 75, 85, 95)
    s_diameter << round(params_hash["RADIUS"]*2/10.) - 2;
    s_width << params_hash["STRIP_WIDTH"];
    m_length_key = s_length.str();
    m_diameter_key = s_diameter.str();
    m_width_key = s_width.str();
}

// Returns DOI vector (in order {mean, median})
std::vector<double> LMDataController::DOI_averages(
                                    const std::string& key){
    auto tag = m_DOI_averages.find(key);
    if (tag == m_DOI_averages.end()){
        std::cout << "ERROR: No such photomultiplier!";
        return {.0,.0};
    } else
    return m_DOI_averages[key];
}

// Saves file with DOI parameter
void LMDataController::create_DOI_file(const char* output_file){
    std::ofstream DOI_file;
    DOI_file.open(output_file);
    DOI_file << "Depth-of-interaction parameters (cm) "
            << ":=" << std::endl;
    DOI_file << "\t" << "PMT_Mean := "
             << m_DOI_averages["PMT"][0] / 10. << std::endl;
    DOI_file << "\t" << "PMT_Median := "
             << m_DOI_averages["PMT"][1] / 10. << std::endl;
    DOI_file << "\t" << "SI_Mean := "
             << m_DOI_averages["SI"][0] / 10. << std::endl;
    DOI_file << "\t" << "SI_Median := "
             << m_DOI_averages["SI"][1] / 10. << std::endl;
    DOI_file << "\t" << "WLS_Mean := "
             << m_DOI_averages["WLS"][0] / 10. << std::endl;
    DOI_file << "\t" << "WLS_Median := "
             << m_DOI_averages["WLS"][1] / 10. << std::endl;
    DOI_file << "END Depth-of-interaction parameters "
             << ":=" << std::endl;
    DOI_file.close();
    std::cout << "Depth of interaction estimates has been written "
              << "to file \"" << output_file  << "\"" << std::endl;
}

// Writes out a new file with z1 and z2 blurred according to PM type
// Calculates DOI mean and median
void LMDataController::export_blured(const PhotoMultiplier &pm){
    if(is_formatted()){
        double sd,               // standard deviation
               PET_edge_radius;  // Tomograph radius
        // in & out streams
        std::ifstream in_file(m_full_path_to_file);
        std::ofstream out_file;
        std::string PM_name,        // photomultiplier
                    one_row;        // iteration row
        // compose out name
        std::ostringstream s_name;
        // vectors for calculating Depth-Of-Interaction
        std::vector<double> DOI_combined, // all events
                            DOI_output;   // mean and median
        // Radius is measured to the edge of scintillator (not in the centre)
        PET_edge_radius = PMBGeometries()["DIAMETER"]
                [DIAMETER]["WIDTH"][std::stof(m_width_key)]["RADIUS"] -
                (HEIGHT/2.);
        switch(pm){
            case PMT: { s_name << "PMT";
                        sd = blur_sigma(LMDataController::PMT);
                        break; }
            case SI: { s_name << "SI";
                       sd = blur_sigma(LMDataController::SI);
                       break; }
            case WLS: { s_name << "WLS";
                        sd = blur_sigma(LMDataController::WLS);
                        break; }
        }
        PM_name = s_name.str();
        s_name << "_" << m_file_name;
        std::cout << "Writing blurred data to "
                  << s_name.str() << "... ";

        // Generator for normal distribution
        std::default_random_engine generator;
        std::normal_distribution<double>distribution(0,sd);
        // The main loop and file output
        out_file.open(s_name.str());
        while (std::getline(in_file, one_row)){
            double x1,y1,z1,t1,
                   x2,y2,z2,t2,
                   the_rest,        // rest of row (cut out)
                   added_blur;      // smeared Z added to Z
            // Blur section
            std::istringstream buffer(one_row);
            std::ostringstream str_buff;
            buffer >> x1 >> y1 >> z1 >> t1
                   >> x2 >> y2 >> z2 >> t2
                   >> the_rest; // a tricky way
            // Add random Gauss to Z1 and Z2
            added_blur = distribution(generator);
            z1 = z1 + added_blur;
            added_blur = distribution(generator);
            z2 = z2 + added_blur;
            // DOI section (calculate mean and median)
            std::vector<double> DOI_vector = DOI_pair(x1, y1, z1,
                                                      x2, y2, z2,
                                                      PET_edge_radius);
            DOI_combined.insert(DOI_combined.end(),
                                DOI_vector.begin(),
                                DOI_vector.end());
            // 2 digits after the decimal point
            str_buff.precision(std::numeric_limits<double>::digits10-13);
            str_buff << std::fixed
                     << x1 << "\t"
                     << y1 << "\t"
                     << z1 << "\t"
                     << t1 << "\t"
                     << x2 << "\t"
                     << y2 << "\t"
                     << z2 << "\t"
                     << t2 << "\n";
            out_file << str_buff.str();
        }
        out_file.close();
        std::cout << "Done!" << std::endl;

        // DOI (continue)
        DOI_output = {
            std::accumulate(DOI_combined.begin(),
                            DOI_combined.end(),0.0) / DOI_combined.size(),
            median(DOI_combined)
        };
        m_DOI_averages[PM_name] = DOI_output;
    } else {
        std::cout << "ERROR: could not blur the data.\n"
                  << "Please import parameters from th geometry"
                  << " parameters .par file." << std::endl;
    }
}

// Eliminates slashes from filename path (only name remains
void LMDataController::cut_pre_path(){
    std::string delimiter = "/";
    size_t slash_pos;
    while((slash_pos = m_file_name.find(delimiter)) != std::string::npos){
        m_file_name = m_file_name.substr(slash_pos+1, m_file_name.size());
    }
}

// Checks out if the name of file is consistent with the geometry
bool LMDataController::is_formatted(){
    try { // validate if floats
        std::stof(m_diameter_key);
        std::stof(m_length_key);
        std::stof(m_width_key);
    } catch (std::exception &err) {
        return false;
    }
    return m_params_parsed &&
           (std::stof(m_diameter_key) == DIAMETER);
    // TODO: other validations
}

// Calculates number of lines in the file
void LMDataController::get_number_of_rows(){
    std::ifstream temp_file(m_full_path_to_file);
    std::string line;
    unsigned number_of_lines = 0;
    while (std::getline(temp_file, line))
        ++number_of_lines;
    m_number_of_rows = number_of_lines;
}

// Returns sigma for smearing data in axial direction
float LMDataController::blur_sigma(const PhotoMultiplier &pm){
    int length, index;
    float sigma,
          coeff_multi[2], // coefficients for multiply
          c_scin(12.6);
    length = std::stoi(m_length_key)*10.;
    coeff_multi[0] = c_scin/(4*sqrt(log(2)));
    coeff_multi[1] = 1/(2*sqrt(2*log(2)));
    switch(length){
        case 200: { index = 0; break; }
        case 500: { index = 1; break; }
        case 1000: { index = 2; break; }
    }
    switch(pm){
        case PMT: { sigma = CRT_PMT[index]*coeff_multi[0]; break; }
        case SI: { sigma = CRT_SI[index]*coeff_multi[0]; break; }
        case WLS: { sigma = FWHM_WLS*coeff_multi[1]; break; }
    }
    return sigma;
}

// Returns median of a float vector
double LMDataController::median(std::vector<double>& in_vector){
    double median;
    size_t size = in_vector.size();
    // sort (stable_sort)
    std::stable_sort(in_vector.begin(),in_vector.end());
    if(size % 2 == 0){
        median = (in_vector[size/2 - 1] + in_vector[size/2]) / 2;
    } else {
        median = in_vector[size/2];
    }
    return median;
}

// Calculates DOI from set of values (array of 2 - for 2 hits)
std::vector<double> LMDataController::DOI_pair(const double& x1,
                                               const double& y1,
                                               const double& z1,
                                               const double& x2,
                                               const double& y2,
                                               const double& z2,
                                               const double& PET_radius){
    std::vector<double> DOI_pair; // for output
    double r_total,
           z_total,
           tilt_angle;
    // all input values (x,y,z) are miltiplied by 10 (cm -> mm)
    r_total = sqrt(pow(x1*10.,2.) + pow(y1*10.,2.)) +
              sqrt(pow(x2*10.,2.) + pow(y2*10.,2.));
    z_total = z2 - z1;
    tilt_angle = PI*(z_total == 0.) + atan(r_total / z_total);
    DOI_pair.push_back(std::abs(sqrt(pow(x1*10.,2.)+pow(y1*10.,2.)) /
                           sin(tilt_angle)) - PET_radius);
    DOI_pair.push_back(std::abs(sqrt(pow(x2*10.,2.)+pow(y2*10.,2.)) /
                           sin(tilt_angle)) - PET_radius);
    return DOI_pair;
}
