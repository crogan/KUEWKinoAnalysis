#ifndef Particle_HH
#define Particle_HH

#include "TLorentzVector.h"

class ParticleList;

class Particle : public TLorentzVector {
public:
  Particle();
    
  virtual ~Particle();

  int Charge() const;
  void SetCharge(int charge);
    
  operator ParticleList() const;
  ParticleList operator + (const Particle& part) const; 
  ParticleList operator + (const ParticleList& parts) const; 
    
private:
  int m_Charge;
    

};


#endif
