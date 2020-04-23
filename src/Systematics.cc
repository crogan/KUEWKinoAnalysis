#include "Systematics.hh"

Systematic::Systematic(const std::string& label){
  m_Label = label;
  m_IsUp = true;
}
    
Systematic::~Systematic(){ }

std::string Systematic::Label() const {
  return m_Label;
}

const Systematic& Systematic::Up() const {
  m_IsUp = true;

  return *this;
}

const Systematic& Systematic::Down() const {
  m_IsUp = false;

  return *this;
}

bool Systematic::IsUp() const {
  return m_IsUp;
}

bool Systematic::IsDown() const {
  return !m_IsUp;
}

bool Systematic::IsDefault() const {
  return IsSame("Default");
}
 
bool Systematic::IsSame(const std::string& label) const {
  return m_Label == label;
}

bool Systematic::IsSame(const Systematic& sys) const {
  return IsSame(sys.Label());
}

bool Systematic::operator == (const Systematics& sys) const {
  return sys == *this;
}

bool Systematic::operator != (const Systematics& sys) const {
  return sys != *this;
}

std::string Systematic::TreeName(const std::string& name) const {

  if(this->IsDefault())
    return name;
  else
    return name+"_"+m_Label+(IsUp() ? "_up" : "_down");
}

Systematic& Systematic::Default(){
  return Systematic::m_Default;
}

Systematics::Systematics(bool include_default){
  m_N = 0;
  
  if(include_default)
    Add(Systematic::Default());
}
    
Systematics::~Systematics(){
  for(int i = 0; i < m_N; i++)
    delete m_Sys[i];
}

const Systematic& Systematics::operator [] (int i) const {
  if(i < 0 || i >= m_N)
    return Systematic::Default();

  return *m_Sys[i];
}

int Systematics::GetN() const {
  return m_N;
}

Systematics& Systematics::Add(const std::string& label){
  if(Contains(label))
    return *this;
      
  m_Sys.push_back(new Systematic(label));
  m_N++;
  
  return *this;
}

Systematics& Systematics::Add(const Systematic& sys){
  if(Contains(sys))
    return *this;
  
  m_Sys.push_back(new Systematic(sys.Label()));
  m_N++;
  
  return *this;
}

Systematics& Systematics::Add(const Systematics& sys){
  int N = sys.GetN();

  for(int i = 0; i < N; i++)
    Add(sys[i]);
  
  return *this;
}

Systematics& Systematics::operator += (const std::string& label){
  return Add(label);
}

Systematics& Systematics::operator += (const Systematic& sys){
  return Add(sys);
}

// Systematics& Systematics::operator += (const Systematics& sys){
//   return Add(sys);
// }

bool Systematics::Contains(const std::string& label) const {
  for(int i = 0; i < m_N; i++)
    if(*m_Sys[i] == label)
      return true;

  return false;
}

bool Systematics::Contains(const Systematic& sys) const {
  for(int i = 0; i < m_N; i++)
    if(*m_Sys[i] == sys)
      return true;

  return false;
}

bool Systematics::operator == (const std::string& label) const {
  return Contains(label);
}

bool Systematics::operator == (const Systematic& sys) const {
  return Contains(sys);
}

bool Systematics::operator != (const std::string& label) const {
  return !Contains(label);
}

bool Systematics::operator != (const Systematic& sys) const {
  return !Contains(sys);
}

Systematic Systematic::m_Default;
