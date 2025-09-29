/*
  TimerInterruptLean - Arduino libary for timer abstraction Copyright (c)
  2025 Wolfgang Schmieder.  All right reserved.

  Contributors:
  - Wolfgang Schmieder

  Project home: https://github.com/dac1e/TimerInterruptLean/

  Preprocessor macros for detection of Arduino Board have been taken from:
    https://github.com/khoih-prog/TimerInterrupt_Generic

  This library is free software; you can redistribute it and/or modify it
  the terms of the GNU Lesser General Public License as under published
  by the Free Software Foundation; either version 3.0 of the License,
  or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
*/


#pragma once

#ifndef TIMERINTERRUPTLEAN_INTERNAL_LOG_H_
#define TIMERINTERRUPTLEAN_INTERNAL_LOG_H_

#include <stdint.h>
#include <Arduino.h>

#define TimerIntLean_ENABLE_LOG false

namespace TimerIntLean_ {

// The following functions are for debugging purpose only

char* uint64toStr( uint64_t num );

template<typename T> void print_variableln(const T& v, const char* const varname, const char* context=nullptr) {
  if(context) {
    Serial.print(context);
    Serial.print(':');
  }
  Serial.print(varname);
  Serial.print('=');
  Serial.println(v);
}

template<typename T> void print_variable(const T& v, const char* const varname, const char* context=nullptr) {
  if(context) {
    Serial.print(context);
    Serial.print(':');
  }
  Serial.print(varname);
  Serial.print('=');
  Serial.print(v);
}

inline void print_variableln(const uint64_t& v, const char* const varname, const char* context=nullptr) {
  if(context) {
    Serial.print(context);
    Serial.print(':');
  }
  Serial.print(varname);
  Serial.print('=');
  Serial.println(uint64toStr(v));
}

inline void print_variable(const uint64_t& v, const char* const varname, const char* context=nullptr) {
  if(context) {
    Serial.print(context);
    Serial.print(':');
  }
  Serial.print(varname);
  Serial.print('=');
  Serial.print(uint64toStr(v));
}

#if !defined(__STRINGIFY)
  #define __STRINGIFY(s) #s
#endif


#if TimerIntLean_ENABLE_LOG
#define logvar(x) ::TimerIntLean_::print_variable(x, __STRINGIFY(x), __FUNCTION__)
#define logvarln(x) ::TimerIntLean_::print_variableln(x, __STRINGIFY(x), __FUNCTION__)
#else
#define logvar(x)
#define logvarln(x)
#endif
} // namespace TimerIntLean_

#endif /* TIMERINTERRUPTLEAN_INTERNAL_LOG_H_ */
