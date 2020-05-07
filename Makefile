ROOTCFLAGS    = $(shell root-config --cflags)
ROOTGLIBS     = $(shell root-config --glibs)

RFCFLAGS    = $(shell restframes-config --cxxflags)
RFGLIBS     = $(shell restframes-config --libs)

CXX            = g++

#CXXFLAGS       = -fPIC -Wall -O3 -g
CXXFLAGS       = -fPIC $(filter-out -stdlib=libc++ -pthread , $(ROOTCFLAGS))
CXXFLAGS       += $(filter-out -stdlib=libc++ -pthread , $(RFCFLAGS))

GLIBS          = $(filter-out -stdlib=libc++ -pthread , $(ROOTGLIBS))
GLIBS         += $(filter-out -stdlib=libc++ -pthread , $(RFGLIBS))
GLIBS         += -lRooFit -lRooFitCore

INCLUDEDIR       = ./include/
SRCDIR           = ./src/
CXX	         += -I$(INCLUDEDIR) -I.
OUTOBJ	         = ./obj/

CC_FILES := $(wildcard src/*.cc)
HH_FILES := $(wildcard include/*.hh)
OBJ_FILES := $(addprefix $(OUTOBJ),$(notdir $(CC_FILES:.cc=.o)))

SOBJ_FILES = $(filter-out AnalysisBase.o SVDiscrTool.o ReducedNtuple.o NtupleBase.o, $(OBJ_FILES))

all : GLIBS += -L/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/lwtnn/2.4-gnimlf3/lib -llwtnn
all : CXX += -I/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/lwtnn/2.4-gnimlf3/include/

local : GLIBS += -L/Users/christopherrogan/GitHub/lwtnn/lib -llwtnn
local : CXX += -I/Users/christopherrogan/GitHub/lwtnn/include

cmssw : GLIBS += -L../../lib/slc7_amd64_gcc700 -lCombineHarvesterCombinePdfs -lHiggsAnalysisCombinedLimit -lCombineHarvesterCombineTools
cmssw : GLIBS += -L/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/lwtnn/2.4-gnimlf3/lib -llwtnn
cmssw : GLIBS += -L/cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_6_5/external/slc7_amd64_gcc700/lib/ -lvdt -lboost_program_options -lboost_filesystem -lboost_regex -lboost_system

cmssw : CXX   += -I../. -I/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/boost/1.67.0/include/
cmssw : CXX   += -I/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/lwtnn/2.4-gnimlf3/include/

locallib : GLIBS += -L/Users/christopherrogan/GitHub/lwtnn/lib -llwtnn
locallib : CXX += -I/Users/christopherrogan/GitHub/lwtnn/include

all: alltargets lib

cmssw: alltargets lib BuildFit.x

local: alltargets lib

locallib: lib

lib: lib/libKUEWKino.so

alltargets: MakeReducedNtuple_NANO.x MakeEventCount_NANO.x BuildFitInput.x

BuildFit.x:  $(SRCDIR)BuildFit.C $(OBJ_FILES) $(HH_FILES)
	$(CXX) $(CXXFLAGS) -o BuildFit.x $(OUTOBJ)/*.o $(GLIBS) $ $<
	touch BuildFit.x

MakeReducedNtuple.x:  $(SRCDIR)MakeReducedNtuple.C $(OBJ_FILES) $(HH_FILES)
	$(CXX) $(CXXFLAGS) -o MakeReducedNtuple.x $(OUTOBJ)/*.o $(GLIBS) $ $<
	touch MakeReducedNtuple.x

MakeEventCount.x:  $(SRCDIR)MakeEventCount.C $(OBJ_FILES) $(HH_FILES)
	$(CXX) $(CXXFLAGS) -o MakeEventCount.x $(OUTOBJ)/*.o $(GLIBS) $ $<
	touch MakeEventCount.x

MakeReducedNtuple_NANO.x:  $(SRCDIR)MakeReducedNtuple_NANO.C $(OBJ_FILES) $(HH_FILES)
	$(CXX) $(CXXFLAGS) -o MakeReducedNtuple_NANO.x $(OUTOBJ)/*.o $(GLIBS) $ $<
	touch MakeReducedNtuple_NANO.x

MakeEventCount_NANO.x:  $(SRCDIR)MakeEventCount_NANO.C $(OBJ_FILES) $(HH_FILES)
	$(CXX) $(CXXFLAGS) -o MakeEventCount_NANO.x $(OUTOBJ)/*.o $(GLIBS) $ $<
	touch MakeEventCount_NANO.x

BuildFitInput.x:  $(SRCDIR)BuildFitInput.C $(OBJ_FILES) $(HH_FILES)
	$(CXX) $(CXXFLAGS) -o BuildFitInput.x $(OUTOBJ)/*.o $(GLIBS) $ $<
	touch BuildFitInput.x

lib/libKUEWKino.so: $(SOBJ_FILES)
	mkdir -p lib
	$(CXX) -shared -o lib/libKUEWKino.so $(OUTOBJ)/*.o $(GLIBS)
	touch lib/libKUEWKino.so

$(OUTOBJ)%.o: src/%.cc include/%.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OUTOBJ)*.o 
	rm -f *.x
	rm -f AutoDict*
	rm -f lib/libKUEWKino.so
