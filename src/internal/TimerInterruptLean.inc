/*
 * TimerInterruptLean.inc
 *
 *  Created on: 27.09.2025
 *      Author: Wolfgang
 */

// Platform independent
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

// Platform dependent
#include "internal/TimerInterruptLeanAVR.inc"
