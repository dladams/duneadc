# test_dune17.py

# David Adams
# July 2017

from dune17 import dune17cDatasets

dss = dune17cDatasets()

print "Dataset count: " + str(len(dss))

for ds in dss:
  print ds
