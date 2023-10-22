import os
import subprocess

#eos cp root://cmseos.fnal.gov///store/user/differentuser/miniaod.root root://cmseos.fnal.gov//store/user/tonjes/miniaodCopy.root


#scan the Output directory, pick up the file tag automatically

subfolders = [ f.path for f in os.scandir("./Output/") if f.is_dir() ]
#print(subfolders)

# grep Queue for possible jobs submitted
for folder in subfolders:
	#print("Processing "+folder)
	datasetname = folder.split("/")[-1]
	filetag = ""
	if( "Summer16" in datasetname):
		filetag = "Summer16_102X"
	if( "Fall17" in datasetname):
		filetag = "Fall17_102X"
	if( "Autumn18" in datasetname):
		filetag = "Autumn18_102X"
#	print("found dataset: ",datasetname, "with filetag: ", filetag)
	#eos cp root://cmseos.fnal.gov///store/user/differentuser/miniaod.root root://cmseos.fnal.gov//store/user/tonjes/miniaodCopy.root
	pathToLocalFile = folder+"/"+datasetname+".root"
	pathToGroupEOS = "root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_v2/"+filetag+"/"+datasetname+".root"
	print("eoscp "+pathToLocalFile+" "+pathToGroupEOS)
	print("")

