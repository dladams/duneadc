# dune17cSamples.py

# David Adams
# August 2017

from dune17bad import badChips
from dune17sel import selectedChips
from dune17Chip import dune17cChip

# List of available DUNE17 datasets.
# group: 0 = all
#        1 = bad ADC clock(before 7/19)
#        2 = good ADC clock
#        3 = good ADC clock horizontal test
#        4 = good ADC clock vertical test
#   isBad true returns bad, nwf and roll
#   isFail true returns fail
# skipSel skips any samples whose chips are selected
# skipBad skips any samples whose chips are bad
def dune17cSamples(group=0, isNwf=False, isBad=False, isFail=False, skipSel=False, skipBad=True):
  pre = "DUNE17-cold_chip"
  sams1 = []       # Early samples with bad ADC clock settings
  sams2 = []       # Subsequent good samples
  samsh = []       # Good samples with horizontal boards
  badsams = []     # Samples otherwise bad
  badsams2 = []    # Samples otherwise bad with vertical boards and good clock settings
  badsamsh = []    # Samples otherwise bad with horizontal boards
  nwfsams = []     # Samples without waveforms
  rollsams = []    # Samples with severe rollback
  failsams = []    # Samples tha fail processing
  failsams2 = []    # Samples tha fail processing
  failsamsh = []    # Samples tha fail processing
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
  badsams.append(pre + "D0060_0807")  # Taken warm but labeled cold.
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
  samsh.append(pre + "D0129_0814")
  samsh.append(pre + "D0130_0814")
  sams2.append(pre + "D0132_0814")
  samsh.append(pre + "D0134_0814")
  samsh.append(pre + "D00135_0814")
  samsh.append(pre + "D0137_0814")
  samsh.append(pre + "D0138_0814")
  samsh.append(pre + "D0141_0815")
  sams2.append(pre + "D0142_0815")
  sams2.append(pre + "D0143_0815")
  samsh.append(pre + "D0144_0815")
  sams2.append(pre + "D0145_0815")
  badsams.append(pre + "D0147_0815")  # Bad waveforms
  badsams.append(pre + "D0148_0815T1253")  # Waveforms absent.
  sams2.append(pre + "D0148_0815T1307")
  samsh.append(pre + "D0149_0815")
  sams2.append(pre + "D0150_0815")
  samsh.append(pre + "D0151_0815")
  samsh.append(pre + "D0152_0815")
  sams2.append(pre + "D0153_0815")
  samsh.append(pre + "D0156_0815")
  sams2.append(pre + "D0157_0815")
  samsh.append(pre + "D0159_0815")
  sams2.append(pre + "D0160_0815T1621")
  badsams.append(pre + "D0160_0815T1635")  # Waveforms absent.
  sams2.append(pre + "D0160_0815T1646")
  samsh.append(pre + "D0161_0815")
  sams2.append(pre + "D0162_0816")
  samsh.append(pre + "D0163_0816")
  sams2.append(pre + "D0164_0816")
  samsh.append(pre + "D0165_0816")
  samsh.append(pre + "D0167_0816")
  sams2.append(pre + "D0168_0816")
  samsh.append(pre + "D0170_0816")
  sams2.append(pre + "D0171_0816")
  sams2.append(pre + "D0174_0816")
  sams2.append(pre + "D0176_0816")
  sams2.append(pre + "D0179_0816")
  sams2.append(pre + "D0180_0816")
  sams2.append(pre + "D0182_0816")
  samsh.append(pre + "D0183_0816")
  sams2.append(pre + "D0184_0816")
  samsh.append(pre + "D0185_0816")
  samsh.append(pre + "D0186_0817")
  sams2.append(pre + "D0189_0817")
  sams2.append(pre + "D0190_0817T1001")
  sams2.append(pre + "D0190_0817T1023:chipD0191")
  sams2.append(pre + "D0190_0817T1049:chipD0193")
  sams2.append(pre + "D0190_0817T1113:chipD0194")
  samsh.append(pre + "D0192_0817")
  sams2.append(pre + "D0196_0817")
  samsh.append(pre + "D0197_0817")
  sams2.append(pre + "D0198_0817")
  sams2.append(pre + "D0199_0817")
  samsh.append(pre + "D0200_0817")
  sams2.append(pre + "D0201_0817")
  samsh.append(pre + "D0202_0817")
  sams2.append(pre + "D0203_0817")
  samsh.append(pre + "D0204_0817")
  sams2.append(pre + "D0205_0817")
  samsh.append(pre + "D0206_0817")
  samsh.append(pre + "D0208_0817")
  sams2.append(pre + "D0209_0817T1525")
  sams2.append(pre + "D0209_0817T1542")
  samsh.append(pre + "D0210_0817")
  sams2.append(pre + "D0211_0817")
  samsh.append(pre + "D0212_0817")
  sams2.append(pre + "D0213_0817")
  samsh.append(pre + "D0214_0817")
  samsh.append(pre + "D0215_0818")
  badsams.append(pre + "D0216_0818T0924")  # Bad waveforms
  badsams.append(pre + "D0216_0818T0945")  # Bad waveforms (bad ADC?)
  failsams.append(pre + "D0217_0818")  # Fit failures.
  samsh.append(pre + "D0218_0818")
  samsh.append(pre + "D0220_0818")
  sams2.append(pre + "D0221_0818")
  sams2.append(pre + "D0222_0818")
  sams2.append(pre + "D0225_0818")
  sams2.append(pre + "D0232_0818")
  sams2.append(pre + "D0234_0818")
  sams2.append(pre + "D0235_0818")
  samsh.append(pre + "D0236_0818")
  sams2.append(pre + "D0239_0818")
  sams2.append(pre + "D0241_0818")
  sams2.append(pre + "D0243_0821")
  sams2.append(pre + "D0244_0821")
  samsh.append(pre + "D0245_0821")
  sams2.append(pre + "D0246_0821T1703")
  sams2.append(pre + "D0246_0821T1850")
  samsh.append(pre + "D0247_0821")
  sams2.append(pre + "D0248_0821")
  sams2.append(pre + "D0249_0822")
  samsh.append(pre + "D0250_0822")
  sams2.append(pre + "D0251_0822")
  samsh.append(pre + "D0252_0822")
  sams2.append(pre + "D0253_0822")
  sams2.append(pre + "D0254_0822")
  samsh.append(pre + "D0255_0822")
  samsh.append(pre + "D0256_0822")
  samsh.append(pre + "D0157_0821:chipD0257")
  samsh.append(pre + "D0257_0821")
  samsh.append(pre + "D0258_0822")
  samsh.append(pre + "D0259_0822")
  samsh.append(pre + "D0260_0822")
  sams2.append(pre + "D0262_0822")
  samsh.append(pre + "D0263_0822")
  sams2.append(pre + "D0264_0822")
  sams2.append(pre + "D0265_0822")
  samsh.append(pre + "D0266_0822")
  sams2.append(pre + "D0267_0822")
  sams2.append(pre + "D0268_0822T1229")
  sams2.append(pre + "D0268_0822T1823")
  sams2.append(pre + "D0269_0823")
  badsams.append(pre + "D0273_0823")  # Bad waveform
  sams2.append(pre + "D0274_0822")
  sams2.append(pre + "D0275_0822")
  sams2.append(pre + "D0277_0822")
  sams2.append(pre + "D0280_0823")
  samsh.append(pre + "D0281_0823")
  samsh.append(pre + "D0282_0823")
  sams2.append(pre + "D0283_0823")
  samsh.append(pre + "D0284_0823")
  samsh.append(pre + "D0285_0823")
  sams2.append(pre + "D0286_0823")
  sams2.append(pre + "D0288_0823")
  sams2.append(pre + "D0289_0823")
  badsams.append(pre + "D0290_0823")  # Bad waveforms
  sams2.append(pre + "D0291_0823")
  samsh.append(pre + "D0292_0823")
  badsams.append(pre + "D0293_0824")  # Bad waveforms
  samsh.append(pre + "D0293_0828")
  samsh.append(pre + "D0294_0823")
  sams2.append(pre + "D0295_0823")
  samsh.append(pre + "D0296_0823")
  sams2.append(pre + "D0297_0823")
  samsh.append(pre + "D0298_0823")
  sams2.append(pre + "D0299_0823")
  sams2.append(pre + "D0301_0823")
  sams2.append(pre + "D0302_0823")
  samsh.append(pre + "D0303_0823")
  sams2.append(pre + "D0304_0823")
  samsh.append(pre + "D0305_0824")
  sams2.append(pre + "D0306_0824")
  samsh.append(pre + "D0307_0824")
  samsh.append(pre + "D0308_0824")
  samsh.append(pre + "D0309_0824")
  samsh.append(pre + "D0310_0824")
  samsh.append(pre + "D0311_0824")
  samsh.append(pre + "D0312_0824")
  samsh.append(pre + "D0313_0824")
  samsh.append(pre + "D0314_0824")
  sams2.append(pre + "D0315_0824")
  badsams.append(pre + "D0316_0824")  # Bad waveforms
  samsh.append(pre + "D0316_0828")
  samsh.append(pre + "D0317_0824")
  samsh.append(pre + "D0319_0824")
  sams2.append(pre + "D0321_0824")
  samsh.append(pre + "D0325_0824")
  sams2.append(pre + "D0326_0824")
  sams2.append(pre + "D0328_0824")
  sams2.append(pre + "D0329_0824")
  sams2.append(pre + "D0331_0824")
  sams2.append(pre + "D0332_0830")
  sams2.append(pre + "D0333_0824")
  badsams.append(pre + "D0334_0825T0925")  # Bad waveforms
  badsams.append(pre + "D0334_0825T0938")  # Bad waveforms
  samsh.append(pre + "D0335_0825")
  sams2.append(pre + "D0336_0824")
  samsh.append(pre + "D0337_0830")
  samsh.append(pre + "D0338_0825")
  samsh.append(pre + "D0340_0825")
  samsh.append(pre + "D0341_0825")
  badsams.append(pre + "D0342_0825")  # Bad waveforms
  sams2.append(pre + "D0344_0825")
  samsh.append(pre + "D0345_0825")
  sams2.append(pre + "D0346_0830")
  sams2.append(pre + "D0351_0825")
  badsams.append(pre + "D0352_0825")  # Bad waveforms
  badsams.append(pre + "D0355_0825")  # Missing waveforms
  sams2.append(pre + "D0357_0825")
  samsh.append(pre + "D0358_0825")
  samsh.append(pre + "D0359_0825")
  badsams.append(pre + "D0360_0825")  # Bad waveforms
  sams2.append(pre + "D0361_0825")
  badsams.append(pre + "D0362_0825")  # Bad waveforms
  sams2.append(pre + "D0363_0825")
  sams2.append(pre + "D0364_0825")
  samsh.append(pre + "D0365_0825")
  sams2.append(pre + "D0366_0825")
  sams2.append(pre + "D0367_0828")
  sams2.append(pre + "D0369_0828")
  sams2.append(pre + "D0371_0828T1109")
  samsh.append(pre + "D0371_0828T1134:chipD0354")
  badsams.append(pre + "D0372_0828")  # Bad waveforms.
  sams2.append(pre + "D0373_0828")
  samsh.append(pre + "D0374_0828")
  sams2.append(pre + "D0375_0828")
  samsh.append(pre + "D0376_0828T1328")
  sams2.append(pre + "D0376_0828T1446")
  sams2.append(pre + "D0377_0828")
  samsh.append(pre + "D0378_0828")
  sams2.append(pre + "D0379_0828")
  sams2.append(pre + "D0380_0828")
  samsh.append(pre + "D0381_0828")
  sams2.append(pre + "D0382_0828")
  samsh.append(pre + "D0383_0828")
  sams2.append(pre + "D0384_0828")
  samsh.append(pre + "D0385_0828")
  sams2.append(pre + "D0387_0830")
  samsh.append(pre + "D0392_0828")
  samsh.append(pre + "D0394_0828")
  samsh.append(pre + "D0396_0828")
  sams2.append(pre + "D0397_0829")
  sams2.append(pre + "D0401_0829")
  sams2.append(pre + "D0404_0829")
  samsh.append(pre + "D0405_0829")
  sams2.append(pre + "D0406_0829")
  samsh.append(pre + "D0407_0829")
  sams2.append(pre + "D0408_0829")
  sams2.append(pre + "D0409_0829")
  samsh.append(pre + "D0410_0829")
  sams2.append(pre + "D0411_0829")
  sams2.append(pre + "D0412_0829")
  samsh.append(pre + "D0413_0829")
  sams2.append(pre + "D0414_0829")
  samsh.append(pre + "D0415_0829")
  sams2.append(pre + "D0416_0829")
  samsh.append(pre + "D0417_0829")
  samsh.append(pre + "D0418_0830")
  samsh.append(pre + "D0419_0829")
  sams2.append(pre + "D0420_0829")
  badsams.append(pre + "D0422_0829")  # Bad waveforms
  sams2.append(pre + "D0423_0829")
  sams2.append(pre + "D0424_0829")
  samsh.append(pre + "D0425_0829")
  samsh.append(pre + "D0426_0829")
  badsams.append(pre + "D0427_0829T1758")  # Bad waveforms
  badsams.append(pre + "D0427_0829T1815")  # Bad waveforms
  sams2.append(pre + "D0428_0829")
  samsh.append(pre + "D0429_0829")
  sams2.append(pre + "D0430_0829")
  samsh.append(pre + "D0431_0829")
  samsh.append(pre + "D0432_0829")
  sams2.append(pre + "D0433_0829")
  sams2.append(pre + "D0434_0829")
  samsh.append(pre + "D0435_0829")
  sams2.append(pre + "D0436_0829")
  sams2.append(pre + "D0438_0830")
  sams2.append(pre + "D0439_0830")
  samsh.append(pre + "D0440_0830")
  sams2.append(pre + "D0441_0830")
  sams2.append(pre + "D0444_0830")
  samsh.append(pre + "D0445_0830")
  sams2.append(pre + "D0446_0830")
  samsh.append(pre + "D0447_0830")
  badsams.append(pre + "D0448_0830")  # Bad waveforms
  samsh.append(pre + "D0449_0830")
  sams2.append(pre + "D0450_0830")
  samsh.append(pre + "D0451_0830")
  sams2.append(pre + "D0452_0830")
  samsh.append(pre + "D0453_0830")
  sams2.append(pre + "D0454_0830")
  samsh.append(pre + "D0455_0830")
  samsh.append(pre + "D0457_0830")
  badsams.append(pre + "D0459_0830")  # Bad waveforms
  samsh.append(pre + "D0460_0830")
  sams2.append(pre + "D0461_0830")
  sams2.append(pre + "D0463_0830")
  samsh.append(pre + "D0465_0830")
  badsams.append(pre + "D0466_0830")  # Bad waveforms
  sams2.append(pre + "D0467_0830")
  samsh.append(pre + "D0468_0830")
  sams2.append(pre + "D0469_0830")
  sams2.append(pre + "D0471_0830")
  badsams.append(pre + "D0472_0830")  # Bad waveforms
  badsams.append(pre + "D0477_0831")  # Bad waveforms
  samsh.append(pre + "D0478_0831")
  samsh.append(pre + "D0480_0831")
  samsh.append(pre + "D0482_0831")
  sams2.append(pre + "D0483_0831")
  samsh.append(pre + "D0484_0831")
  samsh.append(pre + "D0487_0831")
  samsh.append(pre + "D0489_0831")
  samsh.append(pre + "D0491_0831")
  samsh.append(pre + "D0493_0831")
  samsh.append(pre + "D0494_0831")
  samsh.append(pre + "D0496_0831")
  sams2.append(pre + "D0497_0831")
  samsh.append(pre + "D0498_0831")
  sams2.append(pre + "D0499_0831")
  sams2.append(pre + "D0500_0831")
  samsh.append(pre + "D0501_0831")
  sams2.append(pre + "D0502_0831")
  samsh.append(pre + "D0504_0831")
  sams2.append(pre + "D0505_0831")
  sams2.append(pre + "D0510_0901")
  samsh.append(pre + "D0515_0901")
  sams2.append(pre + "D0516_0901")
  badsams.append(pre + "D0517_0901")  # Bad waveforms
  badsams.append(pre + "D0519_0901")  # Bad waveforms
  samsh.append(pre + "D0520_0901")
  samsh.append(pre + "D0521_0901")  # 9/1+
  sams2.append(pre + "D0522_0901")
  sams2.append(pre + "D0524_0901")
  samsh.append(pre + "D0527_0901")
  sams2.append(pre + "D0528_0901")
  samsh.append(pre + "D0529_0901")
  samsh.append(pre + "D0530_0901")
  sams2.append(pre + "D0531_0901")
  samsh.append(pre + "D0532_0901")
  badsams.append(pre + "D0534_0901")  # Bad waveforms
  samsh.append(pre + "D0535_0901")
  sams2.append(pre + "D0536_0901")
  samsh.append(pre + "D0537_0901")
  badsams.append(pre + "D0538_0901")  # Bad waveforms
  sams2.append(pre + "D0539_0901")
  samsh.append(pre + "D0540_0901")
  badsams.append(pre + "D0541_0901")  # Bad waveforms
  samsh.append(pre + "D0542_0901")
  samsh.append(pre + "D0544_0905")
  sams2.append(pre + "D0547_0905")
  samsh.append(pre + "D0550_0905")
  badsams.append(pre + "d0551_0905:chipD0551")  # Bad waveforms
  badsams.append(pre + "d0553_0905:chipD0553")  # Bad waveforms
  samsh.append(pre + "D0554_0905")
  badsams.append(pre + "D0555_0905")  # Bad waveforms
  badsams.append(pre + "D0557_0905")  # Bad waveforms
  sams2.append(pre + "D0558_0905")
  samsh.append(pre + "D0559_0905")
  badsams.append(pre + "D0560_0905")  # Bad waveforms
  badsams.append(pre + "D0562_0905")  # Bad waveforms
  sams2.append(pre + "D0566_0905")
  badsams.append(pre + "D0570_0905")  # Bad waveforms
  samsh.append(pre + "D0571_0905")
  sams2.append(pre + "D0573_0905")
  samsh.append(pre + "D0576_0905")
  sams2.append(pre + "D0577_0905")
  samsh.append(pre + "D0578_0905T2015")
  sams2.append(pre + "D0578_0905T2007:chipD0579")
  sams2.append(pre + "D0580_0906")
  sams2.append(pre + "D0581_0906")
  samsh.append(pre + "D0582_0906")
  sams2.append(pre + "D0583_0906")
  badsams.append(pre + "D0584_0906")  # Bad samples
  samsh.append(pre + "D0585_0906")
  sams2.append(pre + "D0586_0906")
  samsh.append(pre + "D0587_0906")
  sams2.append(pre + "D0588_0906")
  samsh.append(pre + "D0589_0906")
  sams2.append(pre + "D0590_0906")
  samsh.append(pre + "d0591_0906:chipD0591")
  sams2.append(pre + "D0592_0906")
  badsams.append(pre + "D0594_0906")  # Bad waveforms?
  sams2.append(pre + "D0596_0906")
  sams2.append(pre + "D0598_0906")
  samsh.append(pre + "D0600_0906")
  sams2.append(pre + "D0601_0906")
  samsh.append(pre + "D0602_0906")
  badsams.append(pre + "D0603_0906")  # Bad waveforms
  sams2.append(pre + "D0604_0906")
  samsh.append(pre + "D0605_0906")
  badsams.append(pre + "D0607_0906")  # Bad waveforms
  samsh.append(pre + "D0608_0906")
  sams2.append(pre + "D0609_0906")
  samsh.append(pre + "D0610_0906")
  samsh.append(pre + "D0612_0906")
  sams2.append(pre + "D0613_0906")
  samsh.append(pre + "D0615_0907")
  samsh.append(pre + "D0616_0906")
  sams2.append(pre + "D0617_0907")
  samsh.append(pre + "d0618_0907:chipD0618")
  samsh.append(pre + "d0620_0907:chipD0620")
  sams2.append(pre + "D0621_0907")
  samsh.append(pre + "D0622_0907")
  sams2.append(pre + "D0624_0907")
  samsh.append(pre + "D0628_0907")
  sams2.append(pre + "D0630_0907")
  samsh.append(pre + "D0632_0907")
  sams2.append(pre + "D0634_0907")
  samsh.append(pre + "D0635_0907")
  sams2.append(pre + "D0636_0907")
  samsh.append(pre + "D0638_0907")
  samsh.append(pre + "D0639_0907")
  samsh.append(pre + "D0641_0907")
  samsh.append(pre + "D0642_0907")
  samsh.append(pre + "D0644_0907")
  samsh.append(pre + "d0645_0908:chipD0645")
  sams2.append(pre + "D0647_0908")
  samsh.append(pre + "D0648_0908")
  samsh.append(pre + "D0649_0908")
  samsh.append(pre + "D0652_0908")
  samsh.append(pre + "D0658_0908")
  sams2.append(pre + "D0660_0908")
  sams2.append(pre + "D0661_0911")
  samsh.append(pre + "D0662_0911")
  sams2.append(pre + "D0663_0911")
  samsh.append(pre + "D0664_0911")
  samsh.append(pre + "D0666_0908")
  badsams.append(pre + "D0667_0911")  # Bad waveforms (don't reach zero)
  sams2.append(pre + "D0668_0911")
  samsh.append(pre + "D0669_0911")
  sams2.append(pre + "D0671_0911")
  samsh.append(pre + "D0672_0911")
  sams2.append(pre + "D0674_0912")
  badsams.append(pre + "D0676_0912")  # Bad waveforms.
  samsh.append(pre + "D0677_0912")
  samsh.append(pre + "D0678_0912")
  sams2.append(pre + "D0679_0912")
  badsams.append(pre + "D0680_0912")  # Bad waveforms
  samsh.append(pre + "D0681_0912")
  sams2.append(pre + "D0682_0912")
  samsh.append(pre + "D0683_0912")
  sams2.append(pre + "D0684_0912")
  sams2.append(pre + "D0685_0912")
  sams2.append(pre + "D0686_0912")
  sams2.append(pre + "D0687_0912")
  samsh.append(pre + "D0691_0912")  # 9/13+
  sams2.append(pre + "D0692_0912")
  sams2.append(pre + "D0696_0912")
  sams2.append(pre + "D0697_0912")
  samsh.append(pre + "699_0912:chipD0699")
  sams2.append(pre + "D0700_0912")
  sams2.append(pre + "D0704_0913")
  samsh.append(pre + "D0708_0913")
  badsamsh.append(pre + "D0711_0913")  # Bad waveforms.
  sams2.append(pre + "D0712_0913")
  sams2.append(pre + "D0715_0913")
  samsh.append(pre + "D0716_0913")
  badsamsh.append(pre + "D0717_0913")  # Bad waveforms
  sams2.append(pre + "D0718_0913")
  sams2.append(pre + "D0719_0913")
  sams2.append(pre + "D0721_0913")
  sams2.append(pre + "D0722_0913")
  samsh.append(pre + "D0723_0913")
  sams2.append(pre + "D0725_0913")
  samsh.append(pre + "D726_0913:chipD0726")
  samsh.append(pre + "D0727_0913")
  sams2.append(pre + "D0731_0913")
  badsams.append(pre + "D0732_0913")  # Bad waveforms
  samsh.append(pre + "D0733_0913")
  samsh.append(pre + "D0734_0913")
  samsh.append(pre + "D0735_0913")
  samsh.append(pre + "D0736_0913")
  sams2.append(pre + "D0737_0914")
  samsh.append(pre + "D0738_0914")
  samsh.append(pre + "D0740_0914")
  badsams.append(pre + "D0741_0914")  # Bad waveforms
  samsh.append(pre + "D0743_0914")
  samsh.append(pre + "D0745_0914")
  badsams2.append(pre + "D0749_0918T1647")  # Bad waveforms
  sams2.append(pre + "D0749_0918T1713")
  samsh.append(pre + "D0751_0914")
  samsh.append(pre + "D0752_0914")
  samsh.append(pre + "D0754_0914")
  samsh.append(pre + "D0755_0914")
  samsh.append(pre + "D0756_0914")
  samsh.append(pre + "D0757_0914")
  samsh.append(pre + "D0758_0915")
  sams2.append(pre + "D0764_0918")
  sams2.append(pre + "D0766_0918")
  badsams2.append(pre + "D0768_0919")  # Bad waveforms
  badsamsh.append(pre + "D0769_0919T1224")  # Bad waveforms
  badsamsh.append(pre + "D0769_0919T1239")  # Bad waveforms
  badsamsh.append(pre + "D0769_0919T1306")  # Bad waveforms
  badsamsh.append(pre + "D0770_0919")  # Bad waveforms
  sams2.append(pre + "D0775_0919")
  badsamsh.append(pre + "D0776_0919")  # Bad waveforms
  badsams2.append(pre + "D0777_0919")  # Bad waveforms
  sams2.append(pre + "D0778_0919")
  badsams2.append(pre + "D0779_0919")  # Bad waveforms
  sams2.append(pre + "D0780_0919")
  badsams2.append(pre + "D0781_0919")  # Bad waveforms
  sams2.append(pre + "D0782_0919")
  badsams2.append(pre + "D0787_0920")  # Bad waveforms.
  samsh.append(pre + "D0789_0922")
  badsams2.append(pre + "D0791_0921T1029")  # Warm test
  badsams2.append(pre + "D0791_0921T1509")  # Bad waveforms
  badsams2.append(pre + "D0791_0921T1525")  # Bad waveforms
  badsams2.append(pre + "D0791_0922")  # Bad waveforms
  sams2.append(pre + "D0792_0922")
  failsams2.append(pre + "D0794_0921")  # Hangs on chan 8, presumably bad waveforms
  sams2.append(pre + "D0795_0922")
  badsams2.append(pre + "D0792_0920T1420")  # Bad waveforms
  failsams2.append(pre + "D0792_0920T1438")  # Proc hangs on chan 15, bad waveforms
  sams2.append(pre + "D0797_0922")
  samsh.append(pre + "D0801_0922")
  sams2.append(pre + "D0802_0922")
  sams2.append(pre + "D0803_0922")
  samsh.append(pre + "D0804_0922")
  sams2.append(pre + "D0805_0922")
  samsh.append(pre + "D0806_0922")
  sams2.append(pre + "D0807_0922")
  sams2.append(pre + "D0808_0925")
  sams2.append(pre + "D0810_0925")
  sams2.append(pre + "D0811_0925")
  sams2.append(pre + "D0812_0925")
  sams2.append(pre + "D0813_0925")  # 9/26
  sams2.append(pre + "D0814_0925")  # 9/26
  sams2.append(pre + "D0815_0925")  # 9/26
  sams2.append(pre + "D0816_0925")  # 9/26
  sams2.append(pre + "D0817_0925")  # 9/26
  sams2.append(pre + "D0818_0925")  # 9/26
  sams2.append(pre + "D0819_0925")  # 9/26
  samsh.append(pre + "D0820_0925")  # 9/26
  sams2.append(pre + "D0821_0925")  # 9/26
  sams2.append(pre + "D0823_0925")  # 9/26
  samsh.append(pre + "D0824_0925")  # 9/26
  sams2.append(pre + "D0825_0925")  # 9/26
  sams2.append(pre + "D0826_0925")  # 9/26
  sams2.append(pre + "D0827_0925")  # 9/26
  samsh.append(pre + "D0828_0925")  # 9/26
  sams2.append(pre + "D0829_0925")  # 9/26
  sams2.append(pre + "D0830_0925")  # 9/26
  samsh.append(pre + "D0831_0925")  # 9/26
  samsh.append(pre + "D0833_0925")  # 9/26
  sams2.append(pre + "D0834_0925")  # 9/26
  sams2.append(pre + "D0835_0925")  # 9/26
  sams2.append(pre + "D0838_0926")  # 9/26
  if isNwf: outsams = nwfsams
  elif isBad: outsams = badsams + badsams2 + badsamsh + nwfsams + rollsams
  elif isFail: outsams = failsams + failsams2 + failsamsh
  else:
    if   group == 0: outsams = sams1 + sams2 + samsh
    elif group == 1: outsams = sams1
    elif group == 2: outsams = sams2 + samsh
    elif group == 3: outsams = samsh
    elif group == 4: outsams = sams2
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
