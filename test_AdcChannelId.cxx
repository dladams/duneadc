// test_AdcChannelId

#include "AdcChannelId.h"
#include "ErrorCount.h"

int test_AdcChannelId() {
  const string myname = "test_AdcChannelId: ";
  ErrorCount ec(myname + "Test failed: ");
  AdcChannelId idbad;
  AdcChannelId id11(1,1);
  AdcChannelId id12(1,2);
  AdcChannelId id22(2,2);
  ec.check(!idbad.isValid(), "!idbad.isValid()");
  ec.check(id11.isValid(), "id1.isValid()");
  ec.check(id11 < id22, "id11 < id22");
  ec.check(id12 < id22, "id12 < id22");
  ec.check(id11 < id12, "id11 < id12");
  ec.check(idbad < id11, "idbad < id11");
  return ec.count();
}

