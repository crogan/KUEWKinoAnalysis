



f16 = open("corrections2016/grep_meanlambda_2016.txt")
f17 = open("corrections2017/grep_meanlambda_2017.txt")
f18 = open("corrections2018/grep_meanlambda_2018.txt")


data = []


for line in f16:
	#print(line)
	split_line = line.split(" ")
	#print(split_line)
	key = "_".join(split_line[1:4])
	data.append([key, split_line[9]]) 

#print(data16)

for line in f17:
#	print(line)
	split_line = line.split(" ")
	key = "_".join(split_line[1:4])
	data.append([key, split_line[9]])

for line in f18:
	#print(line)
	split_line = line.split(" ")
	key = "_".join(split_line[1:4])
	data.append([key, split_line[9]])


print( "std::map<std::string, double> bLambdas{};")
for dat in data:
	#print(dat)
	print("bLambdas[\""+dat[0]+"\"] = "+str(dat[1])+";")

