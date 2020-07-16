# KUEWKinoAnalysis
**KUEWKinoAnalysis**

---------------------
CMSSW-dependent build 
---------------------

In order for the KUEWKinoAnalysis package to build the `BuildFit.x`
executable, the user must make sure that **CMSSW** is available, and
include the **KEWKinoAnalysis** package in the correct location in the
**CMSSW** directory structure. You will also need the **CombineHarvester**
and **HiggsAnalysis** **CMSSW** packages. These packages must be included
in the **CMSSW** directory structure as:

	- CMSSW_Z_Y_X
	  - src
		- CombineHarvester
		- HiggsAnalysis
		- KUEWKinoAnalysis

You can set up a **CMSSW** area and checkout the required packages by
performing the terminal commands below.  (*Note: You will probably
need CMSSW_10_2_X or later*)

### Setting up environmental variables for CMSSW
- These are commands that I have in my `.bash_profile` script on the
T3 that get called at terminal startup

	source /cvmfs/cms.cern.ch/cmsset_default.sh
	source /cvmfs/cms.cern.ch/crab3/crab.sh
	export SCRAM_ARCH=slc7_amd64_gcc700
	

Once you have checked out a **CMSSW** package with the directory
`CMSSW_Z_Y_X` (*Note: You will probably need CMSSW_10_2_X or later*)
you can checkout the other packages with the terminal commands:

	>$ ./configure
	>$ make
	>$ make install



Installation instructions can be found in the INSTALL file provided with the 
RestFrames distribution.
