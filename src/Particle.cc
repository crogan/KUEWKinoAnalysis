#include "Particle.hh"
#include "ParticleList.hh"

Particle::Particle() : TLorentzVector() {
  m_Charge = 0;
  m_PDGID = 0;
  m_MomPDGID = 0;
  m_ParticleID = kNothing;

  m_RelIso = 0.;
  m_MiniIso = 0.;

  m_Btag = 0.;
  m_BtagID = kNothing;
}
    
Particle::~Particle() {}

int Particle::Charge() const {
  return m_Charge;
}

void Particle::SetCharge(int charge){
  m_Charge = charge;
}

int Particle::MomPDGID() const {
  return m_MomPDGID;
}

void Particle::SetMomPDGID(int pdgid){
  m_MomPDGID = pdgid;
}

int Particle::PDGID() const {
  return m_PDGID;
}

void Particle::SetPDGID(int pdgid){
  m_PDGID = pdgid;
}

ParticleIDType Particle::ParticleID() const {
  return m_ParticleID;
}

void Particle::SetParticleID(ParticleIDType id){
  m_ParticleID = id;
}

double Particle::BtagID() const {
  return m_BtagID;
}

void Particle::SetBtagID(ParticleIDType id){
  m_BtagID = id;
}

double Particle::RelIso() const {
  return m_RelIso;
}

double Particle::MiniIso() const {
  return m_MiniIso;
}

void Particle::SetRelIso(double iso){
  m_RelIso = iso;
}

void Particle::SetMiniIso(double iso){
  m_MiniIso = iso;
}

double Particle::Btag() const {
  return m_Btag;
}

void Particle::SetBtag(double btag){
  m_Btag = btag;
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

bool sortbypt(const Particle& p1, const Particle& p2){
  return (p1.Pt() >= p2.Pt());
}
