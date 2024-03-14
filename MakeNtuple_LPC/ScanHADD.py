import os
import argparse
import subprocess

# Automatically HADD ROOT files from condor jobs

# Scan over directory and HADD ROOT files
def scanHADD(outputDir):
	print("Running over the directory '{0}'.".format(outputDir))
	print("------------------------------------------------------------")
	# get list of directories
	subfolders = [f.path for f in os.scandir(outputDir) if f.is_dir()]
	#print(subfolders)

	# Search through Output datasets for hadded files. If they don't exist, create them.
	for folder in subfolders:
		DataSetName = folder.split("/")[-1]
		print( "Evaluating Dataset "+DataSetName)
		print( "Looking for .root file in base directory..." )
		bash = "ls "+folder+"/*.root | wc -l"
		n_files = int(subprocess.check_output(['bash','-c', bash]).decode())
		
		print("n_files: {0}".format(n_files))
		if (n_files > 0):
			print("found file(s): ")
			os.system("ls "+folder+"/*.root")
			print("")
			print("Nothing to do.")
		elif (n_files == 0):
			print("No file found, performing hadd...")
			
			hadd = "hadd -f -j 8 "+folder+"/"+DataSetName+".root "+folder+"/out/*.root"
			os.system(hadd)

		print("------------------------------------------------------------")	
		print("")	

def main():
	# options
	parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
	parser.add_argument("--directory", "-d", default="Output", help="working directory for condor submission")

	options = parser.parse_args()
	directory = options.directory
	
	scanHADD(directory)

if __name__ == "__main__":
    main()
