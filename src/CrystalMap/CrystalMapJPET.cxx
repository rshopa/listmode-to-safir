/*! Implementation for crystal map class (for SAFIR module)
* author: Roman Shopa
* Roman.Shopa@ncbj.gov.pl */
#include <iostream>
#include <sstream>      // std::ostringstream
#include <fstream>      // std::ofstream
#include <math.h>
#include <assert.h>
#include "../include/CrystalMapJPET.h"

#define PI 3.14159265

// Default constructor
CrystalMapJPET::CrystalMapJPET(const float& R,
                               const float& Z,
                               const unsigned& N_D,
                               const unsigned& N_Z,
                               const bool& header)
                    : m_R(R), m_Z(Z), m_N_d(N_D), m_N_z(N_Z), m_header(header) {}

// Returns one listmode line for give ring and detector indices
std::string CrystalMapJPET::get_line(const unsigned& ring,
                                     const unsigned& detector) {
                    assert(ring < m_N_z);       // maximum ring No = N_Z-1
                    assert(detector < m_N_d);   // maximum detector No = N_D-1
                    std::ostringstream str_buff;
                    std::string s;
                    str_buff << ring << "\t"
                             << detector << "\t"
                             << get_coord(x, ring, detector) << "\t"
                             << get_coord(y, ring, detector) << "\t"
                             << get_coord(z, ring, detector) << "\n";
                    s = str_buff.str();
                    return s;
                }

// Creates crystal_map .txt file as in SAFIR example
void CrystalMapJPET::save_to_file(const char* file_name){
    std::ofstream outfile;
    outfile.open(file_name);
    // adds header if true
    if(m_header){
        outfile << "#ring\t" << "det\t"
                << "x\t" << "y\t" << "z\n";
    }
    for(unsigned i = 0; i < m_N_z; ++i){
        for(unsigned j = 0; j < m_N_d; ++j){
            outfile << get_line(i, j);
        }
    }
    outfile.close();
}

// Returns selected Cartesian coordinate
/* Note: [-250,250] range for Z is wrong, must be -1 ring length,
 * hence (m_Z-m_Z/m_N_z) is used instead of just m_Z */
float CrystalMapJPET::get_coord(const Cartesian& q,
                const unsigned& ring,
                const unsigned& detector){
    switch(q){
        // all values are rounded by 2 digits after comma, i.e. floor(100.*x + .5)/100.
        case x: return floor(100. * (m_R * cos(2*PI*detector/m_N_d)) + .5) / 100.;
        case y: return floor(100. * (m_R * sin(2*PI*detector/m_N_d)) + .5) / 100.;
        case z: return floor(100. * ((m_Z-m_Z/m_N_z) * ring/(m_N_z-1) -
                                     (m_Z-m_Z/m_N_z)/2) + .5) / 100.;
    }
    // default Cartesian returns Z (unused);
    return floor(100. * ((m_Z-m_Z/m_N_z) * ring/(m_N_z-1) -
                         (m_Z-m_Z/m_N_z)/2) + .5) / 100.;
}
