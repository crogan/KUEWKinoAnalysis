#ifndef Particle_HH
#define Particle_HH

#include "TLorentzVector.h"

class ParticleList;

/// Particle ID level
enum ParticleIDType { kNothing, kVeryLoose, kLoose, kMedium, kTight, kVeryTight };

class Particle : public TLorentzVector {
public:
  Particle();
    
  virtual ~Particle();

  int Charge() const;
  void SetCharge(int charge);

  int PDGID() const;
  void SetPDGID(int pdgid);

  int MomPDGID() const;
  void SetMomPDGID(int pdgid);

  ParticleIDType ParticleID() const;
  void SetParticleID(ParticleIDType id);

  double RelIso() const;
  double MiniIso() const;
  void SetRelIso(double iso);
  void SetMiniIso(double iso);

  double Dxy() const;
  double DxyErr() const;
  double Dz() const;
  double DzErr() const;
  double IP3D() const;
  double SIP3D() const;
  void SetDxy(double val);
  void SetDxyErr(double val);
  void SetDz(double val);
  void SetDzErr(double val);
  void SetIP3D(double val);
  void SetSIP3D(double val);

  double Btag() const;
  void SetBtag(double btag);

  double BtagID() const;
  void SetBtagID(ParticleIDType id);
    
  operator ParticleList() const;
  ParticleList operator + (const Particle& part) const; 
  ParticleList operator + (const ParticleList& parts) const; 
    
private:
  int m_Charge;
  int m_PDGID;
  int m_MomPDGID;
  ParticleIDType m_ParticleID;
  
  double m_Btag;
  ParticleIDType m_BtagID;

  // lepton stuff
  
  double m_RelIso;
  double m_MiniIso;

  double m_Dxy;
  double m_DxyErr;
  double m_Dz;
  double m_DzErr;
  double m_IP3D;
  double m_SIP3D;
  
};

bool sortbypt(const Particle& p1, const Particle& p2);


#endif
