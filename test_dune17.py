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

print "Dataset count: " + str(len(dss))

outfName = "DUNE17-cold.txt"
if writeDsfile:
  outf = open(outfName, "w")

for ds in dss:
  print ds
  if writeDsfile: outf.write(ds + "\n")
  
print "Dataset list written to " + outfName
