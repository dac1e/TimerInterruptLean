// Do not remove the include below
#include <assert.h>

#include "TimerIntSandbox.h"
#include "TimerInterruptLean.h"


volatile static uint32_t timeStamp = 0;
volatile static size_t   counter   = 0;

static constexpr size_t TIMER_NO = 1;

class MyTimerInterupt : public TimerInterruptLean<TIMER_NO> {
  void onTimeout() override {
    const uint32_t t =  micros();
    const uint32_t delta = t - timeStamp;
    timeStamp = t;

    Serial.print("timeout #");
    Serial.print(++counter);
    Serial.print(" after ");
    Serial.print(delta / 1000);
    Serial.print('.');
    Serial.print(delta % 1000);
    Serial.println("ms");
  }
};

uint32_t timerSettingsShortPeriod=0;
uint32_t timerSettingsLongPeriod=0;

constexpr unsigned shortPeriodTimeoutDemand_us = 130; //  130 microseconds
constexpr unsigned longPeriodTimeoutDemand_ms = 500; // 500 milliseconds

MyTimerInterupt timer1;

void printTimeoutErr(const TIMER_INTERRUPT_LEAN_ERROR err) {
  if(err == TIMER_INTERRUPT_LEAN_ERROR::PERIOD_TOO_SMALL) {
    Serial.println("period is too small");
  } else
  if(err == TIMER_INTERRUPT_LEAN_ERROR::PERIOD_TOO_LARGE) {
    Serial.println("period too large");
  } else if (err == TIMER_INTERRUPT_LEAN_ERROR::OK) {
    Serial.println("period is within is possible range");
  }
}

void printShortPeriodTimeout()
{
  const uint32_t calculatedtimeout = timer1.timerSettingsToTimeout_ns(timerSettingsShortPeriod);
  Serial.print("Calculated timeout for ");
  Serial.print(shortPeriodTimeoutDemand_us);
  Serial.print("us demand=");
  Serial.print(calculatedtimeout);
  Serial.println("ns");
}

void printLongPeriodTimeout()
{
  const uint32_t calculatedtimeout = timer1.timerSettingsToTimeout_ns(timerSettingsLongPeriod);
  Serial.print("Calculated timeout for ");
  Serial.print(longPeriodTimeoutDemand_ms);
  Serial.print("ms demand= ");
  Serial.print(calculatedtimeout);
  Serial.println("ns");
}

//The setup function is called once at startup of the sketch
void setup()
{
  Serial.begin(9600);

  timer1.begin();

  Serial.print("timer1.minPeriod_ns: ");
  Serial.println(timer1.minPeriod_ns());
  Serial.print("timer1.maxPeriod_ns: ");
  Serial.println(timer1.maxPeriod_ns());

  /**
   * Pre-calculation and storing timer settings is not absolutely required, but it saves processor time.
   * Alternatively the the timer settings could be calculated just before the timer is started.
   */
  timerSettingsShortPeriod = timer1.calculateTimerSettingsForPeriod_us(shortPeriodTimeoutDemand_us);
//  timerSettingsLongPeriod  = timer1.calculateTimerSettingsForPeriod_ms(longPeriodTimeoutDemand_ms);

  /**
   * The following check is not required if you know that the period is valid or the selected timer number.
   */
  const TIMER_INTERRUPT_LEAN_ERROR errShortPeriod = timer1.checkTimeoutPeriod(timerSettingsLongPeriod);
  printTimeoutErr(errShortPeriod);

  const TIMER_INTERRUPT_LEAN_ERROR errLongPeriod = timer1.checkTimeoutPeriod(timerSettingsLongPeriod);
  printTimeoutErr(errLongPeriod);

  assert(errShortPeriod == TIMER_INTERRUPT_LEAN_ERROR::OK && errLongPeriod == TIMER_INTERRUPT_LEAN_ERROR::OK);

  delay(100); // Give enough time for finalizing print outs.
  /**
   * End of check
   */

  printShortPeriodTimeout();
  printLongPeriodTimeout();
  delay(100); // Give enough time for finalizing print outs.

//  // 1 x timeout with long period
//  Serial.println("start 1 x long period");
//  delay(100);
//  timeStamp = micros();
//  timer1.start(timerSettingsLongPeriod, 1);
//  delay(1000); // wait 1 second


  // 3 x timeouts with short 16ms period
  Serial.println("start 3 x short periods");
  delay(100);
  timeStamp = micros();
  timer1.start(timerSettingsShortPeriod, 3);
  delay(3000); // wait 3 seconds

//  // Infinite timeouts with long period
//  Serial.println("start inf x long periods");
//  delay(100);
//  timeStamp = micros();
//  timer1.start(timerSettingsLongPeriod, 0 /* 0=infinite */);
}

// The loop function is called in an endless loop
void loop()
{
}
