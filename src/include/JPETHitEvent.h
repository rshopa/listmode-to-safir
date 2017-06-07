/*! JPETHitEvent.h
* Classes for the transformation of event list into MUPET
* record utilizing SAFIR module
* Note: for unordered_map, use g++ file_name.cxx -std=c++11
* author: Roman Shopa
* Roman.Shopa@ncbj.gov.pl
* TODO: description of parameters */

#include <iostream>
#include <vector>
#include <unordered_map>
#include "CListEventMUPET_simple.h"

#ifndef JPETHitEvent_H
#define JPETHitEvent_H

/*  Note: PSF-files are set in cm units for coordinates and ps - for hit times */

//! Class for hit object which transforms input in the form of Cartesian (X,Y,Z) coordinates
//! into indices for Ring and Detector using J-PET geometry (calibration procedure)
class JPETHit {
public:
    enum Params {Ring ,Detector, TimeOfHit}; // parameters of hit

    //! Constructors and destructor
    //! X,Y,Z -- Cartesian coordinates in mm
    //! Time -- hit time (not implemented at the moment)
    //! Note: new object is not calibrated (flag m_Calibrated = false)
    JPETHit(const float& X,
            const float& Y,
            const float& Z,
            const float& Time);
    JPETHit(const float& X,
            const float& Y,
            const float& Z);
    ~JPETHit();

    //! Compares if both hits contain time parameters or vice-versa
    bool compare_TOF(const JPETHit& outerHit);

    //! Returns TOF flag for external use (1 - with TOF, 0 - without)
    bool flag_TOF(const JPETHit& outerHit);

    //! Returns one given parameter
    template <class ReturnClass>
    ReturnClass get_value(Params parameter);

    //! Returns Ring and Detector indices in STL vector
    std::vector<unsigned> get_position();

    //! Assigns proper indices to Ring and Detector using calibration parameters
    //! Sets Ring and Detector values with flag m_Calibrated = true
    void calibrate(const float& PETRadius,
                   const float& PETLength,
                   const unsigned& PETNrings,
                   const unsigned& PETNdetectors,
                   const float& PETWidth,
                   const float& PETHeight);

private:    
    unsigned m_Ring, m_Det;
    bool m_Calibrated;
    const float m_Time;
    const bool m_isTOF;                    // flag for TOF consideration
    const std::vector<float> m_Coord;

    //! Creates STL vector with Cartesian input coordinates
    std::vector<float> construct_cartesian(const float & x,
                                           const float & y,
                                           const float & z);

    //! Returns angle from 0 to 2*PI from Cartesian pair (X,Y)
    float get_angle(const float& x,
                    const float& y);
};


//! Class for the event containing two hits allowing calibration
//! and transformation into MUPET object (only for Cartesian, no TOF)
//! TODO: add TOF parameters
class JPETEvent {
public:
    //! Constructor for 2 JPETHit objects (hit1 and hit2)
    JPETEvent(const JPETHit& hit1,
              const JPETHit& hit2);
    //! Constructor from pair of Cartesian vectors
    JPETEvent(std::vector<float>& hit1_coords,
              std::vector<float>& hit2_coords);
    //! Destructor
    ~JPETEvent();

    //! TOF flag
    bool check_TOF();

    //! Calibrates both hits and sets flag m_Calibrated to 1
    void calibrate(std::unordered_map<std::string,
                                      float>& params);

    //! Returns MUPET objest
    CListRecordMUPET_simple to_MUPET();
private:
    JPETHit m_Hit1, m_Hit2;         // should not be const
    bool m_flagTOF, m_Calibrated;
};

#include "../MUPET/JPETHitEvent.cxx" // for templates

#endif
