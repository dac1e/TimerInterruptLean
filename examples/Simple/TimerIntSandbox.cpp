// Do not remove the include below
#include "TimerIntSandbox.h"
#include "TimerInterruptLean.h"


static uint32_t timeStamp = 0;
static size_t   counter   = 0;

static constexpr size_t TIMER_NO = 2;

class MyTimer1Interupt : public TimerInterruptLean<TIMER_NO> {
  void onTimeout() override {
    const uint32_t t =  millis();
    const uint32_t delta = t - timeStamp;
    Serial.print("timeout #");
    Serial.print(++counter);
    Serial.print(" after ");
    Serial.print(delta);
    Serial.println("ms");
    timeStamp = t;
  }
};

int32_t timerSettingsLong;
MyTimer1Interupt timer1;

void printErr(const TIMER_INTERRUPT_LEAN_ERROR err) {
  if(err == TIMER_INTERRUPT_LEAN_ERROR::PERIOD_TOO_SMALL) {
    Serial.println("period too small");
    return;
  }
  if(err == TIMER_INTERRUPT_LEAN_ERROR::PERIOD_TOO_LARGE) {
    Serial.println("period too large");
    return;
  }
  Serial.println("period OK");
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

  // Save the 500ms setting for re-use
  timerSettingsLong = timer1.getTimerSettingsForPeriod_ms(500); // 500ms

  // 1 timeout with long 500ms period
  {
    timeStamp = millis();
    Serial.println("start 1 x long");
    // use pre-calculated timer settings
    const TIMER_INTERRUPT_LEAN_ERROR err = timer1.start(timerSettingsLong, 1);
    printErr(err);
    delay(1000); // wait 1 second
  }


  // 3 timeouts with short 16ms period
  {
    const uint32_t timerSettingsShort = timer1.getTimerSettingsForPeriod_ms(16); // 16ms
    delay(200);
    timeStamp = millis();
    Serial.println("start 3 x short");
    const TIMER_INTERRUPT_LEAN_ERROR err = timer1.start(timerSettingsShort, 3);
    printErr(err);
    delay(3000); // wait 3 seconds
  }

  // Infinite timeouts with long 500ms period
  {
    timeStamp = millis();
    Serial.println("start inf x long");
    // use pre-calculated timer settings
    const TIMER_INTERRUPT_LEAN_ERROR err = timer1.start(timerSettingsLong, 0 /* 0=infinite */);
    printErr(err);
  }
}

// The loop function is called in an endless loop
void loop()
{
}
