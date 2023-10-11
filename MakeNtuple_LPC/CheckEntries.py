
import os
import subprocess
import ROOT as rt
from glob import glob

#get list of directories
#subfolders = [ f.path for f in os.scandir("./Output/") if f.is_dir() ]
#gotta do old python 2 stuff to use pyROOT
subfolders = glob("./Output/*")

#print(subfolders)

# Search through Output datasets for hadded files. If they exist, load the tree and print entries
for folder in subfolders:
	DataSetName = folder.split("/")[-1]
#	print( "Evaluating Dataset "+DataSetName)
#	print( "Looking for .root file in base directory..." )
	bash = "ls "+folder+"/*.root | wc -l"
	output1 = int(subprocess.check_output(['bash','-c', bash]).decode())
	#print("output1", output1)
	if (output1>0):
		tfile = rt.TFile.Open(folder+"/"+DataSetName+".root")
		t = tfile.Get("KUAnalysis")
		entries = t.GetEntries()
		print(DataSetName+" "+str(entries))
	if (output1==0):
		print(DataSetName+" N/A")
		
	

