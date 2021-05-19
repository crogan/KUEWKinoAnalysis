#ifndef CategoryTool_h
#define CategoryTool_h

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "Category.hh"

using std::string;
using std::vector;
using std::pair;

///////////////////////////////////////////
////////// CategoryTool class
///////////////////////////////////////////

class CategoryTool {

public:
  CategoryTool();
  virtual ~CategoryTool();

  CategoryList GetCategories(bool maskSR = false) const;
  CategoryList GetCategories_0L(bool maskSR = false) const;
  CategoryList GetCategories_1L(bool maskSR = false) const;
  CategoryList GetCategories_2L(bool maskSR = false) const;
  CategoryList GetCategories_3L(bool maskSR = false) const;

protected:
  FitBin   GetFitBin(int Nlep, int index, bool maskSR = false) const;
  Hadronic GetHadronicRegion(int Njet, int index) const;
  
};

#endif



