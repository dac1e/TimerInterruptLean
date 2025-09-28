/*
 * log.h
 *
 *  Created on: 25.09.2025
 *      Author: Wolfgang
 */

#pragma once

#ifndef TIMERINTERRUPTLEAN_INTERNAL_LOG_H_
#define TIMERINTERRUPTLEAN_INTERNAL_LOG_H_

#include <stdint.h>
#include <Arduino.h>

#define TimerIntLean_ENABLE_LOG false


namespace TimerIntLean_ {

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
#define logvar(x) print_variable(x, __STRINGIFY(x), __FUNCTION__)
#define logvarln(x) print_variableln(x, __STRINGIFY(x), __FUNCTION__)
#else
#define logvar(x)
#define logvarln(x)
#endif
} // namespace TimerIntLean_

#endif /* TIMERINTERRUPTLEAN_INTERNAL_LOG_H_ */
