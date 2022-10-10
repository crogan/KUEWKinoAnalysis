class systematicLabels():
	def __init__(self):
		self.systDict = {}
		self.initLabels()

	def getLabels(self, systematic):
		sysNames = [sys for sys in self.systDict.keys() if systematic in sys]
		sysLabels = [self.systDict[name] for name in sysNames]
		print systematic, sysLabels
		return sysLabels

	def initLabels(self):
		#fake norms
		self.systDict["Fakes_LF_SS"] = r"SS LF fake norm"
		self.systDict["Fakes_elf0"] = r"HF el fake norm"
		self.systDict["Fakes_elf1"] = r"LF el fake norm"
		self.systDict["Fakes_muf0"] = r"HF mu fake norm"
		self.systDict["Fakes_muf1"] = r"LF mu fake norm"
		
		#IDISO + SIP3D norms
		self.systDict["IDISO_elf0"] = r"HF el iso norm"
		self.systDict["IDISO_elf1"] = r"HF el iso norm"
		self.systDict["IDISO_muf0"] = r"HF mu iso norm"
		self.systDict["IDISO_muf1"] = r"HF mu iso norm"
		self.systDict["SIP3D_elf0"] = r"HF el IP norm"
		self.systDict["SIP3D_elf1"] = r"HF el IP norm"
		self.systDict["SIP3D_muf0"] = r"HF mu IP norm"
		self.systDict["SIP3D_muf1"] = r"HF mu IP norm"
		
		#PTISR norms
		self.systDict["PTISR_0L_1jS"] = r"p_T^{ISR} norm 0L 1jS"
		self.systDict["PTISR_0L_2jS"] = r"p_T^{ISR} norm 0L 2jS"
		self.systDict["PTISR_0L_3jS"] = r"p_T^{ISR} norm 0L 3jS"
		self.systDict["PTISR_0L_4jS"] = r"p_T^{ISR} norm 0L 4jS"
		self.systDict["PTISR_0L_5jS"] = r"p_T^{ISR} norm 0L 5jS"
		self.systDict["PTISR_1L_0jS"] = r"p_T^{ISR} norm 1L 0jS"
		self.systDict["PTISR_1L_1jS"] = r"p_T^{ISR} norm 1L 1jS"
		self.systDict["PTISR_1L_2jS"] = r"p_T^{ISR} norm 1L 2jS"
		self.systDict["PTISR_1L_3jS"] = r"p_T^{ISR} norm 1L 3jS"
		self.systDict["PTISR_1L_4jS"] = r"p_T^{ISR} norm 1L 4jS"
		
		#SV norms
		self.systDict["SV_eff"] = r"SV eff. norm"
		self.systDict["SV_eta_other"] = r"SVs from other sources, /eta norm"
		self.systDict["SV_eta_top"] = r"SVs from tops, /eta norm"
		
		#Wjets norms
		self.systDict["WjetsDY0L_0jS_d3"] = r"W+jets + DY norms 0L 0jS"
		self.systDict["WjetsDY0L_1jS_d2"] = r"W+jets + DY norms 0L 1jS"
		self.systDict["WjetsDY0L_2jS_d1"] = r"W+jets + DY norms 0L 2jS"
		self.systDict["WjetsDY0L_3jS_d2"] = r"W+jets + DY norms 0L 3jS"
		self.systDict["WjetsDY0L_4jS_d3"] = r"W+jets + DY norms 0L 4jS"
		self.systDict["WjetsDY0L_5jS_d4"] = r"W+jets + DY norms 0L 5jS"
		self.systDict["Wjets1L_0jS_d0"] = r"W+jets norms 1L 0jS"
		self.systDict["Wjets1L_1jS_d1"] = r"W+jets norms 1L 1jS"
		self.systDict["Wjets1L_2jS_d2"] = r"W+jets norms 1L 2jS"
		self.systDict["Wjets1L_3jS_d3"] = r"W+jets norms 1L 3jS"
		self.systDict["Wjets1L_4jS_d4"] = r"W+jets norms 1L 4jS"
		self.systDict["Wjets2L_0jS_d1"] = r"W+jets norms 2L 0jS"
		self.systDict["Wjets2L_1jS_d2"] = r"W+jets norms 2L 1jS"
		self.systDict["Wjets2L_2jS_d3"] = r"W+jets norms 2L 2jS"
		self.systDict["Wjets3L_0jS_d1"] = r"W+jets norms 3L 0jS"
		self.systDict["Wjets3L_1jS_d2"] = r"W+jets norms 3L 1jS"
		
		#el fake shapes
		self.systDict["elf_Fake_1L_0J_Mperp"] = r"M_{\perp} shape var. 1L 0jS e^{\pm} fakes" 
		self.systDict["elf_Fake_1L_0J_RISR"] = r"R^{ISR} shape var. 1L 0jS e^{\pm} fakes" 
		self.systDict["elf_Fake_1L_1J_Mperp"] = r"M_{\perp} shape var. 1L 1jS e^{\pm} fakes" 
		self.systDict["elf_Fake_1L_1J_RISR"] = r"R^{ISR} shape var. 1L 1jS e^{\pm} fakes" 
		self.systDict["elf_Fake_1L_2J_Mperp"] = r"M_{\perp} shape var. 1L 2jS e^{\pm} fakes" 
		self.systDict["elf_Fake_1L_2J_RISR"] = r"R^{ISR} shape var. 1L 2jS e^{\pm} fakes" 
		self.systDict["elf_Fake_1L_3J_Mperp"] = r"M_{\perp} shape var. 1L 3jS e^{\pm} fakes" 
		self.systDict["elf_Fake_1L_3J_RISR"] = r"R^{ISR} shape var. 1L 3jS e^{\pm} fakes" 
		self.systDict["elf_Fake_1L_ge4J_Mperp"] = r"M_{\perp} shape var. 1L ge4jS e^{\pm} fakes" 
		self.systDict["elf_Fake_1L_ge4J_RISR"] = r"R^{ISR} shape var. 1L ge4jS e^{\pm} fakes" 
		self.systDict["elf_Fake_2L_0J_Mperp"] = r"M_{\perp} shape var. 2L 0jS e^{\pm} fakes" 
		self.systDict["elf_Fake_2L_0J_RISR"] = r"R^{ISR} shape var. 2L 0jS e^{\pm} fakes" 
		self.systDict["elf_Fake_2L_1J_Mperp"] = r"M_{\perp} shape var. 2L 1jS e^{\pm} fakes" 
		self.systDict["elf_Fake_2L_1J_RISR"] = r"R^{ISR} shape var. 2L 1jS e^{\pm} fakes" 
		self.systDict["elf_Fake_2L_2J_Mperp"] = r"M_{\perp} shape var. 2L 2jS e^{\pm} fakes" 
		self.systDict["elf_Fake_2L_2J_RISR"] = r"R^{ISR} shape var. 2L 2jS e^{\pm} fakes" 
		self.systDict["elf_Fake_3L_0J_RISR"] = r"R^{ISR} shape var. 3L 0jS e^{\pm} fakes" 
		self.systDict["elf_Fake_3L_1nclJS_RISR"] = r"R^{ISR} shape var. 3L incljS e^{\pm} fakes" 

		#mu fake shapes
		self.systDict["muf_Fake_1L_0J_Mperp"] = r"M_{\perp} shape var. 1L 0jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_1L_0J_RISR"] = r"R^{ISR} shape var. 1L 0jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_1L_1J_Mperp"] = r"M_{\perp} shape var. 1L 1jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_1L_1J_RISR"] = r"R^{ISR} shape var. 1L 1jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_1L_2J_Mperp"] = r"M_{\perp} shape var. 1L 2jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_1L_2J_RISR"] = r"R^{ISR} shape var. 1L 2jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_1L_3J_Mperp"] = r"M_{\perp} shape var. 1L 3jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_1L_3J_RISR"] = r"R^{ISR} shape var. 1L 3jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_1L_ge4J_Mperp"] = r"M_{\perp} shape var. 1L ge4jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_1L_ge4J_RISR"] = r"R^{ISR} shape var. 1L ge4jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_2L_0J_Mperp"] = r"M_{\perp} shape var. 2L 0jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_2L_0J_RISR"] = r"R^{ISR} shape var. 2L 0jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_2L_1J_Mperp"] = r"M_{\perp} shape var. 2L 1jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_2L_1J_RISR"] = r"R^{ISR} shape var. 2L 1jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_2L_2J_Mperp"] = r"M_{\perp} shape var. 2L 2jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_2L_2J_RISR"] = r"R^{ISR} shape var. 2L 2jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_3L_0J_RISR"] = r"R^{ISR} shape var. 3L 0jS \mu^{\pm} fakes" 
		self.systDict["muf_Fake_3L_1nclJS_RISR"] = r"R^{ISR} shape var. 3L incljS \mu^{\pm} fakes" 
		
		#gammaT norms
		self.systDict["gamT_0L_2jS"] = r"\gamma_T norm 0L 2jS"
		self.systDict["gamT_0L_3jS"] = r"\gamma_T norm 0L 3jS"
		self.systDict["gamT_0L_4jS"] = r"\gamma_T norm 0L 4jS"
		self.systDict["gamT_1L_1jS"] = r"\gamma_T norm 1L 1jS"
		self.systDict["gamT_1L_2jS"] = r"\gamma_T norm 1L 2jS"
		self.systDict["gamT_1L_3jS"] = r"\gamma_T norm 1L 3jS"
		self.systDict["gamT_1L_4jS"] = r"\gamma_T norm 1L 4jS"
		self.systDict["gamT_2L_0jS"] = r"\gamma_T norm 2L 0jS"
		self.systDict["gamT_1L_1jS"] = r"\gamma_T norm 1L 1jS"
		self.systDict["gamT_2L_2jS"] = r"\gamma_T norm 2L 2jS"

		#lumi norms
		self.systDict["lumi_2016"] = r"lumi 2016 norm"
		self.systDict["lumi_2017"] = r"lumi 2017 norm"
		self.systDict["lumi_2018"] = r"lumi 2018 norm"

		#category norms
		self.systDict["norm_1L_0J_el_gold"] = r"norm 1L 0jS gold e^{\pm}"
		self.systDict["norm_1L_0J_el_notGold"] = r"norm 1L 0jS silver/bronze e^{\pm}"
		self.systDict["norm_1L_0J_l_gold"] = r"norm 1L 0jS gold \l"
		self.systDict["norm_1L_0J_mu_gold"] = r"norm 1L 0jS gold \mu^{\pm}"
		self.systDict["norm_1L_0J_mu_notGold"] = r"norm 1L 0jS silver/bronze \mu^{\pm}"
		self.systDict["norm_1L_1J_l_gold"] = r"norm 1L 1jS gold \l"
		self.systDict["norm_1L_2J_l_gold"] = r"norm 1L 2jS gold \l"
		self.systDict["norm_1L_3J_l_gold"] = r"norm 1L 3jS gold \l"
		self.systDict["norm_1L_4J_l_gold"] = r"norm 1L 4jS gold \l"
		self.systDict["norm_2L_0J_OS_gold"] = r"norm 2L 0jS gold OS" 
		self.systDict["norm_2L_0J_SS_gold"] = r"norm 2L 0jS gold SS" 
		self.systDict["norm_2L_0J_ll_gold"] = r"norm 2L 0jS gold \l\l" 
		self.systDict["norm_2L_0J_ll_notGold"] = r"norm 2L 0jS silver/bronze \l\l" 
		self.systDict["norm_2L_1J_Zstar_gold"] = r"norm 2L 1jS \Z* gold"
		self.systDict["norm_2L_1J_noZ_gold"] = r"norm 2L 1jS no \Z* gold"
		self.systDict["norm_2L_2J_Zstar_gold"] = r"norm 2L 2jS \Z* gold"
		self.systDict["norm_2L_2J_noZ_gold"] = r"norm 2L 2jS no \Z* gold"
		self.systDict["norm_2L_elel_notGold"] = r"norm 2L silver/bronze \e^{\pm}e^{\pm}"
		self.systDict["norm_2L_elmu_notGold"] = r"norm 2L silver/bronze \e^{\pm}\mu^{\pm}"
		self.systDict["norm_2L_mumu_notGold"] = r"norm 2L silver/beonze \mu^{\pm}\mu^{\pm}"
		self.systDict["norm_3L_Zstar"] = r"norm 3L \Z*"
		self.systDict["norm_3L_noZ"] = r"norm 3L no \Z*"

		#POG BTAG SFs
		self.systDict["norm_BTAGHF_SF"] = r"HF b-tag SF"
		self.systDict["norm_BTAGLF_SF"] = r"LF b-tag SF"

		#QCD norms
		self.systDict["norm_QCD0L_1jS"] = r"norm QCD 0L 1jS"
		self.systDict["norm_QCD0L_2jS"] = r"norm QCD 0L 2jS"
		self.systDict["norm_QCD0L_3jS"] = r"norm QCD 0L 3jS"
		self.systDict["norm_QCD0L_4jS"] = r"norm QCD 0L 4jS"
		self.systDict["norm_QCD0L_5jS"] = r"norm QCD 0L 5jS"
		self.systDict["norm_QCD1L_0jS"] = r"norm QCD 1L 0jS"
		self.systDict["norm_QCD1L_1jS"] = r"norm QCD 1L 1jS"
		self.systDict["norm_QCD1L_2jS"] = r"norm QCD 1L 2jS"
		self.systDict["norm_QCD1L_3jS"] = r"norm QCD 1L 3jS"
		self.systDict["norm_QCD1L_4jS"] = r"norm QCD 1L 4jS"
		self.systDict["norm_QCD2L_0jS"] = r"norm QCD 2L 0jS"
		self.systDict["norm_QCD2L_1jS"] = r"norm QCD 2L 1jS"
		self.systDict["norm_QCD2L_2jS"] = r"norm QCD 2L 2jS"
		self.systDict["norm_QCD3L_1jS"] = r"norm QCD 3L 1jS"
		 
		#ST norms
		self.systDict["norm_ST0L"] = r"norm single top 0L"
		self.systDict["norm_ST1L"] = r"norm single top 1L"
		self.systDict["norm_ST2L"] = r"norm single top 2L"
		
		#ZDY+DB norms
		self.systDict["norm_ZDYDB1L"] = r"norm ZDY+DB 1L"
		self.systDict["norm_ZDYDB2L"] = r"norm ZDY+DB 2L"
		self.systDict["norm_ZDYDB3L"] = r"norm ZDY+DB 3L"

		#process SFs
		self.systDict["scale_DB"] = r"DB SF"
		self.systDict["scale_TB"] = r"TB SF"
		
		#sig cross-section
		self.systDict["sig_xsec"] = r"signal cross-section"

		#ttbar norms
		self.systDict["ttbar0L_0jS_d4"] = r"norm t\bar{t} 0L 0jS"
		self.systDict["ttbar0L_1jS_d3"] = r"norm t\bar{t} 0L 1jS"
		self.systDict["ttbar0L_2jS_d2"] = r"norm t\bar{t} 0L 2jS"
		self.systDict["ttbar0L_3jS_d1"] = r"norm t\bar{t} 0L 3jS"
		self.systDict["ttbar0L_4jS_d2"] = r"norm t\bar{t} 0L 4jS"
		self.systDict["ttbar0L_5jS_d3"] = r"norm t\bar{t} 0L 5jS"
		self.systDict["ttbar1L_0jS_d4"] = r"norm t\bar{t} 1L 0jS"
		self.systDict["ttbar1L_1jS_d3"] = r"norm t\bar{t} 1L 1jS"
		self.systDict["ttbar1L_2jS_d2"] = r"norm t\bar{t} 1L 2jS"
		self.systDict["ttbar1L_3jS_d1"] = r"norm t\bar{t} 1L 3jS"
		self.systDict["ttbar1L_4jS_d2"] = r"norm t\bar{t} 1L 4jS"
		self.systDict["ttbar2L_0jS_d2"] = r"t\bar{t} SF"
		self.systDict["ttbar2L_1jS_d1"] = r"norm t\bar{t} 2L 1jS"
		self.systDict["ttbar2L_2jS_d0"] = r"norm t\bar{t} 2L 2jS"
		self.systDict["ttbar3L_0jS_d2"] = r"norm t\bar{t} 3L 0jS"
		self.systDict["ttbar3L_1jS_d1"] = r"norm t\bar{t} 3L 1jS"
















