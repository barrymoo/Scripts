#!/usr/bin/env python
import os

userName = os.environ['USER']
os.system('squeue -u ' + userName + ' -o "%6i %3P %65j %2t %.12M %.6D %.4C %.R"')

