#ifndef Systematics_HH
#define Systematics_HH

#include <iostream>
#include <vector>

#include "Criteria.hh"

class Systematics;

///////////////////////////////////////////
////////// Systematic class
///////////////////////////////////////////

class Systematic {
public:
  explicit Systematic(const string& label = "Default");
    
  virtual ~Systematic();

  std::string Label() const;

  const Systematic& Up() const;
  const Systematic& Down() const;
  bool IsUp() const;
  bool IsDown() const;

  bool IsDefault() const;
  bool operator !() const { return IsDefault(); }

  bool IsSame(const string& label) const;
  bool IsSame(const Systematic& sys) const;
  bool operator == (const string& label) const;
  bool operator == (const Systematic& sys) const;
  bool operator != (const string& label) const;
  bool operator != (const Systematic& sys) const;

  // is/isnot this contained in sys
  bool operator == (const Systematics& sys) const;
  bool operator != (const Systematics& sys) const;

  bool operator < (const Systematic& sys) const;
  bool operator > (const Systematic& sys) const;

  std::string TreeName(const string& name) const;

  static Systematic& Default();
    
private:
  std::string m_Label;
  mutable bool m_IsUp;

  static Systematic m_Default;
};

///////////////////////////////////////////
////////// Systematics class
///////////////////////////////////////////

class Systematics {
public:
  explicit Systematics(bool include_default = false);
  Systematics(const Systematics&);
    
  virtual ~Systematics();
  
  Systematics& operator = (const Systematics&);

  Systematic& operator [] (int i) const;

  int GetN() const;

  Systematics& Add(const string& label);
  Systematics& Add(const Systematic& sys);
  Systematics& Add(const Systematics& sys);
  Systematics& operator += (const string& label);
  Systematics& operator += (const Systematic& sys);
  Systematics& operator += (const Systematics& sys);

  bool Contains(const string& label) const;
  bool Contains(const Systematic& sys) const;
  bool operator == (const string& label) const;
  bool operator == (const Systematic& sys) const;
  bool operator != (const string& label) const;
  bool operator != (const Systematic& sys) const;

  Systematics Filter(const string& label) const;
  Systematics FilterOR(const VS& labels) const;
  Systematics FilterAND(const VS& labels) const;
  Systematics Remove(const string& label) const;
  Systematics RemoveOR(const VS& labels) const;
  Systematics RemoveAND(const VS& labels) const;
    
private:
  int m_N;
  std::vector<Systematic*> m_Sys;

  void Clear();

};

///////////////////////////////////////////
////////// SystematicsTool class
///////////////////////////////////////////

class SystematicsTool {
public:
  SystematicsTool();

  virtual ~SystematicsTool();

  Systematics GetWeightSystematics() const;
  
  Systematics GetTreeSystematics() const;

  const Systematics& JESSystematics() const;
  const Systematics& MMSSystematics() const;
  const Systematics& EESSystematics() const;
  const Systematics& METSystematics() const;
  
private:
  void Init();
  
  Systematics m_JESSys; // jet energy scale systematics
  Systematics m_MMSSys; // muon momentum scale
  Systematics m_EESSys; // electron energy scale
  Systematics m_METSys; // MET systematics
  
};


#endif
