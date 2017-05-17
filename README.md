# duneadc

Root macros for processing ADC test data

David Adams  
May 2017

## Usage 

Set up Root and then run it in this directory.

## Reading data

ADC test data comes in many formats: CSV, binary and Root trees. Most provide the ADC waveform
(ADC count for each tick) but early test data at BNL is an ADC-Vin table: number of samples
in bins of  ADC count/bin and input voltage).

The interface `AdcSampleReader` provides clients (you) with access to the data in any of these
formats. The data may be viewed as a raw waveform (where available) or as a table summary. ADC
sample files may be opened using `AdcSampleFinder`, e.g.

```
root> AdcSampleFinder asf;
root> auto prdr = asf.find("201703a_D60", 8);
root> cout << "Sample " << prdr->sample() << " tick count: " << prdr->nsample() << endl;
```

The (smart) pointer prdr is type `AdcSampleReader`.

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

All are P1, cold (LN2), 2 MHz waveforms unless otherwise indicated.

Data is found in `$HOME/data/dune/adc`. Contact me for help in obtaining data or adding samples.

The classes `AdcTestSampleReader` is used to read table and CSV files and `AdcBinarySampleReader`
is used read files in binary format.
Clients are expected to use `AdcSampleFinder`. Otherwise, the metadata and table will not be
filled for binary files.

## Calibration

Coming soon...

## Perfomance evaluation

Coming soon ...

## Reading this file

This file may be viewed on github at
[https://github.com/dladams/duneadc/blob/master/README.md]
or directly with a markdown viewer (e.g. Markdown viewer add-on in firefox).
