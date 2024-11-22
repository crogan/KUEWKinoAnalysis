
#Possible Signals:
# T2tt T2cc T2bb T2bW TChipmSlepSnu TChipmWW TStauStau TSlepSlep
import csv
import numpy as np
import json
import sys

#import ROOT as rt

#datastructure
# dictionary[ mass_point ] = correction factor
#filesToIgnore =[]
filesToIgnore = ["SMS-T2bW_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"]#,

#"SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
#"SMS-T2bW_TuneCP2_13TeV-madgraphMLM-pythia8",
#"SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8"]#,
#"SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
#"SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8"]

#SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8
#SMS-T2bW_TuneCP2_13TeV-madgraphMLM-pythia8
#SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8

lumi16 = 36.333380074;   
lumi17 = 41.479680529;   
lumi18 = 59.832475339; 
lumitot = lumi16+lumi17+lumi18;

def isValidFile( name ):
	global filesToIgnore
	for f in filesToIgnore:
		if f in name:
			return False
	return True

def loadCSV( csvname, signal ):
	corr_dict = {}
	with open(csvname, newline='') as csvfile:
		reader = csv.reader(csvfile, delimiter=',', quotechar='|')
		for row in reader:
			#print(', '.join(row))
			if (signal in row[1]) and isValidFile(row[1]):
				key = row[1].split("_")[-2:]
			#key = '_'.join(key)
				key = (int(key[0]) * 10000) + int(key[1])
				corr = float(row[2])
			#print(row)
			#print(key)
				corr_dict[key] = corr 
	csvfile.close()
	return corr_dict	  

def loadSignal( signal ):
	#csv16 = loadCSV("EOS_2016_SMS.csv",signal)
	#csv17 = loadCSV("EOS_2017_SMS.csv",signal)
	#csv18 = loadCSV("EOS_2018_SMS.csv",signal)
	csv16 = loadCSV("EOS_2016_SMS_NEW_10_15_2024.csv",signal)
	csv17 = loadCSV("EOS_2017_SMS_NEW_10_11_2024.csv",signal)
	csv18 = loadCSV("EOS_2018_SMS_NEW_10_11_2024.csv",signal)

	return [csv16,csv17,csv18]


def characterizeCorrections( corr_dict, tag ):

	minPoint = -1
	minCorr = 999
	maxCorr = -1
	maxPoint = -1
	Ncorr1 = 0
	Ncorrgt1 = 0
	Ncorrlt1 = 0
	Ngp = 0
	lt1=[]
	gt1=[]
	dlt1={}
	dgt1={}
 
	for key in corr_dict:
		Ngp = Ngp +1
		if( corr_dict[key] == 1. ):
			Ncorr1 = Ncorr1 + 1
		if( corr_dict[key] < 1. ):
			Ncorrlt1 = Ncorrlt1 + 1
			lt1.append(corr_dict[key])
			if(corr_dict[key] < 0.95):
				dlt1[key] = corr_dict[key]
			if(corr_dict[key] < minCorr):
				minCorr = corr_dict[key]
				minPoint = key
		if( corr_dict[key] > 1. ):
			Ncorrgt1 = Ncorrgt1 + 1
			gt1.append(corr_dict[key]) 
			dgt1[key] = corr_dict[key]
			if(corr_dict[key] > maxCorr):
				maxCorr = corr_dict[key]
				maxPoint = key
	print("Characterizing ",tag)
	print("Npoints:",Ngp," corr=1:",Ncorr1," corr<1:",Ncorrlt1," corr>1:",Ncorrgt1)
	if( Ncorrlt1 > 0):
		print("Mean corr<1: %5.4f +/- %5.4f" % (np.mean(lt1),np.std(lt1)) )
	if( Ncorrgt1 > 0):
		print("Mean corr>1: %5.4f +/- %5.4f" % (np.mean(gt1),np.std(gt1)) )
	if( Ncorrgt1 > 0):
		print("MaxCorr:",maxPoint,maxCorr)
	if( Ncorrlt1 > 0):
		print("MinCorr:",minPoint,minCorr)
	
	print("Finding points with large corrections (<0.95 or >1)")
	print("N corr < 1:", len(dlt1))
	for key in dlt1:
		print("   ",key,":",dlt1[key])
	print("N corr > 1:", len(dgt1))
	for key in dgt1:
		print("   ",key,":",dgt1[key])
	
	
def ProcessSignal( SIG ):
	d = loadSignal(SIG)
	print("-----------------------------------------------")
	characterizeCorrections( d[0], SIG+'16')
	characterizeCorrections( d[1], SIG+'17')
	characterizeCorrections( d[2], SIG+'18')
	print("-----------------------------------------------")
 	
def loadJSON( jfilename ):
	with open(jfilename, 'r') as jsonfile:
# Reading from json file
		json_object = json.load(jsonfile)
	#print(json_object)
	#print(type(json_object))
	jsonfile.close()
	return json_object

def applyCorrections( dlim, dcorr ):
	c1,c2,c3=1.,1.,1.
	clumi=1.
	lwc_lt1=[]
	lwc_gt1=[]
	dc16,dc17,dc18=dcorr[0],dcorr[1],dcorr[2]
	for key in dlim:
		#print( "Processing ", int(float(key)))
		if int(float(key)) in dc16.keys():
			c1 = dc16[int(float(key))]
		else:
			c1=1.0
			print("key not found 16!", int(float(key)) )
		if int(float(key)) in dc17.keys():
			c2 = dc17[int(float(key))]
		else:
			c2=1.0
			print("key not found 17!", int(float(key)) )
		if int(float(key)) in dc18.keys():
			c3 = dc18[int(float(key))]
		else:
			c3=1.0
			print("key not found 18!", int(float(key)) )
		clumi = (lumi16*c1+lumi17*c2+lumi18*c3)/lumitot
		if( clumi < 1. ):
			lwc_lt1.append(clumi)
		if( clumi >= 1. ):
			lwc_gt1.append(clumi)
		for lim_key in dlim[key]:
			dlim[key][lim_key] = dlim[key][lim_key] * clumi
		#print("Found corrections:",c1,c2,c3)
		#print("lumi corrected:", clumi) 
	print("mean weighted correction < 1. : %5.4f +/- %5.4f" % (np.mean(lwc_lt1), np.std(lwc_lt1)))
	print("max correction: %5.4f" % (max(lwc_gt1)) )
	print("min correction: %5.4f" % (min(lwc_lt1)) )	
	
	return dlim
		
		
#ProcessSignal('T2tt')
#ProcessSignal('T2cc')
#ProcessSignal('T2bW')
#ProcessSignal('TChiWZ')
#ProcessSignal('TChipmWW')
#ProcessSignal('TChipmSlepSnu')
#ProcessSignal('TSlepSlep')
SIG= sys.argv[1]
INPUT= sys.argv[2]
OUTPUT= sys.argv[3]

dcorr = loadSignal( SIG )
print("Characterizing per year -----------------------")
characterizeCorrections( dcorr[0], SIG+'16' )
characterizeCorrections( dcorr[1], SIG+'17' )
characterizeCorrections( dcorr[2], SIG+'18' )

#testjfile = "B135_bugfix16_TChiWZ_limit.json"
#testoutput = "B135_bugfix16_TChiWZ_limit_xsec.json"
j = loadJSON( INPUT )
print("Characterizing combined -----------------------")
corrjson = applyCorrections(j, dcorr)
with open( OUTPUT, "w") as out_data_file:
	json.dump(corrjson, out_data_file,indent=4)
	out_data_file.close() 
#print(json.dumps(corrjson, indent=4))



