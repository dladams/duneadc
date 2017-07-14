# test_dune17.py

# David Adams
# July 2017
#
#
# Tests dune17.py and creates the data file used by filldb17c
# to find DUNE17-cold datasets.

from dune17 import dune17cDatasets
from dune17 import dune17cChips

writeDsfile = True

sams = dune17cDatasets()
badsams = dune17cDatasets(isBad =True)
failsams = dune17cDatasets(isFail =True)
chips = dune17cChips()

print "  Bad sample count is " + str(len(badsams))
print " Fail sample count is " + str(len(failsams))
print " Good sample count is " + str(len(sams))
print "   Good chip count is " + str(len(chips))
  
if writeDsfile:
  outfName = "DUNE17-cold-fail.txt"
  outf = open(outfName, "w")
  for ds in failsams:
    if writeDsfile: outf.write(ds + "\n")
  print "Fail dataset list written to " + outfName
  outfName = "DUNE17-cold-bad.txt"
  outf = open(outfName, "w")
  for ds in badsams:
    if writeDsfile: outf.write(ds + "\n")
  print "Bad dataset list written to " + outfName
  outfName = "DUNE17-cold.txt"
  outf = open(outfName, "w")
  for ds in sams:
    if writeDsfile: outf.write(ds + "\n")
  print "Good dataset list written to " + outfName
