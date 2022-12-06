import ROOT
import json
import numpy as np
import argparse
import os
import re


parser = argparse.ArgumentParser()
parser.add_argument('--datacard','-d',help='datacard used to produce impacts',required=True)
parser.add_argument('--json', '-j', help='input json file',required=True)
parser.add_argument('--sys','-s',nargs='+',help='string(s) to match for systematic to plot',required=True)
#parser.add_argument('--skip',nargs='+',help='string(s) to match for systematic to skip',required=False)
parser.add_argument('--AN',help='run AN fit', action='store_true')
parser.add_argument('--output', '-o', help='path of the output file')
parser.add_argument('--cat0L',help='plot 0L NPs',action='store_true')
parser.add_argument('--cat1L',help='plot 1L NPs',action='store_true')
parser.add_argument('--cat2L',help='plot 2L and NPs',action='store_true')
parser.add_argument('--cat3L',help='plot 3L NPs',action='store_true')
parser.add_argument('--SFs',help='plot process SFs',action='store_true')
parser.add_argument('--catNorms',help='plot lepton category normalizations',action='store_true')
args = parser.parse_args()

odir = ""
if args.json is None:
	print "Need input json file, -j [json]"
	exit()
if args.datacard is None:
	print "Need input datacard, -d [datacard]"
	exit()
if args.sys is None:
	if not args.SFs and not args.catNorms:
		print "Need string(s) to match for systematics, -s [sys1, sys2...]"
		exit()
if args.catNorms and args.SFs:
	print "Choose to plot either SFs or lepton category norms"
	exit()
if args.sys is not None and'BTAG' in args.sys and not args.cat1L and not args.cat2L and not args.cat0L:
	print 'Need lepton number for BTAG systematics: --cat0L or --cat1L or --cat2L'
	exit()
if args.cat1L and args.cat2L and args.cat0L and args.cat3L:
	print 'Pick either 0L, 1L, or 2L to plot'
	exit()
#set lepton # for btags
cut_0L = False
cut_1L = False
cut_2L = False
cut_3L = False
if args.cat0L:
	cut_0L = True
elif args.cat1L:
	cut_1L = True
elif args.cat2L:
	cut_2L = True
elif args.cat3L:
	cut_3L = True


if "/" not in args.datacard:
	odir = "./"
else:
	odir = args.datacard[args.datacard.find("BF")+2:]
	odir = odir[:odir.find("/")]
	odir = "prePostStackPlots"+odir+"/"
	if not os.path.isdir(odir):
		os.mkdir(odir)
		#odir = "./"

if args.output is None:
	args.output = "output"

ANfit = False
if args.AN:
	ANfit = True
if ANfit:
	print 'Using AN fit - /uscms/home/z374f439/nobackup/CMSSW_10_6_5/src/FitDev_KUEWKinoAnalysis/BuildFits/BF_TChiWZ_4000350_bkg_data_allYears_v120_allChan_11_2_2022/datacards/all/TChiWZ/4000350/'
	args.json = '/uscms/home/z374f439/nobackup/CMSSW_10_6_5/src/FitDev_KUEWKinoAnalysis/BuildFits/BF_TChiWZ_4000350_bkg_data_allYears_v120_allChan_11_2_2022/datacards/all/TChiWZ/4000350/'+args.json
	args.datacard = '/uscms/home/z374f439/nobackup/CMSSW_10_6_5/src/FitDev_KUEWKinoAnalysis/BuildFits/BF_TChiWZ_4000350_bkg_data_allYears_v120_allChan_11_2_2022/datacards/all/TChiWZ/4000350/'+args.datacard
	odir = "AN_impacts/"

print 'out directory:',odir

procs = ['ST','norm_QCD','DB','TB','scale_ZDY','ttbar','Wjets']
if args.SFs:
	args.sys = procs


elif args.catNorms:
	args.sys = ['norm_1L','norm_2L','norm_3L']


# Load the json output of combineTool.py -M Impacts
data = {}
with open(args.json) as jsonfile:
    data = json.load(jsonfile)




# We will assume the first POI is the one to plot
POIs = [ele['name'] for ele in data['POIs']]
POI = POIs[0]

legLabels = []
maxPulls = 1
minPulls = 1
n = 0
names = []
syms = []
vals = []
vals_hi = []
vals_lo = []
gr_m = ROOT.TMultiGraph()
leg = ROOT.TLegend(0.15,0.76,0.52,0.9)
leg.SetFillColor(0)
leg.SetShadowColor(0)
leg.SetFillStyle(0)
leg.SetTextFont(42)
leg.SetTextSize(0.04)

#make sure args.sys is split as much as possible for graphs (ie PTISR -> QCD_PTISR, other_PTISR, PTISR)
syst_groups = []
systs = []
for p in data['params']:
	for sys in args.sys:
		if sys in p['name']:
			check = p['name'].replace(sys,"")
			idx = check.find("__")
			name = ""
			if idx != -1:
				name = check[:idx]+"_"+sys
			else:
				name = sys
			if name not in syst_groups:
				syst_groups.append(name)
#match syst names to their plotting group	
for i, s in enumerate(syst_groups):
	syst = []
	for p in data['params']:
		if 'gamT' in s or 'PTISR' in s:
			if p['name'].find(s) == 0:
				#do lepcut if specified
				if cut_0L:
					if "0L" not in p['name']:
						continue
				elif cut_1L:
					if "1L" not in p['name']:
						continue
				elif cut_2L:
					if "2L" not in p['name']:
						continue 
				elif cut_3L:
					if "3L" not in p['name']:
						continue 
				print s, p['name']
				syst.append(p['name'])
		else:
			if p['name'].find(s) != -1:
				#do lepcut if specified
				if cut_0L:
					if "0L" not in p['name']:
						continue
				elif cut_1L:
					if "1L" not in p['name']:
						continue
				elif cut_2L:
					if "2L" not in p['name']:
						continue 
				elif cut_3L:
					if "3L" not in p['name']:
						continue 
				syst.append(p['name'])
	systs.append(syst)

#combine 'other' NPs with 'default' NPs (ie other_BTAG* with BTAG*)
other_idx = 999
for i, s in enumerate(syst_groups):
	if 'other' in s:
		other_idx = i
		break

default_idx = 999
for i, s in enumerate(syst_groups):
	if "_" not in s:
		default_idx = i
		break

syst_group = zip(syst_groups,systs)
print syst_group
print '\n'
if other_idx != 999:
	#syst_group = syst_group[0:other_idx]+syst_group[other_idx+1:]
	for s in syst_group[other_idx][1]:
		syst_group[default_idx][1].append(s)
	syst_group.pop(other_idx)


#check for empty groups
syst_group = [s for s in syst_group if s[1]]

print syst_group
#print len(syst_group)


for i, s_group in enumerate(syst_group):
	vals_group = []
	vals_hi_group = []
	vals_lo_group = []
	name_group = []
	sym_group = []
	for j, sys in enumerate(s_group[1]):
		#print sys
		for p in data['params']:
			shape = False
			procNorm = False
			pre = p['prefit']
			fit = p['fit']
			#calculate vals for lnN NPs
			if p['type'] == 'Gaussian' and sys == p['name']:
				# find syst line in datacard - check it's lnN
				with open(args.datacard) as dc:
					for l in dc:
						if "process" in l or "bin" in l:
							continue
						if p['name'] in l:
							# find first instance of a numeric value
							pre_err = float(re.findall('\d*\.?\d+',l)[-1])
							if "shape" in l:
								shape = True
							break
				if pre_err < 0:
					print "Error: pre_err = ", pre_err, "< 0"
					break
				for proc in procs:
					if proc in p['name']:
						procNorm = True
						break
				a = fit[1]
				a_hi = fit[2] - fit[1]
			        a_lo = fit[1] - fit[0]
				#print 'from json', p['type'], 'a', a, 'a_lo', a_lo, 'a_hi', a_hi
				if shape:
					val = a
					val_hi = a_hi
					val_lo = a_lo
				else:	
					val = (pre_err)**a 
				        val_hi = (pre_err)**(a + a_hi)
				        val_lo = (pre_err)**(a - a_lo)
		
				if abs(val_hi-val) == 0:
					val_hi = val_lo
				if abs(val_lo-val) == 0:
					val_lo = val_hi
	#			print p['name'], p['type'], 'val', val, 'val_lo', val_lo, 'val_hi', val_hi
				vals_group.append(val)
				vals_hi_group.append(val_hi)
				vals_lo_group.append(val_lo)
				name_group.append(p['name'])
				if shape:
					sym_group.append("Delta")
				elif procNorm:
					sym_group.append("theta")
				else:
					sym_group.append("kappa")
			#calculate vals for rateParams
			if p['type'] == 'Unconstrained' and sys == p['name']:
				val = fit[1]
				val_hi = fit[2] - fit[1]
			        val_lo = fit[1] - fit[0]
				if abs(val_hi-val) == 0:
					val_hi = val_lo
				if abs(val_lo-val) == 0:
					val_lo = val_hi
		
				for proc in procs:
					if proc in p['name']:
						procNorm = True
						break
	#			print p['name'], p['type'], 'val', val, 'val_lo', val_lo, 'val_hi', val_hi
				vals_group.append(val)
				vals_hi_group.append(val_hi+val)
				vals_lo_group.append(val_lo+val)
				name_group.append(p['name'])
				if shape:
					sym_group.append("Delta")
				elif procNorm:
					sym_group.append("theta")
				else:
					sym_group.append("kappa")
	vals.append(zip(name_group,vals_group))
	vals_hi.append(zip(name_group,vals_hi_group))
	vals_lo.append(zip(name_group,vals_lo_group))
	names.append(zip(name_group,sym_group))
	n = len(vals)
	if n < 1:
		print sys, "not found in json"
		exit()
	#create a new TMultigraph for each group of NPs

#set x-axis labels - common to all graphs in tmultigraph
if len(names) != len(syst_group):
	print "Error: uneven number of systematic groups",len(syst_group),"and labels",len(names)
	exit()


x_labels_all = []
x_labels_raw = []
for i in range(len(names)):
	match = syst_group[i][0]
	x_labels = []
	diffs = []
#	print "matching for:",match
	for j in range(len(names[i])):
#		print names[i][j][0],names[i][j][1]
		check = names[i][j][0].replace(match,"")
		check = check.replace("other","")
#		print "match:",match, "check:", check
		sym = '#_'+names[i][j][1]
		sym = sym.replace("_","")
		check = check.replace("scale","")
		##subscripts
		sub = "_{#scale[0.8]{"
		lep = "lep"
		jet = "jet"
		#check for lepton split
		if re.search('[0-9]L_',check):
			lep = re.findall('[0-9]L_',check)[0][:-1]
			sub += lep
		elif re.search('[0-9]L',check):
			lep = re.findall('[0-9]L',check)[0]
			sub += lep
		#check for jet split
		if re.search('[0-9]j',check,re.IGNORECASE):
			jet = re.findall('[0-9]J',check, re.IGNORECASE)[0]
			if lep != "lep":
				sub += ","
			sub += jet
		if re.findall('[0-9]jS',sub,re.IGNORECASE):
			sub = sub[:-1]
		sub += '}}'
		##superscripts
		sup = "^{#scale[0.8]{"
		#Mperp/RISR for fake shapes
		if "Mperp" in check:
			sup += "M_{#perp}"
		elif "RISR" in check:
			sup += "R_{ISR}"
		#btag s/isr/2b
		if 'BTAG' in match and re.search('S_[0-9]L',check):
			sup += "B_{S}"
		elif 'BTAG' in match and '_S_ISR_' not in check:
			sup += "B_{ISR}"
		elif 'BTAG' in match and "_S_ISR" in check and "2b" not in check:
			sup += "B_{S,ISR}"
		elif 'BTAG' in match and "_S_ISR" in check and "2b" in check:
			sup += "B_{2b}"
		elif lep not in check and jet not in check or re.search("gold",check,re.IGNORECASE):
			sup += check[1:]
			sup = sup.replace("_",",")
	#	if re.search("gold",check,re.IGNORECASE):
	#		sup += "gold"
		if "notGold" in check:
			sup = sup.replace("notGold","!gold")
		if "el" in sup:
			sup = sup.replace("el","e")	
		if "mu" in sup:
			sup = sup.replace("mu","#mu")
		if "star" in sup:
			sup = sup.replace("star","*")	

	#	if "DY" in check:
	#		sup += "DY"	
		if "ST" in check:
			sup += "ST"	
#		if "QCD" in check:
#			sup += "QCD"
#		if "other" in check:
#			sup += "other"
		sup = sup.replace("eta","#eta")
		sup = sup.replace("#etatop","#eta_{top}")
		sup += '}}'
		sym += sub+sup
		if sym in x_labels:
			continue
#		print sym,"\n"
		x_labels.append(sym)	
		diffs.append(check)
	x_labels_all.append(x_labels)
	x_labels_raw.append(diffs)

#x_labels_all.sort()
#number of total bins
bin_labels = []
bin_labels_raw = []
for i in x_labels_all:
	for j in i:
		bin_labels.append(j)
for i in x_labels_raw:
	for j in i:
		bin_labels_raw.append(j)
bin_labels = np.unique(bin_labels)
bin_labels_raw = np.unique(bin_labels_raw)
nbins = len(bin_labels)
if len(np.shape(bin_labels)) > 1:
	print "Error: multiply defined bin labels",np.shape(bin_labels)
	exit()






#make plots
for i in range(len(syst_group)):
#	print "plot",syst_group[i][0]
	x_plot = []
	y_plot = []
	x_hi_plot = []
	x_lo_plot = []
	y_hi_plot = []
	y_lo_plot = []
	for k, binlab in enumerate(bin_labels):
	#	print "	matching for",binlab
		for j, xlab in enumerate(x_labels_all[i]):
			if binlab == xlab:
	#			print "		k:",k,"j:",j,"binlab:",binlab,"xlab:",xlab,"val:",vals[i][j][1],"lo err:",abs(vals[i][j][1]-vals_lo[i][j][1]),"hi err:",abs(vals[i][j][1]-vals_hi[i][j][1]),"lo:",vals_lo[i][j][1],"hi:",vals_hi[i][j][1] 
				x_plot.append(float(k))
				y_plot.append(vals[i][j][1])
				x_hi_plot.append(0.)
				y_hi_plot.append(abs(vals[i][j][1] - vals_hi[i][j][1]))
				x_lo_plot.append(0.)
				y_lo_plot.append(abs(vals[i][j][1] - vals_lo[i][j][1]))
		#	else:
		#		print "	skipping",binlab,"with",vals[i][j][0]
	if len(x_plot) > nbins:
		print "Error: more x values than bins"
		exit()
	gr = ROOT.TGraphAsymmErrors(int(len(x_plot)),np.array(x_plot),np.array(y_plot),np.array(x_lo_plot),np.array(x_hi_plot),np.array(y_lo_plot),np.array(y_hi_plot))
	gr.SetTitle("")
	gr.SetMarkerSize(0.99)
	gr.SetLineWidth(1)
	gr.SetMarkerStyle(20+i*2)
	gr.SetMarkerColor(800+i*20 + 9)
	gr.SetLineColor(800+i*20 + 9)
	gr_m.Add(gr)
	leg_label = syst_group[i][0]
	leg_label = leg_label.replace("_"," ")
	if "PTISR" in syst_group[i][0]:
		leg_label = leg_label.replace("PTISR",r"p_{T}^{ISR}")
	elif "gamT" in syst_group[i][0]:
		leg_label = leg_label.replace("gamT",r"#gamma_{#perp}")
	if "Wjets" in syst_group[i][0]:
		leg_label = leg_label.replace("Wjets","W + jets and Z / #gamma * + jets")
	if "ttbar" in syst_group[i][0]:
		leg_label = leg_label.replace("ttbar",r"t#bar{t} + jets")
	if "scale_DB" in syst_group[i][0]:
		leg_label = leg_label.replace("scale_DB","di-bosons")
	if "TB" in syst_group[i][0]:
		leg_label = leg_label.replace("tri-bosons")
	if "norm_QCD" in syst_group[i][0]:
		leg_label = leg_label.replace("norm_QCD","QCD multi-jets")
	if "ST" in syst_group[i][0]:
		leg_label = leg_label.replace("ST","single top")
	if "elf" in syst_group[i][0]:
		leg_label = leg_label.replace("elf","#it{e}")
	if "muf" in syst_group[i][0]:
		leg_label = leg_label.replace("muf","#mu")
	if cut_0L:
		leg_label += " 0L"
	elif cut_1L:
		leg_label += " 1L"
	elif cut_2L:
		leg_label += " 2L"
	elif cut_3L:
		leg_label += " 3L"
	leg.AddEntry(gr,leg_label)

if gr_m.GetListOfGraphs().GetEntries() < len(syst_group):
	print "One of the following systs not found in json:",syst_group
	exit()


eps = 0.0015
hlo = 0.09
hhi = 0.22
hbo = 0.27
hto = 0.07


cv = ROOT.TCanvas(args.output,"_".join(args.sys))
cv.cd()
cv.SetRightMargin(0.09)
cv.SetLeftMargin(0.15)
cv.SetBottomMargin(0.15)
gr_m.Draw("ap")
leg.Draw("same")

l = ROOT.TLatex()
l.SetNDC()
l.SetTextAlign(31)
l.SetTextSize(0.04)
l.SetTextFont(42)
#l.DrawLatex(1.-hhi-eps*4+0.15, 1.-hto-0.03, "2017 MC KUEWKino")
l.SetTextAlign(11)
l.SetTextSize(0.04)
l.SetTextFont(42)
l.DrawLatex(hlo+eps*4, 1.-hto-0.02,"#bf{#it{CMS}} work-in-progress")
l.SetTextSize(0.035)
l.SetTextFont(42)
#l.DrawLatex(hlo+eps*4+0.2, 1.-hhi+0.035,"prior uncertainty: %s%%" % args.uncert)

#plotlabel = "%s vals" % args.sys #"#color[7014]{"+lep_labels[0]+"} + ";
l.SetTextColor(1)
l.SetTextAlign(13)
l.SetTextSize(0.04)
l.SetTextFont(42)
#l.DrawLatex(hhi+0.195, 1.-hto+0.01, plotlabel);
l.SetTextAlign(31)
l.SetTextSize(0.04)
l.SetTextFont(42)
l.DrawLatex(1-hlo+eps*4,1-hto-0.02,"CR Fit, 137 fb^{-1} (13 TeV)")

xax_m = gr_m.GetXaxis()
xax_m.SetLabelSize(0.055)
#xax_m.SetTickLength(0.2)
count = 0
last = 0
#set x-axis labels based on differences bw systematic groups
for i in range(int(xax_m.GetXmax())):
	idx = xax_m.FindFixBin(i)
	xax_m.SetBinLabel(idx,bin_labels[i])
	last = i
if last < len(bin_labels)-1:
	for i in range(last+1,len(bin_labels)):
		#print 'i:',i,'idx:',idx,'label:',bin_labels[i]
		idx = xax_m.FindFixBin(i)
		xax_m.SetBinLabel(idx,bin_labels[i])

gr_m.GetYaxis().SetRangeUser(gr_m.GetYaxis().GetXmin(),gr_m.GetYaxis().GetXmax()+0.15)
#gr_m.GetXaxis().SetRangeUser(-0.5,len(x_labels))
gr_m.GetXaxis().SetTitle("")
gr_m.GetYaxis().SetTitle("postfit value #pm error")
gr_m.GetYaxis().SetLabelFont(42)
gr_m.GetYaxis().SetTitleFont(42)
gr_m.GetXaxis().SetLabelFont(42)
gr_m.GetXaxis().SetTitleFont(42)
gr_m.GetXaxis().SetTitleSize(0.04)
gr_m.GetYaxis().SetTitleSize(0.04)


if "Delta" in bin_labels[0]:
	line = ROOT.TLine(0,0,xax_m.GetXmax(),0)
else:
	line = ROOT.TLine(0,1,xax_m.GetXmax(),1)
line.SetLineStyle(7)
line.Draw("same")

oname = ""
if args.cat0L:
	oname = odir+"{}0L_Impacts".format("_".join(args.sys))
elif args.cat1L:
	oname = odir+"{}1L_Impacts".format("_".join(args.sys))
elif args.cat2L:
	oname = odir+"{}2L_Impacts".format("_".join(args.sys))
elif args.cat3L:
	oname = odir+"{}3L_Impacts".format("_".join(args.sys))
else:
	oname = odir+"{}_Impacts".format("_".join(args.sys))

cv.Print(oname+".root")
cv.Print(oname+".pdf")



