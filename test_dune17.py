# test_dune17.py

# David Adams
# July 2017
#
#
# Tests dune17.py and creates the data file used by filldb17c
# to find DUNE17-cold datasets.

from dune17 import dune17cSamples
from dune17 import dune17cChips
from dune17 import dune17tscSamples
from dune17 import dune17tscChips
from dune17 import badChips

writeDsfile = True

def testDataset(dsname, writeDsFile, showChips):

  grpsams = []
  isPDTS = False
  isCETS = False
  if dsname == "DUNE17-cold":
    sams     = dune17cSamples()
    grpsams.append(sams)
    grpsams.append(dune17cSamples(group=1))
    grpsams.append(dune17cSamples(group=2))
    remsams  = dune17cSamples(skipSel=True)
    badsams  = dune17cSamples(isBad =True)
    nwfsams  = dune17cSamples(isNwf =True)
    failsams = dune17cSamples(isFail =True)
    chips = dune17cChips()
    remchips = dune17cChips(skipSel=True)
    isPDTS = True
  elif dsname == "DUNE17ts-cold":
    sams = dune17tscSamples()
    remsams = dune17tscSamples(skipSel=True)
    badsams = dune17tscSamples(isBad=True)
    failsams = dune17tscSamples(isFail=True)
    dnlsams = dune17tscSamples(isDNL=True)
    chips = dune17tscChips()
    remchips = dune17tscChips(skipSel=True)
    isCETS = True
  else:
    print "Invalid dsname: " + dsname
    return

  print dsname + ":"
  print "    Bad sample count is " + str(len(badsams))
  if isPDTS:
    print "    NWF sample count is " + str(len(nwfsams))
  print "   Fail sample count is " + str(len(failsams))
  print "    Rem sample count is " + str(len(remsams))
  if isCETS:
    print "    DNL sample count is " + str(len(dnlsams))
  print "   Good sample count is " + str(len(sams))
  print "     Good chip count is " + str(len(chips))
  ngrp = len(grpsams)
  for igrp in range(1,ngrp):
    print "       Group " + str(igrp) +  " count is " + str(len(grpsams[igrp]))
  print " Rem good chip count is " + str(len(remchips))

  if showChips:
    print "Rem chips: " + str(remchips)
  
  if writeDsfile:
    # Failed samples
    outfName = dsname + "-fail.txt"
    outf = open(outfName, "w")
    for ds in failsams:
      if writeDsfile: outf.write(ds + "\n")
    print "Fail sample list written to " + outfName
    # Bad samples
    outfName = dsname + "-bad.txt"
    outf = open(outfName, "w")
    for ds in badsams:
      if writeDsfile: outf.write(ds + "\n")
    print "Bad sample list written to " + outfName
    # NWF samples
    if isPDTS:
      outfName = dsname + "-nwf.txt"
      outf = open(outfName, "w")
      for ds in nwfsams:
        if writeDsfile: outf.write(ds + "\n")
      print "NWF sample list written to " + outfName
    # DNL samples
    if isCETS:
      outfName = dsname + "-dnl.txt"
      outf = open(outfName, "w")
      for ds in dnlsams:
        if writeDsfile: outf.write(ds + "\n")
      print "DNL sample list written to " + outfName
    # Good unselected samples
    outfName = dsname + "-rem.txt"
    outf = open(outfName, "w")
    for ds in remsams:
      if writeDsfile: outf.write(ds + "\n")
    print "Rem good sample list written to " + outfName
    # Good samples
    outfName = dsname + ".txt"
    outf = open(outfName, "w")
    for ds in sams:
      if writeDsfile: outf.write(ds + "\n")
    print "Good sample list written to " + outfName
    if dsname == "DUNE17-cold":
      # Old samples
      outfName = dsname + "-old.txt"
      outf = open(outfName, "w")
      for ds in grpsams[1]:
        if writeDsfile: outf.write(ds + "\n")
      print "Good sample list written to " + outfName
      # New samples
      outfName = dsname + "-new.txt"
      outf = open(outfName, "w")
      for ds in grpsams[2]:
        if writeDsfile: outf.write(ds + "\n")
      print "Good sample list written to " + outfName

showChips = False
print
testDataset("DUNE17-cold", True, showChips)
print
testDataset("DUNE17ts-cold", True, showChips)
print
print "Fetching bad chip list."
chips = badChips()
print "Bad chip count: " + str(len(chips))
if writeDsfile:
  fname = "DUNE17-badchips.txt"
  outf = open(fname, "w")
  for chip in chips:
    outf.write(str(chip) + "\n")
  print "Bad chip list written to " + fname
print
print "Fetching length all remaining chips."
allRemSams = list(set(dune17cChips(skipSel=True) + dune17tscChips(skipSel=True)))
print "All rem count: " + str(len(allRemSams))

