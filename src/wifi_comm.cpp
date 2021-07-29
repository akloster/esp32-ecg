#include <WiFi.h>
#include <Arduino.h>
#include "global.h"
#include "wifi_comm.h"
#include "consts.h"

WiFiServer wifiServer(3333);

void sendingLoop(void * parameters){
  running = false;
  TickType_t xLastWakeTime;
   
  while (1) {
    WiFiClient client = wifiServer.available(); 
    unsigned long t=0;
    const TickType_t xInterval = 1000 / SAMPLES_PER_SECOND * BUFFER_LENGTH;
    if (client) {
      Serial.println("Client connected");
      running=true;
      
      xLastWakeTime = xTaskGetTickCount();
      while (client.connected()) {
        vTaskDelayUntil( &xLastWakeTime, xInterval);
        //while ((!bufferFlag) && (client.connected()));
        if (!client.connected()) break;
        unsigned long lastT = t;
        unsigned long newT = micros();
        t = newT;
        if (lastT>0) Serial.println(newT-lastT);
        bufferFlag = false;
        client.write((char *)&(sampleBuffer[!currentBuffer]), BUFFER_LENGTH*sizeof(int16_t));
        vTaskDelay(1);
        if (!client.connected()) break;
      } 
      client.stop();
      Serial.println("Client disconnected");
      running = false;
      vTaskDelay(1);
    }
  }
}