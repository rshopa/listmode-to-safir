cmake_minimum_required(VERSION 2.6)
project(SAFIR_JPET_TRANSFORM CXX) # using only C++

add_definitions(-std=c++11 -Wall -Wunused-parameter -Wextra)

set(HEADERS "${CMAKE_CURRENT_LIST_DIR}/include")
set(JPET_PARAMETERS "${CMAKE_CURRENT_LIST_DIR}/JPETParameters")
set(CRYSTAL_MAP "${CMAKE_CURRENT_LIST_DIR}/CrystalMap")
set(MUPET "${CMAKE_CURRENT_LIST_DIR}/MUPET")
set(LM_DATA_CONTROLLER "${CMAKE_CURRENT_LIST_DIR}/LMDataController")

#MESSAGE( STATUS "PATH: " ${CRYSTAL_MAP})

# Headers could be omitted (but not necessary)
list(APPEND CrystalMap_SOURCES
#	"${HEADERS}/JPETParameters.h"
#	"${HEADERS}/CrystalMapJPET.h"
	"${CRYSTAL_MAP}/CrystalMapJPET.cxx"
	"${CRYSTAL_MAP}/GenerateCrystalMapJPET.cxx"
	"${JPET_PARAMETERS}/JPETParameters.cxx"
)
list(APPEND MUPET_SOURCES
#	"${HEADERS}/JPETParameters.h"
	"${JPET_PARAMETERS}/JPETParameters.cxx"
#	"${HEADERS}/JPETHitEvent.h"
	"${MUPET}/JPETHitEvent.cxx"
#	"${HEADERS}/CListEventMUPET_simple.h"
	"${MUPET}/ConvertToMUPET.cxx"

#	"${HEADERS}/ConvertToSAFIR.cxx" # treat as header and do not include!!!
)
list(APPEND DATA_CONTROLLER_SOURCES
#	"${HEADERS}/JPETParameters.h"
	"${JPET_PARAMETERS}/JPETParameters.cxx"
#	"${HEADERS}/CrystalMapJPET.h"
	"${CRYSTAL_MAP}/CrystalMapJPET.cxx"
#	"${HEADERS}/CListEventMUPET_simple.h"
#	"${HEADERS}/JPETHitEvent.h"
	"${MUPET}/JPETHitEvent.cxx"
#	"${HEADERS}/LMDataController.h"
	"${LM_DATA_CONTROLLER}/LMDataController.cxx"
	"${LM_DATA_CONTROLLER}/GenerateBlurredMUPET.cxx"

#	"${HEADERS}/ConvertToSAFIR.cxx" # treat as header and do not include!!!
)

#MESSAGE( STATUS "PATH: " ${CrystalMap_SOURCES})

add_executable(GenerateCrystalMapJPET ${CrystalMap_SOURCES})

add_executable(ConvertToMUPET ${MUPET_SOURCES})

add_executable(GenerateBlurredMUPET ${DATA_CONTROLLER_SOURCES})
