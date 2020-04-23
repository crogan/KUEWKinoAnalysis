#ifndef JSONTool_h
#define JSONTool_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using std::string;

class block {

public:
  block(int run, int lumi_beg, int lumi_end);

  virtual ~block();
  
  int run() const;
  int lumi_beg() const;
  int lumi_end() const;

  void SetLeft(block* block);
  
  void SetRight(block* block);
  
  bool IsGood(int run, int lumi) const;
  
private:
  int m_run;
  int m_lumi_beg;
  int m_lumi_end;

  block* m_left;
  block* m_right;
  
};

bool sortbylumi(block* b1, block* b2);
block* LumiBST(std::vector<block*>& blocks, int start, int end);

class JSONTool {
  
public:
  JSONTool(){
    m_block = nullptr;
  }
  
  virtual ~JSONTool(){
    if(m_block != nullptr)
      delete m_block;
  }

  void BuildMap(const std::string& jsonfile);

  bool IsGood(int run, int lumi);

private:
  block* m_block;
};

#endif






