/*
 * TimerIntLean.h
 *
 *  Created on: 24.09.2025
 *      Author: Wolfgang
 */

#ifndef TIMERINTLEAN_TIMERINTERRUPTLEAN_H_
#define TIMERINTLEAN_TIMERINTERRUPTLEAN_H_

#include <stdint.h>
#include "internal/log.h"

typedef int32_t TimerInterruptLean_timer_settings_t;

enum class TIMER_INTERRUPT_LEAN_ERROR {
  OK,
  PERIOD_TOO_SMALL,
  PERIOD_TOO_LARGE
};


template<unsigned timerNo>
class TimerInterruptLean {
private:
  typedef TimerInterruptLean_timer_settings_t timer_settings_t;
  virtual void onTimeout() = 0;

public:
  bool begin();

  // platform dependent API
  static timer_settings_t calculateTimerSettingsForPeriod_ns(const uint32_t nanoSeconds);

  static uint32_t getScheduledTimeoutPeriod_ns(const timer_settings_t timerSettings);

  static uint32_t minPeriod_ns();
  static uint32_t maxPeriod_ns();

  static TIMER_INTERRUPT_LEAN_ERROR checkTimeoutPeriod(const timer_settings_t timerSettings);
  static TIMER_INTERRUPT_LEAN_ERROR start(const timer_settings_t timerSettings, const uint32_t shotCount);
  static inline void stop();

  static void isr();
private:
  // Pointer to the TimerInterruptLean instance for this timerNo
  static TimerInterruptLean<timerNo>* timerInterruptInstance;
  static void initTimerRegister();
};


#include "internal/TimerInterruptLean.inc"


#endif /* TIMERINTLEAN_TIMERINTERRUPTLEAN_H_ */
