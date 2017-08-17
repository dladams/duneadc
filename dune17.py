# dune17c-datasets.py

# David Adams
# July 2017

# List of available DUNE17 datasets.
# group: 0 = all, 1=bad clock(before 7/19), 2=good clock
#   isBad true returns bad, nwf and roll
def dune17cSamples(group=0, isNwf=False, isBad=False, isFail=False, skipSel=False, skipBad=True):
  pre = "DUNE17-cold_chip"
  sams1 = []       # Early samples with bad ADC clock settings
  sams2 = []       # Subsequent good samples
  samsh = []       # Good samples with horizontal boards
  badsams = []     # Samples otherwise bad
  nwfsams = []     # Samples without waveforms
  rollsams = []    # Samples with severe rollback
  failsams = []    # Samples tha fail processing
  badsams.append(pre + "1")
  badsams.append(pre + "2_0702")          # waveform very small
  sams1.append(pre + "2_0705")
  badsams.append(pre + "3_0702T15")       # square wave
  sams1.append(pre + "3_0702T16")
  sams1.append(pre + "6")
  sams1.append(pre + "7_0705")
  sams2.append(pre + "7_0721")
  sams1.append(pre + "8_0704")
  badsams.append(pre + "8_0713")   # Bad waveforms.
  sams1.append(pre + "9")
  sams1.append(pre + "10")
  sams1.append(pre + "11")
  badsams.append(pre + "12_0701T16")      # square wave
  badsams.append(pre + "12_0701T17")      # square wave
  nwfsams.append(pre + "12_0705T1141")    # waveform is missing
  sams1.append(pre + "12_0705T1142")
  for chip in range(13,17): sams1.append(pre + str(chip))
  sams1.append(pre + "18")
  badsams.append(pre + "19_0707")
  sams2.append(pre + "19_0725")
  sams1.append(pre + "20")
  sams1.append(pre + "21_0626")
  sams1.append(pre + "21_0706")
  sams1.append(pre + "22_0627")
  sams1.append(pre + "22_0706")
  sams1.append(pre + "23_0627")
  sams1.append(pre + "23_0706")
  sams1.append(pre + "24_0627")
  badsams.append(pre + "24_0707")
  sams1.append(pre + "25_0627")
  badsams.append(pre + "25_0706")
  sams1.append(pre + "26")
  badsams.append(pre + "27_0628")
  sams1.append(pre + "27_0707")
  badsams.append(pre + "28_0630")
  badsams.append(pre + "28_0725")
  sams1.append(pre + "29_0630")
  sams1.append(pre + "29_0701")
  sams1.append(pre + "29_0706")
  badsams.append(pre + "30_0630")    # square wave
  sams1.append(pre + "30_0703")
  for chip in range(39,61): sams1.append(pre + str(chip))
  sams1.append(pre + "61_0626")
  sams1.append(pre + "61_0628")
  badsams.append(pre + "62_0628")
  sams2.append(pre + "62_0726")
  sams1.append(pre + "63_0628T17")
  sams1.append(pre + "63_0628T21")
  sams1.append(pre + "63_0628T22")
  sams1.append(pre + "64")
  badsams.append(pre + "65_0628T193")    # waveform near zero
  badsams.append(pre + "65_0628T194")    # waveform near zero
  badsams.append(pre + "65_0628T195")    # waveform near zero
  badsams.append(pre + "65_0628T200")    # waveform near zero
  sams1.append(pre + "65_0629")
  sams1.append(pre + "66")
  sams1.append(pre + "67_0629T10")
  sams1.append(pre + "67_0629T14")
  sams1.append(pre + "69")
  sams1.append(pre + "70")
  sams1.append(pre + "71_0628T12")
  sams1.append(pre + "71_0628T17")
  sams1.append(pre + "72")    # waveforms a bit ugly
  sams2.append(pre + "73")
  sams1.append(pre + "74")
  badsams.append(pre + "75_0628T2220")  # bad test?
  badsams.append(pre + "75_0628T2228")  # bad test?
  badsams.append(pre + "75_0629")       # bad waveform?
  for chip in range(78,83): sams1.append(pre + str(chip))
  sams1.append(pre + "84")
  badsams.append(pre + "85_0630")
  badsams.append(pre + "85_0725")  # Very small waveforms
  sams1.append(pre + "86")
  sams1.append(pre + "87")
  badsams.append(pre + "88")
  badsams.append(pre + "89")
  sams1.append(pre + "90")
  sams1.append(pre + "92_0630T13")
  sams1.append(pre + "92_0630T17")
  for chip in range(93,107): sams1.append(pre + str(chip))
  sams1.append(pre + "108")
  badsams.append(pre + "109")  # Zero or small waveforms
  sams1.append(pre + "110")
  sams1.append(pre + "111")
  sams1.append(pre + "112_0710")
  badsams.append(pre + "112_0720")  # Very small waveforms
  sams2.append(pre + "112_0726")
  sams1.append(pre + "113")
  sams1.append(pre + "114_0710")
  badsams.append(pre + "114_0711")
  sams1.append(pre + "115")
  badsams.append(pre + "116")
  badsams.append(pre + "117")
  sams1.append(pre + "118")
  sams1.append(pre + "119")
  sams1.append(pre + "120")
  badsams.append(pre + "121_0711T16")   # waveform is very small
  nwfsams.append(pre + "121_0711T18")   # No waveform
  sams1.append(pre + "121_0713")
  sams1.append(pre + "122")
  badsams.append(pre + "123")
  sams1.append(pre + "124")
  sams1.append(pre + "125")
  badsams.append(pre + "126")
  sams1.append(pre + "127")
  badsams.append(pre + "128")
  sams1.append(pre + "129")
  badsams.append(pre + "130")   # waveforms near zero
  badsams.append(pre + "131")
  sams1.append(pre + "132")
  sams1.append(pre + "133_0711")
  sams1.append(pre + "133_0712")
  badsams.append(pre + "135")   # Bad waveforms
  sams1.append(pre + "136")
  sams2.append(pre + "137")
  sams1.append(pre + "138")
  sams2.append(pre + "139_0726T16")
  badsams.append(pre + "139_0726T17")  # Bad waveforms
  sams2.append(pre + "140")
  sams2.append(pre + "141_0727")
  sams2.append(pre + "141_0728T09")
  sams2.append(pre + "141_0728T10")
  sams2.append(pre + "141_0728T12")
  sams2.append(pre + "141_0728T13")
  sams2.append(pre + "141_0728T17")
  sams2.append(pre + "141_0728T19")
  sams2.append(pre + "141_0728T22")
  sams2.append(pre + "141_0728T23")
  sams2.append(pre + "141_0729T00")
  sams2.append(pre + "141_0729T17")
  sams2.append(pre + "141_0729T18")
  sams2.append(pre + "141_0729T21")
  badsams.append(pre + "141_0729T22")  # Bad waveform channel 4
  sams2.append(pre + "141_0730T12")
  sams2.append(pre + "141_0730T15")
  badsams.append(pre + "141_0730T17")  # Bad waveforms
  sams2.append(pre + "141_0731T00")
  badsams.append(pre + "141_0731T11")  # Bad waveforms channels 2, 3
  badsams.append(pre + "144")  # Distorted waveforms
  sams2.append(pre + "145")
  sams2.append(pre + "146")
  sams2.append(pre + "147")
  sams2.append(pre + "148")
  sams2.append(pre + "149")
  sams2.append(pre + "150")
  sams2.append(pre + "151")
  sams2.append(pre + "152")
  sams2.append(pre + "154")
  sams2.append(pre + "155")
  sams2.append(pre + "156")
  sams2.append(pre + "157_0721T1208")
  sams2.append(pre + "157_0721T1252")
  sams2.append(pre + "158")
  sams2.append(pre + "171")
  sams2.append(pre + "172")  # Bad waveforms ch 0-2
  sams2.append(pre + "178_0727T1341")
  sams2.append(pre + "178_0727T1354")
  sams2.append(pre + "179")
  sams2.append(pre + "180")
  sams1.append(pre + "190")
  sams1.append(pre + "191")
  sams1.append(pre + "192")
  sams2.append(pre + "193")
  sams1.append(pre + "194")
  sams2.append(pre + "195")
  sams1.append(pre + "196")
  sams1.append(pre + "197")
  badsams.append(pre + "198_0718")   # Bad waveforms
  badsams.append(pre + "198_0719")  # Bad waveforms cha 10-13
  badsams.append(pre + "199")   # Bad waveforms
  sams2.append(pre + "200")
  sams1.append(pre + "201")
  badsams.append(pre + "202_0723")  # Bad waveforms
  sams2.append(pre + "202_0727")
  badsams.append(pre + "203")
  sams1.append(pre + "204")
  sams1.append(pre + "205")
  sams1.append(pre + "206_0717T16")
  sams1.append(pre + "206_0717T20")
  sams1.append(pre + "207")
  sams1.append(pre + "208")
  sams1.append(pre + "209")
  sams2.append(pre + "210")
  badsams.append(pre + "211")  # Some waveforms are bad
  sams1.append(pre + "212")
  sams1.append(pre + "213")
  sams1.append(pre + "214")
  badsams.append(pre + "215_0718T1834")  # Very poor waveforms
  badsams.append(pre + "215_0718T1844")  # Very poor waveforms
  sams1.append(pre + "216")
  sams1.append(pre + "217")
  sams1.append(pre + "218")
  rollsams.append(pre + "220")
  sams2.append(pre + "221")
  sams2.append(pre + "222")
  sams1.append(pre + "223")
  sams1.append(pre + "224")
  sams1.append(pre + "225")
  badsams.append(pre + "226")  # Bad waveforms
  sams2.append(pre + "227")
  sams2.append(pre + "228")
  sams2.append(pre + "229")
  sams2.append(pre + "230")
  sams2.append(pre + "231")
  sams2.append(pre + "232")
  sams1.append(pre + "233")
  sams2.append(pre + "234")
  sams2.append(pre + "235")
  sams2.append(pre + "236")
  sams2.append(pre + "237")
  sams2.append(pre + "238")
  sams2.append(pre + "240")
  sams2.append(pre + "242")
  sams2.append(pre + "243")
  sams1.append(pre + "244")
  sams2.append(pre + "245")
  sams2.append(pre + "246")
  rollsams.append(pre + "247")
  sams2.append(pre + "248")
  badsams.append(pre + "249")
  sams2.append(pre + "294")
  sams2.append(pre + "297")
  sams2.append(pre + "315")
  badsams.append(pre + "319")  # Not reaching 4095?
  sams2.append(pre + "330")
  sams2.append(pre + "331")
  sams2.append(pre + "332")
  sams2.append(pre + "334")
  badsams.append(pre + "335")  # Bad waveforms
  sams2.append(pre + "336")
  sams2.append(pre + "337")
  sams2.append(pre + "338")
  badsams.append(pre + "339")  # Bad waveform in channels 0-4
  rollsams.append(pre + "340")
  sams2.append(pre + "341")
  sams2.append(pre + "342")
  sams2.append(pre + "343")
  badsams.append(pre + "344_0723")   # Bad waveforms
  sams2.append(pre + "344_0726")
  sams2.append(pre + "345")
  sams2.append(pre + "346")
  sams2.append(pre + "347")
  failsams.append(pre + "349")  # crashes in extremum finding for channel 15
  sams2.append(pre + "351")
  sams2.append(pre + "352")
  sams2.append(pre + "353")
  badsams.append(pre + "354_722")
  sams2.append(pre + "354_726")
  sams2.append(pre + "357")
  sams2.append(pre + "358")
  sams2.append(pre + "359")
  sams2.append(pre + "360")
  sams2.append(pre + "361")
  sams1.append(pre + "362")
  sams1.append(pre + "363_0712T13")
  badsams.append(pre + "363_0712T15")   # distorted waveforms
  sams1.append(pre + "364")
  sams1.append(pre + "365")
  nwfsams.append(pre + "366_0712T1810")   # waveform empty
  sams1.append(pre + "366_0712T1812")
  sams2.append(pre + "367")
  sams1.append(pre + "368")
  sams1.append(pre + "369")
  sams1.append(pre + "370")
  sams1.append(pre + "371")
  sams1.append(pre + "372")
  sams1.append(pre + "373")
  sams1.append(pre + "374")
  sams1.append(pre + "376")
  rollsams.append(pre + "377")
  badsams.append(pre + "378")   # Bad waveforms
  sams1.append(pre + "379")
  sams2.append(pre + "381_0726")
  sams2.append(pre + "381_0727")
  sams1.append(pre + "380")
  failsams.append(pre + "382")   # Voltage calibration fails
  sams1.append(pre + "383")
  sams1.append(pre + "384")
  sams1.append(pre + "385")
  sams1.append(pre + "386")
  sams1.append(pre + "387")
  badsams.append(pre + "389_0718")  # Bad waveforms.
  sams2.append(pre + "389_0726")
  sams1.append(pre + "390_0714")
  sams2.append(pre + "390_0721")
  sams1.append(pre + "392")
  sams1.append(pre + "393")
  badsams.append(pre + "391")
  sams1.append(pre + "394_0715")
  sams2.append(pre + "394_0717")
  badsams.append(pre + "5000001")  # Test only Elizabeth 31jul2017
  badsams.append(pre + "5000003")  # Test only Elizabeth 31jul2017
  badsams.append(pre + "-999999999")
  sams2.append(pre + "D0001_0731")
  sams2.append(pre + "D0002_0731") 
  badsams.append(pre + "D0003_0731")  # Bad waveforms chan 0-7
  badsams.append(pre + "D0003_0731")  # Bad waveforms
  sams2.append(pre + "D0004_0731")
  sams2.append(pre + "D0005_0731")
  sams2.append(pre + "D0006_0731")
  badsams.append(pre + "D0007_0731")  # Bad waveforms
  sams2.append(pre + "D0008_0731")
  sams2.append(pre + "D0009_0731")
  badsams.append(pre + "D0011_0731")  # Bad waveforms
  sams2.append(pre + "D0014_0801")
  sams2.append(pre + "D0015_0801")
  sams2.append(pre + "D0019_0801")
  badsams.append(pre + "D0021_0801")  # Bad waveforms
  sams2.append(pre + "D0023_0801")
  sams2.append(pre + "D0025_0801")
  sams2.append(pre + "D0028_0802")
  sams2.append(pre + "D0029_0802")
  badsams.append(pre + "00031_0802")  # Ignore -- bad chip label
  sams2.append(pre + "D0032_0802")
  sams2.append(pre + "D0034_0802")
  sams2.append(pre + "D0036_0802")
  sams2.append(pre + "D0037_0803")
  sams2.append(pre + "D0039_0803")
  badsams.append(pre + "D0041_0803")  # Bad waveforms
  sams2.append(pre + "D0042_0803")
  badsams.append(pre + "D0043_0803")  # Bad waveforms?
  sams2.append(pre + "D0044_0803")
  sams2.append(pre + "D0053_0804")
  #sams2.append(pre + "D0060_0807")  # warm test.
  sams2.append(pre + "D0063_0808")
  sams2.append(pre + "D0069_0808")
  sams2.append(pre + "D0072_0808")
  sams2.append(pre + "D0074_0808")
  sams2.append(pre + "D0075_0809")
  samsh.append(pre + "D0081_0809")
  samsh.append(pre + "D0083_0809")
  sams2.append(pre + "D0085_0809")
  samsh.append(pre + "D0087_0809")
  samsh.append(pre + "D0088_0809")
  samsh.append(pre + "D0090_0809")
  badsams.append(pre + "D0092_0810")  # Bad waveforms
  sams2.append(pre + "D0093_0810")
  badsams.append(pre + "D0095_0810")  # One bad waveform
  badsams.append(pre + "D0098_0810")  # Ch 5-8 wf bad (samsh)
  sams2.append(pre + "D0100_0810")
  sams2.append(pre + "D0104_0810")
  sams2.append(pre + "D0106_0810")
  sams2.append(pre + "D0107_0810")
  sams2.append(pre + "D0108_0810")
  samsh.append(pre + "D0111_0811")
  sams2.append(pre + "D0112_0811")
  samsh.append(pre + "D0113_0811")
  sams2.append(pre + "D0116_0811")
  sams2.append(pre + "D0117_0811")
  sams2.append(pre + "D0119_0811")
  sams2.append(pre + "D0120_0811")
  sams2.append(pre + "D0122_0811")
  samsh.append(pre + "D0123_0811")
  sams2.append(pre + "D0124_0811")
  samsh.append(pre + "D0125_0811")
  sams2.append(pre + "D0126_0811")
  sams2.append(pre + "D0127_0814")
  sams2.append(pre + "D0129_0814")
  sams2.append(pre + "D0130_0814")
  sams2.append(pre + "D0132_0814")
  sams2.append(pre + "D0134_0814")
  sams2.append(pre + "D00135_0814")
  sams2.append(pre + "D0137_0814")
  sams2.append(pre + "D0138_0814")
  sams2.append(pre + "D0141_0815")
  sams2.append(pre + "D0142_0815")
  sams2.append(pre + "D0143_0815")
  sams2.append(pre + "D0144_0815")
  sams2.append(pre + "D0145_0815")
  badsams.append(pre + "D0147_0815")  # Bad waveforms
  badsams.append(pre + "D0148_0815T1253")  # Waveforms absent.
  sams2.append(pre + "D0148_0815T1307")
  sams2.append(pre + "D0149_0815")
  sams2.append(pre + "D0150_0815")
  sams2.append(pre + "D0151_0815")
  sams2.append(pre + "D0152_0815")
  sams2.append(pre + "D0153_0815")
  sams2.append(pre + "D0156_0815")
  sams2.append(pre + "D0157_0815")
  sams2.append(pre + "D0159_0815")
  sams2.append(pre + "D0160_0815T1621")
  badsams.append(pre + "D0160_0815T1635")  # Waveforms absent.
  sams2.append(pre + "D0160_0815T1646")
  sams2.append(pre + "D0161_0815")
  sams2.append(pre + "D0162_0815")  # 8/17
  if isNwf: outsams = nwfsams
  elif isBad: outsams = badsams + nwfsams + rollsams
  elif isFail: outsams = failsams
  else:
    if   group == 0: outsams = sams1 + sams2 + samsh
    elif group == 1: outsams = sams1
    elif group == 2: outsams = sams2 + samsh
    elif group == 3: outsams = samsh
    else:            outsams = []
  if skipSel or skipBad:
    skipChips = []
    if skipSel: skipChips += selectedChips()
    if skipBad: skipChips += badChips()
    keepsams = []
    for sam in outsams:
      chip = dune17cChip(sam)
      #print "Chip " + str(chip)
      if chip not in skipChips:
        keepsams.append(sam)
    return keepsams
  return outsams
 
# List of available DUNE17dla-cold datasets.
def dune17dlacSamples(isBad =False, isFail=False, skipSel=False, skipBad=True):
  pre = "DUNE17dla-cold_chip"
  sams = []
  badsams = []
  failsams = []
  badsams.append(pre + "0_0731_try1")  # There is no chip 0
  sams.append(pre + "141_0731_try1")
  badsams.append(pre + "52_0804_try1")    # This is actually chip D0052
  sams.append(pre + "D0001_0731_try1")
  sams.append(pre + "D0001_0731_try2")
  sams.append(pre + "D0001_0731_try3")
  sams.append(pre + "D0002_0731_try1")
  sams.append(pre + "D0004_0731_try1")
  sams.append(pre + "D0005_0731T12_try1")
  sams.append(pre + "D0005_0731T12_try2")
  sams.append(pre + "D0005_0731T17_try1")
  sams.append(pre + "D0006_0731_try1")
  sams.append(pre + "D0006_0731_try2")
  sams.append(pre + "D0007_0731_try1")
  sams.append(pre + "D0007_0731_try2")
  sams.append(pre + "D0007_0731_try3")
  badsams.append(pre + "D0008_0731T14_try1")  # Small signal
  badsams.append(pre + "D0008_0731T14_try2")  # Small signal
  badsams.append(pre + "D0008_0731T14_try3")  # Small signal
  sams.append(pre + "D0008_0731T18_try1")
  sams.append(pre + "D0008_0731T18_try2")
  sams.append(pre + "D0009_0731_try1")
  badsams.append(pre + "D0011_0731_try1")  # Bad waveforms
  badsams.append(pre + "D0011_0731_try2")  # Bad waveforms
  sams.append(pre + "D0011_0802_try1")
  sams.append(pre + "D0012_0801_try1")
  sams.append(pre + "D0014_0801T1311_try1")
  sams.append(pre + "D0014_0801T1350_try1")
  sams.append(pre + "D0015_0801_try1")
  sams.append(pre + "D0016_0803_try1")
  sams.append(pre + "D0017_0802_try1")
  sams.append(pre + "D0019_0801_try1")
  sams.append(pre + "D0020_0801_try1")
  sams.append(pre + "D0021_0801_try1")
  sams.append(pre + "D0022_0801_try1")
  sams.append(pre + "D0023_0801_try1")
  sams.append(pre + "D0024_0803_try1")
  sams.append(pre + "D0025_0801_try1")
  sams.append(pre + "D0026_0801_try1")
  sams.append(pre + "D0028_0802_try1")
  sams.append(pre + "D0029_0802_try1")
  badsams.append(pre + "D0030_0802T15_try1")  # Small signals
  sams.append(pre + "D0030_0802T17_try1")
  badsams.append(pre + "00031_0802_try1")  # Ignore -- bad chip label
  sams.append(pre + "D0031_0803_try1")
  sams.append(pre + "D0032_0802_try1")
  sams.append(pre + "D0033_0802_try1")
  sams.append(pre + "D0034_0802_try1")
  sams.append(pre + "D0035_0802_try1")
  sams.append(pre + "D0036_0802T19_try1")
  sams.append(pre + "D0036_0802T20_try1")
  sams.append(pre + "D0037_0803_try1")
  sams.append(pre + "D0038_0803_try1")
  sams.append(pre + "D0039_0803_try1")
  badsams.append(pre + "D0040_0804_try1")  # Bad waveforms
  sams.append(pre + "D0041_0803_try1")
  sams.append(pre + "D0042_0803_try1")
  sams.append(pre + "D0043_0803_try1")
  sams.append(pre + "D0044_0803_try1")
  sams.append(pre + "D0045_0803_try1")
  sams.append(pre + "D0046_0803_try1")
  sams.append(pre + "D0047_0803_try1")
  badsams.append(pre + "D0053_0804_try1")  # Bad waveforms
  sams.append(pre + "D0053_0804_try2")
  sams.append(pre + "D0054_0804_try1")
  badsams.append(pre + "D0050_0803_try1")  # Bad waveforms
  sams.append(pre + "D0057_0808_try1")
  badsams.append(pre + "D0060_0807_try1")  # warm test
  sams.append(pre + "D0063_0808_try1")
  sams.append(pre + "D0064_0808_try1")
  sams.append(pre + "D0069_0808_try1")
  sams.append(pre + "D0072_0808_try1")
  sams.append(pre + "D0074_0808_try1")
  sams.append(pre + "D0075_0809_try1")
  sams.append(pre + "D0078_0809_try1")
  sams.append(pre + "D0080_0809_try1")
  sams.append(pre + "D0081_0809_try1")
  sams.append(pre + "D0083_0809_try1")
  sams.append(pre + "D0085_0809_try1")
  sams.append(pre + "D0087_0809_try1")
  sams.append(pre + "D0088_0809_try1")
  sams.append(pre + "D0089_0809_try1")
  sams.append(pre + "D0090_0809_try1")
  sams.append(pre + "D0091_0809_try1")
  badsams.append(pre + "D0092_0810_try1")  # Bad waveforms
  sams.append(pre + "D0093_0810_try1")
  sams.append(pre + "D0095_0810_try1")
  sams.append(pre + "D0097_0810_try1")
  sams.append(pre + "D0098_0810_try1")
  sams.append(pre + "D0100_0810_try1")
  sams.append(pre + "D0103_0810_try1")
  sams.append(pre + "D0104_0810_try1")
  sams.append(pre + "D0105_0810_try1")
  sams.append(pre + "D0106_0810_try1")
  sams.append(pre + "D0107_0810_try1")
  sams.append(pre + "D0108_0810_try1")
  sams.append(pre + "D0110_0811_try1")
  sams.append(pre + "D0111_0811_try1")
  sams.append(pre + "D0112_0811_try1")
  sams.append(pre + "D0113_0811_try1")
  sams.append(pre + "D0116_0811_try1")
  sams.append(pre + "D0117_0811_try1")
  sams.append(pre + "D0119_0811_try1")
  sams.append(pre + "D0120_0811_try1")
  sams.append(pre + "D0122_0811_try1")
  sams.append(pre + "D0123_0811_try1")
  sams.append(pre + "D0124_0811_try1")
  sams.append(pre + "D0125_0811_try1")
  sams.append(pre + "D0126_0811_try1")
  sams.append(pre + "D0127_0814_try1")
  sams.append(pre + "D0129_0814_try1")
  sams.append(pre + "D0130_0814_try1")
  sams.append(pre + "D0131_0814T1229_try1")
  sams.append(pre + "D0131_0814T1247_try1")
  sams.append(pre + "D0132_0814_try1")
  sams.append(pre + "D0133_0814_try1")
  sams.append(pre + "D0134_0814_try1")
  sams.append(pre + "D00135_0814_try1")
  sams.append(pre + "D0137_0814_try1")
  sams.append(pre + "D0138_0814_try1")
  sams.append(pre + "D0139_0814_try1")
  sams.append(pre + "D0141_0815_try1")
  sams.append(pre + "D0142_0815_try1")
  sams.append(pre + "D0143_0815_try1")
  sams.append(pre + "D0144_0815_try1")
  sams.append(pre + "D0145_0815_try1")
  sams.append(pre + "D0146_0815_try1")
  badsams.append(pre + "D0147_0815T1204_try1")  #  Bad waveforms
  badsams.append(pre + "D0147_0815T1237_try1")  #  Bad -- actually chip D0048
  sams.append(pre + "D0149_0815_try1")
  sams.append(pre + "D0150_0815_try1")
  sams.append(pre + "D0151_0815_try1")
  sams.append(pre + "D0152_0815_try1")
  sams.append(pre + "D0153_0815_try1")
  sams.append(pre + "D0156_0815_try1")
  sams.append(pre + "D0157_0815_try1")
  badsams.append(pre + "D0158_0815_try1")  # Bad waveforms
  sams.append(pre + "D0159_0815_try1")
  sams.append(pre + "D0160_0815_try1")
  sams.append(pre + "D0161_0815_try1")
  sams.append(pre + "D0162_0816_try1")  # 8/17
  sams.append(pre + "D0163_0816_try1")  # 8/17
  sams.append(pre + "D0164_0816_try1")  # 8/17
  sams.append(pre + "D0165_0816_try1")  # 8/17
  sams.append(pre + "D0166_0816_try1")  # 8/17
  sams.append(pre + "D0167_0816_try1")  # 8/17
  sams.append(pre + "D0168_0816_try1")  # 8/17
  sams.append(pre + "D0169_0816_try1")  # 8/17
  sams.append(pre + "D0170_0816_try1")  # 8/17
  sams.append(pre + "D0171_0816_try1")  # 8/17
  sams.append(pre + "D0173_0816_try1")  # 8/17
  sams.append(pre + "D0174_0816_try1")  # 8/17
  sams.append(pre + "D0175_0816_try1")  # 8/17
  sams.append(pre + "D0176_0816_try1")  # 8/17
  sams.append(pre + "D0177_0816T1447_try1")  # 8/17
  sams.append(pre + "D0177_0816T1506_try1")  # 8/17
  sams.append(pre + "D0178_0816_try1")  # 8/17
  sams.append(pre + "D0179_0816_try1")  # 8/17
  sams.append(pre + "D0180_0816_try1")  # 8/17
  sams.append(pre + "D0182_0816_try1")  # 8/17
  sams.append(pre + "D0183_0816_try1")  # 8/17
  sams.append(pre + "D0184_0816_try1")  # 8/17
  sams.append(pre + "D0185_0816_try1")  # 8/17
  # DUNE17dla-cold
  if isBad: outsams = badsams
  elif isFail: outsams = failsams
  else: outsams = sams
  if skipSel or skipBad:
    skipChips = []
    if skipSel: skipChips += selectedChips()
    if skipBad: skipChips += badChips()
    keepsams = []
    for sam in outsams:
      chip = dune17cChip(sam)
      #print "Chip " + str(chip)
      if chip not in skipChips:
        keepsams.append(sam)
    return keepsams
  return outsams

# List of available DUNE17ts-cold datasets.
def dune17tscSamples(isBad =False, isFail=False, skipSel=False, skipBad=True, isDNL=False):
  pre = "DUNE17ts-cold_chip"
  sams = []
  dnlsams = []
  badsams = []
  failsams = []
  sams.append(pre + "4")
  sams.append(pre + "5")
  dnlsams.append(pre + "17_DNL")
  dnlsams.append(pre + "21_DNL")
  dnlsams.append(pre + "31_DNL")
  dnlsams.append(pre + "32_DNL")
  dnlsams.append(pre + "33_DNL")
  dnlsams.append(pre + "34_DNL")
  dnlsams.append(pre + "35_DNL")
  dnlsams.append(pre + "36_DNL")
  dnlsams.append(pre + "37")
  dnlsams.append(pre + "38_DNL")
  dnlsams.append(pre + "47_DNL")
  dnlsams.append(pre + "68_DNL")
  dnlsams.append(pre + "76_DNL")
  dnlsams.append(pre + "77_DNL")
  dnlsams.append(pre + "88_DNL")
  dnlsams.append(pre + "89_DNL")
  dnlsams.append(pre + "91")
  sams.append(pre + "109")
  sams.append(pre + "116")
  sams.append(pre + "123")
  sams.append(pre + "126")
  sams.append(pre + "128")
  sams.append(pre + "130")
  sams.append(pre + "131")
  sams.append(pre + "134")
  sams.append(pre + "135")
  sams.append(pre + "142")
  dnlsams.append(pre + "143_DNL07_25_16")
  dnlsams.append(pre + "143_DNL07_25_18")
  sams.append(pre + "144")
  sams.append(pre + "153")
  sams.append(pre + "159")
  sams.append(pre + "160_0717")
  sams.append(pre + "160_0727")
  sams.append(pre + "161")
  sams.append(pre + "162")
  sams.append(pre + "163")
  sams.append(pre + "164")
  sams.append(pre + "165")
  sams.append(pre + "166_0717")
  sams.append(pre + "166_0724")
  sams.append(pre + "168")
  sams.append(pre + "169")
  sams.append(pre + "170")
  sams.append(pre + "171")
  sams.append(pre + "172")
  sams.append(pre + "173")
  sams.append(pre + "174")
  sams.append(pre + "175_0717")
  sams.append(pre + "175_0724")
  sams.append(pre + "176")
  sams.append(pre + "177_0717")
  sams.append(pre + "177_0724")
  sams.append(pre + "178")
  sams.append(pre + "179")
  sams.append(pre + "180")
  sams.append(pre + "181")
  sams.append(pre + "182")
  sams.append(pre + "183_0717")
  sams.append(pre + "183_0724")
  sams.append(pre + "184")
  sams.append(pre + "185_0717")
  sams.append(pre + "185_0727")
  sams.append(pre + "186_0718")
  sams.append(pre + "186_0724")
  sams.append(pre + "187")
  sams.append(pre + "188")
  sams.append(pre + "189")
  sams.append(pre + "199")
  sams.append(pre + "203")
  sams.append(pre + "211")
  sams.append(pre + "215")
  sams.append(pre + "219")
  sams.append(pre + "241")
  sams.append(pre + "243")
  sams.append(pre + "249")
  sams.append(pre + "250")
  sams.append(pre + "251")
  sams.append(pre + "252")
  sams.append(pre + "253")
  sams.append(pre + "254")
  sams.append(pre + "255")
  sams.append(pre + "256")
  sams.append(pre + "257")
  sams.append(pre + "258")
  sams.append(pre + "259_0718")
  sams.append(pre + "259_0725")
  sams.append(pre + "260")
  sams.append(pre + "261")
  sams.append(pre + "262")
  sams.append(pre + "263")
  sams.append(pre + "264")
  sams.append(pre + "265")
  sams.append(pre + "266")
  sams.append(pre + "267")
  sams.append(pre + "268")
  sams.append(pre + "269")
  sams.append(pre + "270")
  sams.append(pre + "272")
  sams.append(pre + "273")
  sams.append(pre + "274")
  sams.append(pre + "275")
  sams.append(pre + "276")
  sams.append(pre + "277")
  sams.append(pre + "278")
  sams.append(pre + "279")
  sams.append(pre + "280")
  sams.append(pre + "281")
  sams.append(pre + "282")
  sams.append(pre + "283")
  sams.append(pre + "284_0720")
  sams.append(pre + "284_0731")
  sams.append(pre + "285")
  sams.append(pre + "286")
  sams.append(pre + "287")
  sams.append(pre + "288_0720")
  #sams.append(pre + "288_0727")  # Actually chip 388 Junbin Zhang 7/28/2017
  sams.append(pre + "289")
  sams.append(pre + "290")
  sams.append(pre + "291")
  sams.append(pre + "292")
  sams.append(pre + "293")
  sams.append(pre + "295")
  sams.append(pre + "296")
  sams.append(pre + "298")
  sams.append(pre + "299")
  sams.append(pre + "300")
  sams.append(pre + "301")
  sams.append(pre + "302")
  sams.append(pre + "304")
  sams.append(pre + "305")
  sams.append(pre + "306")
  sams.append(pre + "307")
  sams.append(pre + "308")
  sams.append(pre + "309")
  sams.append(pre + "310")
  sams.append(pre + "311")
  sams.append(pre + "312")
  sams.append(pre + "313_0725")
  sams.append(pre + "313_0731")
  sams.append(pre + "314")
  sams.append(pre + "316")
  sams.append(pre + "317")
  sams.append(pre + "318")
  sams.append(pre + "319")
  sams.append(pre + "320")
  sams.append(pre + "321")
  sams.append(pre + "322")
  sams.append(pre + "323")
  sams.append(pre + "324")
  sams.append(pre + "325")
  sams.append(pre + "326")
  sams.append(pre + "327")
  sams.append(pre + "328")
  sams.append(pre + "329")
  sams.append(pre + "333")
  sams.append(pre + "335")
  sams.append(pre + "339")
  sams.append(pre + "348")
  sams.append(pre + "350")
  sams.append(pre + "355")
  sams.append(pre + "375")
  sams.append(pre + "386")
  sams.append(pre + "378")
  sams.append(pre + "388_fchip288_0727")  # Junbin Zhang 7/28/2017
  sams.append(pre + "D0001_0731")
  sams.append(pre + "D0002_0731")
  sams.append(pre + "D0003_0731")
  sams.append(pre + "D0004_0731")
  sams.append(pre + "D0005_0731")
  sams.append(pre + "D0006_0731")
  sams.append(pre + "D0007_0731")
  sams.append(pre + "D0008_0731")
  sams.append(pre + "D0009_0731")
  sams.append(pre + "D0010_0801")
  sams.append(pre + "D0011_0731")
  sams.append(pre + "D0012_0731")
  sams.append(pre + "D0013_0731")
  sams.append(pre + "D0014_0731")
  sams.append(pre + "D0015_0731")
  sams.append(pre + "D0016_0731")
  sams.append(pre + "D0017_0731")
  sams.append(pre + "D0018_0731")
  sams.append(pre + "D0019_0731")
  sams.append(pre + "D0020_0731")
  sams.append(pre + "D0021_0731")
  sams.append(pre + "D0022_0731")
  sams.append(pre + "D0023_0731")
  sams.append(pre + "D0024_0731")
  sams.append(pre + "D0025_0731")
  sams.append(pre + "D0026_0731")
  sams.append(pre + "D0027_0731")
  sams.append(pre + "D0028_0731")
  sams.append(pre + "D0029_0731")
  sams.append(pre + "D0030_0731")
  if isBad: outsams = badsams
  elif isFail: outsams = failsams
  elif isDNL: outsams = dnlsams
  else: outsams = sams + dnlsams
  if skipSel or skipBad:
    skipChips = []
    if skipSel: skipChips += selectedChips()
    if skipBad: skipChips += badChips()
    keepsams = []
    for sam in outsams:
      chip = dune17cChip(sam)
      if chip not in skipChips:
        keepsams.append(sam)
    return keepsams
  return outsams

# Return the chip number for a sample.
# Deduces the chip number from the sample name after
# checking a msap that handles misnamed samples.
# ..._chipDVVVVV -> number 10000 + VVVV
def dune17cChip(sam):
  sampleToChipMap = { }     # Better not use this b/c chip # will still be wrong in root files.
  if sam in sampleToChipMap:
    return sampleToChipMap[sam]
  ipos = sam.find("_chip")
  asam = sam[ipos+5:]
  chipoff = 0
  if asam[0] == "D":
    asam = asam[1:]
    chipoff = 10000
  ipos = asam.find("_")
  if ipos >= 0: schip = asam[:ipos]
  else: schip = asam
  if schip.isdigit():
    return int(schip) + chipoff
  return 0
 
# Return all samples for a given chip number.
def dune17cChipSamples(chip):
  dso = []
  for ds in dune17cSamples() + dune17tscSamples():
    if dune17cChip(ds) == chip:
      dso.append(ds)
  return dso

# Return the suffix for a dataset.
def dune17cSuffix(ds):
  ads = ds[16:]
  ipos = ads.find("_")
  if ipos >= 0: suf = ads[ipos+1:]
  else: suf = ""
  return suf

# Return the chips in DUNE17-cold
def dune17cChips(skipSel=False, skipBad=True):
  chips = []
  skipChips = []
  if skipSel: skipChips += selectedChips()
  if skipBad: skipChips += badChips()
  for ds in dune17cSamples():
    chip = dune17cChip(ds)
    if chip not in chips:
      if chip not in skipChips:
        chips.append(chip)
  return chips

# Return the chips in DUNE17dla-cold
def dune17dlacChips(skipSel=False, skipBad=True):
  chips = []
  skipChips = []
  if skipSel: skipChips += selectedChips()
  if skipBad: skipChips += badChips()
  for ds in dune17dlacSamples():
    chip = dune17cChip(ds)
    if chip not in chips:
      if chip not in skipChips:
        chips.append(chip)
  return chips

# Return the chips in DUNE17ts-cold
def dune17tscChips(skipSel=False, skipBad=True):
  chips = []
  skipChips = []
  if skipSel: skipChips += selectedChips()
  if skipBad: skipChips += badChips()
  for sam in dune17tscSamples():
    chip = dune17cChip(sam)
    if chip not in chips:
      if chip not in skipChips:
        chips.append(chip)
  return chips

# Return the chips in DUNE17-cold and DUNE17ts-cold
def allChips(skipSel=False, skipBad=True):
  chips = []
  chips += dune17cChips(skipSel, skipBad)
  chips += dune17tscChips(skipSel, skipBad)
  chips.sort()
  return chips

# Return chips that appear in bad samples.
def dune17BadSampleChips(excludePdts=True, excludeCets=True):
  chips = []
  sams = []
  sams += dune17cSamples(isBad=True)
  sams += dune17tscSamples(isBad=True)
  excludeChips = []
  if excludePdts: excludeChips += dune17cChips()
  if excludeCets: excludeChips += dune17tscChips()
  for sam in sams:
    chip = dune17cChip(sam)
    if chip not in chips:
      if chip not in excludeChips:
        chips.append(chip)
  return chips

# Samples with single ramp in protoDUNE test stand.
def dune17csrSamples():
  sams = []

# Return the dataset name for a sample.
def dune17DatasetName(sam):
  if sam[:16] == "DUNE17-cold_chip": return "DUNE17-cold"
  if sam[:18] == "DUNE17ts-cold_chip": return "DUNE17ts-cold"
  return "UnknownDataset"

# Selected chips.
def selectedChips(sel=0):
  chips = []
  # Selection 16jul2017 from Matt
  sel1 = [1, 2, 3, 7, 8, 9, 12, 13, 14, 15, 18, 20, 22, 25, 39, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 60, 64, 65, 67, 70, 71, 74, 78, 79, 80, 81, 82, 84, 86, 87, 90, 92, 93, 94, 95, 96, 97, 101, 105, 108, 110, 114, 115, 118, 119, 124, 127, 129, 132, 136, 138, 363, 364, 365, 366, 369, 373, 379, 380, 383, 384, 385, 387]
  # Selection 23jul2017 from Matt
  sel2 = [316, 161, 214, 190, 225, 295, 304, 275, 201, 221, 273, 292, 176, 320, 228, 264, 357, 276, 278, 331, 189, 212, 266, 162, 147, 293, 263, 240, 277, 298, 351, 238, 257, 329, 269, 285, 261, 267, 150, 231, 325, 187, 289, 327, 163, 184, 256, 151, 282, 191, 258, 170, 317, 299, 394, 302, 148, 158, 324, 140, 204, 146, 173, 376, 274, 255, 197, 247, 287, 253, 288, 165, 301, 330, 182, 245, 217, 265, 248, 168]
  # Selection 31jul2017 from Matt
  sel3 = [343, 193, 144, 222, 123, 172, 5, 312, 134, 341, 305, 116, 332, 178, 308, 160, 139, 338, 313, 230, 389, 279, 137, 361, 215, 19, 342, 177, 333, 211, 185, 175, 227, 188, 200, 153, 352, 131, 179, 336]

  chips = []
  if sel == 0:
    chips = sel1 + sel2 + sel3
  elif sel == 1: chips = sel1
  elif sel == 2: chips = sel2
  elif sel == 3: chips = sel3
  chips.sort()
  return chips

# Files declared bad.
def badFiles():
  files = []
  files.append("adcTestData_20170803T191545_chipd0013_adcClock0_adcOffset-1_sampleRate2000000_functype3_freq4.000_offset0.700_amplitude1.000.root")    # mislabeled chip
  files.append("adcTestData_20170807T162003_chipD0060_adcClock0_adcOffset-1_sampleRate2000000_functype3_freq4.000_offset0.700_amplitude1.000.root")  # warm test.
  files.append("adcDavidAdamsOnlyData_20170807T160827_chipD0060_adcClock0_adcOffset-1_sampleRate2000000_functype3_freq4.000_offset0.700_amplitude1.000_try1.root")   # warm test
  return files

# Chips declared bad.
def badChips():
  chips = []
  #chips += [133, 167, 256]     # Matt "BAD" 23jul2017 These are FE numbers--not ADC numbers.
  #chips += [49, 64]     # Matt "Damaged" 23jul2017i. These are FE numbers--not ADC numbers.
  chips += [359, 111, 23, 284]  #  Matt missing chips 24jul2017
  chips += [83]  # Elizabeth reports bent pin 25jul2017
  chips += [75]  # Ivan Furic talk at BNL CE meeting 28jul2017
  chips += [109, 378]   # Matt missing chips 31jun2017
  chips.remove(284)   # This chip was tested on CETS on 7/31
  chips += [10003]  # Elizabeth reports this chip has bent pin and will never be used 8/1/2017
  chips += [378]    # Elizabeth reports this chip is damaged 8/2/2017
  chips.sort()
  return chips
