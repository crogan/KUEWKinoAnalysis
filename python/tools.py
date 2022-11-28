# tools.py

import ROOT
import os
import json
import numpy as np
import colors

# creates directory if it does not exist
def makeDir(dir_name):
    if not os.path.exists(dir_name):
        os.makedirs(dir_name)

# load data from json file
def loadJson(input_json):
    with open(input_json, 'r') as f:
        data = json.load(f)
        return data

# setup histogram
def setupHist(hist, title, x_title, y_title, color, line_width, x_limits=[], y_limits=[]):
    hist.SetStats(ROOT.kFALSE)
    hist.SetTitle(title)
    hist.SetLineColor(colors.getColorIndex(color))
    hist.SetLineWidth(line_width)
    
    x_axis = hist.GetXaxis()
    y_axis = hist.GetYaxis()
    
    x_axis.SetTitle(x_title)
    y_axis.SetTitle(y_title)
    if x_limits:
        x_axis.SetRangeUser(x_limits[0], x_limits[1])
    if y_limits:
        y_axis.SetRangeUser(y_limits[0], y_limits[1])

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

# take square root of 1D hist
def sqrtHist1D(hist):
    nbins_x = hist.GetNbinsX()
    #print("nbins_x = {0}".format(nbins_x))
    # set bin content to new value:             f = sqrt(A)
    # set bin error based on error propagation: sigma_f = abs(sigma_A / (2 sqrt(A))) 
    for bin_x in range(1, nbins_x + 1):
        bin_val         = hist.GetBinContent(bin_x)
        bin_error       = hist.GetBinError(bin_x)
        if bin_val <= 0.0:
            print("WARNING: In sqrtHist1D(), bin_x = {0}, bin_val = {1}; skipping bin!".format(bin_x, bin_val))
        else:
            sqrt_bin_val    = np.sqrt(bin_val)
            new_bin_error   = abs(bin_error / (2 * sqrt_bin_val))
            #print("bin_x = {0}, bin_val = {1:.3f}, bin_error = {2:.3f}, sqrt_bin_val = {3:.3f}, new_bin_error = {4:.3f}".format(bin_x, bin_val, bin_error, sqrt_bin_val, new_bin_error))
            hist.SetBinContent(bin_x, sqrt_bin_val)
            hist.SetBinError(bin_x, new_bin_error)
    return

# take square root of 2D hist
def sqrtHist2D(hist):
    nbins_x = hist.GetNbinsX()
    nbins_y = hist.GetNbinsY()
    #print("nbins_x = {0}".format(nbins_x))
    #print("nbins_y = {0}".format(nbins_y))
    # set bin contents to new values
    # bin errors are not set to new values yet
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
    hist1D      = hist2D.ProjectionX()
    # to set custom values
    nbins_x     = hist2D.GetNbinsX()
    nbins_y     = hist2D.GetNbinsY()
    # place cut on y bins; include max bin
    max_bin_y   = nbins_y
    #max_bin_y   = 3
    # set bin contents to new values based on cut
    # bin errors are set from projection, but not updated based on cut; should fix this!
    # you can use addition error propagation to get the correct total error
    for bin_x in range(1, nbins_x + 1):
        y_total = 0.0
        for bin_y in range(1, max_bin_y + 1):
            bin_val = hist2D.GetBinContent(bin_x, bin_y)
            y_total += bin_val
        hist1D.SetBinContent(bin_x, y_total)
    return hist1D


