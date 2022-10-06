# calcWeights.py

import json
import csv

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
    info_json_2017  = "json/samples/Info_UL2017_NanoAODv9.json"
    output_csv      = "csv/samples/Weights_UL2017_NanoAODv9.csv"
    eras            = ["2016", "2017", "2018", "Run2"]
    sample_data = loadJson(info_json_2017)
    
    #for era in eras:
    #    lumi = getLumi(era)
    #    print("era: {0}, lumi: {1:.3f}".format(era, lumi))
    
    column_titles = ["sample", "kfactor", "xsec", "lumi", "nevents", "weight"]
    era = "2017"
    lumi = getLumi(era)
    with open(output_csv, 'w', newline='') as f:
        csv_writer = csv.writer(f)
        csv_writer.writerow(column_titles)
        for sample in sample_data:
            kfactor = sample_data[sample]["kfactor"]
            xsec    = sample_data[sample]["xsec"]
            nevents = sample_data[sample]["nevents"]
            weight  = getWeight(kfactor, xsec, lumi, nevents)
            row     = [sample, kfactor, xsec, lumi, nevents, weight]
            csv_writer.writerow(row)
            print(row)
            #print("sample: {0}, weight: {1:.3f}".format(sample, weight))

def main():
    caclWeights()

if __name__ == "__main__":
    main()

