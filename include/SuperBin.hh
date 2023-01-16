#ifndef SUPERBIN_
#define SUPERBIN_

#include <iostream>

#include "FitReader.hh"
#include "CategoryTree.hh"

#include <TCanvas.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TLatex.h>
#include <TLine.h>

class VI : public std::vector<int>{
public:
  VI();
  VI(const int i);
  VI(const std::vector<int> vi);
  virtual ~VI();

  VI& operator += (int i);
  VI& operator += (const vector<int> vi);

  void printList();

};

class SuperBin{

public:
  SuperBin(const VI index, const double nSig, const double nBkg, const VS mPerpLabel, const VS rIsrLabel, const VS visLabel);
  virtual ~SuperBin();

  VI getIndex() const;
  double getNsig() const;
  double getNbkg() const;
  double getSoverB() const;
  double getBinZbi(const double sys) const;

  VS getMperpLabel() const;
  VS getRisrLabel() const;
  VS getVisLabel() const;

  SuperBin* tryMerge(SuperBin* superBin, double sys);

private:

  VI index_;
  double nSig_;
  double nBkg_;
  double sOverB_;
  VS mPerpLabel_;
  VS rIsrLabel_;
  VS visLabel_;

};

class SuperBinList : public std::vector<SuperBin*>{

public:
  SuperBinList();
  SuperBinList(const std::vector<SuperBin*>& superBinList);
  virtual ~SuperBinList();

  SuperBinList& operator += (SuperBin* superBin);
  SuperBinList& operator += (const SuperBinList& superBin); 

  void PrintSummary(const double sys);
  void SetIdentifier(const string ID);
  void sortByZbi(const double sys);
  void sortBySoverB();
  void PlotListZbi(const string name, const double sys);
  void PlotListZbiMR(const string name, const double sys, const FitBin& fitbin);

  double GetMaxZbi(const double sys);

  SuperBin* GetMaxZbiBin(const double sys);
  SuperBin* MergeBins(const double sys);

private:
  int listSize_;

  string identifier_ = "";

};

#endif
