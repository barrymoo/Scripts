#!/usr/bin/env python
import os
import re
from pwd import getpwuid

user = os.environ['USER']
files = [f for f in os.listdir('/scratch/') if re.search('.*[a-z, A-Z].*', f)]
for i in range(0, len(files)):
    files[i] = '/scratch/' + files[i]
    if getpwuid(os.stat(files[i]).st_uid).pw_name == user:
            os.system('rm -Rf ' + files[i])
