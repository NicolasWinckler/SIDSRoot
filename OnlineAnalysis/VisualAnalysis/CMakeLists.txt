 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Create a library called "libFairTestDetector" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${ZMQ_INCLUDE_DIR}
${BASE_INCLUDE_DIRECTORIES}/fairmq
${BASE_INCLUDE_DIRECTORIES}/base/MQ
${SIDS_SOURCE_DIR}/OnlineAnalysis/VisualAnalysis
${SIDS_SOURCE_DIR}/OnlineAnalysis/VisualAnalysis/data
${SIDS_SOURCE_DIR}/OnlineAnalysis/VisualAnalysis/MQTask
#${SIDS_SOURCE_DIR}/OnlineAnalysis/VisualAnalysis/SubTask
${SIDS_SOURCE_DIR}/OnlineAnalysis/VisualAnalysis/MQDevice
${SIDS_SOURCE_DIR}/OnlineAnalysis/VisualAnalysis/GUI
${SIDS_SOURCE_DIR}/time2root
${SIDS_SOURCE_DIR}/OnlineAnalysis/VisualAnalysis/Models
)

include_directories(${INCLUDE_DIRECTORIES})

If (Boost_FOUND)
  configure_file( ${SIDS_SOURCE_DIR}/OnlineAnalysis/VisualAnalysis/macro/startEsrSampler.sh.in ${SIDS_BINARY_DIR}/bin/startEsrSampler.sh )
  configure_file( ${SIDS_SOURCE_DIR}/OnlineAnalysis/VisualAnalysis/macro/startEsrSink.sh.in ${SIDS_BINARY_DIR}/bin/startEsrSink.sh )
  configure_file( ${SIDS_SOURCE_DIR}/OnlineAnalysis/VisualAnalysis/macro/startPushPull_esr.sh.in ${SIDS_BINARY_DIR}/bin/startEsrPushPull.sh )
  configure_file( ${SIDS_SOURCE_DIR}/OnlineAnalysis/VisualAnalysis/macro/startVisualAnalysis.sh.in ${SIDS_BINARY_DIR}/bin/startVisualAnalysis.sh )
  configure_file( ${SIDS_SOURCE_DIR}/OnlineAnalysis/VisualAnalysis/macro/showfiles.sh.in ${SIDS_BINARY_DIR}/bin/showfiles.sh )
  configure_file( ${SIDS_SOURCE_DIR}/OnlineAnalysis/VisualAnalysis/macro/showSummary.sh.in ${SIDS_BINARY_DIR}/bin/showSummary.sh )
EndIf (Boost_FOUND)

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories(${LINK_DIRECTORIES})

Set(SRCS 
    data/EsrSMSEvent.cxx
    data/EsrDecayEvent.cxx
    data/EsrBinDecayEvent.cxx
    data/EsrInjData.cxx
    data/EsrTree.cxx
    GUI/SidsGui.cxx
    GUI/SidsSummary.cxx
    GUI/SidsDecayTxtField.cxx
    GUI/SidsQualityTagField.cxx
    GUI/SidsFitButtons.cxx
    ${SIDS_SOURCE_DIR}/time2root/header.cxx
    Models/SidsRooOscModel.cxx
)

If (Boost_FOUND)
  Set(NO_DICT_SRCS
    data/MQconfig.cxx
    data/SIDSFileManager.cxx
    MQDevice/EsrSidsSampler.cxx
    MQDevice/EsrFileSink.cxx
    MQTask/EsrSamplerTask.cxx
    #Models/RooMypdf.cpp
    #Models/RooMypdfDict.cpp
  )
EndIf (Boost_FOUND)

Set(LINKDEF SidsVisualAnaLinkDef.h)
Set(LIBRARY_NAME SidsVisualAna)

If (Boost_FOUND)
  Set(DEPENDENCIES
    Base ParBase FairTools MCStack boost_thread pthread
    ${FAIRMQ_DEPENDENCIES}
    boost_system boost_serialization boost_filesystem 
    Matrix Tree Hist RIO Cint Core
    Minuit RooFitCore RooFit RooStats Foam MathMore
    Gui Spectrum)
EndIf (Boost_FOUND)

GENERATE_LIBRARY()

If (Boost_FOUND)
  Set(Exe_Names
    RunTest
    EsrMQSamplerBoost
    #EsrMQFileSinkBoost
    ShowFile
    ShowStatus
  )

  set(Exe_Source
    run/runtest.cxx
    run/runEsrSampler.cxx
    #run/runEsrFileSink.cxx
    run/runFileInfo.cxx
    run/runSummary.cxx
    )


  List(LENGTH Exe_Names _length)
  Math(EXPR _length ${_length}-1)

  ForEach(_file RANGE 0 ${_length})
    List(GET Exe_Names ${_file} _name)
    List(GET Exe_Source ${_file} _src)
    Set(EXE_NAME ${_name})
    Set(SRCS ${_src})
    Set(DEPENDENCIES SidsVisualAna)
    GENERATE_EXECUTABLE()
  EndForEach(_file RANGE 0 ${_length})
EndIf (Boost_FOUND)
