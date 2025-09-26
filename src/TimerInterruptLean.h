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

  static inline void stop();
  static TIMER_INTERRUPT_LEAN_ERROR start(const int32_t timerSettings, const uint32_t shotCount);

  static void isr();
private:
  // The instance for this timerNo
  static TimerInterruptLean<timerNo>* timerInterruptInstance;
  static void initTimerRegister();
};

template<unsigned timerNo> TimerInterruptLean<timerNo> *TimerInterruptLean<timerNo>::timerInterruptInstance = nullptr;

template<unsigned timerNo>
inline bool TimerInterruptLean<timerNo>::begin() {
  if (timerInterruptInstance == nullptr) {
    // Only one instance per timerNo allowed
    timerInterruptInstance = this;
    initTimerRegister();
    return true;
  }
  return false;
}

template<unsigned timerNo>
inline void TimerInterruptLean<timerNo>::isr() {
  if (timerInterruptInstance) {
    timerInterruptInstance->onTimeout();
  }
}

template<unsigned timerNo>
inline int32_t TimerInterruptLean<timerNo>::getTimerSettingsForPeriod_us(
    const uint32_t microSeconds) {
  return getTimerSettingsForPeriod_ns(microSeconds * 1000L);
}

template<unsigned timerNo>
inline int32_t TimerInterruptLean<timerNo>::getTimerSettingsForPeriod_ms(
    const uint32_t milliSeconds) {
  return getTimerSettingsForPeriod_ns(milliSeconds * (1000L * 1000L));
}

#include "internal/TimerInterruptLeanAVR.inc"

#endif /* TIMERINTLEAN_TIMERINTERRUPTLEAN_H_ */
