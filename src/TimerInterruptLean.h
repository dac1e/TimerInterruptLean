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

  /**
   * The timer settings carry prescaler and counter values for the requested period.
   * Those timer settings can be passed to the start() function which will then
   * start the timer with a timeout as per nanoseconds passed to this function.
   */
  static timer_settings_t calculateTimerSettingsForPeriod_ns(const uint32_t nanoSeconds);

  /**
   * Due to the timer resolution, the real period might differ from the demanded.
   * This function will provide the scheduled period.
   */
  static uint32_t getScheduledTimeoutPeriod_ns(const timer_settings_t timerSettings);

  /**
   * Return the minimum allowed period in nanoseconds.
   */
  static uint32_t minPeriod_ns();
  /**
   * Return the maximum allowed period in nanoseconds.
   */
  static uint32_t maxPeriod_ns();

  static TIMER_INTERRUPT_LEAN_ERROR checkTimeoutPeriod(const timer_settings_t timerSettings);

  /**
   * Start the timer.
   * rptCount is the number of timeouts that shall appear before the timer is stopped automatically.
   *  if this is 0, the timer will never be stopped.
   *
   */
  static TIMER_INTERRUPT_LEAN_ERROR start(const timer_settings_t timerSettings, const uint32_t rptCount);

  /**
   * Stop the timer.
   */
  static inline void stop();

  /**
   * An internal function that needs to be public.
   */
  static void isr();

private:
  // Pointer to the TimerInterruptLean instance for this timerNo
  static TimerInterruptLean<timerNo>* timerInterruptInstance;
  static void initTimerRegister();
};


#include "internal/TimerInterruptLean.inc"


#endif /* TIMERINTLEAN_TIMERINTERRUPTLEAN_H_ */
