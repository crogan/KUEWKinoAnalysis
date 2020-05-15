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
  
};

#endif



