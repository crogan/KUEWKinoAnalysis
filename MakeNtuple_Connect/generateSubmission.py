import sys
import os
import argparse
import shutil
import submissionHelper as SH


# Create workspace and condor submit files.
def generateSubmission(outputDir, extra_args):
 	# Ensure that the directory includes "/" at the end.
	if outputDir[-1] != "/":
		outputDir += "/"
	
	print("Directory for condor submission: {0}".format(outputDir))
	print("------------------------------------------------------------")
	
	# Create directory if it does not exist.
	SH.makeDir(outputDir)
	
 	# Load input arguments
	# - we assume that the first argument is the input file list
	# - the absolute path is required for the input file list
	inputList = extra_args[0]
	runFlags  = extra_args[1:]
	runFlags  = " ".join(runFlags)
	
	print("Using input list: {0}".format(inputList))
	print("Adding input flags: {0}".format(runFlags))

	dataSetName = SH.getDataSetName(inputList)
	yearTag = SH.getYearTag(inputList)
	sampleDir = outputDir + dataSetName + "_" + yearTag
	
	print("Preparing sample directory: {0}".format(sampleDir))

	##### Create a workspace (remove existing directory) #####
	if os.path.exists(sampleDir):
		print("Removing existing directory: {0}".format(sampleDir))
		shutil.rmtree(sampleDir)

	# Create directories for work area.
	SH.createWorkArea(sampleDir)

	##### Create condor submission script in src directory #####
	condorSubmitFile = sampleDir + "/src/submit.sh"
	subf = open(condorSubmitFile, "w")
	SH.writeSubmissionBase(subf, sampleDir, dataSetName, yearTag)
	flagSets = SH.processFlags_Split(runFlags)
	#SH.writeQueueList( subf, inputList, dataSetName, yearTag, runFlags )
	#for flagSet in flagSets:
	SH.writeQueueList(subf, inputList, dataSetName, yearTag, flagSets)
	#subf.close()
	
	print("------------------------------------------------------------")
	print("Submission ready, to run use:")
	print("pushd ../ && condor_submit MakeNtuple_Connect/"+condorSubmitFile)

def main():
	# options
	parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
	parser.add_argument("--directory", "-d", default="Output", help="working directory for condor submission")

	# handle unknown arguments that are used for condor submission
	main_args, extra_args = parser.parse_known_args()
	directory = main_args.directory

	#print("main_args: {0}".format(main_args))
	#print("extra_args: {0}".format(extra_args))
	
	generateSubmission(directory, extra_args)

if __name__ == "__main__":
    main()
