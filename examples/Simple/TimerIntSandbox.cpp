// Do not remove the include below
#include "TimerIntSandbox.h"
#include "TimerInterruptLean.h"

extern "C" void TIMER1_COMPA_vect (void) __attribute__ ((signal,used, externally_visible));
//static auto vect = TIMER1_COMPA_vect;
static void (*vect)() = TIMER1_COMPA_vect;

static uint32_t timeStamp = 0;
static size_t   counter   = 0;

class MyTimer1Interupt : public TimerInterruptLean<1> {
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

MyTimer1Interupt timer1;

int32_t timerSettingsLong;

//The setup function is called once at startup of the sketch
void setup()
{
  Serial.begin(9600);
  Serial.print("vect: ");
  Serial.println(reinterpret_cast<uint32_t>(vect));

  timer1.begin();

  Serial.println(timer1.minPeriod_ns());
  Serial.println(timer1.maxPeriod_ns());

  // Save the 500ms setting for re-use
  timerSettingsLong = timer1.getTimerSettingsForPeriod_ms(500); // 500ms

  // 1 timeout with long 500ms period
  timeStamp = millis();
  // use pre-calculated timer settings
  Serial.println("start 1 x long");
  timer1.start(timerSettingsLong, 1);
  delay(1000); // wait 1 second

  const uint32_t timerSettingsShort = timer1.getTimerSettingsForPeriod_ms(200); // 200ms
  timeStamp = millis();
  Serial.println("start 3 x short");
  // 3 timeouts with short 200ms period
  timer1.start(timerSettingsShort, 3);
  delay(6000); // wait 6 seconds

  // Infinite timeouts with long 500ms period
  timeStamp = millis();
  // use pre-calculated timer settings
  Serial.println("start inf long");
  timer1.start(timerSettingsLong, 0);
}

// The loop function is called in an endless loop
void loop()
{
}
