#!/usr/bin/env python
# -*- coding: utf-8 -*-
from os.path import isfile
from re import sub

def readlines2str(lines):
    return "".join(lines).rstrip()

def replace(filename, dictionary):
    # Very Basic Error Checking
    if not isfile(filename):
        raise FileNotFoundError(filename, "'{}': is not a file!".format(filename))
    if not type(dictionary) == dict:
        raise TypeError(dictionary, "'{}': is not a dictionary!".format(dictionary))

    # Open the file and read the lines
    with open(filename, 'r') as f:
        lines = f.readlines()

    # Make the replacements
    for l in range(len(lines)):
        for k, v in dictionary.items():
            lines[l] = sub('{{{{{}}}}}'.format(k), v, lines[l])

    return lines            
