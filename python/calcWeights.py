# calcWeights.py

import csv
import tools

def getLumi(era):
    lumi_json   = "json/samples/Lumis.json"
    lumi_data   = tools.loadJson(lumi_json)
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
    # LowPtElectron UL 2017 NanoAODv9 samples
    era         = "2017"
    info_json   = "json/samples/LowPtElectron_UL{0}_NanoAODv9.json".format(era)
    output_csv  = "csv/samples/LowPtElectron_UL{0}_NanoAODv9_Weights.csv".format(era)
    lumi        = getLumi(era)
    sample_data = tools.loadJson(info_json)
    
    column_titles = ["sample", "kfactor", "xsec", "lumi", "nevents", "weight"]
    
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

def main():
    caclWeights()

if __name__ == "__main__":
    main()

