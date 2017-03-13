// TH1Props.cxx

#include "TH1Props.h"
#include "TH1.h"

//**********************************************************************

TH1Props::TH1Props(TH1* a_ph) : ph(a_ph) { }

//**********************************************************************

double TH1Props::pullMin() const {
  int bin = ph->FindFirstBinAbove(0);
  if ( bin < 0 ) return 0;
  double dev = ph->GetBinLowEdge(bin) - ph->GetMean();
  double rms = ph->GetRMS();
  if ( rms > 0.0 ) dev /= rms;
  return dev;
}

//**********************************************************************

double TH1Props::pullMax() const {
  int bin = ph->FindLastBinAbove(0);
  if ( bin < 0 ) return 0;
  double dev = ph->GetBinLowEdge(bin+1) - ph->GetMean();
  double rms = ph->GetRMS();
  if ( rms > 0.0 ) dev /= rms;
  return dev;
}

//**********************************************************************

double TH1Props::expandedRMS(double apullmax) const {
  if ( apullmax <= 0.0 ) return 0.0;
  double apull1 = -pullMin();
  double apull2 =  pullMax();
  double apull = apull1 > apull2 ? apull1 : apull2;
  double rms = ph->GetRMS();
  if ( rms <= 0 ) return rms;
  double xrms = rms;
  if ( apull > apullmax ) {
    xrms *= apull/apullmax;
  }
  return xrms;
}

//**********************************************************************

double TH1Props::tailFracLo(double apullthresh) const {
  double x = ph->GetMean() - apullthresh*ph->GetRMS();
  int bin = ph->FindFixBin(x);
  int nbin = ph->GetNbinsX();
  double countTail = ph->Integral(0,bin);
  double countTotal = ph->Integral(0, nbin+1);
  double frac = countTotal > 0.0 ? countTail/countTotal : 0.0;
  return frac;
}

//**********************************************************************

double TH1Props::tailFracHi(double apullthresh) const {
  double x = ph->GetMean() + apullthresh*ph->GetRMS();
  int bin = ph->FindFixBin(x);
  int nbin = ph->GetNbinsX();
  double countTail = ph->Integral(bin, nbin+1);
  double countTotal = ph->Integral(0, nbin+1);
  double frac = countTotal > 0.0 ? countTail/countTotal : 0.0;
  return frac;
}

//**********************************************************************

double TH1Props::tailFrac(double apullthresh) const {
  double tailSum = tailFracLo(apullthresh) + tailFracHi(apullthresh);
  if ( tailSum > 1.0 ) tailSum = 1.0;
  return tailSum;
}

//**********************************************************************
