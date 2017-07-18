# test_dune17.py

# David Adams
# July 2017
#
#
# Tests dune17.py and creates the data file used by filldb17c
# to find DUNE17-cold datasets.

from dune17 import dune17cDatasets
from dune17 import dune17cChips
from dune17 import dune17tscDatasets
from dune17 import dune17tscChips

writeDsfile = True

def testDataset(dsname, writeDsFile, showChips):

  if dsname == "DUNE17-cold":
    sams = dune17cDatasets()
    badsams = dune17cDatasets(isBad =True)
    failsams = dune17cDatasets(isFail =True)
    chips = dune17cChips()
  elif dsname == "DUNE17ts-cold":
    sams = dune17tscDatasets()
    badsams = dune17cDatasets(isBad =True)
    failsams = dune17cDatasets(isFail =True)
    chips = dune17tscChips()
  else:
    print "Invalid dsname: " + dsname
    return

  print dsname + ":"
  print "  Bad sample count is " + str(len(badsams))
  print " Fail sample count is " + str(len(failsams))
  print " Good sample count is " + str(len(sams))
  print "   Good chip count is " + str(len(chips))

  if showChips:
    print "Chips: " + str(chips)
  
  if writeDsfile:
    outfName = dsname + "-fail.txt"
    outf = open(outfName, "w")
    for ds in failsams:
      if writeDsfile: outf.write(ds + "\n")
    print "Fail dataset list written to " + outfName
    outfName = dsname + "-bad.txt"
    outf = open(outfName, "w")
    for ds in badsams:
      if writeDsfile: outf.write(ds + "\n")
    print "Bad dataset list written to " + outfName
    outfName = dsname + ".txt"
    outf = open(outfName, "w")
    for ds in sams:
      if writeDsfile: outf.write(ds + "\n")
    print "Good dataset list written to " + outfName

showChips = True
print
testDataset("DUNE17-cold", True, showChips)
print
testDataset("DUNE17ts-cold", True, showChips)
