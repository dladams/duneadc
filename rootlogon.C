{
  gSystem->SetBuildDir(".aclic");
  gROOT->ProcessLine(".L palette.cxx+");
  gROOT->ProcessLine(".L EffNBit.cxx+");
  gROOT->ProcessLine(".L SampleFunction.cxx+");
  gROOT->ProcessLine(".L Sawtooth.cxx+");
  gROOT->ProcessLine(".L TH1Props.cxx+");
  gROOT->ProcessLine(".L histNSigma.cxx+");
  gROOT->ProcessLine(".L drawNSigma.cxx+");
  gROOT->ProcessLine(".L ErrorCount.cxx+");
  gROOT->ProcessLine(".L FileDirectory.cxx+");
  gROOT->ProcessLine(".L AdcChannelId.cxx+");
  gROOT->ProcessLine(".L AdcChannelCalibration.cxx+");
  gROOT->ProcessLine(".L AdcCalibrationTree.cxx+");
  gROOT->ProcessLine(".L AdcSampleReader.cxx+");
  gROOT->ProcessLine(".L AdcBorderFinder.cxx+");
  gROOT->ProcessLine(".L AdcBorderExtremaFinder.cxx+");
  gROOT->ProcessLine(".L AdcTestSampleReader.cxx+");
  gROOT->ProcessLine(".L AdcBinarySampleReader.cxx+");
  gROOT->ProcessLine(".L AdcSampleFinder.cxx+");
  gROOT->ProcessLine(".L AdcBinRecord.cxx+");
  gROOT->ProcessLine(".L AdcVoltageResponse.cxx+");
  gROOT->ProcessLine(".L AdcVoltagePerformance.cxx+");
  gROOT->ProcessLine(".L AdcBinRecordBuilder.cxx+");
  gROOT->ProcessLine(".L AdcBinExtremaFinder.cxx+");
  gROOT->ProcessLine(".L AdcPerformanceTree.cxx+");
  gROOT->ProcessLine(".L AdcSampleAnalyzer.cxx+");
  gROOT->ProcessLine(".L AdcChipAnalyzer.cxx+");
  gROOT->ProcessLine(".L AdcChipStatus.cxx+");
  gROOT->ProcessLine(".L AdcDatasetAnalyzer.cxx+");
  gROOT->ProcessLine(".L AdcChipMetric.cxx+");
  gROOT->ProcessLine(".L AdcTimeAnalyzer.cxx+");
  //dev1 gROOT->ProcessLine(".L AdcPerfHists.cxx+");
  //gROOT->ProcessLine(".L processChip.cxx+");
  gROOT->ProcessLine(".L AdcHist.cxx+");   // obsolete
  gROOT->ProcessLine(".L makeplot.cxx+");   // obsolete
  gROOT->ProcessLine(".L drawChipBounds.C");
  gROOT->ProcessLine(".L drawvlimits.C");
  gROOT->ProcessLine(".L datasetAnalyze.C");
  palette(0);
  int chan = 0;
  gStyle->SetTitleW(1.0);  // So title can extend full width of pad.
}
