// Sawtooth.cxx

#include "Sawtooth.h"

//**********************************************************************

Sawtooth::Sawtooth(double vmin, double vmax, const AdcExtrema exts)
: m_vmin(vmin), m_vmax(vmax), m_isamMin(0), m_wsam(0),
  m_isamLow(0), m_isamHigh(0) {
  Index next = exts.size();
  if ( next < 2 ) return;
  for ( Index iext=1; iext<next; ++iext ) {
    if ( exts[iext-1].isMin() && exts[iext].isMin() ) return;
  }
  m_isamMin = exts[0].isMin() ? exts[0].tick() : exts[1].tick();
  m_wsam = (exts[next-1].tick() - exts[0].tick())/(next - 1);
  m_isamHigh = exts[next-1].tick() + m_wsam;
}

//**********************************************************************

double Sawtooth::value(SampleIndex isam, double* pdvds) const {
  if ( m_vmax <= m_vmin ) return 0.0;
  if ( m_wsam == 0 ) return 0.0;
  SampleIndex xsam = isam > m_isamMin ? isam - m_isamMin : m_isamMin - isam;
  SampleIndex xmod2w = xsam%(2*m_wsam);
  bool up = xmod2w < m_wsam;
  double dxsam = up ? xmod2w : 2*m_wsam - xmod2w;
  double frac = dxsam/m_wsam;
  double val = (1.0-frac)*m_vmin + frac*m_vmax;
  if ( pdvds != nullptr ) {
    double dv = m_vmax - m_vmin;
    double ds = m_wsam;
    double dvds = (ds == 0.0) ? 0.0 : dv/ds;
    if ( !up ) dvds *= -1.0;
    *pdvds = dvds;
  }
  return val;
}

//**********************************************************************
