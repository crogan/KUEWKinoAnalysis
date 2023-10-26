


################
#Set Sample name and a list of job numbers for the resubmission script 
#datasetname='GluGluHToWWToLNuQQ_M125_13TeV_powheg_JHUGenV628_pythia8_Summer16_102X'
#joblist=[604,902]

#datasetname='GluGluHToTauTau_M125_13TeV_powheg_pythia8_Summer16_102X'
#joblist=[0,100,1022,1087,501,7,900]
#joblist=[0]


datasetname='QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X'
joblist=[2763, 3001, 3446, 3459, 3489, 3491, 3494, 3495, 3496, 3602, 3605, 3606, 3607, 3608, 3610, 3612, 3614, 3615, 3616, 3617, 3618, 3620, 3621, 3622, 3625, 3628, 3631, 3632, 3633, 3635, 3637, 3641, 3642, 3644, 3645, 3649, 3651, 3652, 3656, 3658, 3659, 3661, 3664, 3665, 3666, 3667, 3668, 3669, 3670, 3672, 3673, 3674, 3677, 3678, 3685, 3686, 3691, 3693, 3694, 3695, 3698, 3699, 4218, 75]

###############





def parseHeader():
	#get header
	global datasetname
	fsub = open('./Output/'+datasetname+'/src/submit.sh')
	header=[]
	#parse from beginning until '######'
	for line in fsub:
		if '######' in line:
			break
		header.append(line)
	return header

def parseJobNum( jobnumber ):
	#get the specified job
	global datasetname
	fsub = open('./Output/'+datasetname+'/src/submit.sh')
	job=[]
	jobstr = '###### job'+str(jobnumber)
	jobnp1 = '###### job'+str(jobnumber+1)
	inJobArgs=False
	for line in fsub:
		if jobstr in line:
			inJobArgs = True
		if jobnp1 in line:
			inJobArgs = False
			break
		if inJobArgs:
			job.append(line)
	
	return job
def copyAndUpdateJob( job, jobnumber ):
#	print('updating',job)
	updatedJob = []
	for line in job:
#		print('line',line)
		if '$(Process)' in line:
			updatedJob.append( line.replace('$(Process)', str(jobnumber)) )
		else:
			updatedJob.append(line)
	return updatedJob



header = parseHeader()	
#print(header)
fresub = open('./Output/'+datasetname+'/src/resubmit.sh','w')
for jobnumber in joblist:
	updatedJobHeader = copyAndUpdateJob( header, jobnumber)
	updatedJob = copyAndUpdateJob( parseJobNum(jobnumber), jobnumber)
	for line in updatedJobHeader:
		fresub.write(line)
	for line in updatedJob:
		fresub.write(line)

fresub.close()

print("Resubmission script ready! Launch with:")
print("pushd ../ && condor_submit ./MakeNtuple_LPC/Output/"+datasetname+"/src/resubmit.sh")
