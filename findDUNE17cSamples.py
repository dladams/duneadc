# findDUNE17cSamples.py
#
# David Adams
# August 2017

import sys
import os
import json

docpp = True
dotxt = True
topdir = "/home/dladams/data/dune/adc/DUNE17q"
dirs = []
if len(sys.argv) < 3:
  print topdir
  fnames = []
  for dir in os.listdir(topdir):
    path = topdir + "/" + dir
    for stime in os.listdir(path):
      tspath = path + "/" + stime
      ipos = stime.find("T")
      if stime[8] != "T":
        print "Directory is not a timestamp: " << tspath
        continue
      print
      print tspath
      for stype in os.listdir(tspath):
        print "  " + stype
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
    print "File not found: " + jfname
    continue
  jfile = open(jfname, "r")
  jmap = json.load(jfile)
  isCold = str(jmap["isCold"]) == "True"
  stimeFull = str(jmap["session_start_time"])
  stime = stimeFull[4:13]
  if stime in stimes:
    print "Duplicate time: " + stime
    exit(1)
  stimes.append(stime)
  if not isCold:
    print "Skipping warm sample " + dir
    continue
  adcs = jmap["asic_ids"]
  sockets = jmap["asicsockets"]
  nadc = len(adcs)
  if len(sockets) != nadc:
    print "Skipping inconsistent socket count in " + dir
  for iadc in range(0,nadc):
    sadc = adcs[iadc]
    socket = sockets[iadc]
    dfname = dir + "/funcgenMeasurement_asic_" + str(socket) + "-parseBinaryFile.root"
    if not os.path.isfile(dfname):
      print "File not found: " + dfname
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
  oftxt = open("DUNE17q-cold.txt", "w")

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
  print sam + ": " + dfname

if dotxt:
  oftxt.close()
  print "  txt output file: " + oftxt.name

if docpp:
  ofcpp.close()
  print "  cpp output file: " + ofcpp.name

exit(1)

if False:
  for fname in fnames: print fname
  exit(0)
  
docpp = false
if opt == "all":
  print header
elif opt == "save":
  docpp = True
  ofname = topdir + "/fembjson.dat"
  of = open(ofname, "w")
  print header
  of.write(header + "\n")
count = 0
first = True
for fname in fnames:
  tsf = fname.split("/")[7]
  if tsf in noSummaryTimestamps:
    continue
  if not os.path.isfile(fname):
    if opt == "nosum": print tsf
    continue
  f = open(fname)
  s = f.read()
  m = json.loads(s)
  ts = str(m["session_start_time"])
  am_id = int(m["am_ids"][0])
  fm_id = int(m["fm_ids"][0])
  box_id = int(m["box_ids"][0])
  if ts in badTimestamps: continue
  count += 1
  isWarm = int(m["isRoomTemp"])
  adcs = []
  sadcs = m["adc_asics"][0]
  if len(sadcs) == 1:
    sadcs = sadcs[0].split(",")
  for sadc in sadcs:
    if sadc[0] == 'D': sadc = "1" + sadc[1:]
    adc = int(sadc)
    adcs.append(adc)
  line = '%3d.'%count + '%16s'%ts + '%5d'%box_id + '%5d'%am_id + '%5d'%fm_id + '  ['
  firstadc = True
  for adc in adcs:
    line += '%6d'%adc
    if firstadc:
      pyadc = "["
    else:
      pyadc += ", "
    pyadc += str(adc)
    firstadc = False
  line += ']'
  pyadc += "]"
  if isWarm: stemp = "warm"
  else: stemp = "cold"
  line += "  " + stemp
  # Add python text
  if first:
    spyid   = "fembTsIds = {"
    spytemp = "fembTsTemps = {"
    spyadcs = "fembTsAdcs = {"
    pypre = ""
  else:
    pypre = ","
  pypre += '\n  \"' + ts + '":'
  spyid   += pypre + str(box_id)
  spytemp += pypre + '"' + stemp + '"'
  spyadcs += pypre + pyadc
  first = False
  if opt == "all":
    print line
  elif opt == "save":
    print line
    of.write(line + "\n");
  elif opt == "ts":
    print ts

# Finsish python
pyclose = "\n}\n"
spyid   += pyclose
spytemp += pyclose
spyadcs += pyclose

if opt == 'save':
  print "Output file: " + ofname
elif opt == "python":
  ofname = "fembdata.py"
  of = open(ofname, "w")
  of.write(spyid);
  of.write(spytemp);
  of.write(spyadcs);
  print "Output file: " + ofname
  
