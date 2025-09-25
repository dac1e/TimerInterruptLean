/*
 * log.h
 *
 *  Created on: 25.09.2025
 *      Author: Wolfgang
 */

#ifndef TIMERINTERRUPTLEAN_INTERNAL_LOG_H_
#define TIMERINTERRUPTLEAN_INTERNAL_LOG_H_

#include <stdint.h>

#include <Arduino.h>

char* str( uint64_t num );


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
  Serial.println(str(v));
}

inline void print_variable(const uint64_t& v, const char* const varname, const char* context=nullptr) {
  if(context) {
    Serial.print(context);
    Serial.print(':');
  }
  Serial.print(varname);
  Serial.print('=');
  Serial.print(str(v));
}


#define STRINGIFY(s) #s
#define logvar(x) print_variable(x, STRINGIFY(x), __FUNCTION__)
#define logvarln(x) print_variableln(x, STRINGIFY(x), __FUNCTION__)


#endif /* TIMERINTERRUPTLEAN_INTERNAL_LOG_H_ */
