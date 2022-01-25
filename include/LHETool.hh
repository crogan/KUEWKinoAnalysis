#ifndef LHETool_h
#define LHETool_h



class LHETool {
  
public:
  LHETool(){
  }
  
  virtual ~LHETool(){
  }


  double GetWeight(unsigned int nLHEPdfWeight, float* LHEPdfWeight, int updown);

private:
};

#endif






