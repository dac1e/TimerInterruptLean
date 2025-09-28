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

// Platform dependent
#include "internal/TimerInterruptLeanAVR.inc"
