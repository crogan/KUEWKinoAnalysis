#ifndef _EBANKF
#define _EBANKF

#include <iostream>
#include "TCanvas.h"
#include "TF1.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TFitter.h"

#include "E_bank.hh"

class E_bank_fit : public E_bank {

public:
  //special function for extrapolation
  void doLowPtFit(double fitLow, double fitUp, double split_threshold, std::string ffit16, std::string ffit17, std::string ffit18, std::string histPath);
  //if we dont need extrapolation, dont call the function and leave these empty (easier than making a special child class)
  std::map<std::pair<double,double>, std::pair<double,double> > _fitmap16;
  std::map<std::pair<double,double>, std::pair<double,double> > _fitmap17;
  std::map<std::pair<double,double>, std::pair<double,double> > _fitmap18;
  double _split_threshold;
	
  std::vector<double> _syst_low;//low pt systematic

  std::vector<double> _syst_low16;
  std::vector<double> _syst_low17;
  std::vector<double> _syst_low18;

  //overwrite parent members to deal with split threshold
  std::pair<double,double> getPair(double pt, double eta, int year = 0);
  double getEfficiency(double pt, double eta, int year = 0);
  double getError(double pt, double eta, int year = 0);

  void extractFit( TCanvas* hCanv, double fitLow, double fitUp, std::map<std::pair<double,double>, std::pair<double,double> >& _fmap, int year=0);

  E_bank_fit(int year, std::string f16, std::string f17, std::string f18, std::string histPath);

  //void applySystematicsLow(std::vector<double> errors, int year);
  void setSystematicsLow(std::vector<double> errors, int year = 0);

};

#endif
