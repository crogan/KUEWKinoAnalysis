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
parser.add_argument('--output', '-o', help='path of the output file')
args = parser.parse_args()

odir = ""
if args.json is None:
	print("Need input json file, -j [json]")
	exit()
if args.datacard is None:
	print("Need input datacard, -d [datacard]")
	exit()
if args.sys is None:
	print("Need string(s) to match for systematics, -s [sys1, sys2...]")
	exit()

if "/" not in args.datacard:
	odir = "./"
else:
	odir = args.datacard[args.datacard.find("BF")+2:]
	odir = odir[:odir.find("/")]
	odir = "prePostStackPlots"+odir+"/"
	if not os.path.isdir(odir):
		os.mkdir(odir)
		#odir = "./"
print 'out directory:',odir

if args.output is None:
	args.output = "output"

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
leg.SetTextFont(132)
for i, sys in enumerate(args.sys):
	vals_group = []
	vals_hi_group = []
	vals_lo_group = []
	name_group = []
	sym_group = []
	shape = False
	for p in data['params']:
		pre = p['prefit']
		fit = p['fit']
		#calculate vals for lnN NPs
		if p['type'] == 'Gaussian' and sys in p['name']:
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
	
#			print p['name'], p['type'], 'val', val, 'val_lo', val_lo, 'val_hi', val_hi
			vals_group.append(val)
			vals_hi_group.append(val_hi)
			vals_lo_group.append(val_lo)
			name_group.append(p['name'])
			if shape:
				sym_group.append("Delta")
			else:
				sym_group.append("kappa")
		#calculate vals for rateParams
		if p['type'] == 'Unconstrained' and sys in p['name']:
			val = fit[1]
			val_hi = fit[2] - fit[1]
		        val_lo = fit[1] - fit[0]
	
#			print p['name'], p['type'], 'val', val, 'val_lo', val_lo, 'val_hi', val_hi
			vals_group.append(val)
			vals_hi_group.append(val_hi+val)
			vals_lo_group.append(val_lo+val)
			name_group.append(p['name'])
			sym_group.append("theta")
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
if len(names) != len(args.sys):
	print "Error: uneven number of systematic groups",len(args.sys),"and labels",len(names)
	exit()
 
x_labels_all = []
x_labels_raw = []
for i in range(len(names)):
	match = args.sys[i]
	x_labels = []
	diffs = []
	#print "matching for:",match
	for j in range(len(names[i])):
	#	print names[i][j][0],names[i][j][1]
		check = names[i][j][0].replace(match,"")
		sym = '\_'+names[i][j][1]
		sym = sym.replace("_","")
		##subscripts
		sub = "_{"
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
		sub += '}'
		##superscripts
		sup = "^{"
		#Mperp/RISR for fake shapes
		if "Mperp" in check:
			sup += "M_\perp"
		elif "RISR" in check:
			sup += "R_{ISR}"
		elif re.search('BTAG_S_[0-9]',check):
			sup += "BTAG_S"
		elif "BTAG_ISR_" in check:
			sup += "BTAG_ISR"
		elif "BTAG_S_ISR" in check:
			sup += "BTAG_{S,ISR}"
		elif lep not in check and jet not in check:
			sup += check
			if "eta" not in check:
				sup = sup.replace("_","")
		elif re.search("gold",check,re.IGNORECASE):
			sup += "gold"
		elif "DY" in check:
			sup += "DY"	
		if "notGold" in check:
			sup = sup.replace("gold","!gold")
		sup += '}'
		sym += sub+sup
		if sym in x_labels:
			continue
		x_labels.append(sym)	
		diffs.append(check)
		#print "match:",names[i][j][0],"diff:",check,"sym:",sym,"lep:",lep,"jet:",jet	
	x_labels_all.append(x_labels)
	x_labels_raw.append(diffs)

x_labels_all.sort()
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


#for i,sys in enumerate(args.sys):
#	print "i",i,sys
#	for k, binlab in enumerate(bin_labels):
#		for j, xlab in enumerate(x_labels_all[i]):
#			if binlab == xlab: print "k:",k,"j:",j,"binlab:",binlab,"xlab:",xlab 
#			#if binlab.count(u'\\'+xlab) > 1: print "FOUND k:",k,"j:",j,"binlab:",binlab,"xlab:",xlab,"index in binlab:",binlab.index(xlab) 
#			#else: print "k:",k,"j:",j,"xlab:",xlab
	
for i in range(len(args.sys)):
#	print "i",i,args.sys[i]
	x_plot = []
	y_plot = []
	x_hi_plot = []
	x_lo_plot = []
	y_hi_plot = []
	y_lo_plot = []
	for k, binlab in enumerate(bin_labels):
#		print "	matching for",binlab
		for j, xlab in enumerate(x_labels_all[i]):
			if binlab == xlab:
			#	print "		k:",k,"j:",j,"binlab:",binlab,"xlab:",xlab,"val:",vals[i][j][1],"lo err:",abs(vals[i][j][1]-vals_lo[i][j][1]),"hi err:",abs(vals[i][j][1]-vals_hi[i][j][1]),"lo:",vals_lo[i][j][1],"hi:",vals_hi[i][j][1] 
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
	gr.SetMarkerSize(0.95)
	gr.SetLineWidth(1)
	gr.SetMarkerStyle(20+i*2)
	gr.SetMarkerColor(800+i*20 + 9)
	gr.SetLineColor(800+i*20 + 9)
	x = ROOT.Double(999.0)
	y = ROOT.Double(999.0)
	gr_m.Add(gr)
	leg.AddEntry(gr,args.sys[i])



if gr_m.GetListOfGraphs().GetEntries() < len(args.sys):
	print "One of the following systs not found in json:",args.sys
	exit()


eps = 0.0015
hlo = 0.09
hhi = 0.22
hbo = 0.27
hto = 0.07


cv = ROOT.TCanvas(args.output,"_".join(args.sys))
cv.cd()
cv.SetRightMargin(0.09);
cv.SetLeftMargin(0.15);
cv.SetBottomMargin(0.15);
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
l.SetTextFont(132)
#l.DrawLatex(hlo+eps*4+0.2, 1.-hhi+0.035,"prior uncertainty: %s%%" % args.uncert)

#plotlabel = "%s vals" % args.sys #"#color[7014]{"+lep_labels[0]+"} + ";
l.SetTextColor(1);
l.SetTextAlign(13);
l.SetTextSize(0.04);
l.SetTextFont(132);
#l.DrawLatex(hhi+0.195, 1.-hto+0.01, plotlabel);

xax_m = gr_m.GetXaxis()
xax_m.SetLabelSize(0.055)
xax_m.SetTickLength(0.)
#set x-axis labels based on differences bw systematic groups
for i in range(int(xax_m.GetXmax())):
	idx = xax_m.FindFixBin(i)
	xax_m.SetBinLabel(idx,bin_labels[i])

gr_m.GetYaxis().SetRangeUser(gr_m.GetYaxis().GetXmin(),gr_m.GetYaxis().GetXmax()+0.15)
#gr_m.GetXaxis().SetRangeUser(-0.5,len(x_labels))
gr_m.GetXaxis().SetTitle("")
gr_m.GetYaxis().SetTitle("post-fit value \pm error")
gr_m.GetYaxis().SetLabelFont(132)
gr_m.GetYaxis().SetTitleFont(132)
gr_m.GetXaxis().SetLabelFont(132)
gr_m.GetXaxis().SetTitleFont(132)
gr_m.GetXaxis().SetTitleSize(0.04)
gr_m.GetYaxis().SetTitleSize(0.04)
line = ROOT.TLine(0,1,xax_m.GetXmax(),1)
line.SetLineStyle(7)
line.Draw("same")

oname = odir+"{}_Impacts".format("_".join(args.sys))

cv.Print(oname+'.root')
cv.Print(oname+'.pdf')




