/*
 * TimerIntLeanSAM.cpp
 *
 *  Created on: 24.09.2025
 *      Author: Wolfgang
 */


#include "../TimerInterruptLean.h"
#include "TimerIntLeanAVR.h"

namespace TimerIntLean_ {

#if ( defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || \
      defined(__AVR_ATmega640__) || defined(__AVR_ATmega641__))

constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA_2560;
template class TimerInterruptLean<1>;
template class TimerInterruptLean<2>;
template class TimerInterruptLean<3>;
template class TimerInterruptLean<4>;
template class TimerInterruptLean<5>;

#elif ( defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)  || \
        defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) ||    defined(ARDUINO_AVR_ETHERNET) || \
        defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT)   || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO)      || \
        defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_AVR_DUEMILANOVE) )

constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA;
template class TimerInterruptLean<1>;
template class TimerInterruptLean<2>;

#elif ( defined(ARDUINO_AVR_FEATHER328P) || defined(ARDUINO_AVR_METRO) || defined(ARDUINO_AVR_PROTRINKET5) || defined(ARDUINO_AVR_PROTRINKET3) || \
      defined(ARDUINO_AVR_PROTRINKET5FTDI) || defined(ARDUINO_AVR_PROTRINKET3FTDI) )

constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA;

#elif ( defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_LEONARDO_ETH) || defined(ARDUINO_AVR_YUN) || defined(ARDUINO_AVR_MICRO) || \
        defined(ARDUINO_AVR_ESPLORA)  || defined(ARDUINO_AVR_LILYPAD_USB)  || defined(ARDUINO_AVR_ROBOT_CONTROL) || defined(ARDUINO_AVR_ROBOT_MOTOR) || \
        defined(ARDUINO_AVR_CIRCUITPLAY)  || defined(ARDUINO_AVR_YUNMINI) || defined(ARDUINO_AVR_INDUSTRIAL101) || defined(ARDUINO_AVR_LININO_ONE) )

constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA_32U4;
template class TimerInterruptLean<1>;
template class TimerInterruptLean<3>;
template class TimerInterruptLean<4>;

#elif ( defined(ARDUINO_AVR_FLORA8 ) || defined(ARDUINO_AVR_FEATHER32U4) || defined(ARDUINO_AVR_CIRCUITPLAY) || defined(ARDUINO_AVR_ITSYBITSY32U4_5V) || \
        defined(ARDUINO_AVR_ITSYBITSY32U4_3V)  || defined(ARDUINO_AVR_BLUEFRUITMICRO) || defined(ARDUINO_AVR_ADAFRUIT32U4) )

constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA_32U4;
template class TimerInterruptLean<1>;
template class TimerInterruptLean<3>;
template class TimerInterruptLean<4>;

#elif ( defined(__AVR_ATmega32U4__) || defined(ARDUINO_AVR_MAKEYMAKEY ) || defined(ARDUINO_AVR_PROMICRO) || defined(ARDUINO_AVR_FIOV3) || \
        defined(ARDUINO_AVR_QDUINOMINI) || defined(ARDUINO_AVR_LILYPAD_ARDUINO_USB_PLUS_BOARD ) ) && !defined(TEENSYDUINO)

constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA_32U4;
template class TimerInterruptLean<1>;
template class TimerInterruptLean<3>;
template class TimerInterruptLean<4>;

#elif ( defined(__AVR_ATmega328P__) || defined(ARDUINO_AVR_DIGITAL_SANDBOX ) || defined(ARDUINO_REDBOT) || defined(ARDUINO_AVR_SERIAL_7_SEGMENT) )

constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA;
template class TimerInterruptLean<1>;
template class TimerInterruptLean<2>;

#elif ( defined(__AVR_ATmega128RFA1__) || defined(ARDUINO_ATMEGA128RFA1_DEV_BOARD) )

constexpr TimerIntLeanAVR::MICROCONTROLLER_ID MCU = TimerIntLeanAVR::ATMEGA;
template class TimerInterruptLean<1>;
template class TimerInterruptLean<2>;

#else
static_assert(false, "Error, this library does not support the selected board.");
#endif

} // namespace TimerIntLean_

//static constexpr unsigned minTimerNo = TimerIntLean_::TimerIntLeanAVR::minTimerNo();
//static constexpr unsigned maxTimerNo = TimerIntLean_::TimerIntLeanAVR::minTimerNo();

template<unsigned timerNo> int32_t getTimerSettingsForPeriod_ns(const uint32_t nanoSeconds) {
  return TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::getTimerSettingsForPeriod_ns(nanoSeconds);
}

template<unsigned timerNo> uint32_t minPeriod_ns() {
  return TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::minPeriod_ns();
}

template<unsigned timerNo> inline bool isPeriodTooSmall(const int32_t timerSettings) {
  return TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::isPeriodTooSmall(timerSettings);
}

template<unsigned timerNo> uint32_t maxPeriod_ns() {
  return TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::maxPeriod_ns();
}

template<unsigned timerNo> inline bool isPeriodTooBig(const int32_t timerSettings) {
  return TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::isPeriodTooBig(timerSettings);
}

template<unsigned timerNo> void TimerInterruptLean<timerNo>::stop() {
  TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::stop();
}

template<unsigned timerNo> void doOneShot(const int32_t timerSettings) {
  TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::doOneShot(timerSettings);
}

template<unsigned timerNo> void startPeriodic(const int32_t timerSettings) {
  TimerIntLean_::TimerIntLeanAVR::Timer<TimerIntLean_::MCU, timerNo>::startPeriodic(timerSettings);
}
