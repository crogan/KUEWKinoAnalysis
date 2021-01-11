#ifndef testClass_HH
#define testClass_HH

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <vector>
#include <cstdarg>
#include <map>

#include "Criteria.hh"
#include "CategoryTree.hh"

class testClass{
	public:
		testClass(string OutFile, vector<TH1D*> hists);
		testClass(string OutFile);
		virtual ~testClass();
		
		void makeMaps();
		void sumHistograms();
		void scaleHistograms();

	private:
		CategoryTree m_CT;
		CategoryTreeTool m_CTTool;
		int m_nHists;

		string m_outName;
		TFile m_outFile;

		vector<TH1D*> m_hists;
		std::vector<TH1D*> m_sumHists;
		vector<string> m_names;

		map<string,TH1D*> m_namesToHists;
		map<string,TH1D*> m_nameToTotalHist;
		map<string,double> m_namesToNorms;



	
};

#endif
