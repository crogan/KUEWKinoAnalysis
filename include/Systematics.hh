#ifndef Systematics_HH
#define Systematics_HH

#include <iostream>
#include <vector>

class Systematics;

class Systematic {
public:
  Systematic(const std::string& label = "Default");
    
  virtual ~Systematic();

  std::string Label() const;

  const Systematic& Up() const;
  const Systematic& Down() const;
  bool IsUp() const;
  bool IsDown() const;

  bool IsDefault() const;
  bool operator !() const { return IsDefault(); }

  bool IsSame(const std::string& label) const;
  bool IsSame(const Systematic& sys) const;
  bool operator == (const std::string& label) const { return IsSame(label); }
  bool operator == (const Systematic& sys) const { return IsSame(sys); }
  bool operator != (const std::string& label) const { return !IsSame(label); }
  bool operator != (const Systematic& sys) const { return !IsSame(sys); }

  bool operator == (const Systematics& sys) const;
  bool operator != (const Systematics& sys) const;

  std::string TreeName(const std::string& name) const;

  static Systematic& Default();
    
private:
  std::string m_Label;
  mutable bool m_IsUp;

  static Systematic m_Default;
};


class Systematics {
public:
  Systematics(bool include_default = false);
    
  virtual ~Systematics();

  const Systematic& operator [] (int i) const;

  int GetN() const;

  Systematics& Add(const std::string& label);
  Systematics& Add(const Systematic& sys);
  Systematics& Add(const Systematics& sys);
  Systematics& operator += (const std::string& label);
  Systematics& operator += (const Systematic& sys);
  //Systematics& operator += (const Systematics& sys);

  bool Contains(const std::string& label) const;
  bool Contains(const Systematic& sys) const;
  bool operator == (const std::string& label) const;
  bool operator == (const Systematic& sys) const;
  bool operator != (const std::string& label) const;
  bool operator != (const Systematic& sys) const;
    
private:
  int m_N;
  std::vector<Systematic*> m_Sys;

};


#endif
