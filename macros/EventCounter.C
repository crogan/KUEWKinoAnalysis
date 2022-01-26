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

void EventCounter(string filetag = "", string dataset = "", int fileline = 0, double weight = 0.0){
  
  RestFrames::SetStyle();

    TChain* chain = new TChain("Events");
    string inputfile_name = load_file_from_list(dataset+".txt",fileline);
    chain->Add(inputfile_name.c_str());

      SUSYNANOBase* base = new SUSYNANOBase(chain);
      int Nentry = base->fChain->GetEntries();
      int Nevent = 0;
      double Nweight = 0.;
      
      int SKIP = 1;
      
      // event loop
      for(int e = 0; e < Nentry; e += SKIP){
	base->GetEntry(e);
	
	if((e/SKIP)%(std::max(1, int(Nentry/SKIP/10))) == 0)
	  cout << "      event " << e << " | " << Nentry << endl;

         Nevent++;
         Nweight += base->genWeight;

      }
      
      delete base;
      delete chain;
     
      ofstream file;
      file.open("EventCount_Plot_1D_NANO_"+filetag+"_"+dataset+"_"+std::to_string(fileline)+".txt");
      file << Nevent << endl;
      file << Nweight << endl;
       file.close(); 
       gApplication->Terminate(0);
}

