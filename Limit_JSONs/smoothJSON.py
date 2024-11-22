import json
import math
import sys
#fname = sys.argv[1]
#oname = sys.argv[2]
# Opening JSON file
#f = open('B135_bugfix16_TChiWZ_limit.json')
#f = open('B135_bugfix16_T2tt_limit.json')


def loadJSON( jfilename ):
	with open(jfilename, 'r') as jsonfile:
# Reading from json file
		json_object = json.load(jsonfile)
	#print(json_object)
	#print(type(json_object))
	jsonfile.close()
	return json_object
	


def getMpMasses( json_object ):
	Mpset = set()
	for key in json_object:
		mp = math.trunc(float(key)/10000.)
		Mpset.add(mp)
	
	MpSorted = sorted( Mpset )
	#for item in MpSorted:
	#	print(item)
	return MpSorted
	
def getdMfromMP( json_object, Mp):
	dMset = set()
	for key in json_object:
		mp = math.trunc(float(key)/10000.)
		if mp == Mp:
			#print(key, mp, Mp)
			mc = int(float(key)) - mp*10000
			#print(mc)
			dMset.add( mp - mc )
			
	dMsorted = sorted(dMset)
	return dMsorted

def getGriddMSet( grid ):
	griddMSet = set()
	for key in grid:
		for dM in grid[key]:
			griddMSet.add( dM )
	
	return sorted(list(griddMSet))
	
def getGridMpSet( grid ):
	gridMpSet = set()
	for key in grid:
		gridMpSet.add( key)
		
	return sorted(list(gridMpSet))


def gridSnap(Mp, dM, grid):
	for idM in grid[Mp]:
		if dM == idM:
			return[Mp,dM]
	#not found scan -1
	for idM in grid[Mp]:
		if (dM-1) == idM:
			return[Mp,idM]
	#not found scan +1
	for idM in grid[Mp]:
		if (dM+1) == idM:
			return[Mp,idM]
	
	return [-1,-1]
	
def checkMonotonic(beforeVal,midVal,afterVal):
	if( beforeVal < midVal and midVal < afterVal):
		return True
	if( beforeVal > midVal and midVal > beforeVal):
		return True
	else:
		return False

def makeMonotonic(json_object, keyLw, keyMd, keyHi):
	jsonkeys=["exp+1","exp+2","exp-1","exp-2","exp0","obs"]	
	#jsonkeys=["obs"]
	isMonotonic = False
	for jkey in jsonkeys:
		beforeVal = json_object[keyLw][jkey]
		midVal    = json_object[keyMd][jkey]
		afterVal  = json_object[keyHi][jkey]
		isMonotonic = checkMonotonic(beforeVal, midVal, afterVal)
		if( isMonotonic ):
			continue
		else:
			print("found non-monotic", jkey)
			print(keyLw,keyMd,keyHi)
			print(beforeVal,midVal,afterVal)
			print("adjusted to:", (beforeVal+afterVal)/2.)
			json_object[keyMd][jkey] = (beforeVal +afterVal)/2.
	

def smoothMPaxis( json_object, MpSet, dMSet, grid, lowCut, dmcut):
	
	mpCut= []
	for mp in MpSet:
		if mp > lowCut:# and mp < highcut:
			mpCut.append(mp)
	for dM in dMSet:
		print("MPsmooth ",mp,dM, lowCut,dmcut)
		if dM > dmcut:
			print("continue on MPAXIS smooth dm",dM,dmcut)
			continue
		for i, mp in enumerate(mpCut):
			if(i>0 and i<(len(mpCut)-1)):
#				print("assess dM",dM)
#				print( MpSet[i-1] )
#				print( MpSet[i] )
#				print( MpSet[i+1] )	
				snapLw = gridSnap(mpCut[i-1], dM, grid)
				snapMd = gridSnap(mpCut[i], dM, grid)
				snapHi = gridSnap(mpCut[i+1], dM, grid)
				print("found snaps:",snapLw,snapMd,snapHi)
				
				if( snapLw[0] == -1 or snapMd[0] == -1 or snapHi[0] == -1):
					continue
				
				#construct keys
				keyLw = str(snapLw[0]*10000 + (snapLw[0]-snapLw[1]))+".0"
				keyMd = str(snapMd[0]*10000 + (snapMd[0]-snapMd[1]))+".0"
				keyHi = str(snapHi[0]*10000 + (snapHi[0]-snapHi[1]))+".0"
				
				#adjust values if needed
				makeMonotonic(json_object, keyLw,keyMd,keyHi)

				
def smoothdMaxis( json_object, grid, lowCut, highCut):
	
	for mp in grid:
		for i, dm in enumerate(grid[mp]):
			if( dm < lowCut or dm > highCut):
				print("continue on DMAXIS smooth", dm, lowCut, highCut)
				continue
			if(i>0 and i<(len(grid[mp])-1)):
#				print("assess mp", mp)
#				print(grid[mp][i-1])
#				print(grid[mp][i])
#				print(grid[mp][i+1])
				keyLw = str(mp*10000 + (mp-grid[mp][i-1]))+".0"
				keyMd = str(mp*10000 + (mp-grid[mp][i]))+".0"
				keyHi = str(mp*10000 + (mp-grid[mp][i+1]))+".0"
				
				#adjust values if needed
				makeMonotonic(json_object, keyLw,keyMd,keyHi)

INPUT=sys.argv[1]
OUTPUT=sys.argv[2]	
j = loadJSON(INPUT)

CUT_MP = 0
CUT_DM = 0
CUT_DMAXIS_DM_UP = 9999
#CUT_MP_UP = 9999
CUT_MPAXIS_DM_UP = 9999
#specialCuts
if "TChiWZ" in INPUT:
	CUT_MP = 160
	CUT_DM = 5
	CUT_DMAXIS_DM_UP = 80
	#CUT_MPAXIS_DM_UP = 110
if "T2bW" in INPUT:
	CUT_DM = 30
if "T2cc" in INPUT:
	CUT_DM = 20
if "T2tt" in INPUT:
	CUT_DM = 25
if "WW" in INPUT:
	#CUT_MP=130
	CUT_MPAXIS_DM_UP = 100
	CUT_DMAXIS_DM_UP = 1 #skip dm smoothing in extension
	
#j = loadJSON( "B135_bugfix16_TChiWZ_limit.json")
#jout = "B135_bugfix16_TChiWZ_limit_smooth.json"
#OUTPUT=jout
Mpset = getMpMasses(j)
grid = {}
for mp in Mpset:
	grid[ mp ] = getdMfromMP( j, mp)

for key in grid:
	print(key, grid[key])	
	
griddMSet = getGriddMSet( grid)
gridMpSet = getGridMpSet( grid)
#print( griddMSet )
#print( gridMpSet )
print("Smoothing Mp axis--------------------------")
smoothMPaxis(j, gridMpSet, griddMSet, grid,CUT_MP, CUT_MPAXIS_DM_UP)
print("Smoothing dM axis--------------------------")
smoothdMaxis( j, grid,CUT_DM, CUT_DMAXIS_DM_UP )

#order change?
#smoothdMaxis(j,grid,CUT_DM, CUT_DMAXIS_DM_UP)
#smoothMPaxis(j,gridMpSet,griddMSet, grid, CUT_MP, CUT_MPAXIS_DM_UP)

with open( OUTPUT, "w") as out_data_file:
	json.dump(j, out_data_file,indent=4)
	out_data_file.close() 
