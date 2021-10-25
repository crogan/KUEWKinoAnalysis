import ROOT
import json
import numpy as np
import argparse
import os



parser = argparse.ArgumentParser()
parser.add_argument('--input', '-i', help='input json file')
parser.add_argument('--output', '-o', help='path of the output file')
parser.add_argument('--sys','-s',help='name of systematic to plot')
parser.add_argument('--uncert','-u',help='prior uncertainty on systematic')
args = parser.parse_args()

odir = ""

if args.input is None:
	print("Need input json file, -i [json]")
	exit()
if args.output is None:
	odir = "./"
else:
	odir = args.output
print 'out directory:',odir

if args.sys is None:
	args.sys = 'PTISR'
if args.uncert is None:
	print("Need input prior uncertainty, -u [uncertainty]")
	exit()
# Load the json output of combineTool.py -M Impacts
data = {}
with open(args.input) as jsonfile:
    data = json.load(jsonfile)

# We will assume the first POI is the one to plot
POIs = [ele['name'] for ele in data['POIs']]
POI = POIs[0]


leps = ["0L","1L"]
graphs = []

maxPulls = 0
minPulls = 0
for l in leps:
	n = 0
	pulls = []
	sJets = []
	err_hi = []
	err_lo = []
	for p in data['params']:
		pre = p['prefit']
		fit = p['fit']
		if p['type'] != 'Unconstrained' and l in p['name']:
			if args.sys not in p['name']:
				continue
			pre_err_hi = (pre[2] - pre[1])
			pre_err_lo = (pre[1] - pre[0])
			pull = fit[1] - pre[1]
        	        pull = (pull/pre_err_hi) if pull >= 0 else (pull/pre_err_lo)
			pull_hi = fit[2] - pre[1]
        	        pull_hi = (pull_hi/pre_err_hi) if pull_hi >= 0 else (pull_hi/pre_err_lo)
        	        pull_hi = pull_hi - pull
        	        pull_lo = fit[0] - pre[1]
        	        pull_lo = (pull_lo/pre_err_hi) if pull_lo >= 0 else (pull_lo/pre_err_lo)
        	        pull_lo =  pull - pull_lo
		
			pos = p['name'].find('jS')
			sJets.append(int(p['name'][pos-1]))
			#print l, p['name'], 'pull', pull, 'err_lo', pull_lo, 'err_hi', pull_hi, 'sJets', int(p['name'][pos-1])
			pulls.append(pull)
			err_hi.append(pull_hi)
			err_lo.append(pull_lo)
	
	n = len(pulls)
	hi = [sum(x) for x in zip(pulls,err_hi)]
	lo = [x[0]-x[1] for x in zip(pulls,err_lo)]
	if n == 0:
		continue
	if n > 0:
		if np.ceil(max(hi)) > maxPulls:
			maxPulls = np.ceil(max(hi))
		if np.floor(min(lo)) < minPulls:
			minPulls = np.floor(min(lo))
	gr = ROOT.TGraphAsymmErrors(n)
	skip = 0
	n = len(sJets)
	for i in range(n):
		if i != sJets[i-skip]:
			gr.SetPoint(i,i,-999)
			i += 1
			skip += 1
			continue
		gr.SetPoint(i,sJets[i-skip],pulls[i-skip])
		gr.SetPointEYlow(i,err_lo[i-skip])
		gr.SetPointEYhigh(i,err_hi[i-skip])
#		print 'i', i, 'sJets', sJets[i-skip], 'pulls', pulls[i-skip]
		if i == n-1 and skip != 0:
			for j in range(skip):
				gr.SetPoint(n+j,sJets[j-skip],pulls[j-skip])
				gr.SetPointEYlow(n+j,err_lo[j-skip])
				gr.SetPointEYhigh(n+j,err_hi[j-skip])
#				print 'j', j,'j-skip', j-skip, 'n+j',n+j,'sJets', sJets[j-skip], 'pulls', pulls[j-skip]
	gr.SetTitle("")
	gr.SetMarkerSize(0.95)
	gr.SetLineWidth(1)
	graphs.append(gr)
if len(graphs) < 1:
	print args.sys, "not found in json"
	exit()
green = 7012
blue = 7002	


graphs[0].SetMarkerStyle(20)
graphs[0].SetMarkerColor(blue)
graphs[0].SetLineColor(blue)


graphs[1].SetMarkerStyle(22)
graphs[1].SetMarkerColor(green)
graphs[1].SetLineColor(green)
graphs[0].GetXaxis().SetRangeUser(-0.5,5.5)

graphs[0].GetHistogram().SetMaximum(maxPulls)
graphs[0].GetHistogram().SetMinimum(minPulls)
graphs[0].GetXaxis().SetTitle("# s jets")
graphs[0].GetYaxis().SetTitle("pull")
graphs[0].GetYaxis().SetLabelFont(132)
graphs[0].GetYaxis().SetTitleFont(132)
graphs[0].GetXaxis().SetLabelFont(132)
graphs[0].GetXaxis().SetTitleFont(132)
graphs[0].GetXaxis().SetTitleSize(0.04)
graphs[0].GetYaxis().SetTitleSize(0.04)


leg = ROOT.TLegend(0.15,0.76,0.52,0.9)
leg.SetFillColor(0)
leg.SetShadowColor(0)
leg.SetFillStyle(0)
leg.SetTextFont(132)
leg.AddEntry(graphs[0],leps[0])
leg.AddEntry(graphs[1],leps[1])
eps = 0.0015
hlo = 0.09
hhi = 0.22
hbo = 0.27
hto = 0.07


cv = ROOT.TCanvas(args.output,args.sys)
cv.cd()
graphs[0].Draw("ap")
graphs[1].Draw("SAMEP")
leg.Draw("same")

l = ROOT.TLatex()
l.SetNDC()
l.SetTextAlign(31)
l.SetTextSize(0.04)
l.SetTextFont(42)
l.DrawLatex(1.-hhi-eps*4+0.15, 1.-hto-0.03, "2017 MC KUEWKino")
l.SetTextAlign(11)
l.SetTextSize(0.04)
l.SetTextFont(42)
l.DrawLatex(hlo+eps*4, 1.-hto-0.03,"#bf{#it{CMS}} work-in-progress")
l.SetTextSize(0.035)
l.SetTextFont(132)
l.DrawLatex(hlo+eps*4+0.2, 1.-hhi+0.035,"prior uncertainty: %s%%" % args.uncert)

plotlabel = "%s pulls" % args.sys #"#color[7014]{"+lep_labels[0]+"} + ";
l.SetTextColor(1);
l.SetTextAlign(13);
l.SetTextSize(0.04);
l.SetTextFont(132);
l.DrawLatex(hhi+0.195, 1.-hto+0.01, plotlabel);


line = ROOT.TLine(0,0,5.5,0)
line.SetLineStyle(7)
line.Draw("same")

cv.Print(odir+'%s_sJetImpacts.root'%args.sys)
cv.Print(odir+'%s_sJetImpacts.pdf'%args.sys)






