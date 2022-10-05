# calcWeights.py

import json

def loadJson(input_json):
    with open(input_json, 'r') as f:
        data = json.load(f)
        return data

def getLumi(era):
    lumi_json   = "json/samples/Lumis.json"
    lumi_data   = loadJson(lumi_json)
    lumi        = -999
    
    if era.lower() == "run2":
        lumi = lumi_data["2016"] + lumi_data["2017"] + lumi_data["2018"]
    elif era in lumi_data:
        lumi = lumi_data[era]
    else:
        print("ERROR: The era '{0}' was not found in the file '{1}'.".format(era, lumi_json))
    return lumi

def getWeight(kfactor, xsec, lumi, nevents):
    weight = (kfactor * xsec * lumi) / nevents
    return weight

def caclWeights():
    print("Let's go!")
    info_json_2017 = "json/samples/Info_UL2017_NanoAODv9.json"
    eras = ["2016", "2017", "2018", "Run2"]
    samples = ["name"]
    sample_data = loadJson(info_json_2017)
    
    for era in eras:
        lumi = getLumi(era)
        print("era: {0}, lumi: {1:.3f}".format(era, lumi))
    
    era = "2017"
    lumi = getLumi(era)
    for sample in sample_data:
        kfactor = sample_data[sample]["kfactor"]
        xsec    = sample_data[sample]["xsec"]
        nevents = sample_data[sample]["nevents"]
        weight  = getWeight(kfactor, xsec, lumi, nevents)
        print("sample: {0}, weight: {1:.3f}".format(sample, weight))

def main():
    caclWeights()

if __name__ == "__main__":
    main()

