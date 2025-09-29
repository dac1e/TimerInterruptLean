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


#pragma once

#ifndef TIMERINTLEAN_INTERNAL_TIMERINTLEANPLATFORM_H_
#define TIMERINTLEAN_INTERNAL_TIMERINTLEANPLATFORM_H_

namespace TimerIntLean_ {

enum PLATFORM_ID {PLID_AVR, PLID_SAM};
template<unsigned timerNo, enum PLATFORM_ID> class Timer;

}

#endif /* TIMERINTLEAN_INTERNAL_TIMERINTLEANPLATFORM_H_ */
