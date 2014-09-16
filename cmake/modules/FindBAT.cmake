# Author: N. Winckler
# - Try to find BAT
# it requires a standard make install of BAT. search dirs are in /usr/local or /opt/local
# Once done, this will define
#
#  BAT_FOUND - system has BAT
#  BAT_INCLUDE_DIR - the BAT include directories
#  BAT_LIBRARY_DIR - the BAT lib directories
#  BAT_LIBRARIES_SHARED and BAT_LIBRARIES_STATIC - link these to use BAT
#  BAT_DEFINITIONS 

#include(LibFindMacros)
message(STATUS "Looking for BAT...")
# Dependencies
#libfind_package(BAT cuba)
find_package(ROOT 5.34.00 REQUIRED)
find_package(cuba)

if(CUBA_FOUND)
  message("-- The cuba library has been found.")
else (CUBA_FOUND)
  message("-- The cuba library has not been found.")
endIf(CUBA_FOUND)



# Set the list of BAT header files
Set(BAT_HEADERS 
BATCalculator.h            BCEfficiencyFitter.h       
BCGraphFitter.h            BCIntegrate.h              
BCMTFChannel.h             BCMTFSystematicVariation.h 
BCMVCObservable.h          BCMath.h                   
BCParameter.h              BCSummaryTool.h
BCAux.h                    BCEngineMCMC.h             
BCH1D.h                    BCLog.h                    
BCMTFComparisonTool.h      BCMTFTemplate.h            
BCMVCPhysicsModel.h        BCModel.h                  
BCParameterSet.h           BCDataPoint.h              
BCFitter.h                 BCH2D.h                    
BCMTF.h                    BCMTFProcess.h             
BCMVCDataModel.h           BCMVCUncertainty.h         
BCModelManager.h           BCRooInterface.h
BCDataSet.h                BCGoFTest.h                
BCHistogramFitter.h        BCMTFAnalysisFacility.h    
BCMTFSystematic.h          BCMVCMeasurement.h         
BCMVCombination.h          BCModelOutput.h            BCSummaryPriorModel.h    
)

set(LIBBAT libBAT)

set(LIBBAT_MODEL models)
set(LIBBAT_MTF mtf)
set(LIBBAT_MVC mvc)
set(LIBBAT_COMPONENT_LIST ${LIBBAT_MODEL} ${LIBBAT_MTF} ${LIBBAT_MVC})

set(SEARCH_DIRS /usr/local/lib /usr/local/include/BAT /opt/local/lib /opt/local/include/BAT)

Set(BAT_FOUND FALSE)
Set(BAT_DEFINITIONS "")

##########################################################
# Form library file names 
if (APPLE)
  ##set libBAT.dylib libBATmodels.dylib libBATmtf.dylib libBATmvc.dylib
  set(LIBBAT_SHARED libBAT.dylib)
  set(LIBBAT_STATIC libBAT.a)
  FOREACH ( LIBBAT_COMPONENT_LIST  ${LIBBAT_COMPONENT_LIST} )
        set(LIBBAT_COMPONENT_LIST_SHARED ${LIBBAT_COMPONENT_LIST_SHARED} "libBAT${LIBBAT_COMPONENT_LIST}.dylib")
        set(LIBBAT_COMPONENT_LIST_STATIC ${LIBBAT_COMPONENT_LIST_STATIC} "libBAT${LIBBAT_COMPONENT_LIST}.a")
  ENDFOREACH(LIBBAT_COMPONENT_LIST)
  
else (APPLE)

  #set libBAT.so libBATmodels.so libBATmtf.so libBATmvc.so
  set(LIBBAT_SHARED libBAT.so)
  set(LIBBAT_STATIC libBAT.a)
  FOREACH ( LIBBAT_COMPONENT_LIST  ${LIBBAT_COMPONENT_LIST} )
        set(LIBBAT_COMPONENT_LIST_SHARED ${LIBBAT_COMPONENT_LIST_SHARED} "libBAT${LIBBAT_COMPONENT_LIST}.so")
        set(LIBBAT_COMPONENT_LIST_STATIC ${LIBBAT_COMPONENT_LIST_STATIC} "libBAT${LIBBAT_COMPONENT_LIST}.a")
  ENDFOREACH(LIBBAT_COMPONENT_LIST)
endIf (APPLE)


##########################################################
#define include path

find_path(BAT_INCLUDE_DIR NAMES ${BAT_HEADERS} 
  PATHS ${SEARCH_DIRS}
  DOC   "Path to BAT include header files."
)

find_path(BAT_LIBRARY_DIR NAMES libBAT.dylib
  PATHS ${SEARCH_DIRS}
  DOC   "Path to BAT lib files."
)

##########################################################
# define shared and static libBAT libraries
find_library(BAT_LIBRARY_SHARED NAMES ${LIBBAT_SHARED}
  PATHS ${SEARCH_DIRS}
  DOC   "Path to (shared) ${LIBBAT_SHARED}."
)

find_library(BAT_LIBRARY_STATIC NAMES ${LIBBAT_STATIC}
  PATHS ${SEARCH_DIRS}
  DOC   "Path to (static) ${LIBBAT_STATIC}."
)

##########################################################
# define all shared and static libBAT component libraries
list(LENGTH LIBBAT_COMPONENT_LIST_SHARED len1)
math(EXPR len2 "${len1} - 1")
FOREACH(val RANGE ${len2})
  list(GET LIBBAT_COMPONENT_LIST_SHARED ${val} BATlibName_SHARED)
  list(GET LIBBAT_COMPONENT_LIST_STATIC ${val} BATlibName_STATIC)

  #get component names i.e. models, mtf, mvc
  list(GET LIBBAT_COMPONENT_LIST ${val} SUFFIXNAME)
  STRING( TOUPPER ${SUFFIXNAME} USUFFIXNAME )

  find_library(BAT_LIBRARY_${USUFFIXNAME}_SHARED NAMES ${BATlibName_SHARED}
    PATHS ${SEARCH_DIRS}
    DOC   "Path to (shared) ${SUFFIXNAME}."
  )

  find_library(BAT_LIBRARY_${USUFFIXNAME}_STATIC NAMES ${BATlibName_STATIC}
    PATHS ${SEARCH_DIRS}
    DOC   "Path to (static) ${SUFFIXNAME}."
  )


  if(BAT_LIBRARY_${USUFFIXNAME}_SHARED)
  	  set(BAT_LIBRARY_${USUFFIXNAME}_SHARED_FOUND true)
      #message(STATUS "${SUFFIXNAME} has been found --> ${BAT_LIBRARY_${USUFFIXNAME}_SHARED}")
  else(BAT_LIBRARY_${USUFFIXNAME}_SHARED)
      #message(STATUS "${SUFFIXNAME} has not been found. Input name is ${BAT_LIBRARY_${USUFFIXNAME}_SHARED}")
  endif(BAT_LIBRARY_${USUFFIXNAME}_SHARED)

  if(BAT_LIBRARY_${USUFFIXNAME}_STATIC)
  	  set(BAT_LIBRARY_${USUFFIXNAME}_STATIC_FOUND true)
      #message(STATUS "${SUFFIXNAME} has been found --> ${BAT_LIBRARY_${USUFFIXNAME}_STATIC}")
  else(BAT_LIBRARY_${USUFFIXNAME}_STATIC)
      #message(STATUS "${SUFFIXNAME} has not been found. Input name is ${BAT_LIBRARY_${USUFFIXNAME}_STATIC}")
  endif(BAT_LIBRARY_${USUFFIXNAME}_STATIC)

  if(BAT_LIBRARY_${USUFFIXNAME}_SHARED_FOUND AND BAT_LIBRARY_${USUFFIXNAME}_STATIC_FOUND)
  	  set(BAT_LIBRARY_${USUFFIXNAME}_FOUND true)
  else(BAT_LIBRARY_${USUFFIXNAME}_SHARED)
  	  set(BAT_LIBRARY_${USUFFIXNAME}_FOUND false)
  endif(BAT_LIBRARY_${USUFFIXNAME}_SHARED_FOUND AND BAT_LIBRARY_${USUFFIXNAME}_STATIC_FOUND)

ENDFOREACH(val)

##########################################################
#check if main is there
if(BAT_INCLUDE_DIR AND BAT_LIBRARY_SHARED AND BAT_LIBRARY_STATIC)
    set(BAT_FOUND true)
else(BAT_INCLUDE_DIR AND BAT_LIBRARY_SHARED AND BAT_LIBRARY_STATIC)
    set(BAT_FOUND false)
endif(BAT_INCLUDE_DIR AND BAT_LIBRARY_SHARED AND BAT_LIBRARY_STATIC)

#check if all components are there
if(BAT_LIBRARY_MODELS_FOUND AND BAT_LIBRARY_MTF_FOUND AND BAT_LIBRARY_MVC_FOUND)
    set(BAT_LIBRARY_COMPONENTS_FOUND true)
else(BAT_INCLUDE_DIR AND BAT_LIBRARY_SHARED AND BAT_LIBRARY_STATIC)
    set(BAT_LIBRARY_COMPONENTS_FOUND false)
endif(BAT_LIBRARY_MODELS_FOUND AND BAT_LIBRARY_MTF_FOUND AND BAT_LIBRARY_MVC_FOUND)


if(BAT_FOUND)
    add_definitions(-DBAT)
    #set(BAT_LIBRARIES "${BAT_LIBRARY_STATIC};${BAT_LIBRARY_SHARED}")
    if(NOT BAT_FIND_QUIETLY)
      message(STATUS "Looking for BAT... ")
      message(STATUS "- Include dir : ${BAT_INCLUDE_DIR}")
      message(STATUS "- Library dir : ${BAT_LIBRARY_DIR}")
      message(STATUS "Include the following component(s) : ")
      message(STATUS "* main BAT library :")
      message(STATUS "  --> ${BAT_LIBRARY_SHARED}")
      message(STATUS "  --> ${BAT_LIBRARY_STATIC}")
    endif(NOT BAT_FIND_QUIETLY)
else(BAT_FOUND)
    if(NOT BAT_FIND_QUIETLY)
      if(BAT_FIND_REQUIRED)
        message(FATAL_ERROR "Looking for BAT... - Not found")
      else(BAT_FIND_REQUIRED)
        message(STATUS "Looking for BAT... - Not found")
      endif(BAT_FIND_REQUIRED)
    endif(NOT BAT_FIND_QUIETLY)
endif(BAT_FOUND)

if(BAT_FOUND)
    set(BAT_LIBRARIES_SHARED "${BAT_LIBRARY_SHARED}")
    set(BAT_LIBRARIES_STATIC "${BAT_LIBRARY_STATIC}")
endif(BAT_FOUND)



FOREACH(COMPONENT  ${BAT_FIND_COMPONENTS} )
        STRING( TOUPPER ${COMPONENT} UPPERCOMPONENT )
        if ( BAT_LIBRARY_${UPPERCOMPONENT}_FOUND )
          if (NOT BAT_FIND_QUIETLY)
            message(STATUS "* ${COMPONENT} :")
            message(STATUS "  -> ${BAT_LIBRARY_${UPPERCOMPONENT}_SHARED}")
            message(STATUS "  -> ${BAT_LIBRARY_${UPPERCOMPONENT}_STATIC}")
          endif(NOT BAT_FIND_QUIETLY)
          set(BAT_LIBRARIES_SHARED ${BAT_LIBRARIES_SHARED} ${BAT_LIBRARY_${UPPERCOMPONENT}_SHARED})
          set(BAT_LIBRARIES_STATIC ${BAT_LIBRARIES_STATIC} ${BAT_LIBRARY_${UPPERCOMPONENT}_STATIC})
        else(BAT_LIBRARY_${UPPERCOMPONENT}_FOUND)
          #message(FATAL_ERROR "- ${COMPONENT} not found in ${BAT_LIBRARY_DIR}")
          message(STATUS "- ${COMPONENT} not found in ${BAT_LIBRARY_DIR}")
        endif(BAT_LIBRARY_${UPPERCOMPONENT}_FOUND) 
ENDFOREACH(COMPONENT)
message(STATUS "BAT_LIBRARIES_SHARED (1st call) is = ${BAT_LIBRARIES_SHARED}")



mark_as_advanced(BAT_INCLUDE_DIR BAT_LIBRARY_DIR BAT_LIBRARIES_SHARED BAT_LIBRARIES_STATIC)

