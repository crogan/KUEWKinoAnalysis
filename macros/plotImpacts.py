import ROOT
import json
import numpy as np
import argparse
import os
from systematicLabels import systematicLabels


parser = argparse.ArgumentParser()
parser.add_argument('--input', '-i', help='input json file',required=True)
parser.add_argument('--datacard','-d',help='datacard used to produce impacts',required=True)
parser.add_argument('--output', '-o', help='path of the output file')
parser.add_argument('--sys','-s',help='string to match for systematic to plot',required=True)
#parser.add_argument('--uncert','-u',help='prior uncertainty on systematic',required=True)
#parser.add_argument('--channels','-chan',help='lepton channel (can be >1)',nargs='*')
#parser.add_argument('--kinematic','-kin',help='kinematic to look at for fake shapes (ie RISR or Mperp)',choices=['RISR','Mperp'])
args = parser.parse_args()

odir = ""
if args.input is None:
	print("Need input json file, -i [json]")
	exit()
if "/" not in args.input:
	odir = "./"
else:
	odir = args.input[args.input.find("BF")+2:]
	odir = odir[:odir.find("/")]
	odir = "prePostStackPlots"+odir+"/"
	if not os.path.isdir(odir):
		os.mkdir(odir)
		#odir = "./"
print 'out directory:',odir

if args.output is None:
	args.output = "output"
if args.sys is None:
	args.sys = 'PTISR'
if args.datacard is None:
	print("Need input datacard, -d [path/to/datacard]")
	exit()

# Load the json output of combineTool.py -M Impacts
data = {}
with open(args.input) as jsonfile:
    data = json.load(jsonfile)

sysLabels = systematicLabels()


# We will assume the first POI is the one to plot
POIs = [ele['name'] for ele in data['POIs']]
POI = POIs[0]

graphs = []
legLabels = []
maxPulls = 1
minPulls = 1
n = 0
vals = []
vals_hi = []
vals_lo = []
names = []
for p in data['params']:
	pre = p['prefit']
	fit = p['fit']
	#calculate vals for lnN NPs
	if p['type'] == 'Gaussian' and args.sys in p['name']:
		# find syst line in datacard - check it's lnN
		with open(args.datacard) as dc:
			for l in dc:
				if p['name'] in l:
					# find first instance of a numeric value
					pre_err = float(l[l.find('.')-5:l.find('.')+5])
					break
		if pre_err < 0:
			print "Error: pre_err = ", pre_err, "< 0"
			break
		print 'pre_err', pre_err
		a = fit[1]
		a_hi = fit[2] - fit[1]
	        a_lo = fit[1] - fit[0]
		print 'from json', p['type'], 'a', a, 'a_lo', a_lo, 'a_hi', a_hi
		
		val = (pre_err)**a 
	        val_hi = (pre_err)**(a + a_hi)
	        val_lo = (pre_err)**(a - a_lo)

		print p['name'], p['type'], 'val', val, 'val_lo', val_lo, 'val_hi', val_hi
		vals.append(val)
		vals_hi.append(val_hi)
		vals_lo.append(val_lo)
		names.append(sysLabels.getLabels(p['name']))
	#calculate vals for rateParams
	if p['type'] == 'Unconstrained' and args.sys in p['name']:
		val = fit[1]
		val_hi = fit[2] - fit[1]
	        val_lo = fit[1] - fit[0]

		#print p['name'], p['type'], 'val', val, 'val_lo', val_lo, 'val_hi', val_hi
		vals.append(val)
		vals_hi.append(val_hi)
		vals_lo.append(val_lo)
		names.append(sysLabels.getLabels(p['name']))

#flatten list
names = [n for name in names for n in name]


n = len(vals)
if n < 1:
	print args.sys, "not found in json"
	exit()

#set high/low boundaries for graph
hi = [sum(x) for x in zip(vals,vals_hi)]
lo = [x[0]-x[1] for x in zip(vals,vals_lo)]
if np.round(max(hi),1) > maxPulls:
	maxPulls = np.round(max(hi),1)+0.1
if np.round(min(lo),1) < minPulls:
	minPulls = np.round(min(lo),1)-0.1

gr = ROOT.TGraphAsymmErrors(n)
for i,v in enumerate(vals):
	gr.SetPoint(i,i,v)
	gr.SetPointEYlow(i,vals_lo[i])
	gr.SetPointEYhigh(i,vals_hi[i])
	print 'i', i, names[i], 'vals', v, 'err_hi', vals_hi[i], 'err_lo', vals_lo[i]
gr.SetTitle("")
gr.SetMarkerSize(0.95)
gr.SetLineWidth(1)
graphs.append(gr)
xax = gr.GetXaxis()
xax.SetTickLength(0.)
x = 0 
y = 0
for i in range(int(np.ceil(xax.GetXmax()))):
	idx = xax.FindBin(i)
	if i >= len(vals):
		break
	xax.SetBinLabel(idx,names[i])


if len(graphs) < 1:
	print args.sys, "not found in json"
	exit()

for i,gr in enumerate(graphs):
	gr.SetMarkerStyle(20+i*2)
	gr.SetMarkerColor(800+i*20 + 9)
	gr.SetLineColor(800+i*20 + 9)


graphs[0].GetXaxis().SetRangeUser(-0.5,len(vals))
graphs[0].GetHistogram().SetMaximum(maxPulls)
graphs[0].GetHistogram().SetMinimum(minPulls)
graphs[0].GetXaxis().SetTitle("")
graphs[0].GetYaxis().SetTitle("val \pm error")
graphs[0].GetYaxis().SetLabelFont(132)
graphs[0].GetYaxis().SetTitleFont(132)
graphs[0].GetXaxis().SetLabelFont(132)
graphs[0].GetXaxis().SetTitleFont(132)
graphs[0].GetXaxis().SetTitleSize(0.04)
graphs[0].GetYaxis().SetTitleSize(0.04)

'''
leg = ROOT.TLegend(0.15,0.76,0.52,0.9)
leg.SetFillColor(0)
leg.SetShadowColor(0)
leg.SetFillStyle(0)
leg.SetTextFont(132)
for i in range(len(graphs)):
	leg.AddEntry(graphs[i],legLabels[i])
'''
eps = 0.0015
hlo = 0.09
hhi = 0.22
hbo = 0.27
hto = 0.07


cv = ROOT.TCanvas(args.output,args.sys)
cv.cd()
cv.SetRightMargin(0.09);
cv.SetLeftMargin(0.15);
cv.SetBottomMargin(0.15);
graphs[0].Draw("ap")
for i in range(1,len(graphs)):
	graphs[i].Draw("SAMEP")
#leg.Draw("same")

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
#l.DrawLatex(hlo+eps*4+0.2, 1.-hhi+0.035,"prior uncertainty: %s%%" % args.uncert)

plotlabel = "%s vals" % args.sys #"#color[7014]{"+lep_labels[0]+"} + ";
l.SetTextColor(1);
l.SetTextAlign(13);
l.SetTextSize(0.04);
l.SetTextFont(132);
l.DrawLatex(hhi+0.195, 1.-hto+0.01, plotlabel);


line = ROOT.TLine(0,1,len(vals),1)
line.SetLineStyle(7)
line.Draw("same")

oname = odir+"{}_Impacts".format(args.sys)

cv.Print(oname+'.root')
cv.Print(oname+'.pdf')




