// CListEventMUPET_simple.h
// Class to write binary CListEvents for MUPET: simple versio, just data structure.
// Use class CListRecordMUPET to write data to binary file
// author: Jannis Fischer
// jannis.fischer@cern.ch
//
// Note: Order below assumes little endian. If you are using big endian systems, you have to reverse the order of the bit fields.
//
// Minimal example to write one event:
/*
 	#include<iostream>
 	#include<fstream>
 	#include "CListEventMUPET_simple.h"
 	ofstream outfile;
	const char* file_sig = "MUPET CListModeData\0";
	outfile.write( file_sig, 32 ); // 32 bytes are reserved for file signature
	CListRecordMUPET tmprecord;
	tmprecord.raw = 0;
	tmprecord.event_data.ringA = 0;
	tmprecord.event_data.detA =  1;
	tmprecord.event_data.ringB = 0;
	tmprecord.event_data.detB = 99;
	tmprecord.event_data.reserved = 0x0 ;
	tmprecord.event_data.type = 0x0;
	outfile.write( reinterpret_cast<char*>(&tmprecord), sizeof(tmprecord));
	outfile.close();
*/
#include <stdint.h>

#ifndef CListEventMUPET_simple_h
#define CListEventMUPET_simple_h

class CListEventDataMUPET_simple
{
public:
	unsigned ringA : 8;
	unsigned ringB : 8;
	unsigned detA : 16;
	unsigned detB : 16;
	unsigned layerA : 4;
	unsigned layerB : 4;
	unsigned reserved : 6;
	unsigned isRandom : 1;
	unsigned type : 1;
};

class CListTimeDataMUPET_simple
{
public:
	unsigned long time : 48;
	unsigned reserved : 15;
	unsigned type : 1;
};

class CListRecordMUPET_simple
{
	public: union {
		CListEventDataMUPET_simple event_data ;
		CListTimeDataMUPET_simple time_data;
	    uint64_t raw;
	};
};

#endif
