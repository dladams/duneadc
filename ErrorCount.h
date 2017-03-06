// ErrorCount.h

// David Adams
// February 2017
//
// Class to count errors.

#ifndef ErrorCount_H
#define ErrorCount_H

#include <string>

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

private:

  Index m_count;
  std::string m_errprefix;

};

#endif
