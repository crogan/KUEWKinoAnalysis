#include "SuperBin.hh"

//SuperBin Class
//Constructor
SuperBin::SuperBin(const VI index, const double nSig, const double nBkg, const VS mPerpLabel, const VS rIsrLabel, const VS visLabel){
  index_ = index;
  nSig_ = nSig;
  nBkg_ = nBkg;
  sOverB_ = nSig/nBkg;
  mPerpLabel_ = mPerpLabel;
  rIsrLabel_ = rIsrLabel;
  visLabel_ = visLabel;
}

SuperBin::~SuperBin(){}

VI SuperBin::getIndex() const{
  return index_;
}
double SuperBin::getNsig() const{
  return nSig_;
}
double SuperBin::getNbkg() const{
  return nBkg_;
}
double SuperBin::getSoverB() const{
  return sOverB_;
}
double SuperBin::getBinZbi(const double sys) const{
  return FitReader::CalculateZbi(nSig_, nBkg_, sys);
}
VS SuperBin::getMperpLabel() const{
  return mPerpLabel_;
}
VS SuperBin::getRisrLabel() const{
  return rIsrLabel_;
}
VS SuperBin::getVisLabel() const{
  return visLabel_;
}

SuperBin* SuperBin::tryMerge(SuperBin* superBin, double sys){

  double myZbi = this->getBinZbi(sys);
  double theirZbi = superBin->getBinZbi(sys);

  double nSigTotal = this->getNsig() + superBin->getNsig();
  double nBkgTotal = this->getNbkg() + superBin->getNbkg();

  double combinedZbi = FitReader::CalculateZbi(nSigTotal, nBkgTotal, sys);

  if(combinedZbi > myZbi && combinedZbi > theirZbi){
    VI idxList = this->getIndex() += superBin->getIndex();
    VS combinedMperpLabels = this->getMperpLabel() += superBin->getMperpLabel();
    VS combinedRisrLabels  = this->getRisrLabel()  += superBin->getRisrLabel();
    VS combinedVisLabels   = this->getVisLabel()   += superBin->getVisLabel();
    SuperBin* mergedSuperBin = new SuperBin(idxList, nSigTotal, nBkgTotal, combinedMperpLabels, combinedRisrLabels, combinedVisLabels);
    return mergedSuperBin;
  }
  else
    return nullptr;
}

//SuperBinList Class
//Default constructor
SuperBinList::SuperBinList(){}

//Construct superBinList from vector of superBin objects
SuperBinList::SuperBinList(const std::vector<SuperBin*>& superBinList){
  for(auto superBin : superBinList)
    *this += superBin;
  listSize_ = this->size();
}

SuperBinList::~SuperBinList(){}

SuperBinList& SuperBinList::operator += (SuperBin* superBin){
  this->push_back(superBin);
  listSize_ = this->size();
  return *this;
}

SuperBinList& SuperBinList::operator += (const SuperBinList& superBinList){
  for(auto superBin : superBinList)
    this->push_back(superBin);
  listSize_ = this->size();
  return *this;
}

void SuperBinList::PrintSummary(const double sys){  
  this->sortByZbi(sys);

  cout << "Summary for bins in Region " << identifier_ << ": " << endl;
  cout << "bins combined" << "\tindex" << "\tsignal yield" << "\tbackground yield" << "\tZbi" << endl;
  for(int i = 0; i < this->size(); i++){
    cout << "\t" << this->at(i)->getIndex().size() << "\t"; 
    this->at(i)->getIndex().printList();
    cout << "\t" << this->at(i)->getNsig() << "\t\t" 
	 << this->at(i)->getNbkg() << "\t\t" << this->at(i)->getBinZbi(sys) << endl;
  }
}

void SuperBinList::SetIdentifier(const string ID){
  identifier_ = ID;
}

void SuperBinList::sortByZbi(const double sys){
  std::sort(this->begin(), this->end(), [&sys](const SuperBin* lhs, const SuperBin* rhs){return lhs->getBinZbi(sys) > rhs->getBinZbi(sys);});
}

void SuperBinList::sortBySoverB(){
  std::sort(this->begin(), this->end(), [](const SuperBin* lhs, const SuperBin* rhs){return lhs->getSoverB() > rhs->getSoverB();});
}

void SuperBinList::PlotListZbi(const string name, const double sys){

  int nBins = this->size();
  double max = this->GetMaxZbi(sys);
  TH1F* fhist = new TH1F(Form("fhist_%s", name.c_str()), Form("fhist_%s", name.c_str()), nBins, 0., double(nBins));

  for(int b = 1; b < nBins+1; b++){
    fhist->SetBinContent(b, this->at(b-1)->getBinZbi(sys));
    cout << "bin " << b << ": " << endl;
    cout << "Zbi: " << this->at(b-1)->getBinZbi(sys) << endl;
  }

  gROOT->SetBatch(kTRUE);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);

  TCanvas* can = new TCanvas(Form("can_%s", name.c_str()),
                             Form("can_%s", name.c_str()),
                             1200, 700);

  double yline = can->GetBottomMargin() - 0.02;
  double hlo = 0.1;
  double hhi = 0.25;
  double hto = 0.07;
  double hbo = 0.19;

  can->SetLeftMargin(hlo);
  can->SetRightMargin(hhi);
  can->SetBottomMargin(hbo);
  can->SetTopMargin(hto);
  can->SetGridx();
  can->SetGridy();
  can->Draw();
  can->cd();

  fhist->SetMarkerSize(2);
  fhist->SetMarkerColor(kBlack);
  fhist->SetMarkerStyle(8);
  fhist->Draw("hist p");
  fhist->GetXaxis()->CenterTitle();
  fhist->GetXaxis()->SetTitleFont(42);
  fhist->GetXaxis()->SetTitleSize(0.04);
  fhist->GetXaxis()->SetTitleOffset(1.0);
  fhist->GetXaxis()->SetLabelFont(42);
  fhist->GetXaxis()->SetLabelSize(0.05);
  fhist->GetXaxis()->SetLabelOffset(999);
  fhist->GetXaxis()->SetTitle("Bins Merged");
  fhist->GetXaxis()->SetTickSize(0.);
  fhist->GetYaxis()->CenterTitle();
  fhist->GetYaxis()->SetTitleFont(42);
  fhist->GetYaxis()->SetTitleSize(0.04);
  fhist->GetYaxis()->SetTitleOffset(0.85);
  fhist->GetYaxis()->SetLabelFont(42);
  fhist->GetYaxis()->SetRangeUser(0.,max*1.35);
  fhist->GetYaxis()->SetTitle("Z_{bi}");

  double eps = 0.0015;

  TLatex l1,l2;
  l1.SetTextFont(42);
  l1.SetNDC();
  l1.SetTextSize(std::min(0.03, 1.5/double(nBins)));
  l1.SetTextFont(42);
  l1.SetTextAlign(32);
  l1.SetTextAngle(90);

  l2.SetTextFont(42);
  l2.SetNDC();
  l2.SetTextSize(std::min(0.03, 1.5/double(nBins)));
  l2.SetTextFont(60);
  l2.SetTextAlign(32);

  for(int b = 0; b < nBins; b++){
    string label;
    double xpos, ypos1, ypos2;
    label = this->at(b)->getVisLabel()[0];
    xpos = hlo+(1.-hhi-hlo)/double(nBins)*(0.5+b);
    ypos1 = yline+0.01;
    ypos2 = 1 - hto - 4*eps;
    l2.DrawLatex(xpos,ypos1,std::to_string(this->at(b)->getIndex().size()).c_str());
    l1.SetTextColor(7004 + 10*((b%nBins)%8));
    l1.DrawLatex(xpos,ypos2,label.c_str());
  }

  string  m_CMSLabel = "#bf{#it{CMS}} work-in-progress";

  TLatex l3;
  l3.SetTextFont(42);
  l3.SetNDC();

  TLine* line = new TLine();
  line->SetLineWidth(2);
  line->SetLineColor(kBlack);

  l3.SetTextSize(0.025);
  l3.SetTextFont(42);
  l3.SetTextAlign(23);
  line->SetLineWidth(1);
  double lo = hlo;
  double hi = hlo;
 
 
  l3.SetTextAlign(31);
  l3.SetTextSize(0.04);
  l3.SetTextFont(42);
  l3.DrawLatex(1.-hhi-eps*4, 1.-hto+0.02, string("Regions "+identifier_).c_str());
  l3.SetTextAlign(11);
  l3.SetTextSize(0.04);
  l3.SetTextFont(42);
  l3.DrawLatex(hlo+eps*4, 1.-hto+0.02, m_CMSLabel.c_str());
  l3.SetTextSize(0.05);

  can->Update();
  can->SaveAs((name+".pdf").c_str());
}

void SuperBinList::PlotListZbiMR(const string name, const double sys, const FitBin& fitbin){

  int nBins = this->size();
  double max = this->GetMaxZbi(sys);
  TH1F* fhist = new TH1F(Form("fhist_%s", name.c_str()), Form("fhist_%s", name.c_str()), nBins, 0., double(nBins));

  for(int b = 1; b < nBins+1; b++)
    fhist->SetBinContent(b, this->at(b-1)->getBinZbi(sys));

  gROOT->SetBatch(kTRUE);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);

  TCanvas* can = new TCanvas(Form("can_%s", name.c_str()),
                             Form("can_%s", name.c_str()),
                             1200, 700);

  //double yline = can->GetBottomMargin() - 0.02;
  double hlo = 0.1;
  double hhi = 0.25;
  double hto = 0.07;
  double hbo = 0.19;

  can->SetLeftMargin(hlo);
  can->SetRightMargin(hhi);
  can->SetBottomMargin(hbo);
  can->SetTopMargin(hto);
  can->SetGridx();
  can->SetGridy();
  can->Draw();
  can->cd();

  fhist->SetMarkerSize(2);
  fhist->SetMarkerColor(kBlack);
  fhist->SetMarkerStyle(8);
  fhist->Draw("hist p");
  fhist->GetXaxis()->CenterTitle();
  fhist->GetXaxis()->SetTitleFont(42);
  fhist->GetXaxis()->SetTitleSize(0.04);
  fhist->GetXaxis()->SetTitleOffset(1.0);
  fhist->GetXaxis()->SetLabelFont(42);
  fhist->GetXaxis()->SetLabelSize(0.05);
  fhist->GetXaxis()->SetLabelOffset(999);
  fhist->GetXaxis()->SetTitle("");
  fhist->GetXaxis()->SetTickSize(0.);
  fhist->GetYaxis()->CenterTitle();
  fhist->GetYaxis()->SetTitleFont(42);
  fhist->GetYaxis()->SetTitleSize(0.04);
  fhist->GetYaxis()->SetTitleOffset(0.85);
  fhist->GetYaxis()->SetLabelFont(42);
  fhist->GetYaxis()->SetRangeUser(0.,max*1.35);
  fhist->GetYaxis()->SetTitle("Z_{bi}");

  double eps = 0.0015;

  TLatex l1;
  l1.SetTextFont(42);
  l1.SetNDC();
  l1.SetTextSize(std::min(0.03, 1.5/double(nBins)));
  l1.SetTextFont(42);
  l1.SetTextAlign(32);

  for(int b = 0; b < nBins; b++){
    double xpos, ypos;
    xpos = hlo+(1.-hhi-hlo)/double(nBins)*(0.5+b);
    ypos = 1 - hto - 20*eps;
    l1.DrawLatex(xpos,ypos,std::to_string(this->at(b)->getIndex().size()).c_str());
    //l1.SetTextColor(7004 + 10*((b%nBins)%8));
  }

  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();

  int NR = fitbin.NRBins();
  int NB = fitbin.NBins();

  VS mlabels;
  VS rlabels;
  for(int r = 0; r < NR; r++)
    mlabels += fitbin[r].GetMBinLabels();

  int lmax = 0;
  for(int b = 0; b < NB; b++){
    int len = mlabels[b].length();
    if(mlabels[b].find("#infty") != std::string::npos)
      len -= 5;
    if(len > lmax)
      lmax = len;
  }
  
  string space = "";
  for(int l = 0; l < 1.6*lmax; l++)
    space += " ";

  for(int r = 0; r < NR; r++)
    rlabels += fitbin[r].GetRBinLabel();
  
  TLine* line = new TLine();
  line->SetLineWidth(2);
  line->SetLineColor(kBlack);

  l.SetTextSize(0.033);
  l.SetTextFont(42);
  l.SetTextAlign(23);
  line->SetLineWidth(2);
  double lo = hlo;
  double hi = hlo;
  //double lo_ratio = hlo_rat;
  //double hi_ratio = hlo_rat;
  double yline = hbo-0.018*lmax;
  int ib = 0;
  for(int r = 0; r < NR; r++){
    int NM = fitbin[r].NBins();
    lo = hi;
    //lo_ratio = hi_ratio;
    hi = double(NM)/double(NB)*(1.-hhi-hlo) + lo;
    //hi_ratio = double(NM)/double(NB)*(1.-hhi_rat-hlo_rat) + lo_ratio;
    
    line->SetLineStyle(1);
    line->DrawLineNDC(lo + eps, yline,
		      lo + eps, yline + 6*eps);
    line->DrawLineNDC(hi - eps, yline,
		      hi - eps, yline + 6*eps);
   
    line->DrawLineNDC(lo + eps, yline,
		      hi - eps, yline);
  
    line->SetLineStyle(1);
    if(r < NR-1){
      line->DrawLineNDC(hi, yline + 10*eps , hi, 1.-hto);
    }
    l.DrawLatex((hi+lo)/2., yline - 8*eps, rlabels[r].c_str());
  }

  line->SetLineStyle(1);
  l.SetTextAngle(90);
  l.SetTextAlign(32);
  for(int b = 0; b < NB; b++){
    l.DrawLatex(hlo + (1.-hhi-hlo)*(0.5+b)/double(NB), hbo - 4*eps, mlabels[b].c_str());
    if(b > 0)
      line->DrawLineNDC(hlo + (1.-hhi-hlo)/double(NB)*b , hbo , hlo + (1.-hhi-hlo)/double(NB)*b, 1.-hto);
  }
  
  
  l.SetTextAngle(0);
  l.SetTextAlign(32);
  l.SetTextSize(0.035);
  l.DrawLatex(hlo, (hbo+yline)/2.+eps, "M_{#perp}   [GeV] #in");
  l.DrawLatex(hlo, yline - 16*eps, "#scale[1.15]{R_{ISR}} #in");

  string  m_CMSLabel = "#bf{#it{CMS}} work-in-progress";

  TLatex l3;
  l3.SetTextFont(42);
  l3.SetNDC();

  TLine* line2 = new TLine();
  line2->SetLineWidth(2);
  line2->SetLineColor(kBlack);

  l3.SetTextSize(0.025);
  l3.SetTextFont(42);
  l3.SetTextAlign(23);
  line2->SetLineWidth(1);
  //double lo = hlo;
  //double hi = hlo;

  l3.SetTextAlign(31);
  l3.SetTextSize(0.04);
  l3.SetTextFont(42);
  l3.DrawLatex(1.-hhi-eps*4, 1.-hto+0.02, string("Regions "+identifier_).c_str());
  l3.SetTextAlign(11);
  l3.SetTextSize(0.04);
  l3.SetTextFont(42);
  l3.DrawLatex(hlo+eps*4, 1.-hto+0.02, m_CMSLabel.c_str());
  l3.SetTextSize(0.05);

  can->Update();
  can->SaveAs((name+".pdf").c_str());
}

double SuperBinList::GetMaxZbi(const double sys){
  double max = this->at(0)->getBinZbi(sys);

  for(int i = 1; i < this->size(); i++){
    double tempZbi = this->at(i)->getBinZbi(sys);
    if(tempZbi > max)
      max = tempZbi;
  }
  return max;
}

SuperBin* SuperBinList::GetMaxZbiBin(const double sys){
  double max = this->at(0)->getBinZbi(sys);
  SuperBin* maxSbin = this->at(0);

  for(int i = 1; i < this->size(); i++){
    double tempZbi = this->at(i)->getBinZbi(sys);
    if(tempZbi > max)
      maxSbin = this->at(i);
  }
  return maxSbin;
}

SuperBin* SuperBinList::MergeBins(const double sys){

  SuperBin* firstBin = this->at(0);

  for(int b = 1; b < this->size(); b++){
    SuperBin* tempBin = firstBin->tryMerge(this->at(b), sys);
   
    if(tempBin)
      firstBin = tempBin;
  }

  if(firstBin == this->at(0))
    firstBin = GetMaxZbiBin(sys);
  return firstBin;
}

//VI class
VI::VI(){}

VI::VI(const int i){
  *this += i;
}

VI::VI(const std::vector<int> vi){
  for(auto i : vi)
    *this += i;
}

VI::~VI(){}

VI& VI::operator += (int i){
  this->push_back(i);
  return *this;
}

VI& VI::operator += (std::vector<int> vi){
  for(auto i : vi)
    this->push_back(i);
  return *this;
}

void VI::printList(){
  int size = this->size();
  for(int i = 0; i < size; i++){
    if(i < size-1)
      cout << this->at(i) << ", ";
    else
      cout << this->at(i);
  }
}
