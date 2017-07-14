# test_teststand.py

# David Adams
# July 2017
#
#
# Tests teststand.py and creates the sample lists for the test stand.
# to find DUNE17-cold datasets.

from teststand import samplesMarch17

writeDsfile = True

sams = samplesMarch17()

print "Sample count is " + str(len(sams))
  
if writeDsfile:
  outfName = "201703a.txt"
  outf = open(outfName, "w")
  for ds in sams:
    if writeDsfile: outf.write(ds + "\n")
  print "Good dataset list written to " + outfName

if len(sams) != 77:
  print "ERROR: Sample size: " + str(sams)
