// ErrorCount.cxx

#include "ErrorCount.h"

using std::cout;
using std::endl;
using Index = ErrorCount::Index;

//**********************************************************************

ErrorCount::ErrorCount(string errprefix)
: m_count(0),
  m_errprefix(errprefix) { }

//**********************************************************************

Index ErrorCount::count() const {
  return m_count;
}

//**********************************************************************

bool ErrorCount::check(bool expr, string msg) {
  if ( ! expr ) {
    ++m_count;
    if ( msg.size() ) cout << m_errprefix << msg << endl;
  }
  return expr;
}

//**********************************************************************
