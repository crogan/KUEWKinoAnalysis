#ifndef ParticleList_HH
#define ParticleList_HH

#include <vector>
#include "TLorentzVector.h"
#include "Particle.hh"
  
class ParticleList : public std::vector<Particle> {
public:
  ParticleList();
  
  virtual ~ParticleList();
  
  operator TLorentzVector() const;

  int Charge() const;
};

  

#endif
