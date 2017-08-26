# dune17dlacSamples.py

# David Adams
# August 2017

from dune17bad import badChips
from dune17sel import selectedChips
from dune17Chip import dune17cChip

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
  sams.append(pre + "D0162_0816_try1")
  sams.append(pre + "D0163_0816_try1")
  sams.append(pre + "D0164_0816_try1")
  sams.append(pre + "D0165_0816_try1")
  sams.append(pre + "D0166_0816_try1")
  sams.append(pre + "D0167_0816_try1")
  sams.append(pre + "D0168_0816_try1")
  sams.append(pre + "D0169_0816_try1")
  sams.append(pre + "D0170_0816_try1")
  sams.append(pre + "D0171_0816_try1")
  sams.append(pre + "D0173_0816_try1")
  sams.append(pre + "D0174_0816_try1")
  badsams.append(pre + "D0175_0816_try1")  # Bad waveforms. AdcSampleFinder hangs (16min?) in chan 13
  sams.append(pre + "D0176_0816_try1")
  sams.append(pre + "D0177_0816T1447_try1")
  sams.append(pre + "D0177_0816T1506_try1")
  sams.append(pre + "D0178_0816_try1")
  sams.append(pre + "D0179_0816_try1")
  sams.append(pre + "D0180_0816_try1")
  sams.append(pre + "D0182_0816_try1")
  sams.append(pre + "D0183_0816_try1")
  sams.append(pre + "D0184_0816_try1")
  sams.append(pre + "D0185_0816_try1")
  sams.append(pre + "D0186_0817_try1")
  badsams.append(pre + "D0188_0817_try1")  # Bad waveforms
  sams.append(pre + "D0189_0817T0919_try1")
  sams.append(pre + "D0189_0817T1004_try1")
  sams.append(pre + "D0190_0817T1001_try1")
  badsams.append(pre + "D0190_0817T1023_try1:chipD0191")
  sams.append(pre + "D0190_0817T1023_try2:chipD0191")
  sams.append(pre + "D0190_0817T1049_try1:chipD0193")
  sams.append(pre + "D0190_0817T1113_try1:chipD0194")
  sams.append(pre + "D0190_0817T1113_try2:chipD0194")
  sams.append(pre + "D0192_0817_try1")
  sams.append(pre + "D0195_0817_try1")
  sams.append(pre + "D0196_0817_try1")
  sams.append(pre + "D0197_0817_try1")
  sams.append(pre + "D0198_0817_try1")
  sams.append(pre + "D0199_0817_try1")
  sams.append(pre + "D0200_0817_try1")
  sams.append(pre + "D0201_0817_try1")
  sams.append(pre + "D0202_0817_try1")
  sams.append(pre + "D0203_0817_try1")
  sams.append(pre + "D0204_0817_try1")
  sams.append(pre + "D0205_0817_try1")
  sams.append(pre + "D0206_0817_try1")
  sams.append(pre + "D0207_0817_try1")
  sams.append(pre + "D0208_0817_try1")
  badsams.append(pre + "D0209_0817T1525_try1")  # Bad waveforms.
  sams.append(pre + "D0209_0817T1542_try1")
  sams.append(pre + "D0210_0817_try1")
  sams.append(pre + "D0211_0817T1606_try1")
  sams.append(pre + "D0211_0817T1614_try1")
  sams.append(pre + "D0212_0817_try1")
  sams.append(pre + "D0213_0817_try1")
  sams.append(pre + "D0214_0817_try1")
  sams.append(pre + "D0215_0818_try1")
  sams.append(pre + "D0216_0818_try1")
  sams.append(pre + "D0217_0818_try1")
  sams.append(pre + "D0218_0818_try1")
  sams.append(pre + "D0220_0818_try1")
  sams.append(pre + "D0221_0818_try1")
  sams.append(pre + "D0222_0818_try1")
  sams.append(pre + "D0223_0818_try1")
  sams.append(pre + "D0224_0818_try1")
  sams.append(pre + "D0225_0818_try1")
  sams.append(pre + "D0225_0818_try2")
  badsams.append(pre + "D0226_0818_try1")  # Bad waveforms
  sams.append(pre + "D0227_0818_try1")
  badsams.append(pre + "D0229_0818_try1")  # Bad waveforms
  sams.append(pre + "D0230_0818_try1")
  sams.append(pre + "D0231_0818_try1")
  sams.append(pre + "D0232_0818_try1")
  badsams.append(pre + "D0233_0818_try1")  # Bad waveforms
  sams.append(pre + "D0234_0818_try1")
  sams.append(pre + "D0235_0818_try1")
  sams.append(pre + "D0236_0818_try1")
  sams.append(pre + "D0236_0818_try2")
  sams.append(pre + "D0238_0818_try1")
  sams.append(pre + "D0239_0818_try1")
  badsams.append(pre + "D0240_0818_try1")  # Bad waveforms
  sams.append(pre + "D0241_0818_try1")
  sams.append(pre + "D0242_0818_try1")
  sams.append(pre + "D0243_0821_try1")
  sams.append(pre + "D0244_0821_try1")
  sams.append(pre + "D0245_0821_try1")
  sams.append(pre + "D0246_0821T1703_try1")
  sams.append(pre + "D0246_0821T1850_try1")
  sams.append(pre + "D0247_0821_try1")
  sams.append(pre + "D0248_0821_try1")
  sams.append(pre + "D0249_0822_try1")
  sams.append(pre + "D0250_0822_try1")
  sams.append(pre + "D0251_0822_try1")
  sams.append(pre + "D0252_0822_try1")
  sams.append(pre + "D0253_0822_try1")
  sams.append(pre + "D0254_0822_try1")
  sams.append(pre + "D0255_0822_try1")
  sams.append(pre + "D0256_0822_try1")
  sams.append(pre + "D0157_0821_try1:chipD0257")
  sams.append(pre + "D0257_0821_try1")
  sams.append(pre + "D0258_0822_try1")
  sams.append(pre + "D0259_0822_try1")
  sams.append(pre + "D0260_0822T1325_try1")
  sams.append(pre + "D0260_0822T1422_try1")
  sams.append(pre + "D0262_0822_try1")
  sams.append(pre + "D0263_0822T1517_try1")
  sams.append(pre + "D0263_0822T1604_try1")
  sams.append(pre + "D0264_0822_try1")
  sams.append(pre + "D0265_0822_try1")
  sams.append(pre + "D0266_0822_try1")
  sams.append(pre + "D0267_0822_try1")
  sams.append(pre + "D0268_0822T1229_try1")
  sams.append(pre + "D0268_0822T1504_try1")
  sams.append(pre + "D0268_0822T1823_try1")
  sams.append(pre + "D0269_0823T1011_try1")
  badsams.append(pre + "D0269_0822T1841_try1")  # Bad waveforms
  sams.append(pre + "D0270_0822_try1")
  sams.append(pre + "D0271_0822_try1")
  sams.append(pre + "D0272_0822_try1")
  sams.append(pre + "D0273_0823_try1")
  sams.append(pre + "D0274_0822_try1")
  sams.append(pre + "D0275_0822_try1")
  sams.append(pre + "276_0822_try1:chipD0276")
  sams.append(pre + "278_0822_try1:chipD0278")
  sams.append(pre + "D0277_0822_try1")
  sams.append(pre + "D0279_0822_try1")
  sams.append(pre + "D0279_0822_try2")
  sams.append(pre + "D0280_0823_try1")
  sams.append(pre + "D0281_0823_try1")
  sams.append(pre + "D0282_0823_try1")
  sams.append(pre + "D0283_0823_try1")
  sams.append(pre + "D0284_0823_try1")
  sams.append(pre + "D0285_0823_try1")
  sams.append(pre + "D0286_0823_try1")
  sams.append(pre + "D0288_0823T1035_try1")
  sams.append(pre + "D0288_0823T1635_try1")
  sams.append(pre + "D0289_0823_try1")
  sams.append(pre + "D0290_0823_try1")
  sams.append(pre + "D0291_0823_try1")
  sams.append(pre + "D0292_0823_try1")
  badsams.append(pre + "D0293_0824_try1")  # Bad waveforms
  sams.append(pre + "D0294_0823_try1")
  badsams.append(pre + "D0295_0823T1808_try1")  # Bad waveforms
  sams.append(pre + "D0295_0823T1820_try1")
  sams.append(pre + "D0296_0823_try1")
  sams.append(pre + "D0297_0823_try1")
  sams.append(pre + "D0298_0823T1947_try1")
  sams.append(pre + "D0298_0823T1951_try1")
  sams.append(pre + "D0299_0823_try1")
  sams.append(pre + "D0301_0823_try1")
  badsams.append(pre + "D0302_0823T2034_try1")  # Bad waveforms
  sams.append(pre + "D0302_0823T2047_try1")
  sams.append(pre + "D0303_0823_try1")
  sams.append(pre + "D0304_0823_try1")
  sams.append(pre + "D0305_0824_try1")
  sams.append(pre + "D0306_0824_try1")
  sams.append(pre + "D0307_0824_try1")
  sams.append(pre + "D0308_0824_try1")
  sams.append(pre + "D0309_0824_try1")
  sams.append(pre + "D0310_0824_try1")
  sams.append(pre + "D0311_0824_try1")
  sams.append(pre + "D0312_0824_try1")
  sams.append(pre + "D0313_0824_try1")
  sams.append(pre + "D0314_0824_try1")
  sams.append(pre + "D0315_0824_try1")
  badsams.append(pre + "D0316_0824_try1")  # Bad waveforms
  sams.append(pre + "D0317_0824_try1")
  sams.append(pre + "D0318_0824_try1")
  sams.append(pre + "D0319_0824T1551_try1")
  sams.append(pre + "D0319_0824T1556_try1")  # 8/25
  sams.append(pre + "D0319_0824T1556_try2")
  sams.append(pre + "D0320_0824_try1")
  sams.append(pre + "D0321_0824_try1")
  sams.append(pre + "D0323_0824_try1")
  sams.append(pre + "D0324_0824_try1")
  sams.append(pre + "D0325_0824_try1")
  sams.append(pre + "D0326_0824T1744_try1")
  sams.append(pre + "D0326_0824T1853_try1")
  badsams.append(pre + "D0327_0824_try1")  # Bad waveforms
  sams.append(pre + "D0328_0824_try1")
  sams.append(pre + "D0329_0824_try1")
  sams.append(pre + "D0330_0824_try1")
  sams.append(pre + "D0331_0824_try1")
  badsams.append(pre + "D0332_0824_try1")  # Bad waveforms
  sams.append(pre + "D0333_0824_try1")
  badsams.append(pre + "D0334_0825T0925_try1")  # Bad waveforms
  badsams.append(pre + "D0334_0825T0938_try1")  # Bad waveforms
  sams.append(pre + "D0335_0825_try1")
  sams.append(pre + "D0336_0824_try1")
  badsams.append(pre + "D0337_0825_try1")  # Bad waveforms
  sams.append(pre + "D0338_0825_try1")
  sams.append(pre + "D0339_0825T1054_try1")
  badsams.append(pre + "D0339_0825T1213_try1")  # bad waveforms
  sams.append(pre + "D0340_0825_try1")
  sams.append(pre + "D0341_0825_try1")
  badsams.append(pre + "D0342_0825_try1")  # Bad waveforms
  sams.append(pre + "D0344_0825_try1")
  badsams.append(pre + "D0345_0825_try1")  # Bad waveforms
  sams.append(pre + "D0346_0825_try1")
  sams.append(pre + "D0347_0825_try1")
  sams.append(pre + "D0349_0825_try1")  # 8/26
  sams.append(pre + "D0351_0825_try1")  # 8/26
  sams.append(pre + "D0352_0825_try1")  # 8/26
  sams.append(pre + "D0355_0825T1843_try1")  # 8/26
  sams.append(pre + "D0355_0825T1848_try1")  # 8/26
  sams.append(pre + "D0356_0825_try1")  # 8/26
  sams.append(pre + "D0357_0825_try1")  # 8/26
  sams.append(pre + "D0358_0825_try1")  # 8/26
  sams.append(pre + "D0359_0825_try1")  # 8/26
  sams.append(pre + "D0360_0825_try1")  # 8/26
  sams.append(pre + "D0361_0825_try1")  # 8/26
  sams.append(pre + "D0361_0825_try2")  # 8/26
  sams.append(pre + "D0362_0825_try1")  # 8/26
  sams.append(pre + "D0363_0825_try1")  # 8/26
  sams.append(pre + "D0364_0825_try1")  # 8/26
  sams.append(pre + "D0365_0825_try1")  # 8/26
  sams.append(pre + "D0366_0825_try1")  # 8/26
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
