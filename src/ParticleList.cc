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

ParticleList ParticleList::RemoveOverlap(const ParticleList& parts, double deltaR) const {
  ParticleList list;
  int N = this->size();
  int M = parts.size();
  for(int i = 0; i < N; i++){
    bool b_unique = true;
    for(int j = 0; j < M; j++){
      if(this->at(i).DeltaR(parts[j]) < deltaR){
	b_unique = false;
	break;
      }
    }
    if(b_unique)
      list.push_back(this->at(i));
  }

  return list;
}

ParticleList ParticleList::BinaryMerge(int Nmax) const {
  if((Nmax < 1) || (this->size() <= Nmax))
    return *this;

  int N = this->size();
  double Rmin = 2.;
  int imin = -1;
  int jmin = -1;
  for(int i = 0; i < N-1; i++){
    for(int j = i+1; j < N; j++){
      double M2 = TLorentzVector(this->at(i)+this->at(j)).M2();
      double R = M2 > 0. ? (M2 - pow(this->at(i).M()+this->at(j).M(), 2.))/M2 : 0.;
      if(R < Rmin){
	Rmin = R;
	imin = i;
	jmin = j;
      }
    }
  }

  ParticleList list;
  list += this->at(imin).Merge(this->at(jmin));
  for(int i = 0; i < N; i++){
    if(i != imin && i != jmin)
      list += this->at(i);
  }

  return list.BinaryMerge(Nmax);
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
