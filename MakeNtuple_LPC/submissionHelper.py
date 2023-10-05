import os
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
def writeSubmissionBase( subf, dataSetName, yearTag ):
	subf.write("universe = vanilla\n")
	subf.write("executable = execute_script.sh\n")
	subf.write("output = ./MakeNtuple_LPC/Output/"+dataSetName+"_"+yearTag+"/log/job.$(Process).out\n")
	subf.write("error = ./MakeNtuple_LPC/Output/"+dataSetName+"_"+yearTag+"/log/job.$(Process).err\n")
	subf.write("log = ./MakeNtuple_LPC/Output/"+dataSetName+"_"+yearTag+"/log/job.log\n")
	subf.write("transfer_input_files = /uscms/home/z374f439/nobackup/whatever_you_want/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2,MakeNtuple_LPC/config.tgz\n")
	subf.write("should_transfer_files = YES\n")
	subf.write("when_to_transfer_output = ON_EXIT\n")
	outname = dataSetName+"_"+yearTag+".$(Process).root"
	subf.write("transfer_output_files = "+outname+"\n")
	# need to supply absolute path for remap
	#absCWD = os.path.abspath(".") # these cwd give the wrong abs path, there is something special in the environment
	#absCWD = os.getcwd()
	absCWD = os.popen('pwd').readline().rstrip() 
	#print("abs path is "+ absCWD)
	remap= absCWD+"/Output/"+dataSetName+"_"+yearTag+"/out/"+outname
	#print("remap is "+ remap)
	subf.write("transfer_output_remaps = \""+outname+"="+remap+"\"\n")

def writeQueueList( subf, pathToList, dataSetName, yearTag ):
	configArgs = " -tree=Events -eventcount=./config/EventCount.root -filtereff=./config/FilterEff.root -json=./config/GRL_JSON.txt -pu=./config/PU/ -btag=./config/BtagSF/ -lep=./config/LepSF/ -jme=./config/JME/ -svfile=./config/NNmodel.json -metfile=./config/METTrigger/Parameters.csv -prefirefile=./config/Prefire/Prefire.root --slim --sys"
	dataSetArg = " -dataset="+dataSetName
	outFileArg = " -ofile="+dataSetName+"_"+yearTag+".$(Process).root"
	
	#get list of input files
	ilist = open(pathToList)
	jobCtr=0
	for ifile in ilist:
		inFileArg = " -ifile="+ifile.rstrip()
		Args = "Arguments ="+inFileArg+dataSetArg+outFileArg+configArgs+"\n"
		subf.write("\n\n\n")
		subf.write("###### job"+str(jobCtr)+ "######\n")
		subf.write(Args)
		subf.write("Queue\n")	
		jobCtr=jobCtr+1
