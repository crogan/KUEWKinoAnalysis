#ifndef _EBANK
#define _EBANK

#include<vector>
#include<map>
#include <utility>
#include<set>

#include "TFile.h"
#include "TCanvas.h"
#include <iostream>
#include "TList.h"
#include "TH2D.h"

class E_bank {

public:
  //std::vector<double> _binEdge;
  //first pair is key of pt(first) eta(second).  second pair is value of efficiency(first)	
  std::map<std::pair<double,double>, std::pair<double,double> > _map16;
  std::map<std::pair<double,double>, std::pair<double,double> > _map17;
  std::map<std::pair<double,double>, std::pair<double,double> > _map18;
	
	

  //std::pair<double,double> getPair(std::pair<double,double> key, int year);
  //double getEfficiency(std::pair<double,double> key, int year);
  //double getError(std::pair<double,double> key, int year);
	
  //double arg overloads
  std::pair<double,double> getPair(double pt, double eta, int year = 0);
  double getEfficiency(double pt, double eta, int year = 0);
  double getError(double pt, double eta, int year = 0);
  std::pair<double,double> transformValue(double pt, double eta, std::map<std::pair<double,double>, std::pair<double,double> >& map );

  void printMap(std::map<std::pair<double,double>, std::pair<double,double> >& map);
	
  int _year;
  void setYear(int year);

  double _syst0;
  std::vector<double> _syst;// systematics are pt inclusive, elements of syst vector are the eta bins, needs to be ordered in increasing eta
  double _ptUpperEdge;
  double _etaUpperEdge;


  E_bank();
  E_bank(int year, std::string f16, std::string f17, std::string f18, std::string histPath);
  void populateMap(TCanvas* hCanv, std::map<std::pair<double,double>, std::pair<double,double> >& map,  int CanvIdx = 1);
  void applySystematics(double error, int year = 0);
  void applySystematics(std::vector<double> errors, int year = 0);
  void applySystematic_ptRange(double ptLow, double ptHigh, std::vector<double>errors, int year = 0);
  void addError(double error, std::map<std::pair<double,double>, std::pair<double,double> >& map);//keeping this simple function in to add a flat systematic
  void addError(std::vector<double> errors, std::map<std::pair<double,double>,std::pair<double,double> >& map);
  void addError(double ptLow, double ptHigh ,std::vector<double> errors, std::map<std::pair<double,double>, std::pair<double,double> >& map);
};
//default constructor

#endif
