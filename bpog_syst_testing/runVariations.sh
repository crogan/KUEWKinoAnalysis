

#IFILE16=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B125/BFS_B125_BKG16_DATA16_SYS16.root
#IFILE17=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B125/BFS_B125_BKG17_DATA17_SYS17.root
#IFILE18=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B125/BFS_B125_BKG18_DATA18_SYS18.root

#OFILE16=BFI_B125_BKG16_DATA16_SYSYEAR16.root
#OFILE17=BFI_B125_BKG17_DATA17_SYSYEAR17.root
#OFILE18=BFI_B125_BKG18_DATA18_SYSYEAR18.root


IFILE16=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B130/BFS_B130_BKG16_DATA16_SYS16.root
IFILE17=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B130/BFS_B130_BKG17_DATA17_SYS17.root
IFILE18=/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B130/BFS_B130_BKG18_DATA18_SYS18.root

OFILE16=BFI_B130_BKG16_DATA16_SYSYEAR16.root
OFILE17=BFI_B130_BKG17_DATA17_SYSYEAR17.root
OFILE18=BFI_B130_BKG18_DATA18_SYSYEAR18.root


./bpog.x ${IFILE16} ${IFILE17} ${IFILE18} ${OFILE16} ${OFILE17} ${OFILE18}