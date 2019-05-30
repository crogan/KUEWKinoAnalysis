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

ParticleList ParticleList::PtEtaCut(double pt, double eta) const {
  ParticleList list;
  int N = this->size();
  for(int i = 0; i < N; i++)
    if(this->at(i).Pt() >= pt &&
       (eta <= 0. || fabs(this->at(i).Eta()) <= eta))
      list.push_back(this->at(i));

  return list;
}

ParticleList ParticleList::ParticleIDCut(ParticleIDType id) const {
  ParticleList list;
  int N = this->size();
  for(int i = 0; i < N; i++)
    if(this->at(i).ParticleID() >= id)
      list.push_back(this->at(i));

  return list;
}

ParticleList& ParticleList::SortByPt(){
  sort(this->begin(),this->end(),sortbypt);
  return *this;
}

ParticleList ParticleList::operator + (const ParticleList& parts) const {
  ParticleList l1 = *this;
  ParticleList l2 = parts;
  int N = l2.size();
  for(int i = 0; i < N; i++)
    l1.push_back(l2[i]);
  return l1;
}

void ParticleList::operator += (const ParticleList& parts){
  ParticleList l2 = parts;
  int N = l2.size();
  for(int i = 0; i < N; i++)
    (*this).push_back(l2[i]);
}
