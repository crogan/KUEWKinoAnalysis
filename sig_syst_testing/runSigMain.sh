

#iFILE=../BFI_B127/BFI_B127_TChiWZ16_SYS16.root
#testFILE=TEST_GENMET.root
#iFILE=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B127_TChiWZ_SYS/BFI_B127_TChiWZ16_SYS.root

#Build 127 full three years 
#iFILE=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B127/BFI_B127_TChiWZSYS161718.root
#iFILE=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B127/BFI_B127_TChiWZ161718_SIGSYS161718.root
#testFILE=BFI_B127_TChiWZSYS_FullSigSYS

#iFILE=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B130/BFI_B130_TChiWZ161718_SYS.root
#testFILE=BFI_B127_TChiWZ161718_SYSFull

#rm ${testFILE}.root
#cp ${iFILE} ${testFILE}.root
#./sig.x ${testFILE}.root
#root -l -b -q "jankyTTreeMacro.C(\"${testFILE}.root\")"
#hadd -f ${testFILE}_UpdatedProc.root ${testFILE}.root testtree.root

#SIG=TChiWZ
#SIG=TChipmWW
#SIG=T2tt
#SIG=HinoN2C1
#SIG=TSlepSleptot
#SIG=TSlepSlepmueL
#SIG=TSlepSlepmueR
#SIG=TSlepSlepmuLR #crash
#SIG=TSlepSlepeLR #crash
#SIG=TSlepSlepMUL 
#SIG=TSlepSlepMUR
#SIG=TSlepSlepEL
#SIG=TSlepSlepER
#SIG=tthighdM
SIG=T2bW
#BFI_B135_bugfix16_tthighdM161718_SYS161718.root
#BFI_B130_TSlepSleptot161718_SYS.root
#BuildID=B130
BuildID=B135_bugfix16
#iFILE=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_${BuildID}/BFI_${BuildID}_${SIG}161718_SYS.root
iFILE=/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/BFI_B135_bugfix16/BFI_B135_bugfix16_${SIG}161718_SYS161718.root
testFILE=BFI_${BuildID}_${SIG}161718_SYSFull
cp ${iFILE} ${testFILE}.root
./sig.x ${testFILE}.root
root -l -b -q "jankyTTreeMacro.C(\"${testFILE}.root\")"
hadd -f ${testFILE}_UpdatedProc.root ${testFILE}.root testtree.root

#xrdcp ${testFile}_UpdatedProc.root  
