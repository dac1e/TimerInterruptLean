/*
 * log.cpp
 *
 *  Created on: 25.09.2025
 *      Author: Wolfgang
 */


#include "log.h"

namespace TimerIntLean_ {

char* uint64toStr( uint64_t num ) {
  static char buf[22];
  char* p = &buf[sizeof(buf)-1];
  *p = '\0';
  do {
    *--p = '0' + (num%10);
    num /= 10;
  } while ( num > 0 );
  return p;
}

} // namespace TimerIntLean_ {
