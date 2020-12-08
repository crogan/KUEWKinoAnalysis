#include "../include/ScaleFactorTool.hh"

#include "Math/DistFunc.h"

///////////////////////////////////////////
////////// ScaleFactorTool class
///////////////////////////////////////////

double ScaleFactorTool::GetX20BRSF(double MX2, double MX1){

  if(MX2-MX1 < 5.)
    return 1.;

  else
    return 1. + 0.46*ROOT::Math::lognormal_pdf(((MX2-MX1)-5.)/30., 0., 1.1);
  
}

double ScaleFactorTool::GetMETEff(double MET, int updown){
  if(MET <= 0.)
    return 0.;

  double nom = 9.921*ROOT::Math::normal_cdf(MET, 77.5, 78.57);
  
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
