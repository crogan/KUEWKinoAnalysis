import argparse
import os


parser = argparse.ArgumentParser()
parser.add_argument('--input','-i',help='input root directory',required=True)


args = parser.parse_args()

if args.input is None:
	print("Need input root directory, -i [path/to/root/files]")
	exit()
if not os.path.isdir(args.input):
	print("Directory doesn't exist")
	exit()
path = args.input[:args.input.find("root")]
txtFile = "out.txt"
oFile = "resubmit.sh"
os.system("ls -lha "+args.input+" > "+txtFile)
jobNums = []
for i in open(txtFile):
	if " 0 " in i:
		idx1 = i.find("BFI")
		idx2 = i.find(".root")
		rFile = i[idx1:idx2]
		jobNums.append(rFile[rFile.find("_")+1:])
if len(jobNums) == 0:
	print("No jobs to resubmit")
	exit()
for job in jobNums:
	os.system("condor_submit "+path+"src/submit_"+job+".sh")
print(str(len(jobNums))+" job(s) submitted")



