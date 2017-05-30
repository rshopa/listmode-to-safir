/*! LMDataController.h
 * List mode data controller for import, export and transform data.
 * A function for generating the .par file with JPET geometry
 * is implemented
 * author: Roman Shopa
 * Roman.Shopa@ncbj.gov.pl */

//#include <iostream>
//#include <string>
//#include <sstream>
//#include <fstream>
#include <vector>
#include "LMDataController.inl"

#ifndef LMDataController_H
#define LMDataController_H

//! One class for the controller
class LMDataController {
public:
    //! Type of photomultiplier for smearing Z
    enum PhotoMultiplier {PMT, SI, WLS};

    //! Constructor and destructor
    LMDataController(const char* path_to_file);
    ~LMDataController();

    //! Getter for file name (not the full path)
    std::string get_file_name();

    //! Saves file with geometry parameters
    void create_parameters_file(const char* output_file_name);

    //! Imports geometry from the geometry parameters file
    void import_geometry(const char* geometry_file);

    //! Returns DOI vector (in order {mean, median})
    std::vector<double> DOI_averages(const std::string& key);

    //! Saves file with DOI parameters
    void create_DOI_file(const char* output_file_name);

    //! Writes out a new file with 8 columns:
    //! x1 y1 z1 t1 x2 y2 z2 t2,
    //! where both z1 and z2 were blurred according to PM type given
    void export_blured(const PhotoMultiplier& pm);

    // Temporary output
    void print_out(){
//        std::ostringstream s_full_path;
//        s_full_path << m_full_path_to_file;
        std::string str_path(m_full_path_to_file);

        std::cout << "Full path: " << str_path << "\n";
        std::cout << "File name: " << m_file_name << "\n";
        std::cout << "D: " << std::stof(m_diameter_key) << "\n";
        std::cout << "L: " << std::stof(m_length_key) << "\n";
        std::cout << "mm: " << std::stof(m_width_key) << "\n";
    }

private:
    std::unordered_map<std::string, std::vector<double>> m_DOI_averages;

    unsigned m_number_of_rows;
    const char* m_full_path_to_file;
    std::string m_file_name,
                m_diameter_key,
                m_length_key,
                m_width_key;
    bool m_params_parsed = false;
    size_t m_pos_D,
           m_pos_L,
           m_pos_mm;

    //! Eliminates slashes from filename path (only name remains)
    void cut_pre_path();

    //! Checks out if the name of file is consistent with the geometry
    bool is_formatted();

    //! Calculates number of lines in the file
    void get_number_of_rows();

    //! Returns sigma for smearing data in axial direction
    float blur_sigma(const PhotoMultiplier& pm);

    //! Returns median of a float
    double median(std::vector<double>& in_vector);

    //! Calculates DOI from set of values (array of 2 - for 2 hits)
    std::vector<double> DOI_pair(const double& x1,
                                 const double& y1,
                                 const double& z1,
                                 const double& x2,
                                 const double& y2,
                                 const double& z2,
                                 const double& PET_radius);
};

#endif
