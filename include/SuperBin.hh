#ifndef SuperBin_
#define SuperBin_

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
    explicit VI(int i);
    explicit VI(const std::vector<int>& vi);
    virtual ~VI();

    VI& operator += (int i);
    VI& operator += (const vector<int>& vi);

    void printList();

};

class SuperBin{

public:
    SuperBin(const VI& index,
             double nSig,
             double nBkg,
             const VS& mPerpLabel,
             const VS& rIsrLabel,
             const VS& visLabel,
             const double bkgErr = 0.,
             const double sigErr = 0.);
    virtual ~SuperBin();

    VI getIndex() const;
    double GetNsig() const;
    double GetSigErr() const;
    double GetNbkg() const;
    double GetBkgErr() const;
    double GetSoverB() const;
    double GetBinZbi(double sys) const;

    VS GetMperpLabel() const;
    VS GetRisrLabel() const;
    VS GetVisLabel() const;

    void PrintSummary(double sys) const;

    SuperBin* TryMerge(SuperBin* superBin, double sys) const;
    SuperBin* ForceMerge(SuperBin* superBin, double sys) const;

private:

    VI index_;
    double nSig_;
    double sigErr_;
    double nBkg_;
    double bkgErr_;
    //double nData_;
    double sOverB_;
    VS mPerpLabel_;
    VS rIsrLabel_;
    VS visLabel_;
};

class SuperBinList : public std::vector<SuperBin*>{

public:
    SuperBinList();
    explicit SuperBinList(const std::vector<SuperBin*>& superBinList);
    virtual ~SuperBinList();

    SuperBinList& operator += (SuperBin* superBin);
    SuperBinList& operator += (const SuperBinList& superBin);

    void PrintSummary(double sys, bool concise = false);
    void PrintSummaryVis(double sys, bool concise = false);
    void PrintSummaryExplicit(double sys, bool concise = false);
    void SetIdentifier(const string& ID);
    void sortByZbi(double sys);
    void sortBySoverB();
    void PlotListZbi(const string& name, double sys);
    void PlotListZbiMR(const string& name, double sys, const FitBin& fitbin);

    std::vector<int> sortIndexByZbi(double sys) const;

    double GetMaxZbi(double sys);

    SuperBin* GetMaxZbiBin(double sys);
    SuperBin* MergeBins(double sys);
    SuperBin* ForceMergeBins();

private:
    int listSize_;

    bool isSorted_;

    string identifier_;

};

#endif
