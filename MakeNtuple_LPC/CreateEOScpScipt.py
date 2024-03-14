import os
import argparse
import subprocess

# Create EOS copy script.
def createEOSCopyScript(outputDir):
	#eoscp root://cmseos.fnal.gov///store/user/differentuser/miniaod.root root://cmseos.fnal.gov//store/user/tonjes/miniaodCopy.root

	#scan the Output directory, pick up the file tag automatically

	subfolders = [f.path for f in os.scandir(outputDir) if f.is_dir()]
	#print(subfolders)

	# grep Queue for possible jobs submitted
	for folder in subfolders:
		#print("Processing "+folder)
		datasetname = folder.split("/")[-1]
		# Set filetag: currently using background MC directories only (need to update for other samples)!
		filetag = ""
		if("Summer16" in datasetname):
			filetag = "Summer16_102X"
		if("Fall17" in datasetname):
			filetag = "Fall17_102X"
		if("Autumn18" in datasetname):
			filetag = "Autumn18_102X"
	#	print("found dataset: ",datasetname, "with filetag: ", filetag)
		#eoscp root://cmseos.fnal.gov///store/user/differentuser/miniaod.root root://cmseos.fnal.gov//store/user/tonjes/miniaodCopy.root
		pathToLocalFile = folder+"/"+datasetname+".root"
		pathToGroupEOS = "root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_v2/"+filetag+"/"+datasetname+".root"
		print("eoscp "+pathToLocalFile+" "+pathToGroupEOS)
		print("")

def main():
	# options
	parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
	parser.add_argument("--directory", "-d", default="Output", help="working directory for condor submission")

	options = parser.parse_args()
	directory = options.directory
	
	createEOSCopyScript(directory)

if __name__ == "__main__":
    main()
