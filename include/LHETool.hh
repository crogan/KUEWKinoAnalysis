#ifndef LHETool_h
#define LHETool_h

#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <math.h>
#ifdef _CMSSW_
#include "LHAPDF/LHAPDF.h"
#endif

// may need to run from command line:
// export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/lhapdf/6.2.1-pafccj3/lib

class LHETool {
  
public:
  LHETool(){
  }
  
  virtual ~LHETool(){
  }


  double GetWeight(unsigned int nLHEPdfWeight, float* LHEPdfWeight, int Generator_id1, int Generator_id2, float Generator_x1, float Generator_x2, float Generator_scalePDF, int year, int updown);
  void GetFromLHAPDF(int& nPDFWeights, std::vector<double>& PDFWeights, int id1, int id2, float x1, float x2, float scalePDF, int year);
  void AddLHAPDF(int year);

private:

#ifdef _CMSSW_
  LHAPDF::PDFSet pdfset;
  std::vector<LHAPDF::PDF*> pdfs;
#endif

};

#endif






