// Do not remove the include below

#include "TimerIntSandbox.h"
#include "TimerInterruptLean.h"

// A convenient alias for the timer settings type
typedef TimerInterruptLean_timer_settings_t ts_t;

#define ENABLE_INFINITE_EXAMPLE true

volatile static unsigned long timeStamp = 0;
volatile static size_t   counter   = 0;

// Use timer number 1 for this example.
static constexpr size_t TIMER_NO = 1;

// For convenience:
// 1 millisecond = 1000 microseconds = 1000000 nanoseconds
// 1 microsecond = 1000 nanoseconds
constexpr uint32_t shortPeriodTimeoutDemand_ns =  16384000L; //  16.2384 milliseconds ->  16384000 nanoseconds
constexpr uint32_t longPeriodTimeoutDemand_ns  = 512345000L; // 512.3450 milliseconds -> 512345000 nanoseconds


static void printPeriodErr(const uint32_t timeoutDemand_us, const TIMER_INTERRUPT_LEAN_ERROR err, const uint32_t minPeriod_ns, const uint32_t maxPeriod_ns );
static void printScheduledPeriod(const ts_t timerSettings, const uint32_t periodTimeoutDemand_ns);


ts_t timerSettingsShortPeriod=-1;
ts_t timerSettingsLongPeriod=-1;

/* Derive a class that will receive the timer timeout */
class MyTimerInterupt : public TimerInterruptLean<TIMER_NO> {

  void onTimeout() override {
    const unsigned long t = micros();
    const unsigned long delta =t - timeStamp;
    timeStamp = t;

    // Note: This function is executed within interrupt service context.
    // Hence, if we are doing to many Serial.print() calls here, the
    // Serial send buffer may overflow which could result in a lock up.
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

  delay(100); // after download we give time for the IDE serial monitor to recover.

  /**
   * Pre-calculation and storing timer settings is not absolutely required, but it saves processor time.
   * Alternatively the the timer settings could be calculated just before the timer is started.
   */
  timerSettingsShortPeriod = myTimer.calculateTimerSettingsForPeriod_ns(shortPeriodTimeoutDemand_ns);
  timerSettingsLongPeriod  = myTimer.calculateTimerSettingsForPeriod_ns(longPeriodTimeoutDemand_ns);


  /**
   * Print out some details about the timer.
   */
  // Get the possible period range.
  const uint32_t minPeriod_ns = myTimer.minPeriod_ns();
  const uint32_t maxPeriod_ns = myTimer.maxPeriod_ns();

  bool shortPeriodOk = true;

  { // The following check is not required if you know that the period is valid for the selected timer.
    const TIMER_INTERRUPT_LEAN_ERROR errShortPeriod = myTimer.checkTimeoutPeriod(timerSettingsShortPeriod);
    printPeriodErr(shortPeriodTimeoutDemand_ns, errShortPeriod, minPeriod_ns, maxPeriod_ns);
    // Stop on error
    shortPeriodOk = (errShortPeriod == TIMER_INTERRUPT_LEAN_ERROR::OK);
  }

  bool longPeriodOk = true;


  { // The following check is not required if you know that the period is valid or the selected timer.
    const TIMER_INTERRUPT_LEAN_ERROR errLongPeriod = myTimer.checkTimeoutPeriod(timerSettingsLongPeriod);
    printPeriodErr(longPeriodTimeoutDemand_ns, errLongPeriod, minPeriod_ns, maxPeriod_ns);
    // Stop on error
    longPeriodOk = (errLongPeriod == TIMER_INTERRUPT_LEAN_ERROR::OK);
  }

  delay(100); // Give enough time for finalizing print outs.

  // Due to the resolution of the timer, the scheduled period may differ from the demanded period.
  // We print the scheduled vs. the demanded period here:
  Serial.println();
  printScheduledPeriod(timerSettingsShortPeriod, shortPeriodTimeoutDemand_ns);
  printScheduledPeriod(timerSettingsLongPeriod, longPeriodTimeoutDemand_ns);

  delay(100); // Give enough time for finalizing print outs.

  /**
   * Now do the real work.
   */
  if(longPeriodOk) {
    // 2 x timeout with long period
    Serial.println("\r\nStart 2 x long period");
    delay(100);
    timeStamp = micros();
    myTimer.start(timerSettingsLongPeriod, 2);
    delay(2000); // wait 2 seconds
  }

  if(shortPeriodOk) {
    // 3 x timeouts with short period
    Serial.println("\r\nStart 3 x short periods");
    delay(100);
    timeStamp = micros();
    myTimer.start(timerSettingsShortPeriod, 3);
    delay(1000); // wait 1 second
  }

#if ENABLE_INFINITE
  delay(2000); // wait 2 more seconds so that the output from the previous 3 timeouts can conveniently
               // be watched on the serial monitor.

  if(longPeriodOk) {
    // Infinite timeouts with long period
    Serial.println("\r\nStart inf x long periods");
    delay(100);
    timeStamp = micros();
    myTimer.start(timerSettingsLongPeriod, 0 /* 0=infinite */);
  }
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
    Serial.print("\r\n\t..error, period is beyond range ");
  } else
  if(err == TIMER_INTERRUPT_LEAN_ERROR::PERIOD_TOO_LARGE) {
    Serial.print("\r\n\t..error, period is beyond range ");
  } else if (err == TIMER_INTERRUPT_LEAN_ERROR::OK) {
    Serial.print("\r\n\t..success, period is within valid range ");
  }
  Serial.print("[");
  Serial.print(minPeriod_ns);
  Serial.print("ns ..");
  Serial.print(maxPeriod_ns);
  Serial.println("ns]");
}

void printScheduledPeriod(const ts_t timerSettings, const uint32_t periodTimeoutDemand_ns)
{
  const uint32_t scheduledTimeout = myTimer.getScheduledTimeoutPeriod_ns(timerSettings);
  if(scheduledTimeout > 0 && scheduledTimeout < UINT32_MAX) {
    Serial.print("Info: Scheduled period for ");
    Serial.print(periodTimeoutDemand_ns);
    Serial.print("ns is ");
    Serial.print(scheduledTimeout);
    Serial.print("ns, deviation is ");

    const int32_t delta =
        static_cast<int64_t>(scheduledTimeout) - static_cast<int64_t>(periodTimeoutDemand_ns);
    Serial.print(delta);
    Serial.println("ns");
  } else {
    Serial.print("Info: period ");
    Serial.print(periodTimeoutDemand_ns);
    Serial.println("ns could not be scheduled.");
  }
}
