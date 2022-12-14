#include "../include/ScaleFactorTool.hh"
#include "ReducedBase.hh"

#include "Math/DistFunc.h"

///////////////////////////////////////////
////////// ScaleFactorTool class
///////////////////////////////////////////

double ScaleFactorTool::GetX20BRSF(const string& filename, const string& treename){

  if(filename.find("TChiWZ") == string::npos ||
     filename.find("ZToLL") == string::npos)
    return 1.;
  
   int MP, MC;
  
  sscanf(treename.c_str(), "SMS_%d_%d", &MP, &MC);
  
  if(MP-MC < 5)
    return 1.;

  else
    return 1. + 0.46*ROOT::Math::lognormal_pdf(double((MP-MC)-5)/30., 0., 1.1);
  
}

double ScaleFactorTool::GetMETEff(double MET, int updown){
  if(MET <= 0.)
    return 0.;

  double nom = 0.9921*ROOT::Math::normal_cdf(MET, 77.5, 78.57);
  
  if(updown == 0)
    return nom;
  if(updown > 0)
    return std::min(1., nom*(1.01 + (MET < 300. ? 0.2e-5*(MET-300.)*(MET-300.) : 0.)));
  if(updown < 0)
    return std::min(1., nom*(0.99 + (MET < 300. ? -0.2e-5*(MET-300.)*(MET-300.) : 0.)));

  return 1.;
}

double ScaleFactorTool::GetMETSF(double MET, int updown){
  if(MET <= 0.)
    return 1.;

  double nom = 0.9921*ROOT::Math::normal_cdf(MET, 77.5, 78.57);
  nom /= 0.9974*ROOT::Math::normal_cdf(MET, 79.22, 65.05);

  if(updown == 0)
    return nom;
  if(updown > 0)
    return nom*(1.01 + (MET < 300. ? 0.2e-5*(MET-300.)*(MET-300.) : 0.));
  if(updown < 0)
    return nom*(0.99 + (MET < 300. ? -0.2e-5*(MET-300.)*(MET-300.) : 0.));

  return 1.;
}

bool ScaleFactorTool::DileptonEvent(ReducedBase* base){

  int NWnu = 0;
  for(int i = 0; i < base->genNnu; i++)
    if(fabs(base->genMomPDGID_nu->at(i)) == 24)
      NWnu++;

  if(NWnu >= 2)
    return true;

  int NZlep = 0;
  for(int i = 0; i < base->genNlep; i++)
    if(fabs(base->genMomPDGID_lep->at(i)) == 23)
      NZlep++;

  if(NZlep >= 2)
    return true;

  return false;

}

void ScaleFactorTool::AddBtagFolder(const string& btagfold){
  m_BtagSFTool.BuildMap(btagfold);
}  

double ScaleFactorTool::GetBtagSFWeight(ReducedBase* base, int year, bool fastsim, bool HForLF, int updown, ParticleIDType tag){
 
  int Njet = base->Njet;
  int iflavor;
  double EFF, SF, PT;

  double probMC   = 1.;
  double probDATA = 1.;
  
  for(int i = 0; i < Njet; i++){
    if(abs(base->Flavor_jet->at(i)) == 5)
      iflavor = 0;
    else if(abs(base->Flavor_jet->at(i)) == 4)
      iflavor = 1;
    else
      iflavor = 2;


   
    if(HForLF && iflavor == 2)
      continue;
    if(!HForLF && iflavor != 2)
      continue;

    PT = base->PT_jet->at(i);
    
    EFF = m_BtagSFTool.EFF(PT, year, iflavor, fastsim);
    SF  = m_BtagSFTool.SF(PT, year, iflavor, updown);

//    std::cout << "year: "<< year<< " fastsim: " << fastsim << " HForLF: " << HForLF << " updown: "<< updown <<"\n";
//    std::cout << "iflavor:" <<iflavor << " PT:  " << PT << " EFF: " << EFF << " SF: " << SF << " base:  " << base->BtagID_jet->at(i) << std::endl;
    if(fastsim)
      SF *= m_BtagSFTool.SF(PT, year, iflavor, updown, fastsim);

    if(base->BtagID_jet->at(i) >= tag){
      probMC   *= EFF;
      probDATA *= SF*EFF;
    } else {
      probMC   *= (1.-EFF);
      probDATA *= (1.-SF*EFF);
    }
  }

  if(probMC <= 0. || probDATA <= 0.)
    return 1.;
 
  return probDATA/probMC;

}
