# tools.py

import os
import json
import numpy as np

# creates directory if it does not exist
def makeDir(dir_name):
    if not os.path.exists(dir_name):
        os.makedirs(dir_name)

# load data from json file
def loadJson(input_json):
    with open(input_json, 'r') as f:
        data = json.load(f)
        return data

# get luminosity for era from json file
def getLumi(era, lumi_json):
    lumi_data   = loadJson(lumi_json)
    lumi        = -999
    
    if era.lower() == "run2":
        lumi = lumi_data["2016"] + lumi_data["2017"] + lumi_data["2018"]
    elif era in lumi_data:
        lumi = lumi_data[era]
    else:
        print("ERROR: The era '{0}' was not found in the file '{1}'.".format(era, lumi_json))
    return lumi

# take square root of hist
def sqrtHist(hist):
    nbins_x = hist.GetNbinsX()
    nbins_y = hist.GetNbinsY()
    #print("nbins_x = {0}".format(nbins_x))
    #print("nbins_y = {0}".format(nbins_y))
    for bin_x in range(1, nbins_x + 1):
        for bin_y in range(1, nbins_y + 1):
            bin_val         = hist.GetBinContent(bin_x, bin_y)
            sqrt_bin_val    = np.sqrt(bin_val)
            #print("bin_x = {0}, bin_y = {1}, bin_val = {2:.3f}, sqrt_bin_val = {3:.3f}".format(bin_x, bin_y, bin_val, sqrt_bin_val))
            hist.SetBinContent(bin_x, bin_y, sqrt_bin_val)
    return

# get 1D hist from 2D hist
def get1DHist(hist2D):
    # project to 1D
    hist1D = hist2D.ProjectionX()
    # to set custom values
    nbins_x = hist2D.GetNbinsX()
    nbins_y = hist2D.GetNbinsY()
    max_y   = nbins_y
    #max_y   = 3
    for bin_x in range(1, nbins_x + 1):
        y_total = 0.0
        for bin_y in range(1, max_y + 1):
            bin_val = hist2D.GetBinContent(bin_x, bin_y)
            y_total += bin_val
        hist1D.SetBinContent(bin_x, y_total)
    return hist1D


