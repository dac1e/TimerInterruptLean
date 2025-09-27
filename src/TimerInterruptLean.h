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

enum class TIMER_INTERRUPT_LEAN_ERROR {
  OK,
  PERIOD_TOO_SMALL,
  PERIOD_TOO_LARGE
};

template<unsigned timerNo>
class TimerInterruptLean {
  virtual void onTimeout() = 0;

public:
  bool begin();

  // platform dependent API
  static int32_t getTimerSettingsForPeriod_ms(const uint32_t milliSeconds);
  static int32_t getTimerSettingsForPeriod_us(const uint32_t microSeconds);
  static int32_t getTimerSettingsForPeriod_ns(const uint32_t nanoSeconds);

  static uint32_t minPeriod_ns();
  static uint32_t maxPeriod_ns();

  static TIMER_INTERRUPT_LEAN_ERROR start(const int32_t timerSettings, const uint32_t shotCount);
  static inline void stop();

  static void isr();
private:
  // The instance for this timerNo
  static TimerInterruptLean<timerNo>* timerInterruptInstance;
  static void initTimerRegister();
};


#include "internal/TimerInterruptLean.inc"


#endif /* TIMERINTLEAN_TIMERINTERRUPTLEAN_H_ */
