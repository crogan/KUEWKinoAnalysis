# ------------------------------------------- #
# add space delimited flags (--slim is already automatically included)
# - signal: --sms --sys 
# - fastsim signal: --sms --fastsim --sys
# - data: --data
# - MC: --sys
# - all samples: -filetag=[Summer16_102X,Fall17_102X,Autumn18_102X], according to year [2016,2017,2018]
# - job splitting: use -split=N, where N is the number of jobs per input file
# ------------------------------------------- #

##Launched
python generateSubmission.py /uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Summer16_102X/DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt --sys -filetag=Summer16_102X -split=20

#python generateSubmission.py /uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Fall17_102X/DYJetsToLL_M-4to50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8.txt --sys -filetag=Fall17_102X


python generateSubmission.py /uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Summer16_102X/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt --sys -filetag=Summer16_102X -split=2
#python generateSubmission.py /uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Summer16_102X/DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt --sys

## NotLaunched
#python generateSubmission.py /uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Summer16_102X/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt --sys
#python generateSubmission.py /uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Summer16_102X/DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt --sys
#python generateSubmission.py /uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Summer16_102X/DYJetsToLL_M-50_HT-70to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt --sys
#python generateSubmission.py /uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Summer16_102X/DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt --sys

