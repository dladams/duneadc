# findDUNE17cSamples.py
#
# David Adams
# August 2017

import sys
import os
import json

myname = "findDUNE17cSamples: "
dbg = 1

docpp = True
dotxt = True
topdir = "/home/dladams/data/dune/adc/DUNE17q"
dirs = []
if len(sys.argv) < 3:
  if dbg > 1: print myname + topdir
  fnames = []
  for dir in os.listdir(topdir):
    path = topdir + "/" + dir
    for stime in os.listdir(path):
      tspath = path + "/" + stime
      ipos = stime.find("T")
      if stime[8] != "T":
        print myname + "Directory is not a timestamp: " << tspath
        continue
      if dbg > 1:
        print myname
        print myname + myname + tspath
      for stype in os.listdir(tspath):
        if dbg > 1: print myname + "  " + stype
        typath = tspath + "/" + stype
        dirs.append(typath)

else:
  dirs = sys.argv[2:]

# Loop over data directories.
samtups = []
stimes = []
filedict = {}
for dir in dirs:
  jfname = dir + "/params.json"
  if not os.path.isfile(jfname):
    if dbg > 1: print myname + "File not found: " + jfname
    continue
  jfile = open(jfname, "r")
  jmap = json.load(jfile)
  isCold = str(jmap["isCold"]) == "True"
  stimeFull = str(jmap["session_start_time"])
  stime = stimeFull[4:13]
  if stime in stimes:
    print myname + "Duplicate time: " + stime
    exit(1)
  stimes.append(stime)
  if not isCold:
    print myname + "Skipping warm sample " + dir
    continue
  adcs = jmap["asic_ids"]
  sockets = jmap["asicsockets"]
  nadc = len(adcs)
  if len(sockets) != nadc:
    print myname + "Skipping inconsistent socket count in " + dir
  for iadc in range(0,nadc):
    sadc = adcs[iadc]
    socket = sockets[iadc]
    dfname = dir + "/funcgenMeasurement_asic_" + str(socket) + "-parseBinaryFile.root"
    if not os.path.isfile(dfname):
      if dbg > 1: print myname + "File not found: " + dfname
      continue
    samtups.append((sadc,stime,dfname))

# Build a unique abbreviation for each time.
stimes.sort()
ntim = len(stimes)
tdict = {}
for itim in range(0, ntim):
  stime = stimes[itim]
  if itim == 0:
    abTimeLast = ""
    abTime = stime[0:4]
  else:
    abTimeLast = abTime
    abTime = abTimeNext
  if itim == ntim-1:
    abTimeNext = ""
  else:
    abTimeNext = stimes[itim+1][0:4]
  if abTime == abTimeLast or abTime == abTimeNext:
    tdict[stime] = stime
  else:
    tdict[stime] = abTime

if docpp:
  oftxt = open("DUNE17q-cold-all.txt", "w")

if docpp:
  ofcpp = open("DUNE17qcSamples.cpp", "w")
  ofcpp.write("std::map<std::string, std::string> DUNE17qcSampleMap;\n")

sampre = "DUNE17q-cold_chip"
ltop = len(topdir) + 1
# Loop over sample pairs and build sample names
for sadc, stime, dfpath in samtups:
  sam = sampre + sadc + "_" + tdict[stime]
  dfname = dfpath[ltop:]
  if dotxt: oftxt.write(sam + '\n')
  if docpp: ofcpp.write('DUNE17qcSampleMap["' + sam + '"] = "' + dfname + '";\n')
  if dbg > 1: print myname + sam + ": " + dfname

if dotxt:
  oftxt.close()
  if dbg > 0: print myname + "  txt output file: " + oftxt.name

if docpp:
  ofcpp.close()
  if dbg > 0: print myname + "  cpp output file: " + ofcpp.name
