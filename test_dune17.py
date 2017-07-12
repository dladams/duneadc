# test_dune17.py

# David Adams
# July 2017
#
#
# Tests dune17.py and creates the data file used by filldb17c
# to find DUNE17-cold datasets.

from dune17 import dune17cDatasets

writeDsfile = True

dss = dune17cDatasets()
baddss = dune17cDatasets(isBad =True)

print "Bad dataset count is " + str(len(baddss))
print "Good dataset count is " + str(len(dss))
  
if writeDsfile:
  outfName = "DUNE17-cold-bad.txt"
  outf = open(outfName, "w")
  for ds in baddss:
    if writeDsfile: outf.write(ds + "\n")
  print "Good dataset list written to " + outfName
  outfName = "DUNE17-cold.txt"
  outf = open(outfName, "w")
  for ds in dss:
    if writeDsfile: outf.write(ds + "\n")
  print "Good dataset list written to " + outfName
