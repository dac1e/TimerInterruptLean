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


#if defined ARDUINO_ARCH_AVR

#include "TimerIntLeanAVR.h"

namespace TimerIntLean_ {

#if ( defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || \
      defined(__AVR_ATmega640__) || defined(__AVR_ATmega641__))

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA_2560;

#elif ( defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)  || \
        defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) ||    defined(ARDUINO_AVR_ETHERNET) || \
        defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT)   || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO)      || \
        defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_AVR_DUEMILANOVE) )

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA;

#elif ( defined(ARDUINO_AVR_FEATHER328P) || defined(ARDUINO_AVR_METRO) || defined(ARDUINO_AVR_PROTRINKET5) || defined(ARDUINO_AVR_PROTRINKET3) || \
      defined(ARDUINO_AVR_PROTRINKET5FTDI) || defined(ARDUINO_AVR_PROTRINKET3FTDI) )

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA;
  TIMER1_INT_LEAN_ISR_INSTATIATE()
  TIMER2_INT_LEAN_ISR_INSTATIATE()

#elif ( defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_LEONARDO_ETH) || defined(ARDUINO_AVR_YUN) || defined(ARDUINO_AVR_MICRO) || \
        defined(ARDUINO_AVR_ESPLORA)  || defined(ARDUINO_AVR_LILYPAD_USB)  || defined(ARDUINO_AVR_ROBOT_CONTROL) || defined(ARDUINO_AVR_ROBOT_MOTOR) || \
        defined(ARDUINO_AVR_CIRCUITPLAY)  || defined(ARDUINO_AVR_YUNMINI) || defined(ARDUINO_AVR_INDUSTRIAL101) || defined(ARDUINO_AVR_LININO_ONE) )

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA_32U4;

#elif ( defined(ARDUINO_AVR_FLORA8 ) || defined(ARDUINO_AVR_FEATHER32U4) || defined(ARDUINO_AVR_CIRCUITPLAY) || defined(ARDUINO_AVR_ITSYBITSY32U4_5V) || \
        defined(ARDUINO_AVR_ITSYBITSY32U4_3V)  || defined(ARDUINO_AVR_BLUEFRUITMICRO) || defined(ARDUINO_AVR_ADAFRUIT32U4) )

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA_32U4;
  TIMER1_INT_LEAN_ISR_INSTATIATE()
  TIMER3_INT_LEAN_ISR_INSTATIATE()
  TIMER4_INT_LEAN_ISR_INSTATIATE()

#elif ( defined(__AVR_ATmega32U4__) || defined(ARDUINO_AVR_MAKEYMAKEY ) || defined(ARDUINO_AVR_PROMICRO) || defined(ARDUINO_AVR_FIOV3) || \
        defined(ARDUINO_AVR_QDUINOMINI) || defined(ARDUINO_AVR_LILYPAD_ARDUINO_USB_PLUS_BOARD ) ) && !defined(TEENSYDUINO)

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA_32U4;

#elif ( defined(__AVR_ATmega328P__) || defined(ARDUINO_AVR_DIGITAL_SANDBOX ) || defined(ARDUINO_REDBOT) || defined(ARDUINO_AVR_SERIAL_7_SEGMENT) )

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA;

#elif ( defined(__AVR_ATmega128RFA1__) || defined(ARDUINO_ATMEGA128RFA1_DEV_BOARD) )

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA;

#else
static_assert(false, "Error, this library does not support the selected board.");
#endif

} // namespace TimerIntLean_

/*
 * AVR implementation of platform dependent TimerInterrupt API.
 */

template<unsigned timerNo> void TimerInterruptLean<timerNo>::initTimerRegister() {
  TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::init();
}

template<unsigned timerNo> typename TimerInterruptLean<timerNo>::timer_settings_t TimerInterruptLean<timerNo>::calculateTimerSettingsForPeriod_ns(const uint32_t nanoSeconds) {
  const int32_t  result = TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::calculateTimerSettingsForPeriod_ns(nanoSeconds);
  return result;
}

template<unsigned timerNo> uint32_t TimerInterruptLean<timerNo>::getScheduledTimeoutPeriod_ns(const timer_settings_t timerSettings) {
  const uint32_t result = TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::getScheduledTimeoutPeriod_ns( timerSettings);
  return result;
}

template<unsigned timerNo> uint32_t TimerInterruptLean<timerNo>::minPeriod_ns() {
  const uint32_t result = TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::minPeriod_ns();
  return result;
}

template<unsigned timerNo> uint32_t TimerInterruptLean<timerNo>::maxPeriod_ns() {
  const uint32_t result = TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::maxPeriod_ns();
  return result;
}

template<unsigned timerNo> TIMER_INTERRUPT_LEAN_ERROR TimerInterruptLean<timerNo>::checkTimeoutPeriod(const timer_settings_t timerSettings) {
  logvarln(timerSettings);
  if(TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::isPeriodTooSmall(timerSettings)) {
    return TIMER_INTERRUPT_LEAN_ERROR::PERIOD_TOO_SMALL;
  }

  if(TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::isPeriodTooLarge(timerSettings)) {
    return TIMER_INTERRUPT_LEAN_ERROR::PERIOD_TOO_LARGE;
  }
  return TIMER_INTERRUPT_LEAN_ERROR::OK;
}

template<unsigned timerNo> void TimerInterruptLean<timerNo>::stop() {
  TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::stop();
}

template<unsigned timerNo> TIMER_INTERRUPT_LEAN_ERROR TimerInterruptLean<timerNo>::start(const timer_settings_t timerSettings, const uint32_t rptCount) {
  if(TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::isPeriodTooSmall(timerSettings)) {
    return TIMER_INTERRUPT_LEAN_ERROR::PERIOD_TOO_SMALL;
  }

  if(TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::isPeriodTooLarge(timerSettings)) {
    return TIMER_INTERRUPT_LEAN_ERROR::PERIOD_TOO_LARGE;
  }

  TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::start(timerSettings, rptCount);
  return TIMER_INTERRUPT_LEAN_ERROR::OK;
}

#endif // #if defined ARDUINO_ARCH_AVR
