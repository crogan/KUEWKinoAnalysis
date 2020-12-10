#include <TChain.h>
#include "include/ReducedBase_slim.hh"
using namespace std;
void avgPt(){
TString path = "/home/t3-ku/crogan/NTUPLES/NANO/NEW_31_05_20/Fall17_102X/";
TChain* chain = new TChain("KUAnalysis");
chain->Add(path+"DY*.root");
ReducedBase* base = new ReducedBase(chain);
TH1D* ptAvg = new TH1D("ptAvg","ptAvg",1000,0,4000);
if(base->fChain->GetEntries() < 1) gApplication->Terminate();

for(int i = 0; i < base->fChain->GetEntries(); i++){
base->GetEntry(i);
float avg = 0;
if(base->Nlep < 1) continue;
for(int j = 0; j < base->Nlep; j++){
avg += base->PT_lep->at(j);
}
avg /= base->Nlep;
ptAvg->Fill(avg);
}
cout << "mean: " << ptAvg->GetMean() << endl;
cout << "max bin: " << ptAvg->GetMaximumBin() << endl;
cout << "max bin content: " << ptAvg->GetBinContent(ptAvg->GetMaximumBin()) << endl;
cout << "max bin edges: " << ptAvg->GetBinLowEdge(ptAvg->GetMaximumBin()) <<  " to " << ptAvg->GetBinLowEdge(ptAvg->GetMaximumBin())+4 << endl;
cout << "max bin center: " << ptAvg->GetBinCenter(ptAvg->GetMaximumBin()) << endl;
cout << "\n" << endl;
cout << "Looking at bins: 500 to 1000, bin edges: " << ptAvg->GetBinLowEdge(500) << " to " << ptAvg->GetBinLowEdge(1000)+4 << endl; 
cout << "Integral over these bins: " << ptAvg->Integral(500,1000) << endl;
ptAvg->Draw();

}
