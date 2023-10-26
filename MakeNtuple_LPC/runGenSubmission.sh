# ------------------------------------------------- #
# bash script to call "python generateSubmission.py"
# - edit this script as needed
#
# python options
# - directory (optional): use -d <output_directory> for submission directory; default is "Output"
#
# add space delimited flags for condor (--slim is already automatically included)
# - signal: --sms --sys 
# - fastsim signal: --sms --fastsim --sys
# - data: --data
# - MC: --sys
# - all samples: -filetag=[Summer16_102X,Fall17_102X,Autumn18_102X], according to year [2016,2017,2018]
# - job splitting: use -split=N, where N is the number of jobs per input file
#
# Important: you must use the full path to the input file list as the first "condor" argument!!
# ------------------------------------------------- #

# Example: 2016 period H data using a custom directory name and splitting.
# python generateSubmission.py /uscms/home/caleb/nobackup/KU_Compressed_SUSY/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Summer16_102X_Data/MET/MET_Run2016H-02Apr2020-v2_2016.txt -d Output_2016_data_v5 --data -filetag=Summer16_102X -split=20

##TEST Launches
#python generateSubmission.py /uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Summer16_102X/DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt --sys -filetag=Summer16_102X -split=20
#python generateSubmission.py /uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/Fall17_102X/DYJetsToLL_M-4to50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8.txt --sys -filetag=Fall17_102X


