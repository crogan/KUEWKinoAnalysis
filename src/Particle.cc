#include "Particle.hh"
#include "ParticleList.hh"

Particle::Particle() : TLorentzVector() {
  m_Charge = 0;
}
    
Particle::~Particle() {}

int Particle::Charge() const {
  return m_Charge;
}

void Particle::SetCharge(int charge){
  m_Charge = charge;
}
    
Particle::operator ParticleList() const {
  ParticleList list;
  list.push_back(*this);
  return list;
}

ParticleList Particle::operator + (const Particle& part) const {
  ParticleList list;
  list.push_back(*this);
  list.push_back(part);
  return list;
}

ParticleList Particle::operator + (const ParticleList& parts) const {
  ParticleList list = parts;
  list.push_back(*this);
  return list;
}

