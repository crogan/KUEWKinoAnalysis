#include <iostream>
#include <map>
#include <TStyle.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TLine.h>
#include <TSystem.h>
#include <TMultiGraph.h>
#include <TEfficiency.h>

#include "FitReader.hh"
#include "shapeComparison.hh"
#include "plotShapeComparison.hh"
#include "RestFrames/RestFrames.hh"

///////////////////////////////////////////
////////// FitReader class
///////////////////////////////////////////

FitReader::FitReader(const string& inputfile,
                     const string& otherfile,
                     const string& otherfold)
        : m_File(inputfile.c_str(), "READ") {


    if(otherfile != ""){
        m_FilePtr = new TFile(otherfile.c_str(), "READ");
        if(!m_FilePtr || !m_FilePtr->IsOpen())
            m_FilePtr = nullptr;
        m_FileFold = otherfold+"/";
    } else {
        m_FilePtr = nullptr;
    }
    ReadProcesses();

    ReadCategories();
}

FitReader::~FitReader(){
    m_File.Close();
}

void FitReader::ReadProcesses(){
    if(!m_File.IsOpen())
        return;

    TTree* tree = (TTree*)m_File.Get("Process");
    if(!tree)
        return;

    m_ProcBranch.InitGet(tree);

    ProcessList ProcSys;

    int N = tree->GetEntries();
    for(int i = 0; i < N; i++){
        tree->GetEntry(i);

        Process p = m_ProcBranch.GetProcess();
        if((p.Name().find("Up") != std::string::npos) ||
           (p.Name().find("Down") != std::string::npos))
            ProcSys += p;
        else
            m_Proc += p;
    }
    if(m_FilePtr) m_Proc += Process("total_background",kBkg);

    delete tree;
    int Nproc = m_Proc.GetN();
    int Nsys  = ProcSys.GetN();
    for(int p = 0; p < Nproc; p++){
        Systematics sys;
        string proc = m_Proc[p].Name();
        for(int s = 0; s < Nsys; s++){
            string label = ProcSys[s].Name();
            if((proc.find("Fakes") == std::string::npos) &&
               (label.find("Fakes") != std::string::npos))
                continue;
            if(label.find(proc+"_") == std::string::npos) continue;
            if(label.find(proc) == 0){
                label.replace(0,proc.length()+1,"");
                if(label.find("Up") != std::string::npos)
                    label.replace(label.find("Up"),2,"");
                if(label.find("Down") != std::string::npos)
                    label.replace(label.find("Down"),4,"");
                sys += Systematic(label);
            }
        }
        if(sys.GetN() > 0)
            m_ProcSys[m_Proc[p]] = sys;
        m_Sys += sys;
    }
}

void FitReader::ReadCategories(){
    if(!m_File.IsOpen())
        return;

    TTree* tree = (TTree*)m_File.Get("Category");
    if(!tree)
        return;

    m_CatBranch.InitGet(tree);

    int N = tree->GetEntries();
    for(int i = 0; i < N; i++){
        tree->GetEntry(i);
        Category cat = m_CatBranch.GetCategory();
        if(m_CatLabel.count(cat.GetLabel()) == 0)
            m_CatLabel[cat.GetLabel()] = true;
        else
            continue;
        m_Cat += cat;
        if(m_Chan.count(cat.Label()) == 0)
            m_Chan[cat.Label()] = CategoryList();
        m_Chan[cat.Label()] += cat;
    }

    delete tree;
}

bool FitReader::HasSystematic(const Process& proc, const Systematic& sys) const {
    if(m_ProcSys.count(proc) == 0)
        return false;

    return m_ProcSys[proc].Contains(sys);
}

double FitReader::Integral(const Category&   cat,
                           const Process&    proc,
                           const Systematic& sys) const {
//cout << "Integral" << endl;
    const TH1D* hist = GetHistogram(cat, proc, sys);
    if(!hist)
        return 0.;
//cout << "Integral end " << hist->Integral() << endl;
    return hist->Integral();
}


TH1D* FitReader::GetIntegralHist(const string& name,
                                 const CategoryList& cats,
                                 const ProcessList&  procs,
                                 const Systematic&   sys) const {
    int Np = procs.GetN();
    int Nc = cats.GetN();

    TH1D* hist = nullptr;
    TH1D* histd = nullptr;

    for(int p = 0; p < Np; p++){
        for(int c = 0; c < Nc; c++){
            if(!IsFilled(cats[c], procs[p], sys))
                continue;

            if(!hist){
                histd = (TH1D*) GetHistogram(cats[c], procs[p], sys)->Clone("dum");
                histd->Rebin(histd->GetNbinsX());
                hist = (TH1D*) new TH1D(name.c_str(),name.c_str(), 1, 0., 1.);
                hist->SetBinContent(1, histd->GetBinContent(1));
                hist->SetBinError(1, histd->GetBinError(1));
                delete histd;
            } else {
                TH1D* dum = (TH1D*) GetHistogram(cats[c], procs[p], sys)->Clone("dum");
                dum->Rebin(dum->GetNbinsX());
                hist->SetBinContent(1, hist->GetBinContent(1)+dum->GetBinContent(1));
                hist->SetBinError(1, sqrt(hist->GetBinError(1)*hist->GetBinError(1)+dum->GetBinError(1)*dum->GetBinError(1)));
                delete dum;
            }
        }
    }

    return hist;
}

TH1D* FitReader::GetAddedHist(const string&       name,
                              const CategoryList& cats,
                              const ProcessList&  procs,
                              const Systematic&   sys) const {
    int Np = procs.GetN();
    int Nc = cats.GetN();
    TH1D* hist = nullptr;

    for(int p = 0; p < Np; p++){
        for(int c = 0; c < Nc; c++){
            if(!IsFilled(cats[c], procs[p], sys))
                continue;
            //if(procs[p].Name() != "total_background" && name.find("plothist_0_") != std::string::npos && p == 0) cout << cats[c].Label()+"_"+cats[c].GetLabel() << " " << procs[p].Name() << endl;
            if(!hist){
                hist = (TH1D*) GetHistogram(cats[c], procs[p], sys)->Clone(name.c_str());
            } else {
                hist->Add(GetHistogram(cats[c], procs[p], sys));
            }
        }
    }
    //if(hist)
    //cout << "Added hist bins: " << hist->GetNbinsX() << endl;

    return hist;
}

vector<double> FitReader::GetAddedHistValues(const CategoryList& cats,
                                             const ProcessList&  procs,
                                             const Systematic&   sys) const {

    const FitBin& fitbin = cats[0].GetFitBin();
    int NBins = fitbin.NBins();

    vector<double> histValues;

    int Np = procs.GetN();
    int Nc = cats.GetN();

    TH1D* hist = nullptr;
    TString name = "dummy";

    for(int p = 0; p < Np; p++){
        for(int c = 0; c < Nc; c++){
            //cout << cats[c].Label()+"_"+cats[c].GetLabel()+"/"+procs[p].Name() << endl;
            if(!IsThere(cats[c], procs[p], sys)){
                //cout << cats[c].Label()+"_"+cats[c].GetLabel()+"/"+procs[p].Name() << endl;
                continue;
            }
            if(!hist){
                hist = (TH1D*) GetHistogram(cats[c], procs[p], sys)->Clone(name);
                //cout << hist->GetTitle() << endl;
                //cout << cats[c].Label()+"_"+cats[c].GetLabel()+"/"+procs[p].Name() << endl;
            }
            else {
                //hist = (TH1D*) GetHistogram(cats[c], procs[p], sys);
                //cout << GetHistogram(cats[c], procs[p], sys)->GetTitle() << endl;
                hist->Add(GetHistogram(cats[c], procs[p], sys));
                //cout << cats[c].Label()+"_"+cats[c].GetLabel()+"/"+procs[p].Name() << endl;
            }
        }
    }
    if(hist){
        int bins = hist->GetNbinsX();
        //if(bins < 10)
        //cout << hist->GetTitle() << endl;

        for(int b = 0; b < bins; b++)
            histValues.push_back(hist->GetBinContent(b+1));

        delete hist;
    }
    else{
        //cout << "Didn't find the histogram!" << endl;
        for (int i = 0; i < NBins; i++)
            histValues.push_back(0.);
    }

    return histValues;
}

vector<double> FitReader::GetAddedHistErrors(const CategoryList &cats,
                                  const ProcessList &procs,
                                  const Systematic &sys) const {

    const FitBin& fitbin = cats[0].GetFitBin();
    int NBins = fitbin.NBins();

    vector<double> histValues;

    int Np = procs.GetN();
    int Nc = cats.GetN();

    TH1D* hist = nullptr;
    TString name = "dummy";

    for(int p = 0; p < Np; p++){
        for(int c = 0; c < Nc; c++){
            if(!IsThere(cats[c], procs[p], sys)){
                continue;
            }
            if(!hist){
                hist = (TH1D*) GetHistogram(cats[c], procs[p], sys)->Clone(name);
            }
            else {
                hist->Add(GetHistogram(cats[c], procs[p], sys));
            }
        }
    }
    if(hist){
        int bins = hist->GetNbinsX();

        for(int b = 0; b < bins; b++) {
            histValues.push_back(hist->GetBinError(b + 1));
            //cout << histValues[b] << endl;
        }
        delete hist;
    }
    else{
        for (int i = 0; i < NBins; i++)
            histValues.push_back(0.);
    }

    return histValues;
}

const TH1D* FitReader::GetHistogram(const Category&   cat,
                                    const Process&    proc,
                                    const Systematic& sys) const {
    if(!IsFilled(cat, proc, sys))
        return nullptr;
    if(!sys){
        string shist = cat.Label()+"_"+cat.GetLabel()+"/"+proc.Name();
        if(!m_FilePtr)
            return m_ProcHist[proc][cat];
        else {
            if (proc.Type() == kData) { //if there is another file, and the histogram is data, get the info from the TGraphAsymmErrors
                TGraphAsymmErrors *gr = (TGraphAsymmErrors *) m_FilePtr->Get(
                        (m_FileFold + cat.Label() + "_" + cat.GetLabel() + "/data").c_str());
                if (gr == nullptr) {
                    m_ProcHist[proc][cat] = (TH1D *) m_FilePtr->Get((m_FileFold + shist).c_str());
                } else { //else if TGraphAsymmErrors is found
                    double x, y;
                    m_ProcHist[proc][cat] = new TH1D(
                            (m_FileFold + cat.Label() + "_" + cat.GetLabel() + "/data").c_str(), "data",
                            (int) gr->GetN(), 0., (double) gr->GetN());
                    for (int i = 0; i < gr->GetN(); i++) {
                        gr->GetPoint(i, x, y);
                        m_ProcHist[proc][cat]->SetBinContent(i + 1, y);
                        m_ProcHist[proc][cat]->SetBinError(i + 1, gr->GetErrorY(i));
                    }
                }
            } else { //else if there is another file but it's not data, just get the histogram
                //cout << "filename: " << (m_FileFold + shist).c_str() << endl;
                m_ProcHist[proc][cat] = (TH1D *) m_FilePtr->Get((m_FileFold + shist).c_str());
                //cout << "saved it" << endl;
            }
        }
        return m_ProcHist[proc][cat];
    } else {
//cout << "GetHistogram end" << endl;
        return (sys.IsUp() ? m_ProcHistSys[proc][sys][cat].first :
                m_ProcHistSys[proc][sys][cat].second);
    }
}

const TH2D* FitReader::GetHistogram2D(const Category&   cat,
                                      const Process&    proc,
                                      const Systematic& sys) const {
    //cout << "Getting histogram!" << endl;
    if(!IsFilled2D(cat, proc, sys)){
        //cout << "hist not filled!" << endl;
        return nullptr;
    }
    //cout << cat.GetLabel() << " " << proc.Name() << " hist integral: " << m_ProcHist_2D[proc][cat]->Integral() << endl;
    if(!sys){
        //cout << "no sys, hist filled" << endl;
        return m_ProcHist_2D[proc][cat];
    } else {
        cout << "sys" << endl;
        return (sys.IsUp() ? m_ProcHistSys_2D[proc][sys][cat].first :
                m_ProcHistSys_2D[proc][sys][cat].second);
    }
}

bool FitReader::IsFilled(const Category&   cat,
                         const Process&    proc,
                         const Systematic& sys) const {
    //cout << "IsFilled" << endl;
    /*if(proc.Type() != kBkg)
        cout << cat.Label()+"_"+cat.GetLabel() << endl;*/
    if(!sys){
        if(m_ProcHist.count(proc) == 0)
            m_ProcHist[proc] = map<Category,TH1D*>();
        if(m_ProcHist[proc].count(cat) == 0){
            string shist = cat.Label()+"_"+cat.GetLabel()+"/"+proc.Name();
            //cout << shist << endl;
            if(!m_FilePtr){ //if there is no file pointer, just take histogram from original file
                m_ProcHist[proc][cat] = (TH1D*) m_File.Get(shist.c_str());
            } else {
                if(proc.Type() == kData){ //if there is another file, and the histogram is data, get the info from the TGraphAsymmErrors
                    TGraphAsymmErrors* gr = (TGraphAsymmErrors*)m_FilePtr->Get((m_FileFold+cat.Label()+"_"+cat.GetLabel()+"/data").c_str());
                    if(gr == nullptr){
                        m_ProcHist[proc][cat] = (TH1D*) m_FilePtr->Get((m_FileFold+shist).c_str());
                    } else { //else if TGraphAsymmErrors is found
                        double x, y;
                        m_ProcHist[proc][cat] = new TH1D((m_FileFold+cat.Label()+"_"+cat.GetLabel()+"/data").c_str(),"data",(int)gr->GetN(),0.,(double)gr->GetN());
                        for(int i = 0; i < gr->GetN(); i++){
                            gr->GetPoint(i,x,y);
                            m_ProcHist[proc][cat]->SetBinContent(i+1,y);
                            m_ProcHist[proc][cat]->SetBinError(i+1,gr->GetErrorY(i));
                        }
                    }
                } else { //else if there is another file but it's not data, just get the histogram
                    //cout << "filename: " << (m_FileFold+shist).c_str() << endl;
                    m_ProcHist[proc][cat] = (TH1D*) m_FilePtr->Get((m_FileFold+shist).c_str());
                    //cout << "saved it" << endl;
                }
            }
        }
        return m_ProcHist[proc][cat];

        //else if there is a systematic in the function args
    } else {
//cout << "do IsFilled w sys" << endl;
        if(m_ProcHistSys.count(proc) == 0)
            m_ProcHistSys[proc] = map<Systematic,map<Category,pair<TH1D*,TH1D*> > >();
//cout << "proc count: " << m_ProcHistSys.count(proc) << endl;   
        if(m_ProcHistSys[proc].count(sys) == 0)
            m_ProcHistSys[proc][sys] = map<Category,pair<TH1D*,TH1D*> >();
//cout << "sys count: " << m_ProcHistSys[proc].count(sys) << endl;   
//cout << "cat count: " << m_ProcHistSys[proc][sys].count(cat) << endl;   
        if(m_ProcHistSys[proc][sys].count(cat) == 0){
            m_ProcHistSys[proc][sys][cat] = pair<TH1D*,TH1D*>(nullptr,nullptr);

            string label = cat.Label()+"_"+cat.GetLabel();
            string shistUp   = label+"/"+proc.Name()+"_"+sys.Label()+"Up";
            string shistDown = label+"/"+proc.Name()+"_"+sys.Label()+"Down";
            m_ProcHistSys[proc][sys][cat].first  = (TH1D*) m_File.Get(shistUp.c_str());
            m_ProcHistSys[proc][sys][cat].second = (TH1D*) m_File.Get(shistDown.c_str());
        }

//   cout << "up hist: " << m_ProcHistSys[proc][sys][cat].first->GetName() << endl;  
//   cout << "down hist: " << m_ProcHistSys[proc][sys][cat].second->GetName() << endl;  
//   cout << "IsFilled end" << endl; 
        return (sys.IsUp() ? m_ProcHistSys[proc][sys][cat].first :
                m_ProcHistSys[proc][sys][cat].second);
    }
}

bool FitReader::IsThere(const Category&   cat,
                        const Process&    proc,
                        const Systematic& sys) const {
    TH1D* hist = nullptr;
    TH1D* hist2 = nullptr;
    if(!sys){
        // if(m_ProcHist.count(proc) == 0)
        // m_ProcHist[proc] = map<Category,TH1D*>();
        // if(m_ProcHist[proc].count(cat) == 0){
        string shist = cat.Label()+"_"+cat.GetLabel()+"/"+proc.Name();
        if(proc.Type() == kData || !m_FilePtr)
            hist = (TH1D*) m_File.Get(shist.c_str());
        else
            hist = (TH1D*) m_FilePtr->Get((m_FileFold+shist).c_str());

        if(hist){
            delete hist;
            return true;
        } else
            return false;
        //   }

        return m_ProcHist[proc][cat];

    } else {
        // if(m_ProcHistSys.count(proc) == 0)
        //   m_ProcHistSys[proc] = map<Systematic,map<Category,pair<TH1D*,TH1D*> > >();
        // if(m_ProcHistSys[proc].count(sys) == 0)
        //   m_ProcHistSys[proc][sys] = map<Category,pair<TH1D*,TH1D*> >();
        // if(m_ProcHistSys[proc][sys].count(cat) == 0){
        //   m_ProcHistSys[proc][sys][cat] = pair<TH1D*,TH1D*>(nullptr,nullptr);

        string label = cat.Label()+"_"+cat.GetLabel();
        string shistUp   = label+"/"+proc.Name()+"_"+sys.Label()+"Up";
        string shistDown = label+"/"+proc.Name()+"_"+sys.Label()+"Down";

//      hist  = m_ProcHistSys[proc][sys][cat].first  = (TH1D*) m_File.Get(shistUp.c_str());
//      hist2 = m_ProcHistSys[proc][sys][cat].second = (TH1D*) m_File.Get(shistDown.c_str());
        hist  = (TH1D*) m_File.Get(shistUp.c_str());
        hist2 = (TH1D*) m_File.Get(shistDown.c_str());

        bool there = true;
        if(hist)
            delete hist;
        else
            there = false;
        if(hist2)
            delete hist2;
        else
            there = false;
        return there;
    }

    return (sys.IsUp() ? m_ProcHistSys[proc][sys][cat].first :
            m_ProcHistSys[proc][sys][cat].second);
}


bool FitReader::IsFilled2D(const Category&   cat,
                           const Process&    proc,
                           const Systematic& sys) const {

    if(!sys){
        if(m_ProcHist_2D.count(proc) == 0)
            m_ProcHist_2D[proc] = map<Category,TH2D*>();
        if(m_ProcHist_2D[proc].count(cat) == 0){
            string shist = cat.Label()+"_"+cat.GetLabel()+"/"+proc.Name()+"_2D";
            if(proc.Type() == kData || !m_FilePtr)
                m_ProcHist_2D[proc][cat] = (TH2D*) m_File.Get(shist.c_str());
            else
                m_ProcHist_2D[proc][cat] = (TH2D*) m_FilePtr->Get((m_FileFold+shist).c_str());
        }

        return m_ProcHist_2D[proc][cat];

    } else {
        if(m_ProcHistSys_2D.count(proc) == 0)
            m_ProcHistSys_2D[proc] = map<Systematic,map<Category,pair<TH2D*,TH2D*> > >();
        if(m_ProcHistSys_2D[proc].count(sys) == 0)
            m_ProcHistSys_2D[proc][sys] = map<Category,pair<TH2D*,TH2D*> >();
        if(m_ProcHistSys_2D[proc][sys].count(cat) == 0){
            m_ProcHistSys_2D[proc][sys][cat] = pair<TH2D*,TH2D*>(nullptr,nullptr);

            string label = cat.Label()+"_"+cat.GetLabel();
            string shistUp   = label+"/"+proc.Name()+"_"+sys.Label()+"Up_2D";
            string shistDown = label+"/"+proc.Name()+"_"+sys.Label()+"Down_2D";

            m_ProcHistSys_2D[proc][sys][cat].first  = (TH2D*) m_File.Get(shistUp.c_str());
            m_ProcHistSys_2D[proc][sys][cat].second = (TH2D*) m_File.Get(shistDown.c_str());
        }

        return (sys.IsUp() ? m_ProcHistSys[proc][sys][cat].first :
                m_ProcHistSys[proc][sys][cat].second);
    }
}

void FitReader::PrintCategories(bool verbose){
    cout << "*** Fit Categories ***" << endl;
    int N = m_Cat.GetN();
    for(int i = 0; i < N; i++)
        cout << m_Cat[i].Label()+"_"+m_Cat[i].GetLabel() << endl;
    cout << endl;
}

void FitReader::PrintProcesses(bool verbose){
    cout << "*** Fit Processes ***" << endl;
    int N = m_Proc.GetN();
    for(int i = 0; i < N; i++){
        cout << m_Proc[i].Name() << endl;
        if(m_ProcSys.count(m_Proc[i]) == 0 || !verbose)
            continue;
        cout << "  Sys:  " << m_ProcSys[m_Proc[i]][0].Label() << endl;
        int Nsys = m_ProcSys[m_Proc[i]].GetN();
        for(int s = 1; s < Nsys; s++)
            cout << "        " << m_ProcSys[m_Proc[i]][s].Label() << endl;
    }
    cout << endl;
}

const ProcessList&  FitReader::GetProcesses() const {
    return m_Proc;
}

const CategoryList& FitReader::GetCategories(const string& channel) const {
    if(m_Chan.count(channel) > 0)
        return m_Chan[channel];

    return m_Cat;
}

VS FitReader::GetChannels() const {
    VS list;
    for(auto c : m_Chan)
        list += c.first;

    return list;
}

const Systematics& FitReader::GetSystematics() const {
    return m_Sys;
}

string FitReader::GetSignalTitle(const string& label){
    size_t p = label.rfind("_");
    if(p == std::string::npos)
        return label;

    string title = label.substr(0, p);
    int    mass  = stoi(label.substr(p+1,label.length()-p));
    return title+" "+std::to_string(mass/10000)+" "+std::to_string(mass%100000);
}

double FitReader::CalculateZbi(double Nsig, double Nbkg, double deltaNbkg){
    double Nobs = Nsig+Nbkg;
    double tau = 1./Nbkg/(deltaNbkg*deltaNbkg);
    double aux = Nbkg*tau;
    double Pvalue = TMath::BetaIncomplete(1./(1.+tau),Nobs,aux+1.);
    double sigma = sqrt(2.)*TMath::ErfcInverse(Pvalue*2);

    return (isnan(sigma))?0:sigma;
}

double FitReader::SuperBinValue(vector<double> sig_yields, vector<double> bkg_yields, double sys, vector<string> cat_label, bool verbose){

    double zbi = -999.;
    double max_zbi = -999.;

    map<double, int> sig_map;
    map<double, int>::reverse_iterator ritr;

    for(unsigned int i = 0; i < bkg_yields.size(); i++){
        zbi = CalculateZbi(sig_yields[i], bkg_yields[i], sys);
        sig_map.insert(std::make_pair(zbi,i));
    }
    /*
    if(verbose){
      cout << "\nInput Summary: \n";
      cout << "index" << "\tNbkg" << "\tNsig" << "\tZbi" << "\t" << "\tCategory" << endl;
      for (ritr = sig_map.rbegin(); ritr != sig_map.rend(); ++ritr) {
        cout << ritr->second << '\t' << bkg_yields[ritr->second] << '\t'
         << sig_yields[ritr->second] << '\t' << ritr->first << '\t' << cat_label[ritr->second]
             << '\n';
      }
      cout << endl;
    }
    */
    vector<int> idx_order;

    for (ritr = sig_map.rbegin(); ritr != sig_map.rend(); ++ritr)
        idx_order.push_back(ritr->second);

    max_zbi = sig_map.rbegin()->first;

    double bkg_sum = bkg_yields[idx_order[0]];
    double sig_sum = sig_yields[idx_order[0]];

    vector<int> bins_combined;
    bins_combined.push_back(idx_order[0]);

    for(unsigned int i = 1; i < idx_order.size(); i++){

        bkg_sum += bkg_yields[idx_order[i]];
        sig_sum += sig_yields[idx_order[i]];
        zbi = CalculateZbi(sig_sum, bkg_sum, sys);

        if(zbi < max_zbi){
            bkg_sum -= bkg_yields[idx_order[i]];
            sig_sum -= sig_yields[idx_order[i]];
            continue;
        }
        else{
            max_zbi = zbi;
            bins_combined.push_back(idx_order[i]);
        }
    }
    //print bin indices that were combined
    if(verbose /*&& max_zbi > 0. && bins_combined.size() > 1*/){
        cout << "\nInput Summary: \n";
        cout << "index" << "\tNbkg" << "\tNsig" << "\tZbi" << "\t" << "\tCategory" << endl;
        for (ritr = sig_map.rbegin(); ritr != sig_map.rend(); ++ritr) {
            cout << ritr->second << '\t' << bkg_yields[ritr->second] << '\t'
                 << sig_yields[ritr->second] << '\t' << ritr->first << '\t' << cat_label[ritr->second]
                 << '\n';
        }
        cout << endl;
        cout << "Bins combined: ";
        for(unsigned int i = 0; i < bins_combined.size(); i++){
            if(i < bins_combined.size()-1)
                cout << bins_combined[i] << ", ";
            else
                cout << bins_combined[i] << ".";
        }
        cout << endl
             << "Total Background Yield: " << bkg_sum << endl
             << "Total Signal Yield: "     <<       sig_sum << endl
             << "Combined Bin Zbi: " << max_zbi << endl
             << endl;
    }

    return max_zbi;
}

std::tuple<int, int, std::string> FitReader::SigMass(const std::string sig) const{
    int parent_mass = -999;
    int child_mass = -999;

    std::string sigType = sig.substr(0, sig.find("_"));
    std::string mass_diff = sig.substr(sig.find("_") + 1);
    int size = mass_diff.size();

    if(size == 7){
        parent_mass = std::stoi(mass_diff.substr(0,3));
        child_mass = std::stoi(mass_diff.substr(3,size));
    }

    if(size == 8){
        parent_mass = std::stoi(mass_diff.substr(0,4));
        child_mass = std::stoi(mass_diff.substr(4,size));
    }

    return std::make_tuple(parent_mass, child_mass, sigType);
}

int nthOccurrence(const std::string& str, const std::string& findMe, int nth){
    size_t  pos = 0;
    int     cnt = 0;

    while(cnt != nth){
        pos++;
        pos = str.find(findMe, pos);
        if (pos == std::string::npos)
            return -1;
        cnt++;
    }
    return pos;
}

VS FitReader::GetSignalProcs(const int min_mass_diff, const int max_mass_diff,
                             const int exclude_below, const int exclude_above) const{

    VS sig_procs;

    TList* fileList = m_File.GetListOfKeys();

    TIter ele1(fileList);
    ele1.Next();
    TKey* key1 = (TKey*)ele1();
    TObject* dir1 = key1->ReadObj();

    auto dir = (TDirectory*)dir1;
    TList* ls = dir->GetListOfKeys() ;

    if (!ls) {
        printf("<E> No keys found in file\n");
        exit(1);
    }

    TIter next(ls);
    TKey* key;
    TObject* obj;

    while ((key = (TKey*)next())) {
        obj = key->ReadObj();
        string title = obj->GetTitle();
        if(title.find("_T") != std::string::npos && title.find("_TB") == std::string::npos){
            int pos1 = title.find("_T")+1;
            int pos2 = nthOccurrence(title, "_", 3);
            string sigTypeMass = title.substr(pos1, pos2-pos1);

            auto mass_tuple = SigMass(sigTypeMass);
            int parent_mass = std::get<0>(mass_tuple);
            int child_mass = std::get<1>(mass_tuple);
            int mass_diff = parent_mass-child_mass;

            if(parent_mass >= exclude_below && parent_mass <= exclude_above && mass_diff >= min_mass_diff && mass_diff <= max_mass_diff){
                sig_procs.a(sigTypeMass);

            }
        }
    }

    return sig_procs;
}

/*SuperBin FitReader::GetSuperBin(const VS& proc_bkg,
                                const VS& proc_sig,
                                const CategoryTree& CT,
                                const vector<std::tuple<string, string, string>>& binIDs) {

    int binSize = int(binIDs.size());
    vector<string> categoryIDs(binSize), RISRIDs(binSize), MperpIDs(binSize);
    for(int i=0; i < binSize; i++){
        categoryIDs[i] = std::get<0>(binIDs[i]);
        RISRIDs[i] = std::get<1>(binIDs[i]);
        MperpIDs[i] = std::get<2>(binIDs[i]);
    }

    RestFrames::SetStyle();

    CategoryList CatList = GetCategories();

    vector<const CategoryTree*> CatTrees;
    CT.GetListDeepest(CatTrees);

    int Nbkg = int(proc_bkg.size());
    int Nsig = int(proc_sig.size());
    int Nvis = int(CatTrees.size());
    int Ncat = CatList.GetN();

    int Depth = CT.GetDepth();

    ProcessList bkgs = FetchProcs(proc_bkg);
    ProcessList sigs = FetchProcs(proc_sig);

    Nsig = sigs.GetN();
    Nbkg = bkgs.GetN();

    return SuperBin(VI(), 0, 0, VS(), VS(), VS());
}*/
