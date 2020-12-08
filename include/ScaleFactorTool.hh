#ifndef ScaleFactorTool_HH
#define ScaleFactorTool_HH

#include <iostream>
#include <vector>

///////////////////////////////////////////
////////// ScaleFactorTool class
///////////////////////////////////////////

class ScaleFactorTool {
public:
  ScaleFactorTool() {}
    
  virtual ~ScaleFactorTool() {}

  double GetX20BRSF(double MX2, double MX1);

  double GetMETEff(double MET, int updown = 0);
  double GetMETSF(double MET, int updown = 0);

};

#endif
