import sys
import os
import shutil
import submissionHelper as SH


odir = 'Output/'
#Load input arguments ()
#Require Absolute Path
inputList = sys.argv[1] 
runFlags = sys.argv[2:]
runFlags = " ".join(runFlags)
print("Using input list: "+ inputList )
print("Adding input flags: "+ runFlags )

#testlists
#inputList = '/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Fall17_102X/DYJetsToLL_M-4to50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8.txt'
#inputList = '/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Fall17_102X_Data/MET/MET_Run2017B-02Apr2020-v1_2017.txt'


dataSetName = SH.getDataSetName(inputList)
yearTag = SH.getYearTag(inputList)
#print(yearTag)



#####################Create a workspace (remove existing directory)############
if os.path.exists(odir+dataSetName+"_"+yearTag):
	print("removing existing directory "+odir+dataSetName+"_"+yearTag)
	shutil.rmtree(odir+dataSetName+"_"+yearTag)
os.mkdir(odir+dataSetName+"_"+yearTag)
os.mkdir(odir+dataSetName+"_"+yearTag+"/src")
os.mkdir(odir+dataSetName+"_"+yearTag+"/log")
os.mkdir(odir+dataSetName+"_"+yearTag+"/out")


####################Create submission script in src directory##################
subf = open(odir+dataSetName+"_"+yearTag+"/src/submit.sh","w")
SH.writeSubmissionBase( subf, dataSetName, yearTag )
flagSets = SH.processFlags_Split(runFlags)
#SH.writeQueueList( subf, inputList, dataSetName, yearTag, runFlags )
#for flagSet in flagSets:
SH.writeQueueList(subf, inputList, dataSetName, yearTag, flagSets)
#subf.close()

print("submission ready, to run use:")
print("pushd ../ && condor_submit MakeNtuple_LPC/Output/"+dataSetName+"_"+yearTag+"/src/submit.sh")
print("")

















































