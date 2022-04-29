# KUEWKinoAnalysis
**KUEWKinoAnalysis** code package implementing the KU EWKino Analysis

---------------------
CMSSW-dependent build 
---------------------

In order for the KUEWKinoAnalysis package to build the `BuildFit.x` executable,
the user must make sure that **CMSSW** is available and include the **KEWKinoAnalysis** package
in the correct location in the **CMSSW** directory structure.
You will also need the **CombineHarvester** and **HiggsAnalysis** **CMSSW** packages.
These packages must be included in the **CMSSW** directory structure as:

- CMSSW_Z_Y_X
  - src
    - CombineHarvester
    - HiggsAnalysis
    - KUEWKinoAnalysis

You can set up a **CMSSW** area and checkout the required packages by performing the terminal commands below. 

### Setting up environmental variables for CMSSW
These are commands that I have in my `.bash_profile` script on the T3 that get called at terminal startup
```
    >$ source /cvmfs/cms.cern.ch/cmsset_default.sh
    >$ source /cvmfs/cms.cern.ch/crab3/crab.sh
    >$ export SCRAM_ARCH=slc7_amd64_gcc700
```    
### Setting up CMSSW area
You will need CMSSW_10_6_5

    >$ scram project CMSSW CMSSW_10_6_5
    >$ cd CMSSW_10_6_5/src
    >$ cmsenv
    
### Checking out required packages
    >$ git clone https://github.com/zflowers/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
    >$ git clone https://github.com/zflowers/CombineHarvester.git CombineHarvester
    >$ git clone https://github.com/crogan/KUEWKinoAnalysis.git KUEWKinoAnalysis
Switch to the KU branch in HiggsAnalysis and the connect branch in CombineHarvester
    >$ git checkout KU  
    >$ git checkout connect


### Build/compile  everything
Build the CMSSW packages (*Note: We're still in the* `CMSSW_10_6_5/src` *directory*)

    >$ scram b
Build **KUEWKinoAnalysis** (with the `BuildFit.x` executable). (*Note:
at this point you will need to have set the **RestFrames**
environmental varables set, i.e. have done* `source setup_RestFrames.sh`)

    >$ cd KUEWKinoAnalysis
    >$ make cmssw


### Running combineTool.py on cms connect syntax
To run text2workspace (T2W):
Go to the directory with the datacard for a given mass point

    >$ combineTool.py -M T2W -i datacard.txt -m MASS_Value --job-mode connect -o workspace.root --input-file ../../../FitInput_KUEWKino_2017.root --sub-opts='+ProjectName=cms.org.ku" \n request_memory = 8 GB \n'

Note that mass value will typically be the name of the directory that the datacard is in (ex: 5000325) and the input file is the path to the output root file from BuildFit.x

To run AsymptoticLimits:
Go to the directory that was the output of BuildFit.x 

    >$ combineTool.py -M AsymptoticLimits -d */*/*/datacard.txt --job-mode connect --input-file FitInput_KUEWKino_2017.root --sub-opts='+ProjectName="cms.org.ku" \n request_memory = 8 GB \n'

To run impacts:
Go to the directory that the workspace (and datacard) live in

    >$ combineTool.py -M Impacts -d workspace.root -m MASS_Value --doInitialFit --robustFit 1 --job-mode connect --sub-opts='+ProjectName="cms.org.ku" \n request_memory = 8 GB \n'
    >$ combineTool.py -M Impacts -d workspace.root -m 5000325 --input-file ../ --job-mode connect --sub-opts='+ProjectName="cms.org.ku" \n request_memory = 8 GB \n' 

Note that the second step only should be ran after the jobs in the first step finish
The input file is just the entire directory that the workspace lives in
