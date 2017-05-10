// ErrorCount.h

// David Adams
// February 2017
//
// Class to count errors.

#ifndef ErrorCount_H
#define ErrorCount_H

#include <string>
#include <iostream>

class ErrorCount {

public:

  using Index = unsigned int;

  // Ctor.
  ErrorCount(std::string errprefix ="");

  // Return the error count.
  Index count() const;

  // Check an expression. Returns if the epxression is true or false.
  // Increments the error count if it is false.
  // Writes msg prefixed by errprefix to cout if expr is false and msg is not empty.
  bool check(bool expr, std::string msg ="");

  // Check equality. Displays values on failure if msg is not null.
  template<class T1, class T2>
  bool checkequal(const T1& val1, const T2& val2, std::string msg) {
    bool ok = val1 == val2;
    if ( ! ok ) {
      ++m_count;
      if ( msg.size() ) cout << m_errprefix << msg << ": " << val1 << " != " << val2 << endl;
    }
    return ok;
  }

private:

  Index m_count;
  std::string m_errprefix;

};

#endif
