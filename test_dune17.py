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

writeDsfile = True

def testDataset(dsname, writeDsFile, showChips):

  grpsams = []
  if dsname == "DUNE17-cold":
    sams     = dune17cSamples()
    grpsams.append(sams)
    grpsams.append(dune17cSamples(group=1))
    grpsams.append(dune17cSamples(group=2))
    remsams  = dune17cSamples(skipSel=True)
    badsams  = dune17cSamples(isBad =True)
    failsams = dune17cSamples(isFail =True)
    chips = dune17cChips()
    remchips = dune17cChips(skipSel=True)
  elif dsname == "DUNE17ts-cold":
    sams = dune17tscSamples()
    remsams = dune17tscSamples(skipSel=True)
    badsams = dune17tscSamples(isBad=True)
    failsams = dune17tscSamples(isFail=True)
    chips = dune17tscChips()
    remchips = dune17tscChips(skipSel=True)
  else:
    print "Invalid dsname: " + dsname
    return

  print dsname + ":"
  print "    Bad sample count is " + str(len(badsams))
  print "   Fail sample count is " + str(len(failsams))
  print "    Rem sample count is " + str(len(remsams))
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
allRemSams = list(set(dune17cChips(skipSel=True) + dune17tscChips(skipSel=True)))
print "All rem count: " + str(len(allRemSams))

