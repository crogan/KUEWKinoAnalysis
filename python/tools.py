# tools.py

import json

def loadJson(input_json):
    with open(input_json, 'r') as f:
        data = json.load(f)
        return data

