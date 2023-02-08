
#./MakeReducedNtuple_NANO.x -ifile=input.root -ofile=output.root -dataset=dataset_name -filetag=sample_tag

#INPUT=root://cmsxrootd.fnal.gov//store/mc/RunIIFall17NanoAODv7/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8/NANOAODSIM/PUFall17Fast_Nano02Apr2020_102X_mc2017_realistic_v8-v1/260000/DC7E9D54-1353-8F4E-98B2-A4C019734D72.root
#INPUT=root://cmsxrootd.fnal.gov//store/mc/RunIISummer16NanoAODv7/SMS-TChiWZ_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/NANOAODSIM/PUSummer16v3Fast_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/100000/1487ECC3-D216-6242-A5C5-90532D53613A.root

#smaller file 
#INPUT=root://cmsxrootd.fnal.gov//store/mc/RunIISummer16NanoAODv7/SMS-TChiWZ_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/NANOAODSIM/PUSummer16v3Fast_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/130000/2D7C2347-CC4A-B94D-A637-0BC95C9B3A51.root

#smallest other sample with genmet genht filter
INPUT=root://cmsxrootd.fnal.gov//store/mc/RunIISummer16NanoAODv7/TChiWZ_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/NANOAODSIM/PUSummer16v3Fast_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/260000/F2E35129-DE8F-CB43-B8A3-DE4972DB4B9A.root



OUTPUT=TestNtuple.root

DSET=TChiWZ #_OS-SS  #crab_TTJets-DiLept-FullSim-2016-Test 
#FTAG=Fall17_102X
FTAG=Summer16_102X

#-eventcount
EVTCT=./root/EventCount/EventCount_NANO_Summer16_102X_SMS.root
#-filtereff
FILTEREFF=./root/FilterEff/SMS-TChiWZ_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root

#-btag
BTAGFOLD=./BtagSF/
#-svfile
SVFILE=./json/lwtnn/nano_train_model.json
#-metfile
#METFILE=./csv/METTrigger/ #Parameters.csv
METFILE=./csv/METTrigger/Parameters.csv
#METFILE=METParameters.csv

#-pu
PUFOLD=./root/PU/
#-json
JSON=./json/GoodRunList/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt
#-jme
JME=./data/JME/

#-prefirefile
PREFIRE=./root/C1mN2_wino_13TeV.root
#-tree
TREE=Events


./MakeReducedNtuple_NANO.x -ifile=${INPUT} -ofile=${OUTPUT} -dataset=${DSET} -filetag=${FTAG} -eventcount=${EVTCT} -filtereff=${FILTEREFF} -btag=${BTAGFOLD} -svfile=${SVFILE} -metfile=${METFILE} -pu=${PUFOLD} -jme=${JME} -tree=${TREE} --sms

