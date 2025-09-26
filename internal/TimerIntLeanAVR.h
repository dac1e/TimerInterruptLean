/*
 * TimerIntLeanAVR.h
 *
 *  Created on: 24.09.2025
 *      Author: Wolfgang
 */

#ifndef TIMERINTLEAN_INTERNAL_TIMERINTLEANAVR_H_
#define TIMERINTLEAN_INTERNAL_TIMERINTLEANAVR_H_

#include "Arduino.h"
#undef min
#undef max

#include "log.h"

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
  #define TCCR2A TCCR2
  #define TCCR2B TCCR2
  #define COM2A1 COM21
  #define COM2A0 COM20
  #define OCR2A OCR2
  #define TIMSK2 TIMSK
  #define OCIE2A OCIE2
  #define TIMER2_COMPA_vect TIMER2_COMP_vect
  #define TIMSK1 TIMSK
#endif

namespace TimerIntLean_ {

namespace TimerIntLeanAVR {

enum MICROCONTROLLER_ID {
  ATMEGA, ATMEGA_2560, ATMEGA_32U4, ATMEGA_TINY25
};

static constexpr uint64_t KILO=1000LL;         //  1e3
static constexpr uint64_t MEGA=KILO*KILO;      //  1e6
static constexpr uint64_t GIGA=MEGA*KILO;      //  1e9
static constexpr uint64_t PETA=GIGA*MEGA;      // 1e15


// Generic timer
static constexpr unsigned MAX_PRESCALE_TX = 1024;
static const unsigned PRESCALER_TX[] = {0,1,8,64,256,MAX_PRESCALE_TX};
static constexpr size_t PRESCALER_TX_CNT = sizeof(PRESCALER_TX) / sizeof(PRESCALER_TX[0]);
static constexpr uint64_t MAX_PERIOD_FEMTOSEC_TX = UINT64_MAX / MAX_PRESCALE_TX;
static const uint64_t TIMER_PERIOD_FEMTOSEC_TX[] = {
    PRESCALER_TX[0]*PETA/F_CPU,
    PRESCALER_TX[1]*PETA/F_CPU,
    PRESCALER_TX[2]*PETA/F_CPU,
    PRESCALER_TX[3]*PETA/F_CPU,
    PRESCALER_TX[4]*PETA/F_CPU,
    PRESCALER_TX[5]*PETA/F_CPU,
};

// Timer T2
static constexpr unsigned MAX_PRESCALE_T2 = 1024;
static const unsigned PRESCALER_T2[] = {0,1,8,32,64,256,MAX_PRESCALE_T2};
static constexpr size_t PRESCALER_T2_CNT = sizeof(PRESCALER_T2) / sizeof(PRESCALER_T2[0]);
static constexpr uint64_t MAX_PERIOD_FEMTOSEC_T2 = UINT64_MAX / MAX_PRESCALE_T2;

static const uint64_t TIMER_PERIOD_FEMTOSEC_T2[] = {
    PRESCALER_T2[0]*PETA/F_CPU,
    PRESCALER_T2[1]*PETA/F_CPU,
    PRESCALER_T2[2]*PETA/F_CPU,
    PRESCALER_T2[3]*PETA/F_CPU,
    PRESCALER_T2[4]*PETA/F_CPU,
    PRESCALER_T2[5]*PETA/F_CPU,
    PRESCALER_T2[6]*PETA/F_CPU,
};

enum TIMER_SETTING_ERROR {
  TIME_PERIOD_TOO_SMALL = -2,
  TIME_PERIOD_TOO_BIG   = -1,
};

template<typename COUNTER_T>
inline int32_t mergeCompareAndPrescale(COUNTER_T tc, uint8_t prescaler) {
//  logvarln(tc);
//  logvarln(prescaler);
  // store prescaler in most significant byte and counter in 3 least significant bytes
  const int32_t result = ((static_cast<int32_t>(prescaler) & 0x07) << 24) + static_cast<COUNTER_T>(tc);
//  logvarln(result);
  return result;
}

template<typename COUNTER_T>
inline COUNTER_T dispatchCompare(int32_t timerCounterAndPrescaler) {
//  logvarln(timerCounterAndPrescaler);
  constexpr COUNTER_T mask = ((static_cast<COUNTER_T>(1) << (8 * sizeof(COUNTER_T)))-1);
//  logvarln(mask);
  const COUNTER_T result = timerCounterAndPrescaler & mask;
//  logvarln(result);
  return result;
}

template<typename COUNTER_T>
inline uint8_t dispatchPrescaler(int32_t timerCounterAndPrescaler) {
  const uint8_t result = (timerCounterAndPrescaler >> 24) & 0x07;
//  logvarln(result);
  return result;
}

template<typename COUNTER_T>
static int32_t calculateCompareAndPrescale_(const uint32_t periodNanoSec, const uint64_t* const TIMER_PERIOD_FEMTOSEC, const size_t PRESCALER_COUNT) {
  static constexpr uint64_t maxCounterValue = (1LL << (8*sizeof(COUNTER_T)));
//  logvarln(maxCounterValue);

  const uint64_t periodFemtoSec = periodNanoSec * MEGA;
//  logvarln(periodFemtoSec);
  uint8_t prescaler = 1;
  while(prescaler < PRESCALER_COUNT) {
    const uint64_t tc = periodFemtoSec / TIMER_PERIOD_FEMTOSEC[prescaler];
//    logvarln(prescaler);
//    logvarln(TIMER_PERIOD_FEMTOSEC[prescaler]);
//    logvarln(tc);
    if(not tc) {
      return TIME_PERIOD_TOO_SMALL;
    }

    // Note that tc may be 0 which means 256 respectively 65536 clocks until interrupt is signaled
    if(tc <= maxCounterValue) {
      return mergeCompareAndPrescale(tc, prescaler);
    }

    prescaler++;
  }
  return TIME_PERIOD_TOO_BIG;
}

template<typename COUNTER_T>
uint32_t maxPeriod_ns_(const uint64_t* const TIMER_PERIOD_FEMTOSEC, const size_t PRESCALER_COUNT) {
  const uint64_t periodFemtoSec = (1LL << (8*sizeof(COUNTER_T))) * TIMER_PERIOD_FEMTOSEC[PRESCALER_COUNT-1];
//  logvarln(periodFemtoSec);
  return periodFemtoSec / MEGA;
}

template<typename COUNTER_T>
uint32_t minPeriod_ns_(const uint64_t* const TIMER_PERIOD_FEMTOSEC, const size_t PRESCALER_COUNT) {
  const uint64_t periodFemtoSec = 1LL * TIMER_PERIOD_FEMTOSEC[1];
//  logvarln(periodFemtoSec);
  return periodFemtoSec / MEGA;
}

/**
 * Declare timer spec
 */
template<enum MICROCONTROLLER_ID id, unsigned timerNo> class TimerSpec;


/**
 * 16 bit Timer1 timer spec for any microcontroller
 */
#if defined TCNT1
template<enum MICROCONTROLLER_ID id> struct TimerSpec<id, 1> {
  typedef uint16_t counter_t;
  static uint32_t maxPeriod_ns() {return maxPeriod_ns_<counter_t>(TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);}
  static uint32_t minPeriod_ns() {return minPeriod_ns_<counter_t>(TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);}
  static void clearTCCR() {TCCR1A = 0; TCCR1B = 0;};
  static void setCounter(counter_t v) {
//    logvarln(v);
    TCNT1 = v;
//    logvarln(TCNT1);
  }
  static void setPrescaler(const uint8_t v) {
//    logvarln(v);
//    logvarln(TCCR1B);
    const uint8_t x = (TCCR1B & ~0x07) | v;
//    logvarln(x);
    TCCR1B = x;
//    logvarln(TCCR1B);
  }
  static void setCompareValue(const counter_t v) {
//    logvarln(v);
    OCR1A = v;
//    logvarln(OCR1A);
    }
  static void setClearCounterOnCompareMatch(const uint8_t v) {bitWrite(TCCR1B, WGM12, v);}
#if defined TIMSK1
  static void enableCompareMatchInterrupt(const uint8_t v) {
//    logvarln(v);
    bitWrite(TIMSK1, OCIE1A, v);
//    logvarln(TIMSK1);
  }
#elif defined TIMSK
  static void enableCompareMatchInterrupt(const uint8_t v) {
    logvarln(v);
    bitWrite(TIMSK, OCIE1A, v);
    logvarln(TIMSK);
  }
#endif
  static constexpr uint64_t MAX_PERIOD_FEMTOSEC = MAX_PERIOD_FEMTOSEC_TX;
  static constexpr size_t PRESCALER_COUNT = PRESCALER_TX_CNT;
  static const uint64_t* const TIMER_PERIOD_FEMTOSEC;
  static int32_t calculateCompareAndPrescale(const uint32_t periodNanoSec) {
    return calculateCompareAndPrescale_<counter_t>(periodNanoSec, TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);
  }
};
template<enum MICROCONTROLLER_ID id>
const uint64_t* const TimerSpec<id, 1>::TIMER_PERIOD_FEMTOSEC = TIMER_PERIOD_FEMTOSEC_TX;
#endif // #if defined TIMSK1 || defined TIMSK


/**
 * 8 bit Timer1 timer spec for ATMEGA_TINY25
 */
#if defined TCNT1
template<> struct TimerSpec<ATMEGA_TINY25, 1> {
  typedef uint8_t counter_t;
  static uint32_t maxPeriod_ns() {return maxPeriod_ns_<counter_t>(TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);}
  static uint32_t minPeriod_ns() {return minPeriod_ns_<counter_t>(TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);}
  static void clearTCCR() {TCCR1A = 0; TCCR1B = 0;};
  static void setPrescaler(const uint8_t v) {TCCR1B = (TCCR1B & ~0x07) | v;}
  static void setCounter(counter_t v) {TCNT1 = v;}
  static void setCompareValue(const counter_t v) {OCR1A = v;}
  static void setClearCounterOnCompareMatch(const uint8_t v) {bitWrite(TCCR1A, WGM11, v);}
#if defined TIMSK1
  static void enableCompareMatchInterrupt(const uint8_t v) {bitWrite(TIMSK1, OCIE1A, v);}
#elif defined TIMSK
  static void enableCompareMatchInterrupt(const uint8_t v) {bitWrite(TIMSK, OCIE1A, v);}
#endif
  static constexpr uint64_t MAX_PERIOD_FEMTOSEC = MAX_PERIOD_FEMTOSEC_TX;
  static constexpr size_t PRESCALER_COUNT = PRESCALER_TX_CNT;
  static const uint64_t* const TIMER_PERIOD_FEMTOSEC;
  static int32_t calculateCompareAndPrescale(const uint32_t periodNanoSec) {
    return calculateCompareAndPrescale_<counter_t>(periodNanoSec, TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);
  }
};
template<>
const uint64_t* const TimerSpec<ATMEGA_TINY25, 1>::TIMER_PERIOD_FEMTOSEC = TIMER_PERIOD_FEMTOSEC_TX;
#endif // #if defined TIMSK1 || defined TIMSK

/**
 * 8 bit Timer2 timer spec for any microcontroller
 */
#if defined TCNT2
template<enum MICROCONTROLLER_ID id> struct TimerSpec<id, 2> {
  typedef uint8_t counter_t;
  static uint32_t maxPeriod_ns() {return maxPeriod_ns_<counter_t>(TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);}
  static uint32_t minPeriod_ns() {return minPeriod_ns_<counter_t>(TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);}
  static void clearTCCR() {TCCR2A = 0; TCCR2B = 0;};
  static void setPrescaler(const uint8_t v) {TCCR2B = (TCCR2B & ~0x07) | v;}
  static void setCounter(counter_t v) {TCNT2 = v;}
  static void setCompareValue(const counter_t v) {OCR2A = v;}
  static void setClearCounterOnCompareMatch(const uint8_t v) {bitWrite(TCCR2A, WGM21, v);}
  static void enableCompareMatchInterrupt(const uint8_t v) {bitWrite(TIMSK2, OCIE2A, v);}
  static constexpr uint64_t MAX_PERIOD_FEMTOSEC = MAX_PERIOD_FEMTOSEC_T2;
  static constexpr size_t PRESCALER_COUNT = PRESCALER_T2_CNT;
  static const uint64_t* const TIMER_PERIOD_FEMTOSEC;
  static int32_t calculateCompareAndPrescale(const uint32_t periodNanoSec) {
    return calculateCompareAndPrescale_<counter_t>(periodNanoSec, TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);
  }
};
template<enum MICROCONTROLLER_ID id>
const uint64_t* const TimerSpec<id, 2>::TIMER_PERIOD_FEMTOSEC = TIMER_PERIOD_FEMTOSEC_T2;
#endif // #if defined TIMSK2

/**
 * 16 bit Timer3 timer spec for any microcontroller
 */
#if defined TCNT3
template<enum MICROCONTROLLER_ID id> struct TimerSpec<id, 3> {
  typedef uint16_t counter_t;
  static uint32_t maxPeriod_ns() {return maxPeriod_ns_<counter_t>(TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);}
  static uint32_t minPeriod_ns() {return minPeriod_ns_<counter_t>(TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);}
  static void clearTCCR() {TCCR3A = 0; TCCR3B = 0;};
  static void setCounter(counter_t v) {TCNT3 = v;}
  static void setPrescaler(const uint8_t v) {TCCR3B = (TCCR3B & ~0x07) | v;}
  static void setCompareValue(const counter_t v) {OCR3A = v;}
  static void setClearCounterOnCompareMatch(const uint8_t v) {bitWrite(TCCR3B, WGM32, v);}
  static void enableCompareMatchInterrupt(const uint8_t v) {bitWrite(TIMSK3, OCIE3A, v);}
  static constexpr uint64_t MAX_PERIOD_FEMTOSEC = MAX_PERIOD_FEMTOSEC_TX;
  static constexpr size_t PRESCALER_COUNT = PRESCALER_TX_CNT;
  static const uint64_t* const TIMER_PERIOD_FEMTOSEC;
  static int32_t calculateCompareAndPrescale(const uint32_t periodNanoSec) {
    return calculateCompareAndPrescale_<counter_t>(periodNanoSec, TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);
  }
};
template<enum MICROCONTROLLER_ID id>
const uint64_t* const TimerSpec<id, 3>::TIMER_PERIOD_FEMTOSEC = TIMER_PERIOD_FEMTOSEC_TX;
#endif

/**
 * 16 bit Timer4 timer spec for any microcontroller
 */
#if defined TCNT4
template<enum MICROCONTROLLER_ID id> struct TimerSpec<id, 4> {
  typedef uint16_t counter_t;
  static uint32_t maxPeriod_ns() {return maxPeriod_ns_<counter_t>(TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);}
  static uint32_t minPeriod_ns() {return minPeriod_ns_<counter_t>(TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);}
  static void clearTCCR() {TCCR4A = 0; TCCR4B = 0;};
  static void setCounter(counter_t v) {TCNT4 = v;}
  static void setPrescaler(const uint8_t v) {TCCR4B = (TCCR4B & ~0x07) | v;}
  static void setCompareValue(const counter_t v) {OCR4A = v;}
  static void setClearCounterOnCompareMatch(const uint8_t v) {bitWrite(TCCR4B, WGM42, v);}
  static void enableCompareMatchInterrupt(const uint8_t v) {bitWrite(TIMSK4, OCIE4A, v);}
  static constexpr uint64_t MAX_PERIOD_FEMTOSEC = MAX_PERIOD_FEMTOSEC_TX;
  static constexpr size_t PRESCALER_COUNT = PRESCALER_TX_CNT;
  static const uint64_t* const TIMER_PERIOD_FEMTOSEC;
  static int32_t calculateCompareAndPrescale(const uint32_t periodNanoSec) {
    return calculateCompareAndPrescale_<counter_t>(periodNanoSec, TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);
  }
};
template<enum MICROCONTROLLER_ID id>
const uint64_t* const TimerSpec<id, 4>::TIMER_PERIOD_FEMTOSEC = TIMER_PERIOD_FEMTOSEC_TX;
#endif

/**
 * 16 bit Timer4 timer spec for ATMEGA_32U4
 */
#if defined TCNT4
template<> struct TimerSpec<ATMEGA_32U4, 4> {
  typedef uint16_t counter_t;
  static uint32_t maxPeriod_ns() {return maxPeriod_ns_<counter_t>(TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);}
  static uint32_t minPeriod_ns() {return minPeriod_ns_<counter_t>(TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);}
  static void clearTCCR() {TCCR4A = 0; TCCR4B = 0;};
  static void setCounter(counter_t v) {TCNT4 = v;}
  static void setPrescaler(const uint8_t v) {TCCR4B = (TCCR4B & ~0x07) | v;}
  static void setCompareValue(const counter_t v) {OCR4A = v;}
  static void setClearCounterOnCompareMatch(const uint8_t v) {bitWrite(TCCR4B, CS43, v);} // TODO this may not be correct, atmega32u4
  static void enableCompareMatchInterrupt(const uint8_t v) {bitWrite(TIMSK4, OCIE4A, v);}
  static constexpr uint64_t MAX_PERIOD_FEMTOSEC = MAX_PERIOD_FEMTOSEC_TX;
  static constexpr size_t PRESCALER_COUNT = PRESCALER_TX_CNT;
  static const uint64_t* const TIMER_PERIOD_FEMTOSEC;
  static int32_t calculateCompareAndPrescale(const uint32_t periodNanoSec) {
    return calculateCompareAndPrescale_<counter_t>(periodNanoSec, TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);
  }
};
template<>
const uint64_t* const TimerSpec<ATMEGA_32U4, 4>::TIMER_PERIOD_FEMTOSEC = TIMER_PERIOD_FEMTOSEC_TX;
#endif


/**
 * 16 bit Timer5 timer spec for any microcontroller
 */
#if defined TCNT5
template<enum MICROCONTROLLER_ID id> struct TimerSpec<id, 5> {
  typedef uint16_t counter_t;
  static uint32_t maxPeriod_ns() {return maxPeriod_ns_<counter_t>(TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);}
  static uint32_t minPeriod_ns() {return minPeriod_ns_<counter_t>(TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);}
  static void clearTCCR() {TCCR5A = 0; TCCR5B = 0;};
  static void setCounter(counter_t v) {TCNT5 = v;}
  static void setPrescaler(const uint8_t v) {TCCR5B = (TCCR5B & ~0x07) | v;}
  static void setCompareValue(const counter_t v) {OCR4A = v;}
  static void setClearCounterOnCompareMatch(const uint8_t v) {bitWrite(TCCR5B, WGM52, v);}
  static void enableCompareMatchInterrupt(const uint8_t v) {bitWrite(TIMSK4, OCIE5A, v);}
  static constexpr uint64_t MAX_PERIOD_FEMTOSEC = MAX_PERIOD_FEMTOSEC_TX;
  static constexpr size_t PRESCALER_COUNT = PRESCALER_TX_CNT;
  static const uint64_t* const TIMER_PERIOD_FEMTOSEC;
  static int32_t calculateCompareAndPrescale(const uint32_t periodNanoSec) {
    return calculateCompareAndPrescale_<counter_t>(periodNanoSec, TIMER_PERIOD_FEMTOSEC, PRESCALER_COUNT);
  }
};
template<enum MICROCONTROLLER_ID id>
const uint64_t* const TimerSpec<id, 5>::TIMER_PERIOD_FEMTOSEC = TIMER_PERIOD_FEMTOSEC_TX;
#endif

template<unsigned timerNo> struct IsrHook {
  static uint8_t mInterruptCnt;
//  static uint8_t mIgnoreInterruptCnt;
  static void (*mStopFunction)();
  static void isr() {
//    if(mIgnoreInterruptCnt) {
//      --mIgnoreInterruptCnt;
//    } else {
      TimerInterruptLean<timerNo>::isr();
      if(not --mInterruptCnt) {
        mStopFunction();
      }
//    }
  }
};

template<unsigned timerNo> void (*IsrHook<timerNo>::mStopFunction)() = nullptr;
//template<unsigned timerNo> uint8_t IsrHook<timerNo>::mIgnoreInterruptCnt = 0;
template<unsigned timerNo> uint8_t IsrHook<timerNo>::mInterruptCnt = 1;

template<enum MICROCONTROLLER_ID id, unsigned timerNo> struct Timer {
  typedef typename TimerSpec<id, timerNo>::counter_t counter_t;

  static void init() {
    noInterrupts();
    IsrHook<timerNo>::mStopFunction = stop;
    TimerSpec<id, timerNo>::enableCompareMatchInterrupt(0);
    TimerSpec<id, timerNo>::clearTCCR();
    TimerSpec<id, timerNo>::setPrescaler(dispatchPrescaler<counter_t>(0));
    TimerSpec<id, timerNo>::setClearCounterOnCompareMatch(1);
    interrupts();
  }

  static int32_t getTimerSettingsForPeriod_ns(const uint32_t nanoSeconds) {
    return TimerSpec<id, timerNo>::calculateCompareAndPrescale(nanoSeconds);
  }

  static uint32_t minPeriod_ns() {return TimerSpec<id, timerNo>::minPeriod_ns();}
  static inline bool isPeriodTooSmall(const int32_t timerSettings) {
    return timerSettings == TIME_PERIOD_TOO_SMALL;
  }

  static uint32_t maxPeriod_ns() {return TimerSpec<id, timerNo>::maxPeriod_ns();}
  static inline bool isPeriodTooBig(const int32_t timerSettings) {
    return timerSettings == TIME_PERIOD_TOO_BIG;
  }

  static inline void stop() {
    noInterrupts();
    TimerSpec<id, timerNo>::enableCompareMatchInterrupt(0);
    TimerSpec<id, timerNo>::setPrescaler(0);
    interrupts();
  }

  static void startMultipleShots(const int32_t timerSettings, const size_t shotCount) {
    noInterrupts();
//    IsrHook<timerNo>::mIgnoreInterruptCnt = 0;
    IsrHook<timerNo>::mInterruptCnt = shotCount;

    TimerSpec<id, timerNo>::setCounter(0);
    TimerSpec<id, timerNo>::setCompareValue(dispatchCompare<counter_t>(timerSettings));
    TimerSpec<id, timerNo>::setPrescaler(dispatchPrescaler<counter_t>(timerSettings));
    TimerSpec<id, timerNo>::enableCompareMatchInterrupt(1);
    interrupts();
  }

  static void startPeriodic(const int32_t timerSettings) {
  }
};

} // namespace TimerIntLeanAVR

} // namespace TimerIntLean_

#endif /* TIMERINTLEAN_INTERNAL_TIMERINTLEANAVR_H_ */
