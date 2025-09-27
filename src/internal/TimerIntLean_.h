/*
 * TimerIntLeanPlatform.h
 *
 *  Created on: 24.09.2025
 *      Author: Wolfgang
 */

#ifndef TIMERINTLEAN_INTERNAL_TIMERINTLEANPLATFORM_H_
#define TIMERINTLEAN_INTERNAL_TIMERINTLEANPLATFORM_H_

namespace TimerIntLean_ {

enum PLATFORM_ID {PLID_AVR, PLID_SAM};
template<unsigned timerNo, enum PLATFORM_ID> class Timer;

}

#endif /* TIMERINTLEAN_INTERNAL_TIMERINTLEANPLATFORM_H_ */
