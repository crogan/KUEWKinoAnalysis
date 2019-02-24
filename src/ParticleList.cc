#include "ParticleList.hh"

ParticleList::ParticleList() : std::vector<Particle>() {}
  
ParticleList::~ParticleList() {}
  
ParticleList::operator TLorentzVector() const {
  TLorentzVector vec;
  int N = this->size();
  for(int i = 0; i < N; i++)
    vec += this->at(i);
  return vec;
}

int ParticleList::Charge() const {
  int charge = 0;
  int N = this->size();
  for(int i = 0; i < N; i++)
    charge += this->at(i).Charge();
  return charge;
}


