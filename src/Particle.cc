#include "Particle.hh"
#include "ParticleList.hh"

Particle::Particle() : TLorentzVector() {
  m_Charge = 0;
  m_PDGID = 0;
  m_MomPDGID = 0;
  m_SourceID = 0;
  m_ParticleID = kNothing;

  m_RelIso = 0.;
  m_MiniIso = 0.;
  m_Dxy = 0.;
  m_DxyErr = 0.;
  m_DxySig = 0.;
  m_Dz = 0.;
  m_DzErr = 0.;
  m_IP3D = 0.;
  m_SIP3D = 0.;

  m_D3d = 0.;
  m_D3dSig = 0.;
  m_CosTheta = 0.;
  m_Ndof = 0.;
  m_ProbB = 0.;
  m_ProbC = 0.;
  
  m_Flavor = 0;
  m_Ntrk = 0;
  m_PAngle = 0.;
  m_Chi2 = 0.;
  m_X = 0.;
  m_Y = 0.;
  m_Z = 0.;
  
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

int Particle::SourceID() const {
  return m_SourceID;
}

void Particle::SetSourceID(int sourceid){
  m_SourceID = sourceid;
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

ParticleIDType Particle::BtagID() const {
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

double Particle::Dxy() const {
  return m_Dxy;
}

double Particle::DxyErr() const {
  return m_DxyErr;
}

double Particle::DxySig() const {
  return m_DxySig;
}

double Particle::Dz() const {
  return m_Dz;
}

double Particle::DzErr() const{
  return m_DzErr;
}

double Particle::IP3D() const {
  return m_IP3D;
}

double Particle::SIP3D() const {
  return m_SIP3D;
}

double Particle::D3d() const {
  return m_D3d;
}

double Particle::D3dSig() const {
  return m_D3dSig;
}

double Particle::CosTheta() const {
  return m_CosTheta;
}

double Particle::Ndof() const {
  return m_Ndof;
}

double Particle::ProbB() const {
  return m_ProbB;
}

double Particle::ProbC() const {
  return m_ProbC;
}

int Particle::Flavor() const {
  return m_Flavor;
}

int Particle::Ntrk() const {
  return m_Ntrk;
}

double Particle::PAngle() const {
  return m_PAngle;
}

double Particle::Chi2() const {
  return m_Chi2;
}

double Particle::X() const {
  return m_X;
}

double Particle::Y() const {
  return m_Y;
}

double Particle::Z() const {
  return m_Z;
}

void Particle::SetDxy(double val){
  m_Dxy = val;
}

void Particle::SetDxyErr(double val){
  m_DxyErr = val;
}

void Particle::SetDxySig(double val){
  m_DxySig = val;
}

void Particle::SetDz(double val){
  m_Dz = val;
}

void Particle::SetDzErr(double val){
  m_DzErr = val;
}

void Particle::SetIP3D(double val){
  m_IP3D = val;
}

void Particle::SetSIP3D(double val){
  m_SIP3D = val;
}

void Particle::SetD3d(double val){
  m_D3d = val;
}

void Particle::SetD3dSig(double val){
  m_D3dSig = val;
}

void Particle::SetCosTheta(double val){
  m_CosTheta = val;
}

void Particle::SetNdof(double val){
  m_Ndof = val;
}

void Particle::SetProbB(double val){
  m_ProbB = val;
}

void Particle::SetProbC(double val){
  m_ProbC = val;
}

void Particle::SetFlavor(int val){
  m_Flavor = val;
}

void Particle::SetNtrk(int val){
  m_Ntrk = val;
}

void Particle::SetPAngle(double val){
  m_PAngle = val;
}

void Particle::SetChi2(double val){
  m_Chi2 = val;
}

void Particle::SetX(double val){
  m_X = val;
}

void Particle::SetY(double val){
  m_Y = val;
}

void Particle::SetZ(double val){
  m_Z = val;
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

Particle Particle::Merge(const Particle& p) const {
  Particle part = *this;
  if(p.Btag() > part.Btag())
    part.SetBtag(p.Btag());
  if(p.BtagID() > part.BtagID())
    part.SetBtagID(p.BtagID());
  
  part += p;

  return part;
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
