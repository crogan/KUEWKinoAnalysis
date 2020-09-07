class shapeAnalyzer{
	public:

	shapeAnalyzer(TString file); //for histograms within one file
	// shapeAnalyzer(TString fNom, TString fUp, TString fDown); //for histograms across files (looking at ie pT variations)

	virtual ~shapeAnalyzer();

	void formatPlots();
	void drawHists(Int_t iHist, Int_t iProc,TString histName,TString sys);
	TH1D* getHist(Int_t i);
	void getKeys(Int_t iKey);

	void Analyze();

	//think about a generic "init" function
	//think about initializing process names and histogram names (Fakes_elf0, etc.) separately
	//
	private:
		TFile* iFile;
		TFile* oFile;
		std::vector<TH1D*> hists;
		std::vector<TString> fakesVec;
		std::vector<TString> procVec;
		TList* keyList;
		Int_t nKeys;
		TDirectory* oldDir;
		TDirectory* newDir;
		// TH1D* nomHist;
		// TH1D* upHist;
		// TH1D* downHist;
}