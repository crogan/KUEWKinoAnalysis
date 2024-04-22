

#iFILE=../BFI_B127/BFI_B127_TChiWZ16_SYS16.root
#testFILE=TEST_GENMET.root
#iFILE=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B127_TChiWZ_SYS/BFI_B127_TChiWZ16_SYS.root

#Build 127 full three years 
#iFILE=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B127/BFI_B127_TChiWZSYS161718.root
#iFILE=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B127/BFI_B127_TChiWZ161718_SIGSYS161718.root
#testFILE=BFI_B127_TChiWZSYS_FullSigSYS

iFILE=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B130/BFI_B130_TChiWZ161718_SYS.root
testFILE=BFI_B127_TChiWZ161718_SYSFull

#rm ${testFILE}.root
#cp ${iFILE} ${testFILE}.root
#./sig.x ${testFILE}.root
#root -l -b -q "jankyTTreeMacro.C(\"${testFILE}.root\")"
hadd -f ${testFILE}_UpdatedProc.root ${testFILE}.root testtree.root
