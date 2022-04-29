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
```
- CMSSW_Z_Y_X
  - src
    - CombineHarvester
    - HiggsAnalysis
    - KUEWKinoAnalysis
```

You can set up a **CMSSW** area and checkout the required packages by performing the terminal commands below. 

### Setup environmental variables for CMSSW
Put these commands in `~/.bash_profile` or `~/.bashrc` so that they are run when your terminal session starts:
```
source /cvmfs/cms.cern.ch/cmsset_default.sh
source /cvmfs/cms.cern.ch/crab3/crab.sh
export SCRAM_ARCH=slc7_amd64_gcc700
```    
Note that assigning this SCRAM_ARCH (slc7_amd64_gcc700) is required before
checking out CMSSW_10_6_5 so that the framework can compile successfully.
For this use case, SCRAM_ARCH does matter!
Compile errors have been observed in KUEWKinoAnalysis using CMSSW_10_6_5
when SCRAM_ARCH is not set before setup.

### Setup CMSSW area
Checkout CMSSW_10_6_5.
```
cmsrel CMSSW_10_6_5
cd CMSSW_10_6_5/src
cmsenv
```
    
### Check out required packages
Download these repos in the `CMSSW_10_6_5/src` directory.
Use the KU branch of HiggsAnalysis, the connect branch of CombineHarvester, and the master branch of KUEWKinoAnalysis.
```
git clone -b KU https://github.com/zflowers/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
git clone -b connect https://github.com/zflowers/CombineHarvester.git CombineHarvester
git clone -b master https://github.com/crogan/KUEWKinoAnalysis.git KUEWKinoAnalysis
```

### Setup RestFrames
Setup the RestFrames framework in the `CMSSW_10_6_5/src` directory.
```
wget --no-check-certificate http://stash.osgconnect.net/+zflowers/RestFrames_vNew.tar
tar -xvf RestFrames_vNew.tar
cd RestFrames
./configure --prefix=$CMSSW_BASE/src/restframe_build
make -j8
make install
cd ..
```

### Build/compile
Build the CMSSW packages from the `CMSSW_10_6_5/src` directory.
```
scram b -j8
```
Then build **KUEWKinoAnalysis** (with the `BuildFit.x` executable).
Note that you will need to run the RestFrames setup script
(in this case the one specific to CMS connect) before compiling.
Also note that we compile using CMSSW, which is required.
Compile and run issues have been observed when compiling without CMSSW.
```
cd KUEWKinoAnalysis
source scripts/setup_RestFrames_connect.sh
make cmssw -j8
```

### Running combineTool.py on cms connect syntax
To run text2workspace (T2W):
Go to the directory with the datacard for a given mass point
```
combineTool.py -M T2W -i datacard.txt -m MASS_Value --job-mode connect -o workspace.root --input-file ../../../FitInput_KUEWKino_2017.root --sub-opts='+ProjectName=cms.org.ku" \n request_memory = 8 GB \n'
```

Note that mass value will typically be the name of the directory that the datacard is in (ex: 5000325) and the input file is the path to the output root file from BuildFit.x

To run AsymptoticLimits:
Go to the directory that was the output of BuildFit.x 
```
combineTool.py -M AsymptoticLimits -d */*/*/datacard.txt --job-mode connect --input-file FitInput_KUEWKino_2017.root --sub-opts='+ProjectName="cms.org.ku" \n request_memory = 8 GB \n'
```

To run impacts:
Go to the directory that the workspace (and datacard) live in
```
combineTool.py -M Impacts -d workspace.root -m MASS_Value --doInitialFit --robustFit 1 --job-mode connect --sub-opts='+ProjectName="cms.org.ku" \n request_memory = 8 GB \n'
combineTool.py -M Impacts -d workspace.root -m 5000325 --input-file ../ --job-mode connect --sub-opts='+ProjectName="cms.org.ku" \n request_memory = 8 GB \n' 
```

Note that the second step only should be ran after the jobs in the first step finish
The input file is just the entire directory that the workspace lives in
