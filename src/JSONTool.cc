#include "JSONTool.hh"

block::block(int run, int lumi_beg, int lumi_end){
  m_run = run;
  m_lumi_beg = lumi_beg;
  m_lumi_end = lumi_end;
  
  m_left = nullptr;
  m_right = nullptr;
}

block::~block(){
  if(m_left != nullptr)
    delete m_left;
  if(m_right != nullptr)
    delete m_right;
}

int block::run() const { return m_run; }
int block::lumi_beg() const { return m_lumi_beg; }
int block::lumi_end() const { return m_lumi_end; }

void block::SetLeft(block* block){
  if(block == nullptr)
    return;

  if(m_left != nullptr)
    delete m_left;
    
  m_left = block;
}
  
void block::SetRight(block* block){
  if(block == nullptr)
    return;

  if(m_right != nullptr)
    delete m_left;
    
  m_right = block;
}
  
bool block::IsGood(int run, int lumi) const {
  if(run == m_run){
    if(lumi >= m_lumi_beg &&
       lumi <= m_lumi_end)
      return true;

    if(lumi < m_lumi_beg){
      if(m_left != nullptr)
	return m_left->IsGood(run, lumi);
      else return false;
    } else {
      if(m_right != nullptr)
	return m_right->IsGood(run, lumi);
    }
  }

  if(run < m_run){
    if(m_left != nullptr)
      return m_left->IsGood(run, lumi);
  } else {
    if(m_right != nullptr)
      return m_right->IsGood(run, lumi);
  }
  return false;
}

bool sortbylumi(block* b1, block* b2){
  if(b1->run() == b2->run())
    return (b1->lumi_beg() < b2->lumi_beg());
      
  return (b1->run() < b2->run());
}

block* LumiBST(std::vector<block*>& blocks, int start, int end){
  if(start > end)
    return nullptr;

  int mid = (start + end)/2;
  block* root = blocks[mid];

  root->SetLeft(LumiBST(blocks, start, mid-1));
  root->SetRight(LumiBST(blocks, mid+1, end));

  return root;
}

void JSONTool::BuildMap(const std::string& jsonfile){

  std::ifstream ifile(jsonfile.c_str());
  if(!ifile.is_open()){
    std::cout << "can't open file " << jsonfile << std::endl;
    return;
  }

  string line;
  size_t found, end;

  int run, lumi_beg, lumi_end;

  std::vector<block*> lumi_blocks;
  
  while(getline(ifile,line)){
    //remove whitespace
    //line.erase(remove(line.begin(), line.end(), ' '), line.end());
    line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

    while(line.find("\"") != string::npos){
      // parse run number
      found = line.find("\"");
      line.erase(0,found+1);
      found = line.find("\"");
      string srun = line.substr(0,found);
      run = stoi(srun);
      line.erase(0,found+1);

      // parse lumi sections
      found  = line.find("[[");
      end = line.find("]]");
      string slumis = line.substr(found+1,end-1);
      line.erase(0,end+2);

      while(slumis.find("[") != string::npos){
	found = slumis.find("[");
	end   = slumis.find("]");
	string slumi = slumis.substr(found,end);
	slumis.erase(0, end+1);
	sscanf(slumi.c_str(),"[%d,%d]", &lumi_beg, &lumi_end);

	if(lumi_beg > lumi_end)
	  continue;

	lumi_blocks.push_back(new block(run, lumi_beg, lumi_end));
      }
    }
  }
  ifile.close();

  sort(lumi_blocks.begin(),lumi_blocks.end(), sortbylumi);
    
  int N = lumi_blocks.size();
  if(N <= 0)
    return;

  if(m_block != nullptr)
    delete m_block;
    
  m_block = LumiBST(lumi_blocks, 0, N-1);
}

bool JSONTool::IsGood(int run, int lumi){
  if(m_block == nullptr)
    return false;

  return m_block->IsGood(run, lumi);
}




