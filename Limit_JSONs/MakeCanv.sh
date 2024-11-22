

###############################################
SIGMODE=$1 #T2bW=1 TSlepSnu=2 T2tt=3 TChiWZ=4 HinoN2C1=5 TChipmWW=6 T2cc=7 TChipmWWSuper=8 TChiWZSuper=9 HinoN2C1Super=10 #T2ttSuper=11

SRCMODE=$2 #original=1 xseccorrected=2 xsec+smooth=3

SIG=x
ENUM=y

if [ ${SIGMODE} -eq 1 ]
then
  SIG=T2bW
  ENUM=kT2bW
fi

if [ ${SIGMODE} -eq 2 ]
then
  SIG=TSlepSnu
  ENUM=kTSlSnu
fi

if [ ${SIGMODE} -eq 3 ]
then
  SIG=T2tt
  ENUM=kT2tt
fi

if [ ${SIGMODE} -eq 4 ]
then
  SIG=TChiWZ
  ENUM=kTChiWZ
fi

if [ ${SIGMODE} -eq 5 ]
then
  SIG=HinoN2C1
  ENUM=kHN2C1
fi

if [ ${SIGMODE} -eq 6 ]
then
  SIG=TChipmWW
  ENUM=kTChiWW
fi

if [ ${SIGMODE} -eq 7 ]
then
  SIG=T2cc
  ENUM=kT2cc
fi

if [ ${SIGMODE} -eq 8 ]
then
  SIG=TChipmWWSuper
  ENUM=kTChiWW
fi

if [ ${SIGMODE} -eq 9 ]
then
  SIG=TChiWZSuper
  ENUM=kTChiWZ
fi

if [ ${SIGMODE} -eq 10 ]
then
  SIG=HinoN2C1Super
  ENUM=kHN2C1
fi

if [ ${SIGMODE} -eq 11 ]
then
  SIG=T2ttSuper
  ENUM=kT2tt
fi

#SIG=TSlepSleptot
#SIG=TSlepSlepmueL
#SIG=TSlepSlepmueR
#SIG=TSlepSlepmuLR
#SIG=TSlepSlepeLR
#SIG=TSlepSlepMUL
#SIG=TSlepSlepMUR
#SIG=TSlepSlepEL
#SIG=TSlepSlepER
#ENUM=kTSlSl

JSONSRC=xx
JSON=x
CANVSRC=yy
CANV=y
if [ ${SRCMODE} -eq 1 ]
then
  JSONSRC=../Limit_JSONs/original
  JSON=${JSONSRC}/B135_bugfix16_${SIG}_limit.json
  CANVSRC=../Limit_JSONs/original_canvs
  CANV=${CANVSRC}/B135_bugfix16_${SIG}_canv.root
fi

if [ ${SRCMODE} -eq 2 ]
then
  JSONSRC=../Limit_JSONs/xsec_corrected
  JSON=${JSONSRC}/B135_bugfix16_${SIG}_limit_xsec.json
  CANVSRC=../Limit_JSONs/xsec_canvs
  CANV=${CANVSRC}/B135_bugfix16_${SIG}_xsec_canv.root
fi

if [ ${SRCMODE} -eq 3 ]
then
  JSONSRC=../Limit_JSONs/smoothed
  JSON=${JSONSRC}/B135_bugfix16_${SIG}_limit_xsec_smooth.json
  CANVSRC=../Limit_JSONs/smooth_canvs
  CANV=${CANVSRC}/B135_bugfix16_${SIG}_xsec_smooth_canv.root
fi 

pushd ../macros
root -l -b -q "myParseLimitJSON.C(\"${JSON}\", true, ${ENUM},\" ${CANV}\")"
popd
