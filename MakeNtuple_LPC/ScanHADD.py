
import os
import subprocess

#get list of directories
subfolders = [ f.path for f in os.scandir("./Output/") if f.is_dir() ]
#print(subfolders)

# Search through Output datasets for hadded files. If they don't exist, create them.
for folder in subfolders:
	DataSetName = folder.split("/")[-1]
	print( "Evaluating Dataset "+DataSetName)
	print( "Looking for .root file in base directory..." )
	bash = "ls "+folder+"/*.root | wc -l"
	output1 = int(subprocess.check_output(['bash','-c', bash]).decode())
	#print("output1", output1)
	if (output1>0):
		print("found file: ")
		os.system("ls "+folder+"/*.root")
	if (output1==0):
		print("No file found, performing hadd...")
		
		hadd = "hadd -f -j 8 "+folder+"/"+DataSetName+".root "+folder+"/out/*.root"
		os.system( hadd )
		
	 	
	print("------------------------------------------------------------")	
	print("")	

