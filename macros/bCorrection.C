//#include "RestFrames/RestFrames.hh"
#include "../include/SampleTool.hh"
#include "../include/Process.hh"
#include "../include/BtagSFTool.hh"
#include "RestFrames/RestFrames.hh"

using namespace std;
using namespace RestFrames;



void GetEffHists(std::vector<std::string>& flist, std::vector<TH1D*>& nums, std::vector<TH1D*>& dens, std::string flavor){

 std::string hist_str1 = "Histograms/hist_btag_flavor";
 std::string hist_str2 = "_num";
 std::string hist_str_num = hist_str1 + flavor + hist_str2;
 hist_str2 = "_den";
 std::string hist_str_den = hist_str1 + flavor + hist_str2; 

 TFile* f;
 for(int i =0; i<flist.size(); i++){
   f = TFile::Open(flist[i].c_str());
   nums[i] = (TH1D*) f->Get(hist_str_num.c_str());
   dens[i] = (TH1D*) f->Get(hist_str_den.c_str()); 
   nums[i]->SetDirectory(0);
   dens[i]->SetDirectory(0);
   //nums[i]->Draw("SAME");
   f->Close();
 } 
 
}

TEfficiency* joinEfficiencies( std::vector<TH1D*>& nums, std::vector<TH1D*>& dens ){
	
  for(int i=1; i<nums.size(); i++){
    nums[0]->Add(nums[i]);
    dens[0]->Add(dens[i]);
  }
  TEfficiency* teff = new TEfficiency(*nums[0], *dens[0]);
  return teff;
}

TEfficiency* loadBaseEfficiencies(std::string year, std::string flavor){

  TFile* f;
  f = TFile::Open("../BtagSF/BtagEff.root");
  TEfficiency* effT;
  std::string s1 = "BtagEff_";
  std::string s2 = "_flavor";
  std::string s = s1+year+s2+flavor;
  effT = (TEfficiency*) f->Get(s.c_str());
  effT->SetDirectory(0);
  f->Close(); 
  return effT;
  
}

TH1D* getProcSF(std::string proc,BtagSFTool& btool, TEfficiency* effT, TEfficiency* effP, int year, int flav, int updown, bool FastSim, std::string proc2=""){

  //get bin centers and Nbins
  TH1D* hEff = (TH1D*)effT->GetTotalHistogram();
  TH1D* hlam = (TH1D*)hEff->Clone();
  std::string hname = "hlam"+ std::to_string(year)+"_flav"+ std::to_string(flav);
  std::string htitle ="Process SF #lambda "+std::to_string(year)+" flavor"+std::to_string(flav);
  hlam->SetName(hname.c_str());
  hlam->SetTitle(htitle.c_str());
  int nbin = hEff->GetNbinsX();
  double mean, stddev;
  mean = 0.;
  stddev =0.;
  std::vector<double> vlam{};
  double pt,SF,eT,eP,lam,errT,errP,ddT,ddP,errlam;
  for(int i=1; i<=nbin; i++){
    pt = hEff->GetXaxis()->GetBinCenter(i);
    SF = btool.SF(pt, year, flav, updown, FastSim); 
    eT = effT->GetEfficiency(i);
    eP = effP->GetEfficiency(i);
    lam = ((1.-SF*eP)*(1-eT))/((1-eP)*(1-SF*eT));
    errT = effT->GetEfficiencyErrorUp(i);
    errP = effP->GetEfficiencyErrorUp(i);
    ddP = ((SF-1)*(eT-1))/((1-eP)*(1-eP)*(SF*eT-1));
    ddT = ((SF-1)*(SF*eP-1))/((eP-1)*(1-SF*eT)*(1-SF*eT));
    errlam = sqrt(errT*errT*ddT*ddT + errP*errP*ddP*ddP );
    std::cout<<"(pt,SF,effT,effP,lam,err): "<<pt<<" "<<SF<<" "<<eT<<" "<<eP<<" "<<lam<<" "<<errlam<<"\n";
    hlam->SetBinContent(i,lam);
    hlam->SetBinError(i,errlam);
    mean += lam;
    vlam.push_back(lam);
  }
  mean = mean/double(nbin);
  for(int i=0; i<vlam.size(); i++){
	stddev += (vlam[i] - mean)*(vlam[i] - mean);
  }
  if(proc2 != "")
	proc = proc2;
  stddev = sqrt(stddev/double(nbin));
  std::cout<<"Process: "<<proc<<" "<<year<<" flavor"<<flav<<" U/D:"<<updown<<" Fast:"<<FastSim << " Mean lambda = "<<mean<< " +/- " << stddev <<"\n";  

  return hlam;
}

void bCorrection(std::string Proc, int year, int flav, int updown, bool FastSim ){

  std::string NtuplePath = "root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_v2/";
//  int year = 2017;
//  int flav = 1;
//  std::string flavor = "1";
//  std::string Proc = "Wjets";
  std::string yearstr = std::to_string(year);
  std::string flavstr = std::to_string(flav);

 //
 int PTINDEX=-1;
 bool isWjets = false;
 std::string proc2;
 proc2 = Proc;
 if( Proc.find("Wjets") != std::string::npos && (year == 2017 || year ==2018)){
	isWjets = true;
	if(Proc.find("Pt-100To250") != std::string::npos){
		PTINDEX=0;
	}
	if(Proc.find("Pt-250To400") != std::string::npos){
                PTINDEX=1;
        }
	if(Proc.find("Pt-400To600") != std::string::npos){
                PTINDEX=2;
        }
	if(Proc.find("Pt-50To100") != std::string::npos){
                PTINDEX=3;
        }
	if(Proc.find("Pt-600ToInf") != std::string::npos){
		PTINDEX=4;
	}
	proc2 = "Wjets";
	
  } 

  SampleTool ST(NtuplePath, year);
  ProcessList plist = ST.Get(proc2);

  plist.Print();
  int NFiles = ST.NTrees(plist[0]);

  std::string fname{};
  std::vector<std::string> flist{};
  //std::vector<TH1D*> nums(NFiles);
  //std::vector<TH1D*> dens(NFiles);

  for(int i =0; i<NFiles; i++){
    fname = ST.FileName(plist[0], i);
    std::cout<<fname<<"\n";
    flist.push_back(fname);
  }

  if(isWjets){
	std::cout<<"found Wjets, loading slice "<< flist[PTINDEX]<<"\n";
	std::string temp = flist[PTINDEX];
	flist.clear();
	flist.push_back(temp);
	NFiles=1;
  }
  std::cout<<"files to be processed for: "<<Proc<<" "<<proc2<<"\n";
  for(int i=0; i<NFiles; i++){
	std::cout<<flist[i]<<"\n";
  }
  std::vector<TH1D*> nums(NFiles);
  std::vector<TH1D*> dens(NFiles); 

  GetEffHists(flist, nums, dens, flavstr);
 // nums[0]->Draw();
  TEfficiency* effP = joinEfficiencies(nums,dens);
 // nums[0]->Draw();
 // effP->Draw();
  
  TEfficiency* effT = loadBaseEfficiencies(yearstr, flavstr);
  //effT->Draw("SAMES");
 
  BtagSFTool btool;
  btool.BuildMap("../BtagSF/");
 // double testSF;
 // int updown = 0;
 // bool FastSim = false;
 /* for(int i=1; i<200; i=i+10){
    testSF = btool.SF(i, year, flav, updown, FastSim);
    std::cout<<"pt "<<i<<" SF: "<< testSF << "   ";
  }
*/
  TCanvas* c1 = new TCanvas("c1","c1");
  TH1D* hlam;
  if(isWjets)
	  hlam = getProcSF(proc2,btool,effT,effP,year,flav,updown,FastSim,Proc);
  else
  hlam = getProcSF(proc2,btool,effT,effP,year,flav,updown,FastSim);
  hlam->GetYaxis()->SetRangeUser(0.98, 1.02);
  hlam->Fit("pol0");
  hlam->Draw();
  
  
  std::string outname;
  outname = "../bCorrection/lambda_"+Proc+"_"+yearstr+"_flavor"+flavstr+".pdf";
  c1->SaveAs(outname.c_str()); 

}
