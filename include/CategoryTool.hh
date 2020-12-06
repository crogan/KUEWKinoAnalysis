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

  CategoryList GetCategories() const;
  CategoryList GetCategories_RandD() const;
  CategoryList GetCategories_0L() const;
  CategoryList GetCategories_1L() const;
  CategoryList GetCategories_2L() const;
  CategoryList GetCategories_3L() const;

protected:
  FitBin   GetFitBin(int index) const;
  Hadronic GetHadronicRegion(int Njet, int index) const;
  
};

#endif



