import os
import argparse
import subprocess
import ROOT as rt
from glob import glob

# Check number of events
def checkEntries(outputDir):
	print("Running over the directory '{0}'.".format(outputDir))
	print("------------------------------------------------------------")
	# get list of directories
	# - have to use old python 2 method; we have to use python 2 to use pyROOT
	subfolders = glob(outputDir + "/*")
	#print(subfolders)

	# Search through Output datasets for hadded files. If they exist, load the tree and print entries
	for folder in subfolders:
		DataSetName = folder.split("/")[-1]
		#print("Evaluating Dataset "+DataSetName)
		#print("Looking for .root file in base directory...")
		bash = "ls "+folder+"/*.root | wc -l"
		n_files = int(subprocess.check_output(['bash','-c', bash]).decode())

		#print("n_files: {0}".format(n_files))
		if (n_files > 0):
			tfile = rt.TFile.Open(folder+"/"+DataSetName+".root")
			t = tfile.Get("KUAnalysis")
			entries = t.GetEntries()
			print(DataSetName+" "+str(entries))
		if (n_files == 0):
			print(DataSetName+" N/A")
		
def main():
	# options
	parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
	parser.add_argument("--directory", "-d", default="Output", help="working directory for condor submission")

	options = parser.parse_args()
	directory = options.directory
	
	checkEntries(directory)

if __name__ == "__main__":
    main()
