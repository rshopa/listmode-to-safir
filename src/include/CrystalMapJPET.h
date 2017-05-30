/*! CrystalMapJPET.h
* Class for generating crystal map for SAFIR module
* author: Roman Shopa
* Roman.Shopa@ncbj.gov.pl
* TODO: description of parameters */
#include <iostream>

#ifndef CrystalMapJPET_H
#define CrystalMapJPET_H

class CrystalMapJPET {
public:
    enum Cartesian { x, y, z }; // which axis to derive

    //! Default constructor
    //! R - PET radius (mm)
    //! Z - PET axial length (mm)
    //! N_D - number of strips
    //! N_Z - number of "cuts" along strip (corresponds to number of PET rings)
    //! header - flag for adding header at the beginning of the file
    CrystalMapJPET(const float& R,
                   const float& Z,
                   const unsigned& N_D,
                   const unsigned& N_Z = 100,
                   const bool& header = true);
    //! Destructor
    ~CrystalMapJPET(){}

    //! Returns one listmode line for give ring and detector indices
    std::string get_line(const unsigned& ring,
                         const unsigned& detector);

    //! Creates crystal_map .txt file as in SAFIR example
    void save_to_file(const char* file_name);

private:
    float m_R, m_Z;         // radius and axial length of JPET
    unsigned m_N_d, m_N_z;  // number of strips and "rings"
    bool m_header;

    //! Returns selected Cartesian coordinate
    float get_coord(const Cartesian& q,
                    const unsigned& ring,
                    const unsigned& detector);
};

#endif
