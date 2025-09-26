/*
 * TimerIntLeanSAM.cpp
 *
 *  Created on: 24.09.2025
 *      Author: Wolfgang
 */


#include "../TimerInterruptLean.h"
#include "TimerIntLeanAVR.h"

#include "log.h"

#if not defined DISABLE_TIMER1_INT_LEAN
#define TIMER1_INT_LEAN_ISR_INSTATIATE() template class TimerInterruptLean<1>; ISR(TIMER1_COMPA_vect) {TimerIntLean_::TimerIntLeanAVR::IsrHook<1>::isr();}
#else
#define TIMER1_INT_LEAN_ISR_INSTATIATE()
#endif

#if not defined DISABLE_TIMER2_INT_LEAN
#define TIMER2_INT_LEAN_ISR_INSTATIATE() template class TimerInterruptLean<2>; ISR(TIMER2_COMPA_vect) {TimerIntLean_::TimerIntLeanAVR::IsrHook<2>::isr();}
#else
#define TIMER2_INT_LEAN_ISR_INSTATIATE()
#endif

#if not defined DISABLE_TIMER3_INT_LEAN
#define TIMER3_INT_LEAN_ISR_INSTATIATE() template class TimerInterruptLean<3>; ISR(TIMER3_COMPA_vect) {TimerIntLean_::TimerIntLeanAVR::IsrHook<3>::isr();}
#else
#define TIMER3_INT_LEAN_ISR_INSTATIATE()
#endif

#if not defined DISABLE_TIMER4_INT_LEAN
#define TIMER4_INT_LEAN_ISR_INSTATIATE() template class TimerInterruptLean<4>; ISR(TIMER4_COMPA_vect) {TimerIntLean_::TimerIntLeanAVR::IsrHook<4>::isr();}
#else
#define TIMER4_INT_LEAN_ISR_INSTATIATE()
#endif

#if not defined DISABLE_TIMER5_INT_LEAN
#define TIMER5_INT_LEAN_ISR_INSTATIATE() template class TimerInterruptLean<5>; ISR(TIMER5_COMPA_vect) {TimerIntLean_::TimerIntLeanAVR::IsrHook<5>::isr();}
#else
#define TIMER5_INT_LEAN_ISR_INSTATIATE()
#endif

namespace TimerIntLean_ {

#if ( defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || \
      defined(__AVR_ATmega640__) || defined(__AVR_ATmega641__))

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA_2560;
  template class TimerInterruptLean<1>;
  template class TimerInterruptLean<2>;
  template class TimerInterruptLean<3>;
  template class TimerInterruptLean<4>;
  template class TimerInterruptLean<5>;

  TIMER1_INT_LEAN_ISR_INSTATIATE()
  TIMER2_INT_LEAN_ISR_INSTATIATE()
  TIMER3_INT_LEAN_ISR_INSTATIATE()
  TIMER4_INT_LEAN_ISR_INSTATIATE()
  TIMER5_INT_LEAN_ISR_INSTATIATE()

#elif ( defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)  || \
        defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) ||    defined(ARDUINO_AVR_ETHERNET) || \
        defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT)   || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO)      || \
        defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_AVR_DUEMILANOVE) )

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA;
  TIMER1_INT_LEAN_ISR_INSTATIATE()
  TIMER2_INT_LEAN_ISR_INSTATIATE()

#elif ( defined(ARDUINO_AVR_FEATHER328P) || defined(ARDUINO_AVR_METRO) || defined(ARDUINO_AVR_PROTRINKET5) || defined(ARDUINO_AVR_PROTRINKET3) || \
      defined(ARDUINO_AVR_PROTRINKET5FTDI) || defined(ARDUINO_AVR_PROTRINKET3FTDI) )

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA;
  TIMER1_INT_LEAN_ISR_INSTATIATE()
  TIMER2_INT_LEAN_ISR_INSTATIATE()

#elif ( defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_LEONARDO_ETH) || defined(ARDUINO_AVR_YUN) || defined(ARDUINO_AVR_MICRO) || \
        defined(ARDUINO_AVR_ESPLORA)  || defined(ARDUINO_AVR_LILYPAD_USB)  || defined(ARDUINO_AVR_ROBOT_CONTROL) || defined(ARDUINO_AVR_ROBOT_MOTOR) || \
        defined(ARDUINO_AVR_CIRCUITPLAY)  || defined(ARDUINO_AVR_YUNMINI) || defined(ARDUINO_AVR_INDUSTRIAL101) || defined(ARDUINO_AVR_LININO_ONE) )

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA_32U4;
  TIMER1_INT_LEAN_ISR_INSTATIATE()
  TIMER3_INT_LEAN_ISR_INSTATIATE()
  TIMER4_INT_LEAN_ISR_INSTATIATE()

#elif ( defined(ARDUINO_AVR_FLORA8 ) || defined(ARDUINO_AVR_FEATHER32U4) || defined(ARDUINO_AVR_CIRCUITPLAY) || defined(ARDUINO_AVR_ITSYBITSY32U4_5V) || \
        defined(ARDUINO_AVR_ITSYBITSY32U4_3V)  || defined(ARDUINO_AVR_BLUEFRUITMICRO) || defined(ARDUINO_AVR_ADAFRUIT32U4) )

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA_32U4;
  TIMER1_INT_LEAN_ISR_INSTATIATE()
  TIMER3_INT_LEAN_ISR_INSTATIATE()
  TIMER4_INT_LEAN_ISR_INSTATIATE()

#elif ( defined(__AVR_ATmega32U4__) || defined(ARDUINO_AVR_MAKEYMAKEY ) || defined(ARDUINO_AVR_PROMICRO) || defined(ARDUINO_AVR_FIOV3) || \
        defined(ARDUINO_AVR_QDUINOMINI) || defined(ARDUINO_AVR_LILYPAD_ARDUINO_USB_PLUS_BOARD ) ) && !defined(TEENSYDUINO)

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA_32U4;
  TIMER1_INT_LEAN_ISR_INSTATIATE()
  TIMER3_INT_LEAN_ISR_INSTATIATE()
  TIMER4_INT_LEAN_ISR_INSTATIATE()

#elif ( defined(__AVR_ATmega328P__) || defined(ARDUINO_AVR_DIGITAL_SANDBOX ) || defined(ARDUINO_REDBOT) || defined(ARDUINO_AVR_SERIAL_7_SEGMENT) )

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA;
  TIMER1_INT_LEAN_ISR_INSTATIATE()
  TIMER2_INT_LEAN_ISR_INSTATIATE()

#elif ( defined(__AVR_ATmega128RFA1__) || defined(ARDUINO_ATMEGA128RFA1_DEV_BOARD) )

  constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA;
  TIMER1_INT_LEAN_ISR_INSTATIATE()
  TIMER2_INT_LEAN_ISR_INSTATIATE()

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

template<unsigned timerNo> int32_t TimerInterruptLean<timerNo>::getTimerSettingsForPeriod_ns(const uint32_t nanoSeconds) {
  const int32_t result = TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::getTimerSettingsForPeriod_ns(nanoSeconds);
  logvarln(result);
  return result;
}

template<unsigned timerNo> uint32_t TimerInterruptLean<timerNo>::minPeriod_ns() {
  const uint32_t result = TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::minPeriod_ns();
  logvarln(result);
  return result;
}

template<unsigned timerNo> inline bool TimerInterruptLean<timerNo>::isPeriodTooSmall(const int32_t timerSettings) {
  const bool result = TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::isPeriodTooSmall(timerSettings);
  logvarln(result);
  return result;
}

template<unsigned timerNo> uint32_t TimerInterruptLean<timerNo>::maxPeriod_ns() {
  const uint32_t result = TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::maxPeriod_ns();
  logvarln(result);
  return result;
}

template<unsigned timerNo> inline bool TimerInterruptLean<timerNo>::isPeriodTooBig(const int32_t timerSettings) {
  const bool result = TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::isPeriodTooBig(timerSettings);
  logvarln(result);
  return result;
}

template<unsigned timerNo> void TimerInterruptLean<timerNo>::stop() {
  TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::stop();
}

template<unsigned timerNo> void TimerInterruptLean<timerNo>::startMultipleShots(const int32_t timerSettings, const size_t shotCount) {
  TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::startMultipleShots(timerSettings, shotCount);
}

template<unsigned timerNo> void TimerInterruptLean<timerNo>::startPeriodic(const int32_t timerSettings) {
  TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::startPeriodic(timerSettings);
}
