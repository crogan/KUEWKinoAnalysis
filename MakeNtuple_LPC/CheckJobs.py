
import os
import subprocess

#get list of directories
subfolders = [ f.path for f in os.scandir("./Output/") if f.is_dir() ]
#print(subfolders)

# grep Queue for possible jobs submitted
for folder in subfolders:
	DataSetName = folder.split("/")[-1]
	print( "Evaluating Dataset "+DataSetName)
	bash = "grep -c \"Queue\" "+folder+"/src/submit.sh"
#	print("N Jobs Queued")
	#os.system(bash)
	output1 = int(subprocess.check_output(['bash','-c', bash]).decode())
#	print("the output"+ str(output))
	#print("N Output Produced")
	bash = "ls "+folder+"/out | wc -l"
	#os.system(bash)
	output2 = int(subprocess.check_output(['bash','-c', bash]).decode())
	print("Number of Jobs Queued: %4d, Number of Output Files: %4d" % (output1,output2) )
	if ( output1 != output2 ):
		print("Mismatch!!!!!!!!!! Jobs Failed!!!!!!!!!!!")
	print("Checking for files under 50 kB...")
	bash = "find "+folder+"/out -type f -size -50k"
	output3 = subprocess.check_output(['bash','-c', bash]).decode()
	output3 = output3.split("\n")
	output3.remove('')
	print("Found "+str(len(output3))+" small files")
	for smallFile in output3:
		print(smallFile)

	print("Checking Error Logs...")
	bash = "grep -v -e \"Warning\" -e \"WARNING\" -e \"TTree::SetBranchStatus\" -e \"libXrdSecztn.so\" "+ folder +"/log/*.err"
	os.system(bash)
	#output4 = subprocess.check_output(['bash','-c',bash]).decode()
	#print(output4)	
	print("------------------------------------------------------------")	
	print("")	
# grep N of output root files


# check sizes of small files 

# Get Entries
