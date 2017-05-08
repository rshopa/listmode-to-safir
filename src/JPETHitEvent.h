/*! JPETHitEvent.h
* Classes transformation of event list into MUPET record
* utilizing SAFIR module
* Note: for unordered_map, use g++ file_name.cxx -std=c++11
* author: Roman Shopa
* Roman.Shopa@ncbj.gov.pl
* TODO: description of parameters */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>    // abs, pow
#include <assert.h>
#include "CListEventMUPET_simple.h"

#define PI 3.14159265

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
    JPETHit(const float& X, const float& Y, const float& Z, const float& Time)
        : m_Calibrated(false), m_Time(Time), m_isTOF(true),
          m_Coord(construct_cartesian(X,Y,Z)) {}
    JPETHit(const float& X, const float& Y, const float& Z)
        : m_Calibrated(false), m_Time(.0), m_isTOF(false),
          m_Coord(construct_cartesian(X,Y,Z)) {}
    ~JPETHit(){}

    //! Compares if both hits contain time parameters or vice-versa
    bool compare_TOF(const JPETHit& outerHit){
        return m_isTOF == outerHit.m_isTOF;
    }

    //! Returns TOF flag for external use (1 - with TOF, 0 - without)
    bool flag_TOF(const JPETHit& outerHit){
        return m_isTOF && outerHit.m_isTOF;
    }

    //! Returns one given parameter
    template <class ReturnClass>
    ReturnClass get_value(Params parameter){
        assert(m_Calibrated);            // assert if false (calibration is essential)
        switch (parameter) {
            case Ring: return m_Ring;
            case Detector: return m_Det;
            case TimeOfHit: {
                if(!m_isTOF) std::cout << "WARNING: TOF is not set (default is 0.0 *s)!" << std::endl;
                return m_Time;
            }
        }
        // Time of hit is the default return (unused)
        return m_Time;
    }

    //! Returns Ring and Detector indices in STL vector
    std::vector<unsigned> get_position(){
        assert(m_Calibrated);           // assert if false (calibration is essential)
        std::vector<unsigned> vec;
        vec.push_back(m_Ring);
        vec.push_back(m_Det);
        return vec;
    }

    //! Assigns proper indices to Ring and Detector using calibration parameters
    //! Sets Ring and Detector values with flag m_Calibrated = true
    void calibrate(const float& PETRadius,
                   const float& PETLength,
                   const unsigned& PETNrings,
                   const unsigned& PETNdetectors){
        // first validate x^2+y^2=R^2 (with the precision of .5 mm)
        assert(std::abs(PETRadius - sqrt(std::abs(pow(m_Coord[0], 2.)
                                         + pow(m_Coord[1], 2.)))) <= .5);
        float* p_ang = new float;                                          // temporary pointer
        *p_ang = get_angle(m_Coord[0], m_Coord[1]);
        m_Det = unsigned(int(*p_ang == 0) +
                         ceilf(PETNdetectors*(*p_ang)/(2*PI))-1);          // exception for 0
        m_Ring = unsigned(int(m_Coord[2] == 0) +
                          ceilf(PETNrings*float(m_Coord[2])/PETLength)-1); // exception for 0
        m_Calibrated = true;
        delete p_ang;                                                      // unused anymore
    }

private:    
    unsigned m_Ring, m_Det;
    bool m_Calibrated;
    const float m_Time;
    const bool m_isTOF;                    // flag for TOF consideration
    const std::vector<float> m_Coord;

    //! Creates STL vector with Cartesian input coordinates
    std::vector<float> construct_cartesian(const float & x,
                                           const float & y,
                                           const float & z){
        std::vector<float> vec;
        vec.push_back(x);
        vec.push_back(y);
        vec.push_back(z);
        return vec;
    }

    //! Returns angle from 0 to 2*PI from Cartesian pair (X,Y)
    float get_angle(const float& x, const float& y){
        float angle = atan2(y,x);
        if(angle>=0) return angle;
        else return angle + 2*PI;
    }
};


//! Class for the event containing two hits allowing calibration
//! and transformation into MUPET object (only for Cartesian, no TOF)
//! TODO: add TOF parameters
class JPETEvent {
public:
    //! Constructor for 2 JPETHit objects (hit1 and hit2)
    JPETEvent(const JPETHit& hit1, const JPETHit& hit2)
        : m_Hit1(hit1), m_Hit2(hit2), m_Calibrated(false) {
        if(!m_Hit1.compare_TOF(m_Hit2))
            std::cout << "WARNING: TOF flags differ for two hits!" << std::endl;
        m_flagTOF = m_Hit1.flag_TOF(m_Hit2);  // sets flag for TOF (TODO later)
    }
    //! Constructor from pair of Cartesian vectors
    JPETEvent(std::vector<float>& hit1_coords,
              std::vector<float>& hit2_coords)
        : m_Hit1(JPETHit(hit1_coords[0],
                         hit1_coords[1],
                         hit1_coords[2])),
          m_Hit2(JPETHit(hit2_coords[0],
                         hit2_coords[1],
                         hit2_coords[2])),
          m_Calibrated(false) {
        m_flagTOF = m_Hit1.flag_TOF(m_Hit2);  // sets flag for TOF (TODO later)
    }
    ~JPETEvent(){}

    //! TOF flag - might be deprecated or in TODO list
    bool check_TOF(){
        return m_flagTOF;
    }

    //! Calibrates both hits and sets flag m_Calibrated to 1
    void calibrate(std::unordered_map<std::string, float>& params) {
        m_Hit1.calibrate(params["RADIUS"],
                         params["LENGTH"],
                         params["RINGS"],
                         params["DETECTORS"]);
        m_Hit2.calibrate(params["RADIUS"],
                         params["LENGTH"],
                         params["RINGS"],
                         params["DETECTORS"]);
        m_Calibrated = true;
    }

    //! Returns MUPET objest
    CListRecordMUPET_simple to_MUPET(){
        assert(m_Calibrated);               // Must be calibrated in order to transform data
        CListRecordMUPET_simple tmprecord;
        tmprecord.raw = 0;
        tmprecord.event_data.ringA = m_Hit1.get_position()[0];
        tmprecord.event_data.ringB = m_Hit2.get_position()[0];
        tmprecord.event_data.detA =  m_Hit1.get_position()[1];
        tmprecord.event_data.detB = m_Hit2.get_position()[1];
        tmprecord.event_data.layerA = 0;
        tmprecord.event_data.layerB = 0;
        tmprecord.event_data.reserved = 0x0;
        tmprecord.event_data.type = 0x0;
//        std::cout << "Raw: " << tmprecord.raw << std::endl;
        return tmprecord;
    }

private:
    JPETHit m_Hit1, m_Hit2;         // should not be const
    bool m_flagTOF, m_Calibrated;
};
