// Do not remove the include below
#include <assert.h>

#include "TimerIntSandbox.h"
#include "TimerInterruptLean.h"

// A convenient alias for the timer settings type
typedef TimerInterruptLean_timer_settings_t ts_t;

#define ENABLE_INFINITE_EXAMPLE true

volatile static unsigned long timeStamp = 0;
volatile static size_t   counter   = 0;
static constexpr size_t TIMER_NO = 1;

ts_t timerSettingsShortPeriod=0;
ts_t timerSettingsLongPeriod=0;

constexpr uint32_t shortPeriodTimeoutDemand_ns =  32768000L; //  32.768 milliseconds ->  32768000 nanoseconds
constexpr uint32_t longPeriodTimeoutDemand_ns  = 512345000L; // 512.345 milliseconds -> 512345000 nanoseconds


static void printPeriodErr(const uint32_t timeoutDemand_us, const TIMER_INTERRUPT_LEAN_ERROR err, const uint32_t minPeriod_ns, const uint32_t maxPeriod_ns );
static void printShortPeriodScheduledPeriod(const ts_t timerSettings);
static void printLongPeriodScheduledPeriod(const ts_t timerSettings);


/* Derive a class that will receive the timer timeout */
class MyTimerInterupt : public TimerInterruptLean<TIMER_NO> {
  void onTimeout() override {
    const unsigned long t = micros();
    const unsigned long delta =t - timeStamp;
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

static MyTimerInterupt myTimer;

//The setup function is called once at startup of the sketch
void setup()
{
  Serial.begin(9600);

  myTimer.begin();

  /**
   * Pre-calculation and storing timer settings is not absolutely required, but it saves processor time.
   * Alternatively the the timer settings could be calculated just before the timer is started.
   */
  const uint32_t minPeriod_ns = myTimer.minPeriod_ns();
  const uint32_t maxPeriod_ns = myTimer.maxPeriod_ns();

  timerSettingsShortPeriod = myTimer.calculateTimerSettingsForPeriod_ns(shortPeriodTimeoutDemand_ns);

  { // The following check is not required if you know that the period is valid for the selected timer.
    const TIMER_INTERRUPT_LEAN_ERROR errShortPeriod = myTimer.checkTimeoutPeriod(timerSettingsLongPeriod);
    printPeriodErr(shortPeriodTimeoutDemand_ns, errShortPeriod, minPeriod_ns, maxPeriod_ns);
    // Stop on error
    assert(errShortPeriod == TIMER_INTERRUPT_LEAN_ERROR::OK);
  }

  timerSettingsLongPeriod  = myTimer.calculateTimerSettingsForPeriod_ns(longPeriodTimeoutDemand_ns);
  { // The following check is not required if you know that the period is valid or the selected timer.
    const TIMER_INTERRUPT_LEAN_ERROR errLongPeriod = myTimer.checkTimeoutPeriod(timerSettingsLongPeriod);
    printPeriodErr(longPeriodTimeoutDemand_ns, errLongPeriod, minPeriod_ns, maxPeriod_ns);
    // Stop on error
    assert(errLongPeriod == TIMER_INTERRUPT_LEAN_ERROR::OK);
  }

  delay(100); // Give enough time for finalizing print outs.
  /**
   * End of check
   */

  // Due to the resolution of the timer, the scheduled period may differ from the demanded period.
  // We print the scheduled vs. the demanded period here:
  printShortPeriodScheduledPeriod(timerSettingsShortPeriod);
  printLongPeriodScheduledPeriod(timerSettingsLongPeriod);

  delay(100); // Give enough time for finalizing print outs.

  // 2 x timeout with long period
  Serial.println("\r\nStart 2 x long period");
  delay(100);
  timeStamp = micros();
  myTimer.start(timerSettingsLongPeriod, 2);
  delay(2000); // wait 2 seconds


  // 3 x timeouts with short 16ms period
  Serial.println("\r\nStart 3 x short periods");
  delay(100);
  timeStamp = micros();
  myTimer.start(timerSettingsShortPeriod, 3);
  delay(3000); // wait 3 seconds

#if ENABLE_INFINITE
  // Infinite timeouts with long period
  Serial.println("\r\nStart inf x long periods");
  delay(100);
  timeStamp = micros();
  myTimer.start(timerSettingsLongPeriod, 0 /* 0=infinite */);
#endif
}

// The loop function is called in an endless loop
void loop()
{
}


void printPeriodErr(const uint32_t timeoutDemand_ns, const TIMER_INTERRUPT_LEAN_ERROR err, const uint32_t minPeriod_ns, const uint32_t maxPeriod_ns) {
  Serial.print("Checking ");
  Serial.print(timeoutDemand_ns);
  Serial.print("ns timeout period..");
  if(err == TIMER_INTERRUPT_LEAN_ERROR::PERIOD_TOO_SMALL) {
    Serial.print("\t..error, period is beyond range ");
  } else
  if(err == TIMER_INTERRUPT_LEAN_ERROR::PERIOD_TOO_LARGE) {
    Serial.print("\t..error, period is beyond range ");
  } else if (err == TIMER_INTERRUPT_LEAN_ERROR::OK) {
    Serial.print("\t..success, period is within valid range ");
  }
  Serial.print("[");
  Serial.print(minPeriod_ns);
  Serial.print("ns ..");
  Serial.print(maxPeriod_ns);
  Serial.println("ns]");
}

void printShortPeriodScheduledPeriod(const ts_t timerSettings)
{
  const uint32_t scheduledTimeout = myTimer.getScheduledTimeoutPeriod_ns(timerSettings);
  Serial.print("Info: Scheduled period for ");
  Serial.print(shortPeriodTimeoutDemand_ns);
  Serial.print("ns is ");
  Serial.print(scheduledTimeout);
  Serial.print("ns, deviation is ");

  const uint32_t delta = shortPeriodTimeoutDemand_ns - scheduledTimeout;
  Serial.print(delta);
  Serial.println("ns");
}

void printLongPeriodScheduledPeriod(const ts_t timerSettings)
{
  const uint32_t scheduledTimeout = myTimer.getScheduledTimeoutPeriod_ns(timerSettings);
  Serial.print("Info: Scheduled period for ");
  Serial.print(longPeriodTimeoutDemand_ns);
  Serial.print("ns is ");
  Serial.print(scheduledTimeout);
  Serial.print("ns, deviation is ");

  const int32_t delta =
      static_cast<int64_t>(longPeriodTimeoutDemand_ns) - static_cast<int64_t>(scheduledTimeout);
  Serial.print(delta);
  Serial.println("ns");
}

