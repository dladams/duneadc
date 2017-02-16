// histNSigma.cxx

#include "histNSigma.h"

TH1* histNSigma(TH1* ph, double nsig, int nrange) {
  //for ( int ich=0; ich<=ph->GetNbinsX()+1; ++ich ) ph->SetBinError(ich, 0.0);
  TH1* phnsig = (TH1*) ph->Clone();
  bool havelo = false;
  double xlo = -2.e6;
  double xhi = 2.e6;
  double mean = ph->GetMean();
  double dnsig = nsig*ph->GetRMS();
  double xnsiglo = mean - dnsig;
  double xnsighi = mean + dnsig;
  double dx = ph->GetBinLowEdge(2) - ph->GetBinLowEdge(1);
  for ( int ich=1; ich<=ph->GetNbinsX()+1; ++ich ) {
    double xl = ph->GetBinLowEdge(ich);
    double xc = ph->GetXaxis()->GetBinCenter(ich);
    double dx = 2.0*(xc - xl);
    double nent = ph->GetBinContent(ich);
    double valsig = ( xc < xnsiglo || xc > xnsighi ) ? nent : 0;
    phnsig->SetBinContent(ich, valsig);
    phnsig->SetBinError(ich, 0.0);
    if ( nent > 0.0 && !havelo ) {
      xlo = xl;
      havelo = true;
    }
    if ( havelo && nent != 0 ) xhi = xl + dx;
  }
  if ( nrange >= 0 ) {
    double xrlo = xlo - nrange*dx;
    double xrhi = xhi + nrange*dx;
    phnsig->GetXaxis()->SetRangeUser(xrlo, xrhi);
  }
  phnsig->SetFillStyle(1001);
  phnsig->SetFillColor(phnsig->GetLineColor());
  return phnsig;
}
