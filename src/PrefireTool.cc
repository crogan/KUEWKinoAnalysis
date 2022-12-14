#include "../include/PrefireTool.hh"

#include <iostream>
#include <string>
#include <TFile.h>

PrefireTool::~PrefireTool(){
}

PrefireTool::PrefireTool(int year, bool UseEMpT, std::string rootfile){

 m_year = year;
 m_UseEMpT = UseEMpT;
 
 TFile* r_file = TFile::Open(rootfile.c_str());
 
 if(m_year == 2016){
   m_photonmap = *(TH2F*)r_file->Get("L1prefiring_photonpt_2016BtoH");
   if(m_UseEMpT){
     m_jetmap = *(TH2F*)r_file->Get("L1prefiring_jetempt_2016BtoH");
   }
   else{
     m_jetmap = *(TH2F*)r_file->Get("L1prefiring_jetpt_2016BtoH");
   }
 }
 else if(m_year == 2017 || m_year == 2018){
   m_photonmap = *(TH2F*)r_file->Get("L1prefiring_photonpt_2017BtoF");
   if(m_UseEMpT){
     m_jetmap = *(TH2F*)r_file->Get("L1prefiring_jetempt_2017BtoF");
   }
   else{
     m_jetmap = *(TH2F*)r_file->Get("L1prefiring_jetpt_2017BtoF");
   }
 }
 else
   std::cout << "Input wrong year for prefire tool!" << std::endl;

 
 m_jetmap.SetDirectory(0);
 m_photonmap.SetDirectory(0);
 r_file->Close();

}

double PrefireTool::GetPrefireProbability(bool jet, double eta, double pt, double maxpt, int updown){
 TH2F map;
 if(jet)
  map = m_jetmap;
 else
  map = m_photonmap;
 int bin = map.FindBin(eta, std::min(pt, maxpt - 0.01));
 double pref_prob = map.GetBinContent(bin);
 double stat = map.GetBinError(bin);
 double syst = 0.2*pref_prob;

 if(updown == 1){
   pref_prob = std::min(pref_prob + sqrt(stat * stat + syst * syst), 1.);
 }
 else if(updown == -1){
   pref_prob = std::max(pref_prob - sqrt(stat * stat + syst * syst), 0.);
 }
 return pref_prob;
}

bool PrefireTool::Get_UseEMpT(){
 return m_UseEMpT;
}
