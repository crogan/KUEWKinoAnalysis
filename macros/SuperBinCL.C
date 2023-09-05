#include <iostream>
#include <random>
#include <map>

#include <TH1D.h>
#include <TChain.h>
#include <TRandom3.h>
#include <TLine.h>
#include "../include/tree.h"

double Mean(const std::vector<double> &dataset);
double Sigma(const std::vector<double> &dataset);
int GenRandomNumber(int max_entry);
void HistFromVec(const std::vector<double>& data, const std::string& name, const int nbins, double xLine = -999);

using namespace std;

void SuperBinCL(){

    //string filename = "fileWithTree_data.root";
    string filename = "fileWithTree_background_new.root";

    auto tch = new TChain("tree");
    tch->Add(filename.c_str());
    tree handle(tch);

    const int max_iter = 10;
    const int n_entries = tch->GetEntries();

    TRandom3 randGen;
    vector<string> names = {
            "bJets1L", "bJets2L", "bJetsSV", "bJetsCompressed",
            "bJetsModerate", "electroweak", "ossf", "3L"
    };
    vector<int> data = {50, 15, 38, 104, 83, 26, 12 ,21};
    //vector<double> signal = {3.6, 6.8, 8.9, 1.3, 13.6, 15.7, 1.3, 3.3};
    vector<double> signal = {20., 9.5, 12.8, 24.5, 27.5, 16.5, 7., 9.8};
    //vector<double> signal(8,0.);
    vector<int> count_sb(8, 0);
    vector<int> count_b(8, 0);
    map<string, vector<double>> yield_data;
    map<string, vector<double>> bin_yields;


    for (const string& name : names) {
        yield_data[name] = {};  // Initialize with an empty vector
        bin_yields[name] = {};
    }

    for(int e = 0; e < n_entries; e++){
        vector<double> super_bins(8, 0.);

        handle.GetEntry(e);
        double bJets1L = handle.BJets1L(e);
        super_bins[0] = bJets1L;

        // 2L
        double bJets2L = handle.BJets2L(e);
        super_bins[1] = bJets2L;

        // SV
        double bJetsSV = handle.BJetsSV(e);
        super_bins[2] = bJetsSV;

        // Total b+jets compressed yield
        double bJetsCompressed = bJets1L + bJets2L + bJetsSV;
        super_bins[3] = bJetsCompressed;

        // b + jets moderate compression
        double bJetsModerate = handle.BJetsModerate(e);
        super_bins[4] = bJetsModerate;

        // electroweak-like
        double electroweak = handle.Electroweak(e);
        super_bins[5] = electroweak;

        // OSSF compressed
        double ossf = handle.OSSF(e);
        super_bins[6] = ossf;

        // 3L
        double threeL = handle.ThreeL(e);
        super_bins[7] = threeL;

        for(int y = 0; y < super_bins.size(); y++){
            bin_yields[names[y]].push_back(super_bins[y]);

            if(super_bins[y] > data[y])
                count_b[y]++;
        }
    }

    int iterations = 1e4;
    for(int i = 0; i < iterations; i++){

        if (i % 100 == 0) {
            fprintf(stdout, "\r  Processed events: %8d of %8d ", i, iterations);
        }
        fflush(stdout);

        vector<double> super_bins(8, 0.);
        int random_entry = GenRandomNumber(n_entries);
        handle.GetEntry(random_entry);

        //cout << "\nEntry: " << random_entry << endl;
        // b + jets compressed
        // 1L
        double bJets1L = handle.BJets1L(random_entry);
        //cout << "b+jets 1L: " << bJets1L << endl;
        super_bins[0] = bJets1L;

        // 2L
        double bJets2L = handle.BJets2L(random_entry);
        //cout << "b+jets 2L: " << bJets2L << endl;
        super_bins[1] = bJets2L;

        // SV
        double bJetsSV = handle.BJetsSV(random_entry);
        //cout << "b+jets SV: " << bJetsSV << endl;
        super_bins[2] = bJetsSV;

        // Total b+jets compressed yield
        double bJetsCompressed = bJets1L + bJets2L + bJetsSV;
        //cout << "b+jets compressed: " << bJetsCompressed << endl;
        super_bins[3] = bJetsCompressed;

        // b + jets moderate compression
        double bJetsModerate = handle.BJetsModerate(random_entry);
        //cout << "b+jets moderate: " << bJetsModerate << endl;
        super_bins[4] = bJetsModerate;

        // electroweak-like
        double electroweak = handle.Electroweak(random_entry);
        //cout << "electroweak bin: " << electroweak << endl;
        super_bins[5] = electroweak;
        // OSSF compressed
        double ossf = handle.OSSF(random_entry);
        //cout << "OSSF compressed: " << ossf << endl;
        super_bins[6] = ossf;
        // 3L
        double threeL = handle.ThreeL(random_entry);
        //cout << "3L super bin: " << threeL << endl;
        super_bins[7] = threeL;

        for(int y = 0; y < super_bins.size(); y++){
            double bkg_yield = super_bins[y];
            double sig_yield = randGen.Poisson(signal[y]);
            double total = bkg_yield + sig_yield;
            yield_data[names[y]].push_back(total);

            if(total > data[y])
                count_sb[y]++;
        }

    }

    int c = 0;
    for(auto const &name : names) {
        HistFromVec(yield_data[name], name, 100, data[c]);
        HistFromVec(bin_yields[name], name+"_yield", 100, data[c]);
        c++;
    }

    for(int y = 0; y < 8; y++){
        cout << endl << names[y] << ": % of time Nbkg+Nsig > Nobs: "<< count_sb[y] << "/" << iterations << " = "
             << (static_cast<double>(count_sb[y])/iterations)*100 << endl;
        cout << "CLb = " << (static_cast<double>(count_b[y])/n_entries)*100 << endl;
        cout << "CLs = " << (1.-static_cast<double>(count_sb[y])/iterations)/(1.-static_cast<double>(count_b[y])/n_entries)*100 << endl;
        cout << "Mean: " << Mean(bin_yields[names[y]]) << endl;
        cout << "Standard Deviation: " << Sigma(bin_yields[names[y]]) << endl;
    }

}

int GenRandomNumber(int max_entry) {
    if (max_entry <= 0) {
        std::cerr << "Error: max_entry must be greater than 0." << std::endl;
        return -1; // Return a sentinel value to indicate an error
    }

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, max_entry - 1);

    return distribution(generator);
}

void HistFromVec(const std::vector<double>& data, const std::string& name, const int nbins, double xLine) {
    // Find the min and max of the data
    double data_min = *std::min_element(data.begin(), data.end());
    double data_max = *std::max_element(data.begin(), data.end());

    double bin_size = (data_max-data_min)/nbins;

    // Create a histogram with nbins between data_min and data_max
    TH1D* hist = new TH1D(name.c_str(), "", nbins, data_min-2*bin_size, data_max+2*bin_size);

    // Fill the histogram with the data
    for (auto d : data) {
        hist->Fill(d);
    }

    hist->SetLineColor(kBlack);
    hist->SetLineStyle(1);
    hist->SetLineWidth(2);

    // Set x and y axis labels
    hist->GetXaxis()->SetTitle(name.c_str());
    hist->GetYaxis()->SetTitle("Events");
    hist->GetXaxis()->CenterTitle();
    hist->GetYaxis()->CenterTitle();

    TLine* line = new TLine(xLine, hist->GetMinimum(), xLine, hist->GetMaximum());
    line->SetLineColor(kRed);

    // Set grid
    gStyle->SetOptStat(0);
    gStyle->SetGridStyle(3);
    gStyle->SetGridColor(kGray+2);

    // Create a TCanvas
    TCanvas canvas("canvas", "", 800, 600);

    // Set canvas margins
    canvas.SetLeftMargin(0.12);
    canvas.SetRightMargin(0.05);
    canvas.SetTopMargin(0.05);
    canvas.SetBottomMargin(0.12);

    // Draw the histogram
    hist->Draw("hist");
    line->Draw("SAME");

    // Update canvas
    canvas.SetGrid();
    canvas.SaveAs((name+".pdf").c_str());

    // Clean up memory
    delete hist;
}

double Mean(const vector<double> &dataset) {

    double mean = 0.0;
    int denominator = dataset.size();
    for (int i = 0; i < dataset.size(); i++) {
        mean += dataset[i];
    }
    mean /= denominator;
    return mean;
}

double Sigma(const vector<double> &dataset) {

    double mean = Mean(dataset);

    double variance = 0.0;
    double denominator = dataset.size();
    for (int i = 0; i < dataset.size(); i++) {
        double deviation = dataset[i] - mean;
        variance += deviation * deviation;
    }
    variance /= denominator;

    double std_dev = std::sqrt(variance);
    return std_dev;
}