#define timeSeconds 15

#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problemss
#include "driver/rtc_io.h"

void init_pir(int pir_pin);
bool warning();
