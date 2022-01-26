#include <iostream>
#include <string>
#include <fstream>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TColor.h>
#include <TColorWheel.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TLorentzVector.h>
#include <TApplication.h>

#include "SUSYNANOBase_slim.hh"

#include "RestFrames/RestFrames.hh"

using namespace std;


string g_PlotTitle;
string g_Xname;
double g_Xmin;
double g_Xmax;
double g_NX;

using namespace RestFrames;

string load_file_from_list(string fileListName, int line_number)
{
 std::ifstream infile(fileListName);
 string line = "";
 int counter = 0;
 while(getline(infile,line))
 {
  if(counter == line_number) return line;
  counter++;
 }
 return line;
}

void Condor_Plot_1D_NANO(string filetag = "", string dataset = "", int fileline = 0, double weight = 0.0){
  
  RestFrames::SetStyle();

  string g_Label = "";
  g_Xname = "HT  [GeV]";

  g_Xmin = 0.;
  g_Xmax = 3000.; 
  g_NX = 150;

  g_PlotTitle = "";

    TH1D* hist = new TH1D(("hist_"+filetag+"_"+dataset+"_"+std::to_string(fileline)).c_str(), ("hist_"+filetag+"_"+dataset+"_"+std::to_string(fileline)).c_str(), g_NX, g_Xmin, g_Xmax);
    TChain* chain = new TChain("Events");
    string inputfile_name = load_file_from_list(dataset+".txt",fileline);
    chain->Add(inputfile_name.c_str());

      SUSYNANOBase* base = new SUSYNANOBase(chain);
      int Nentry = base->fChain->GetEntries();
      
      int SKIP = 1;
      
      // event loop
      for(int e = 0; e < Nentry; e += SKIP){
	base->GetEntry(e);
	
	if((e/SKIP)%(std::max(1, int(Nentry/SKIP/10))) == 0)
	  cout << "      event " << e << " | " << Nentry << endl;

        //double Gen_HT = 0.;
        //for(int g = 0; g < int(base->nGenPart); g++)
        //{
        // if(base->GenPart_status[g] == 23)
        // {
        //  Gen_HT += base->GenPart_pt[g];
        // }
        //}	
	//hist->Fill(Gen_HT, base->genWeight*weight*double(SKIP)/1.e7);
	//hist->Fill(base->genWeight);
	hist->Fill(base->LHE_HT, base->genWeight*weight*double(SKIP)/1.e7);
      }
      
      delete base;
      delete chain;
     
       TFile* output_file = new TFile(("output_Plot_1D_NANO_"+filetag+"_"+dataset+"_"+std::to_string(fileline)+".root").c_str(),"RECREATE");
       hist->Write();
       output_file->Close(); 
       gApplication->Terminate(0);
}

