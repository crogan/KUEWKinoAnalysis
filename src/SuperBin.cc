#include "SuperBin.hh"

//SuperBin Class
//Constructor
SuperBin::SuperBin(const VI &index, const double nSig, const double nBkg, const VS &mPerpLabel, const VS &rIsrLabel,
                   const VS &visLabel) :
        index_(index),
        nSig_(nSig),
        nBkg_(nBkg),
        sOverB_(nSig / nBkg),
        mPerpLabel_(mPerpLabel),
        rIsrLabel_(rIsrLabel),
        visLabel_(visLabel) {}

SuperBin::~SuperBin() = default;

VI SuperBin::getIndex() const {
    return index_;
}

double SuperBin::GetNsig() const {
    return nSig_;
}

double SuperBin::GetNbkg() const {
    return nBkg_;
}

double SuperBin::GetSoverB() const {
    return sOverB_;
}

double SuperBin::GetBinZbi(const double sys) const {
    double zBi = 0.;
    if (nBkg_ > 0.)
        zBi = FitReader::CalculateZbi(nSig_, nBkg_, sys);
    return zBi;
}

VS SuperBin::GetMperpLabel() const {
    return mPerpLabel_;
}

VS SuperBin::GetRisrLabel() const {
    return rIsrLabel_;
}

VS SuperBin::GetVisLabel() const {
    return visLabel_;
}

SuperBin *SuperBin::TryMerge(SuperBin *superBin, double sys) const {

    double myZbi = this->GetBinZbi(sys);
    double theirZbi = superBin->GetBinZbi(sys);

    double nSigTotal = this->GetNsig() + superBin->GetNsig();
    double nBkgTotal = this->GetNbkg() + superBin->GetNbkg();

    double combinedZbi = 0.;

    if (nBkgTotal > 0)
        combinedZbi = FitReader::CalculateZbi(nSigTotal, nBkgTotal, sys);

    if (combinedZbi > myZbi && combinedZbi > theirZbi) {
        VI idxList = this->getIndex() += superBin->getIndex();
        VS combinedMperpLabels = this->GetMperpLabel() += superBin->GetMperpLabel();
        VS combinedRisrLabels = this->GetRisrLabel() += superBin->GetRisrLabel();
        VS combinedVisLabels = this->GetVisLabel() += superBin->GetVisLabel();
        auto mergedSuperBin = new SuperBin(idxList, nSigTotal, nBkgTotal, combinedMperpLabels, combinedRisrLabels,
                                           combinedVisLabels);
        return mergedSuperBin;
    } else
        return nullptr;
}

SuperBin *SuperBin::ForceMerge(SuperBin *superBin, double sys) const {

    double nSigTotal = this->GetNsig() + superBin->GetNsig();
    double nBkgTotal = this->GetNbkg() + superBin->GetNbkg();

    VI idxList = this->getIndex() += superBin->getIndex();
    VS combinedMperpLabels = this->GetMperpLabel() += superBin->GetMperpLabel();
    VS combinedRisrLabels = this->GetRisrLabel() += superBin->GetRisrLabel();
    VS combinedVisLabels = this->GetVisLabel() += superBin->GetVisLabel();
    auto mergedSuperBin = new SuperBin(idxList, nSigTotal, nBkgTotal, combinedMperpLabels, combinedRisrLabels,
                                       combinedVisLabels);

    return mergedSuperBin;
}

void SuperBin::PrintSummary(double sys) const {

    std::cout << "Category: " << visLabel_[0] << endl;
    std::cout << "Mperp: " << mPerpLabel_[0] << ", RISR: " << rIsrLabel_[0] << endl;
    std::cout << "Background: " << nBkg_ << ", Signal: " << nSig_ << "Zbi: " << GetBinZbi(sys) << endl;
    std::cout << endl;

}

//SuperBinList Class
//Default constructor
SuperBinList::SuperBinList() :
        listSize_(0) {
    isSorted_ = false;
}

//Construct superBinList from vector of superBin objects
SuperBinList::SuperBinList(const std::vector<SuperBin *> &superBinList) {
    isSorted_ = false;
    for (auto superBin: superBinList)
        *this += superBin;
    listSize_ = int(this->size());
}

SuperBinList::~SuperBinList() = default;

SuperBinList &SuperBinList::operator+=(SuperBin *superBin) {
    this->push_back(superBin);
    listSize_ = int(this->size());
    return *this;
}

SuperBinList &SuperBinList::operator+=(const SuperBinList &superBinList) {
    for (auto superBin: superBinList)
        this->push_back(superBin);
    listSize_ = int(this->size());
    return *this;
}

void SuperBinList::PrintSummary(const double sys, bool concise) {

    std::vector<int> idxVec = sortIndexByZbi(sys);

    //int sb_size = this->size();

    int count = 0;
    cout << "Summary for bins in Region " << identifier_ << " (" << listSize_ << " processes): " << endl;
    cout << "bins combined" << "\tsignal yield" << "\tbackground yield" << "\tZbi" << endl;
    for (auto i: idxVec) {

        if (count == 5 && concise)
            break;

        cout << "\t" << this->at(i)->getIndex().size() << "\t";
        cout << this->at(i)->GetNsig() << "\t\t"
             << this->at(i)->GetNbkg() << "\t\t\t" << this->at(i)->GetBinZbi(sys) << endl;

        VS mperp_label = this->at(i)->GetMperpLabel();
        VS risr_label = this->at(i)->GetRisrLabel();
        cout << "\t\tMperp RISR bin: " << "(RISR:" << risr_label[0] << ", " << "Mperp:" << mperp_label[0] << ")"
             << endl;

        VS vis_label = this->at(i)->GetVisLabel();
        if (vis_label.size() > 1) {
            cout << "\t\tcategories combined: ";
            for (int j = 0; j < vis_label.size(); j++) {
                if (j > 0 && j % 3 == 0)
                    cout << "\n\t\t\t\t     ";
                if (j < vis_label.size() - 1)
                    cout << vis_label[j] << ", ";
                else
                    cout << vis_label[j];
            }
            cout << "\n\n";
        } else
            cout << "\t\tmax Zbi category: " << vis_label[0] << "\n\n";
        count++;
    }
}

void SuperBinList::PrintSummaryVis(const double sys, bool concise) {

    std::vector<int> idxVec = sortIndexByZbi(sys);

    int count = 0;
    cout << "Summary for bins in Region " << identifier_ << " (" << listSize_ << " processes): " << endl;
    cout << "bins combined" << "\tsignal yield" << "\tbackground yield" << "\tZbi" << endl;
    for (auto i: idxVec) {

        if (count == 5 && concise)
            break;

        cout << "\t" << this->at(i)->getIndex().size() << "\t";
        cout << this->at(i)->GetNsig() << "\t\t"
             << this->at(i)->GetNbkg() << "\t\t\t" << this->at(i)->GetBinZbi(sys) << endl;

        VS vis_label = this->at(i)->GetVisLabel();
        cout << "\t\tCategory: " << vis_label[0] << endl;

        VS mperp_label = this->at(i)->GetMperpLabel();
        VS risr_label = this->at(i)->GetRisrLabel();
        if (mperp_label.size() > 1 && risr_label.size() > 1) {
            cout << "\t\tbins combined: ";
            for (int j = 0; j < mperp_label.size(); j++) {
                if (j > 0 && j % 3 == 0)
                    cout << "\n\t\t\t       ";
                if (j < mperp_label.size() - 1)
                    cout << "(RISR:" << risr_label[j] << ", " << "Mperp:" << mperp_label[j] << "), ";
                else
                    cout << "(RISR:" << risr_label[j] << ", " << "Mperp:" << mperp_label[j] << ")";
            }
            cout << "\n\n";
        } else
            cout << "\t\tmax Zbi bin: " << "(RISR:" << risr_label[0] << ", " << "Mperp:" << mperp_label[0] << ")"
                 << "\n\n";
        count++;
    }
}

void SuperBinList::PrintSummaryExplicit(double sys, bool concise) {

    std::vector<int> idxVec = sortIndexByZbi(sys);

    int count = 0;
    cout << "Summary for bins in Region " << identifier_ << " (" << listSize_ << " processes): " << endl;
    //cout << "bins combined" << "\tsignal yield" << "\tbackground yield" << "\tZbi" << endl;
    for (auto i: idxVec) {

        if (count == 5 && concise)
            break;

        cout << "bins combined" << "\tsignal yield" << "\tbackground yield" << "\tZbi" << endl;
        cout << "\t" << this->at(i)->getIndex().size() << "\t";
        cout << this->at(i)->GetNsig() << "\t\t"
             << this->at(i)->GetNbkg() << "\t\t\t" << this->at(i)->GetBinZbi(sys) << endl;

        VI index = this->at(i)->getIndex();
        VS m_label = this->at(i)->GetMperpLabel();
        VS r_label = this->at(i)->GetRisrLabel();
        VS v_label = this->at(i)->GetVisLabel();
        int index_size = int(index.size());

        if (index_size == 1) {
            cout << "Category: " << v_label[0] << ", Mperp: " << m_label[0] << ", RISR: " << r_label[0] << endl;
        } else if (index_size > 1) {
            cout << "Categories combined: ";
            for (int j = 0; j < index_size; j++) {
                if (j > 0)
                    cout << "\t\t     ";
                cout << v_label[j] << ", Mperp: " << m_label[j] << ", RISR: " << r_label[j]
                     /*<< " (background: " << this->at(j)->GetNbkg() << ", signal: " << this->at(j)->GetNsig()
                     << ", Zbi: " << this->at(j)->GetBinZbi(sys) << ")"*/ << endl;
            }
        }
        cout << endl;
        count++;
    }
}

void SuperBinList::SetIdentifier(const string &ID) {
    identifier_ = ID;
}

void SuperBinList::sortByZbi(const double sys) {
    isSorted_ = true;
    std::sort(this->begin(), this->end(), [&sys](const SuperBin *lhs, const SuperBin *rhs) {
        return lhs->GetBinZbi(sys) >
               rhs->GetBinZbi(sys);
    });
}

void SuperBinList::sortBySoverB() {
    isSorted_ = true;
    std::sort(this->begin(), this->end(), [](const SuperBin *lhs, const SuperBin *rhs) {
        return lhs->GetSoverB() >
               rhs->GetSoverB();
    });
}

std::vector<int> SuperBinList::sortIndexByZbi(const double sys) const {
    std::vector<int> indexVec;
    std::vector<double> vecZbi;

    for (int i = 0; i < this->size(); i++) {
        indexVec.push_back(i);
        vecZbi.push_back(this->at(i)->GetBinZbi(sys));
    }

    std::sort(indexVec.begin(), indexVec.end(),
              [&vecZbi](const int lhs, const int rhs) { return vecZbi[lhs] > vecZbi[rhs]; });
    return indexVec;
}

void SuperBinList::PlotListZbi(const string& name, const double sys) {

    int nBins = int(this->size());
    double max = this->GetMaxZbi(sys);
    TH1F *fhist = new TH1F(Form("fhist_%s", name.c_str()), Form("fhist_%s", name.c_str()), nBins, 0., double(nBins));

    for (int b = 1; b < nBins + 1; b++)
        fhist->SetBinContent(b, this->at(b - 1)->GetBinZbi(sys));

    gROOT->SetBatch(kTRUE);
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(11111111);

    auto can = new TCanvas(Form("can_%s", name.c_str()),
                           Form("can_%s", name.c_str()),
                           1200, 700);

    double yline = can->GetBottomMargin() - 0.02;
    Float_t hlo = 0.1;
    Float_t hhi = 0.25;
    Float_t hto = 0.07;
    Float_t hbo = 0.19;

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
    fhist->GetYaxis()->SetRangeUser(0., max * 1.35);
    fhist->GetYaxis()->SetTitle("Z_{bi}");

    double eps = 0.0015;

    TLatex l1, l2;
    l1.SetTextFont(42);
    l1.SetNDC();
    l1.SetTextSize(Float_t(std::min(0.03, 1.5 / double(nBins))));
    l1.SetTextFont(42);
    l1.SetTextAlign(32);
    l1.SetTextAngle(90);

    l2.SetTextFont(42);
    l2.SetNDC();
    l2.SetTextSize(Float_t(std::min(0.03, 1.5 / double(nBins))));
    l2.SetTextFont(60);
    l2.SetTextAlign(32);

    for (int b = 0; b < nBins; b++) {
        string label;
        double xpos, ypos1, ypos2;
        label = this->at(b)->GetVisLabel()[0];
        xpos = hlo + (1. - hhi - hlo) / double(nBins) * (0.5 + b);
        ypos1 = yline + 0.01;
        ypos2 = 1 - hto - 4 * eps;
        l2.DrawLatex(xpos, ypos1, std::to_string(this->at(b)->getIndex().size()).c_str());
        l1.SetTextColor(7004 + 10 * ((b % nBins) % 8));
        l1.DrawLatex(xpos, ypos2, label.c_str());
    }

    string m_CMSLabel = "#bf{#it{CMS}} work-in-progress";

    TLatex l3;
    l3.SetTextFont(42);
    l3.SetNDC();

    auto line = new TLine();
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
    l3.DrawLatex(1. - hhi - eps * 4, 1. - hto + 0.02, string("Regions " + identifier_).c_str());
    l3.SetTextAlign(11);
    l3.SetTextSize(0.04);
    l3.SetTextFont(42);
    l3.DrawLatex(hlo + eps * 4, 1. - hto + 0.02, m_CMSLabel.c_str());
    l3.SetTextSize(0.05);

    can->Update();
    can->SaveAs((name + ".pdf").c_str());
}

void SuperBinList::PlotListZbiMR(const string& name, const double sys, const FitBin &fitbin) {

    int nBins = int(this->size());
    double max = this->GetMaxZbi(sys);
    auto fhist = new TH1F(Form("fhist_%s", name.c_str()), Form("fhist_%s", name.c_str()), nBins, 0., double(nBins));

    for (int b = 1; b < nBins + 1; b++)
        fhist->SetBinContent(b, this->at(b - 1)->GetBinZbi(sys));

    gROOT->SetBatch(kTRUE);
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(11111111);

    auto can = new TCanvas(Form("can_%s", name.c_str()),
                           Form("can_%s", name.c_str()),
                           1200, 700);

    //double yline = can->GetBottomMargin() - 0.02;
    Float_t hlo = 0.1;
    Float_t hhi = 0.25;
    Float_t hto = 0.07;
    Float_t hbo = 0.19;

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
    fhist->GetYaxis()->SetRangeUser(0., max * 1.35);
    fhist->GetYaxis()->SetTitle("Z_{bi}");

    double eps = 0.0015;

    TLatex l1;
    l1.SetTextFont(42);
    l1.SetNDC();
    l1.SetTextSize(Float_t(std::min(0.03, 1.5 / double(nBins))));
    l1.SetTextFont(42);
    l1.SetTextAlign(32);

    for (int b = 0; b < nBins; b++) {
        double xpos, ypos;
        xpos = hlo + (1. - hhi - hlo) / double(nBins) * (0.5 + b);
        ypos = 1 - hto - 20 * eps;
        l1.DrawLatex(xpos, ypos, std::to_string(this->at(b)->getIndex().size()).c_str());
        //l1.SetTextColor(7004 + 10*((b%nBins)%8));
    }

    TLatex l;
    l.SetTextFont(42);
    l.SetNDC();

    int NR = fitbin.NRBins();
    int NB = fitbin.NBins();

    VS mlabels;
    VS rlabels;
    for (int r = 0; r < NR; r++)
        mlabels += fitbin[r].GetMBinLabels();

    int lmax = 0;
    for (int b = 0; b < NB; b++) {
        int len = int(mlabels[b].length());
        if (mlabels[b].find("#infty") != std::string::npos)
            len -= 5;
        if (len > lmax)
            lmax = len;
    }

    string space;
    for (int i = 0; i < 1.6 * lmax; i++)
        space += " ";

    for (int r = 0; r < NR; r++)
        rlabels += fitbin[r].GetRBinLabel();

    auto line = new TLine();
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
    double yline = hbo - 0.018 * lmax;
    int ib = 0;
    for (int r = 0; r < NR; r++) {
        int NM = fitbin[r].NBins();
        lo = hi;
        //lo_ratio = hi_ratio;
        hi = double(NM) / double(NB) * (1. - hhi - hlo) + lo;
        //hi_ratio = double(NM)/double(NB)*(1.-hhi_rat-hlo_rat) + lo_ratio;

        line->SetLineStyle(1);
        line->DrawLineNDC(lo + eps, yline,
                          lo + eps, yline + 6 * eps);
        line->DrawLineNDC(hi - eps, yline,
                          hi - eps, yline + 6 * eps);

        line->DrawLineNDC(lo + eps, yline,
                          hi - eps, yline);

        line->SetLineStyle(1);
        if (r < NR - 1) {
            line->DrawLineNDC(hi, yline + 10 * eps, hi, 1. - hto);
        }
        l.DrawLatex((hi + lo) / 2., yline - 8 * eps, rlabels[r].c_str());
    }

    line->SetLineStyle(1);
    l.SetTextAngle(90);
    l.SetTextAlign(32);
    for (int b = 0; b < NB; b++) {
        l.DrawLatex(hlo + (1. - hhi - hlo) * (0.5 + b) / double(NB), hbo - 4 * eps, mlabels[b].c_str());
        if (b > 0)
            line->DrawLineNDC(hlo + (1. - hhi - hlo) / double(NB) * b, hbo, hlo + (1. - hhi - hlo) / double(NB) * b,
                              1. - hto);
    }

    l.SetTextAngle(0);
    l.SetTextAlign(32);
    l.SetTextSize(0.035);
    l.DrawLatex(hlo, (hbo + yline) / 2. + eps, "M_{#perp}   [GeV] #in");
    l.DrawLatex(hlo, yline - 16 * eps, "#scale[1.15]{R_{ISR}} #in");

    string m_CMSLabel = "#bf{#it{CMS}} work-in-progress";

    TLatex l3;
    l3.SetTextFont(42);
    l3.SetNDC();

    auto line2 = new TLine();
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
    l3.DrawLatex(1. - hhi - eps * 4, 1. - hto + 0.02, string("Regions " + identifier_).c_str());
    l3.SetTextAlign(11);
    l3.SetTextSize(0.04);
    l3.SetTextFont(42);
    l3.DrawLatex(hlo + eps * 4, 1. - hto + 0.02, m_CMSLabel.c_str());
    l3.SetTextSize(0.05);

    can->Update();
    can->SaveAs((name + ".pdf").c_str());
}

double SuperBinList::GetMaxZbi(const double sys) {
    double max = this->at(0)->GetBinZbi(sys);

    for (int i = 1; i < this->size(); i++) {
        double tempZbi = this->at(i)->GetBinZbi(sys);
        if (tempZbi > max)
            max = tempZbi;
    }
    return max;
}

SuperBin *SuperBinList::GetMaxZbiBin(const double sys) {
    double max = this->at(0)->GetBinZbi(sys);
    SuperBin *maxSbin = this->at(0);

    for (int i = 1; i < this->size(); i++) {
        double tempZbi = this->at(i)->GetBinZbi(sys);
        if (tempZbi > max)
            maxSbin = this->at(i);
    }
    return maxSbin;
}

SuperBin *SuperBinList::MergeBins(const double sys) {

    SuperBin *firstBin = this->at(0);

    for (int b = 1; b < this->size(); b++) {
        SuperBin *tempBin = firstBin->TryMerge(this->at(b), sys);

        if (tempBin)
            firstBin = tempBin;
    }

    if (firstBin == this->at(0))
        firstBin = GetMaxZbiBin(sys);
    return firstBin;
}

SuperBin* SuperBinList::ForceMergeBins(){

    VI indices;
    VS vis, mlabels, rlabels;
    double background_yield = 0.;
    double signal_yield = 0.;

    for (auto const &bin : *this){
        indices += bin->getIndex();
        vis += bin->GetVisLabel();
        mlabels += bin->GetMperpLabel();
        rlabels += bin->GetRisrLabel();
        background_yield += bin->GetNbkg();
        signal_yield += bin->GetNsig();
    }

    return new SuperBin(indices, signal_yield, background_yield, mlabels, rlabels, vis);
}


//VI class
VI::VI() = default;

VI::VI(const int i) {
    *this += i;
}

VI::VI(const std::vector<int>& vi) {
    for (auto i: vi)
        *this += i;
}

VI::~VI() = default;

VI &VI::operator+=(int i) {
    this->push_back(i);
    return *this;
}

VI &VI::operator+=(const std::vector<int>& vi) {
    for (auto i: vi)
        this->push_back(i);
    return *this;
}

void VI::printList() {
    int size = int(this->size());
    for (int i = 0; i < size; i++) {
        if (i < size - 1)
            cout << this->at(i) << ", ";
        else
            cout << this->at(i);
    }
}
