# duneadc

Root macros for processing ADC test data

David Adams  
May 2017

## Introduction

This repository (duneadc) holds Root macros for use in analyzing LAr TPC ADC test data.
For assistance, please contadt the author.

## News

June 1, 2017   
The version committed today changes the name of the class that holds calibration tree data
from AdcChannelCalibration to AdcTreeChannelCalibrationData. This means that calibration
trees created with earlier versions are no longer readable with AdcCalibrationTree.

## Usage 

Set up Root and then run it in this directory.

## Reading data

ADC test data comes in many formats: CSV, binary and Root trees. Most provide the ADC waveform
(ADC count for each tick) but early test data at BNL is an ADC-Vin table: number of samples
in bins of  ADC count/bin and input voltage).

The interface [AdcSampleReader](AdcSampleReader.h) provides clients (you) with access to the
data in any of these formats.
The data may be viewed as a raw waveform (where available) or as a table summary. ADC
sample files may be opened using [AdcSampleFinder](AdcSampleFinder.h), e.g.

```
root> AdcSampleFinder asf;
root> auto prdr = asf.find("201703a_D60", 8);
root> cout << "Sample " << prdr->sample() << " tick count: " << prdr->nsample() << endl;
```

The (smart) pointer prdr is to type `AdcSampleReader`.

In addition to reading and opening the input file, the sample reader ensures that the table
summary and input voltage calibration are available. It determines the data format from the sample
name and then creates a reader of the appropriate type.

Some of the available sample names are

| Sample name | Description |
|-------------|-------------|
|             |             |
| 2016XXX     | October-December 2016 BNL data. Some is in table format. |
|             |             |
| 201701a_CC  | January 2017 BNL scan of 25 chips. The chip number is CC = 00, 02, ... 35. Bad Vin. |
| 201701b_CC  | Same as 201701a_CC with some ticks removed to fix the Vin calibration. Use this. |
| 201701c_CC  | Same as 201701b_CC repeated for chips CC = 02 and 04. |
| 201701d_CC  | Same as 201701a_CC but at 1 MHz sampling. |
| 201702TT_CC | Same as 201701c_CC for dates Feb TT = 16, 17, ..., 23. |
| 201703TT_CC | More like 201702TT_CC for dates March TT = 06 and 07? |
|             |             |
| 201703a_DCC | March 2017 BNL scan of 77 P1 chips. The chip number is CC = 01, 03, ..., 79. |
| 201703b_D02_TT | BNL  long-term study of chip D02. The time sample is TT = 1a, 2a, ..., 9c. |
| 201706_cots | BNL study of LN COTS (commercial off-the-shelf) chips in Jun 2017. |
| 201706_cotsw | As above but at RT (room temperature)
| 201706_cotsb | Repeat of parts of 201706_cots. |
| 201706_cotsbw | As above but at RT (room temperature)
| 201707_cotsBB | BNL study of LN COTS (commercial off-the-shelf) chips in Jul 2017. BB=01,...10. |
| 201708_cotsBB | BNL study of LN COTS (commercial off-the-shelf) chips in Aug 2017.  BB=01,...04.|
| DUNE17-cold_chipCC | BNL standard (4 MHz signal gen) testing of protoDUNE chips. |
| DUNE17dla-cold_chipCC | BNL standard data taken before the main run (aka try or David Adams data) |
| DUNE17ts-cold_chipCC | BNL test stand (1 MHz signal gen) testing of protoDUNE chips. |
| DUNE17-test_chipCC | Subset of protoDUNE chips tested is same way as samples 201703a. |

All are P1, cold (LN2), 2 MHz waveforms unless otherwise indicated.

Data is found in `$HOME/data/dune/adc`. Contact me for help in obtaining data or adding samples.
A failed attempt to open a sample should produce a message showing the expected file name
and location.

The classes `AdcTestSampleReader` is used to read table and CSV files, `AdcBinarySampleReader`
to read files in binary format and `AdcFembTreeSampleReader` for those in the Root tree format..
Clients are recommended to use `AdcSampleFinder`. Otherwise, the metadata, voltage calibration and table
may not be available.

## Input voltage calibration

Data files typically do not provide an accurate input voltage calibration. As noted above, the reader
or finder adds this information which is then available through the reader interface.

## Response calibration

At present, two calibrations for an ADC channel are supported: linear and ultimate.

The linear calibration is
```
  V = gain*code + offset
```
where V is the calibrated input voltage and code is the ADC code/bin, e.g., for a 12-bit ADC
such as P1, in the range `0 < code < 4095` with the values 0 and 4095 flagging underflow and
overflow, respectively. The calibration constants are `gain` and `offset`.

The ultimate calibration has an input voltage for each ADC code:
```
  V = calMeans[code]
```
These values are typically obtained for a single channel sample by taking the mean of the input voltages
that populate each ADC bin. The quality of each bin is characterized by the standard deviation of
its voltage distribution `calRmss[code]` and the tail fraction `calTails[code]`. The latter is the
fraction of input voltage samples that are more than some distance from the mean. In recent
studies this limit has been set to 5 mV.

The linear and ultimate calibration data are stored in the class
[AdcChannelCalibration](AdcChannelCalibration.h) and include the above variables:

| [Type](AdcTypes.h) | Name |
|------|------|
| ShortIndex       | chip |
| ShortIndex       | chan |
| AdcTime          | time |
| Float            | gain |
| Float            | offset |
| FloatVector      | calMeans |
| FloatVector      | calRmss |
| FloatVector      | calTails |
| ShortIndexVector | calCounts |

The first three fields specify the data sample: the chip index, channel number and time
at which the data was recorded. The latter enables us to distinguish multiple data runs
with the same chip, e.g. long-term studies.

The last field `calCounts[code]` gives the number of voltage samples contributing to the
ADC bin.

Bins are typically flagged as bad if the RMS for a code is above some threshold, e.g.
```
calRmss[code] > 1.0 mV
```
Samples falling in such bins might be replaced with interpolated values (rather than the
calibrated value) as part of the data preparation for reconstruction.

Similarly, bins with large tail fractions might also be flagged as bad.

The class [AdcCalibrationTree](AdcCalibrationTree.h) facilitates storing calibration data
in a root tree and later retrieving it from that tree. The tree variable names are the same as above.

## Perfomance evaluation

The class [AdcChipAnalyzer](AdcChipAnalyzer.h) may be used to carry out calibration and performance analysis,
fill the corresponding trees and generate plots with behavior controlled by parameters passed
to the ctor. Production processing of DUNE data is carried out by constructing one object of this
type for each sample. It then fills the calibration and performance trees and can then be called
to produce any of the standard plots. For an exaple of use, see [processDataset.C], then script called
in production processing.

## Validation

There are a few tests in duneadc but not near as many as I would like.

To run all tests with default arguments:
```
root> .X test_all.C
```

To verify you can properly read some data files (and so they are installed) use:
```
root> .L test_AdcSampleFinder.cxx+
root> test_AdcSampleFinder()
```
This will read and check one channel from 201703a_D22 and one from 201703b_D02_6b. If all is well,
the final messsage will indicate that all test pass. You can provide arguments specifying
the respective channel numbers for the samples with -1 to skip the sample.
E.g. to test channel 4 for 201703b_D02_6b:
```
root> test_AdcSampleFinder(-1, 4)
```

There is also a test for the chip analyzer:
```
root> .L test_AdcChipAnalyzer.cxx+
root> test_AdcChipAnalyzer(4);
```
The argument is the number of channels to analyze.

## Reading this file

This file may be viewed on github at
[https://github.com/dladams/duneadc/blob/master/README.md]
or directly with a markdown viewer (e.g. Markdown viewer add-on in firefox).
