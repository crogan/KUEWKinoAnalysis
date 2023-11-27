import os
import re

# Create directory if it does not exist.
def makeDir(dir_name):
    if not os.path.exists(dir_name):
        os.makedirs(dir_name)

# Create directories for work area.
def createWorkArea(sampleDir):
	os.mkdir(sampleDir)
	os.mkdir(sampleDir+"/src")
	os.mkdir(sampleDir+"/log")
	os.mkdir(sampleDir+"/out")

def getDataSetName(pathToList):
        tmp = pathToList.split('/')
        tmp = tmp[-1].split('.')
        #print(tmp[0])
        return tmp[0]

def getYearTag(pathToList):
	tag = "Summer16_102X"
	if tag in pathToList:
		return tag 
	tag = "Fall17_102X"
	if tag in pathToList:
		return tag
	tag = "Autumn18_102X"
	if tag in pathToList:
		return tag
	print("tag not found")
	return tag

# Write the header of the condor submit file
def writeSubmissionBase(subf, sampleDir, dataSetName, yearTag):
	subf.write("universe = vanilla\n")
	subf.write("executable = execute_script.sh\n")
	subf.write("use_x509userproxy = true\n")
	subf.write("output = ./MakeNtuple_Connect/"+sampleDir+"/log/job.$(Process).out\n")
	subf.write("error = ./MakeNtuple_Connect/"+sampleDir+"/log/job.$(Process).err\n")
	subf.write("log = ./MakeNtuple_Connect/"+sampleDir+"/log/job.log\n")
	#subf.write("transfer_input_files = /uscms/home/z374f439/nobackup/whatever_you_want/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2,MakeNtuple_Connect/config.tgz\n")
	#update path of sandbox for connect
	#subf.write("transfer_input_files = /ospool/cms-user/zflowers/public/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2, MakeNtuple_Connect/config.tgz\n")
	#update to use HTTP protocol
	subf.write("transfer_input_files = https://stash.osgconnect.net/cms-user/zflowers/public/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2, https://stash.osgconnect.net/cms-user/mlazarovits/CMSSW_10_6_5/src/KUEWKinoAnalysis/MakeNtuple_Connect/config.tgz\n")
	subf.write("should_transfer_files = YES\n")
	subf.write("when_to_transfer_output = ON_EXIT\n")
	outputFile = dataSetName+"_"+yearTag+".$(Process).root"
	subf.write("transfer_output_files = "+outputFile+"\n")
	#set up environment options on node
	subf.write('+ProjectName="cms.org.ku"\n')
	subf.write('+REQUIRED_OS="rhel7"\n')
	subf.write('+RequiresCVMFS = True\n')
	# Need to supply absolute path for remap
	#absCWD = os.path.abspath(".") # these cwd give the wrong abs path, there is something special in the environment
	#absCWD = os.getcwd()
	absCWD = os.popen('pwd').readline().rstrip() 
	#print("abs path is "+ absCWD)
	remap = absCWD+"/"+sampleDir+"/out/"+outputFile
	#print("remap is "+ remap)
	subf.write("transfer_output_remaps = \""+outputFile+"="+remap+"\"\n")

def processFlags_Split(runFlags):
	# Turn runflags into a lists of lists
	if("split" in runFlags):
		flag_delim = runFlags.split(" ")
		splitarg = [s for s in flag_delim if "-split=" in s]
		splitarg = splitarg[0] # this better have only 1 arg
		m = re.search(r'(?<=\=)\w+', splitarg)
		nChunk = m.group(0)
		#print("splitarg = "+m)
		print("Split arg found, "+splitarg+ " splitting each file into "+nChunk+" jobs ")
		# Remove user split and modify to correct usage for MakeReducedNtuple
		#print("flag_delim ", flag_delim)
		newBaseFlags = flag_delim[:]
		newBaseFlags.remove(splitarg)
		#print("newBaseFlags ",newBaseFlags)
		iChunks = range(1,int(nChunk)+1)
		#print("iChunks ", iChunks)
		newFlagSet = []
		for iChunk in iChunks:
			newbasecopy = newBaseFlags[:]
			newbasecopy.append("-split="+str(iChunk)+","+nChunk)
			newbasecopy = " ".join(newbasecopy)
			newFlagSet.append(newbasecopy)
		
		return newFlagSet
	else:
		# No splitting, return base args in a list
		print("No file splitting argument detected")
		return [runFlags]

# Write each job to the condor submit file.
def writeQueueList(subf, pathToList, dataSetName, yearTag, runFlagsSet):
	configArgs = " -tree=Events -eventcount=./config/EventCount.root -filtereff=./config/FilterEff.root -json=./config/GRL_JSON.txt -pu=./config/PU/ -btag=./config/BtagSF/ -lep=./config/LepSF/ -jme=./config/JME/ -svfile=./config/NNmodel.json -metfile=./config/METTrigger/Parameters.csv -prefirefile=./config/Prefire/Prefire.root --slim" #--sys"
	dataSetArg = " -dataset="+dataSetName
	outFileArg = " -ofile="+dataSetName+"_"+yearTag+".$(Process).root"
	
	# Get list of input files
	ilist = open(pathToList)
	
	# Write jobs to the condor submit file
	jobCtr = 0
	for ifile in ilist:
		for runFlags in runFlagsSet:
			inFileArg = " -ifile="+ifile.rstrip()
			Args = "Arguments ="+inFileArg+dataSetArg+outFileArg+configArgs+" "+runFlags+"\n"
			subf.write("\n\n\n")
			subf.write("###### job"+str(jobCtr)+ " ######\n")
			subf.write(Args)
			subf.write("Queue\n")
			jobCtr += 1
