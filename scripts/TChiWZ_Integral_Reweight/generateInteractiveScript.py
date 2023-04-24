

#bool flags per year
DO16=False
DO17=False
DO18=True


#2016 list
FLIST16 = '/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Summer16_102X_SMS/TChiWZ_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt'


#2017 list
FLIST17 = '/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Fall17_102X_SMS/TChiWZ_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8.txt'

#2018 list
FLIST18 = '/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Autumn18_102X_SMS/TChiWZ_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8.txt'


#ARGS16 without ofile and ifile FOR EACH ARG I MANUALLY PUT IN THE CONNECT DIR, YOU NEED THE CONNECT DIR
ARGS16 = '-tree=Events --sms -dataset=TChiWZ_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8 -filetag=Autumn18_102X -eventcount=./Summer16_102X_SMS_RWT/config/EventCount.root -filtereff=./Summer16_102X_SMS_RWT/config/FilterEff.root -json=./Summer16_102X_SMS_RWT/config/GRL_JSON.txt -pu=./Summer16_102X_SMS_RWT/config/PU/ -btag=./Summer16_102X_SMS_RWT/config/BtagSF/ -jme=./Summer16_102X_SMS_RWT/config/JME/ -svfile=./Summer16_102X_SMS_RWT/config/NNmodel.json -metfile=./Summer16_102X_SMS_RWT/config/METTrigger/Parameters.csv -prefirefile=./Summer16_102X_SMS_RWT/config/Prefire/Prefire.root'


#ARGS17 without ofile and ifile
ARGS17 = '-tree=Events --sms -dataset=TChiWZ_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8 -filetag=Fall17_102X -eventcount=./Fall17_102X_SMS_RWT/config/EventCount.root -filtereff=./Fall17_102X_SMS_RWT/config/FilterEff.root -json=./Fall17_102X_SMS_RWT/config/GRL_JSON.txt -pu=./Fall17_102X_SMS_RWT/config/PU/ -btag=./Fall17_102X_SMS_RWT/config/BtagSF/ -jme=./Fall17_102X_SMS_RWT/config/JME/ -svfile=./Fall17_102X_SMS_RWT/config/NNmodel.json -metfile=./Fall17_102X_SMS_RWT/config/METTrigger/Parameters.csv -prefirefile=./Fall17_102X_SMS_RWT/config/Prefire/Prefire.root'


#ARGS18 without ofile and ifile
ARGS18 = '-tree=Events --sms -dataset=TChiWZ_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8 -filetag=Autumn18_102X -eventcount=./Autumn18_102X_SMS_RWT/config/EventCount.root -filtereff=./Autumn18_102X_SMS_RWT/config/FilterEff.root -json=./Autumn18_102X_SMS_RWT/config/GRL_JSON.txt -pu=./Autumn18_102X_SMS_RWT/config/PU/ -btag=./Autumn18_102X_SMS_RWT/config/BtagSF/ -jme=./Autumn18_102X_SMS_RWT/config/JME/ -svfile=./Autumn18_102X_SMS_RWT/config/NNmodel.json -metfile=./Autumn18_102X_SMS_RWT/config/METTrigger/Parameters.csv -prefirefile=./Autumn18_102X_SMS_RWT/config/Prefire/Prefire.root'

OFILE16 = 'TChiWZ_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Summer16_102X'
OFILE17 = 'TChiWZ_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X'    #append a number and .root to these
OFILE18 = 'TChiWZ_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X'

#ive prelogged the number of files in each list
RANGE16 = range(0,17+1)
RANGE17 = range(0,65+1)
RANGE18 = range(0,46+1)


#need directory of all packaged up extra crap
#DIR16='Summer16_102X_SMS_RWT' 
#DIR17='Fall17_102X_SMS_RWT'
#DIR18='Autumn18_102X_SMS_RWT'


FLIST=''
ARGS=''
OFILE=''
RANGE=[]

if DO16:
	FLIST=FLIST16
	ARGS=ARGS16
	OFILE=OFILE16
	RANGE=RANGE16
if DO17:
	FLIST=FLIST17
        ARGS=ARGS17
        OFILE=OFILE17
	RANGE=RANGE17
if DO18:
	FLIST=FLIST18
        ARGS=ARGS18
        OFILE=OFILE18
	RANGE=RANGE18

#read in all files
f = open(FLIST, "r")
files=f.read()
files=files.split('\n')
#print(files)

##the bash script will run in base kuewkino

for i in RANGE:
	#print(OFILE+'_'+str(i)+'.root')
	#print(files[i])
	OFILE_i = OFILE+'_'+str(i)+'.root'
	cmd = './MakeReducedNtuple_NANO.x '
	cmd = cmd+'-ifile='+files[i]+' '
	cmd = cmd+'-ofile='+OFILE_i+' '
	cmd = cmd+ARGS
	print("echo RUNNING FILE "+str(i))
	print(cmd)
		
	
