MAIN_DIR  = ${GO_DIR}/SIDSRoot/OnlineAnalysis/AutomaticAnalysis
PACKAGE   = PVWAVE2ESRIO
WAVE_EXE  = ${WAVE_DIR}/bin/bin.linux64/wave
SED_EXE   = sed
C_DIR     = c
SO_DIR    = c
SOLIB     = ${SO_DIR}/${PACKAGE}.so
ESRIOLIB  = ${SO_DIR}/EsrIO.so
makeall_profile = ${PACKAGE}makeall.pro
init_infile     = ac-stuff/${PACKAGE}init.in
init_profile    = ${PACKAGE}init.pro

CXX = $(shell root-config --cxx)
LD = $(shell root-config --ld)
CINT = rootcint

OS_NAME:=$(shell uname -s | tr A-Z a-z)
ifeq ($(OS_NAME),darwin)
STDINCDIR := /opt/local/include
STDLIBDIR := /opt/local/lib
else
STDINCDIR := -I${C_DIR} -I${ESRIO} -I${ESRIO}/data -I${ESRIO}/SubTask -I${SIMPATH}/include
STDLIBDIR := 
endif

CPPFLAGS := $(shell root-config --cflags) -I$(STDINCDIR)
LDFLAGS := $(shell root-config --glibs) -L$(STDLIBDIR)

CPPFLAGS += -g -fPIC -std=c++11
LDFLAGS += -lgsl -lgslcblas -lfftw3 -lm

TARGET = ${SOLIB}

SRC = ${C_DIR}/esrio_cw.c

esrio_srcfiles = \
        ${ESRIO}/data/EsrSMSEvent.cxx \
        ${ESRIO}/data/EsrDecayEvent.cxx \
        ${ESRIO}/data/EsrBinDecayEvent.cxx \
        ${ESRIO}/data/EsrInjData.cxx \
        ${ESRIO}/SubTask/EsrTree.cxx \
	${ESRIO}/SidsVisualAnaDict.cxx
        #${C_DIR}/esrio_dict.cxx
esrio_hfiles = \
        ${ESRIO}/data/EsrSMSEvent.h \
        ${ESRIO}/data/EsrDecayEvent.h \
        ${ESRIO}/data/EsrBinDecayEvent.h \
        ${ESRIO}/data/EsrInjData.h \
        ${ESRIO}/SubTask/EsrTree.h \
	${ESRIO}/SidsVisualAnaDict.h
        
esrio_objfiles = $(esrio_srcfiles:.cxx=.o)

OBJ = $(SRC:.c=.o)

all : esriolib $(TARGET) initfile cprfiles

%.o : %.c
	$(CXX) $(CPPFLAGS) -o $@ -c $< \
	${STDINCDIR}

%.o : %.cxx
	$(CXX) $(CPPFLAGS) -o $@ -c $< \
	${STDINCDIR}

#${C_DIR}/esrio_dict.cxx : $(esrio_hfiles) ${C_DIR}/esrio_linkdef.h 
#	$(CINT) -f ${C_DIR}/esrio_dict.cxx -c $(esrio_hfiles) ${C_DIR}/esrio_linkdef.h


${ESRIO}/SidsVisualAnaDict.cxx : $(esrio_hfiles) ${ESRIO}/SidsVisualAnaLinkDef.h 
	$(CINT) -f ${ESRIO}/SidsVisualAnaDict.cxx -c $(esrio_hfiles) ${ESRIO}/SidsVisualAnaLinkDef.h

$(TARGET) : $(OBJ)
	$(LD) $(CPPFLAGS) -shared -o ${SOLIB} $(OBJ) ${esrio_objfiles} $(LDFLAGS)

esriolib : ${esrio_objfiles}
	$(LD) $(CPPFLAGS) -shared -o ${ESRIOLIB} ${esrio_objfiles}
        
initfile : \
	${MAIN_DIR}/${init_infile}
	  @echo ""
	  @echo " ******************** Preparing ${init_profile}"
	  @echo ""
	  cd ${MAIN_DIR}; \
	  ${SED_EXE} "s?${initpro}, xxx?${initpro}, '${MAIN_DIR}'?g" \
	    ${init_infile} > ${init_profile}

cprfiles : \
	${makeall_profile}
	  @echo ""
	  @echo " ******************** Preparing PV-WAVE files"
	  @echo ""
	  cd ${MAIN_DIR}; \
	  unset WAVE_STARTUP; ${WAVE_EXE} ${makeall_profile}

clean :
	rm -f ${C_DIR}/*.o $(TARGET) $(ESRIOLIB) \
        ${esrio_objfiles} ${C_DIR}/esrio_dict.* \
        *~
