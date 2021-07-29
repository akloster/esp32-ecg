#include <WiFi.h>
#include <Arduino.h>
#include <Wire.h>
#include "ADS1X15.h"
#include "consts.h"
#include "wifi_comm.h"
#include "./wifi_secrets.h" // const char * ssid, password
#include "scan_i2c.h"

int16_t sampleBuffer[2][BUFFER_LENGTH];
uint16_t  timingBuffer[2][BUFFER_LENGTH];

volatile unsigned int currentBuffer;
volatile boolean bufferFlag; 
boolean running=false;
 
volatile unsigned int interruptCounter;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}

ADS1015 * adc1;

TaskHandle_t SendingTask;

void setup() {
  Wire.begin(22,21);
  Wire.setClock(400000);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
 
  wifiServer.begin();
  delay(500);
  scanI2C();

  // 0x48 is the standard address, when ADDR is pulled low.
  adc1 = new ADS1015(0x48);
  adc1->begin();
  adc1->setGain(2);
  adc1->setDataRate(6); 

  // Setup a timer to run at the specified SPS
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000/SAMPLES_PER_SECOND, true);
  timerAlarmEnable(timer);
  // The sending taks must run on core 1. Not because you can't issue a send
  // command from core 0, but because that would take too much time and so
  // the reading loop would loose samples.
  xTaskCreatePinnedToCore(
    sendingLoop,   /* Task function. */
    "Sending Task",     /* name of task. */
    20000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &SendingTask,      /* Task handle to keep track of created task */
    1);          
}
// # main arduino loop__
void loop() {
  currentBuffer=0;
  bufferFlag = false;
  // Wait for client to connect
  while (!running) vTaskDelay(1);
  while (running){
    int elapsed; // previously used to gather timing information for debugging
    int elapsed2 = 0;
    for (int i=0; i<BUFFER_LENGTH; i++){
      while (interruptCounter==0);
      portENTER_CRITICAL(&timerMux);
      elapsed = interruptCounter;
      interruptCounter=0;
      portEXIT_CRITICAL(&timerMux);
      elapsed2 += elapsed;
      
      sampleBuffer[currentBuffer][i] = adc1->readADC(0);
      // timingBuffer is currently not used
      // timingBuffer[currentBuffer][i] = micros();
    }
    currentBuffer = !currentBuffer;
    bufferFlag = true;
  }
}

