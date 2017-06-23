/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "Patterns.h"
#include "LedController.h"
#include "LogBuffer.h"

const char* host = "esp8266-webupdate";
const char* ssid = "Planet SR388";
const char* password = "HERBERTHOOVER";

bool updating = false;

ESP8266WebServer server(80);
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

PatternBase* pattern;

void setup(void){
  
  LedController::init();
  LogBuffer::Clear();

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  if(WiFi.waitForConnectResult() == WL_CONNECTED){
    MDNS.begin(host);
    server.on("/", HTTP_GET, [](){
      updating = true;
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/html", serverIndex);
    });
    server.on("/log", HTTP_GET, [](){
      server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
      server.sendHeader("Pragma", "no-cache");
      server.sendHeader("Expires", "-1");
      server.setContentLength(CONTENT_LENGTH_UNKNOWN);
      server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
      server.sendContent("<h1>Logs</h1>");
      while (!LogBuffer::IsEmpty()) {
        server.sendContent("<p>" + String(LogBuffer::Read()) + "</p>");
      }
      LogBuffer::Clear();
      server.client().stop(); // Stop is needed because we sent no content length
    });
    server.on("/update", HTTP_POST, [](){
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
      ESP.restart();
    },[](){
      HTTPUpload& upload = server.upload();
      if(upload.status == UPLOAD_FILE_START){
        Serial.setDebugOutput(true);
        WiFiUDP::stopAll();
        Serial.printf("Update: %s\n", upload.filename.c_str());
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if(!Update.begin(maxSketchSpace)){//start with max available size
          Update.printError(Serial);
        }
      } else if(upload.status == UPLOAD_FILE_WRITE){
        if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
          Update.printError(Serial);
        }
      } else if(upload.status == UPLOAD_FILE_END){
        if(Update.end(true)){ //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      }
      yield();
    });
    server.begin();
    MDNS.addService("http", "tcp", 80);
  
    Serial.printf("Ready! Open http://%s.local in your browser\n", host);
  } else {
    Serial.println("WiFi Failed");
  }
  //check for 10
  for(int i = 0; i < 30; i++) {
    server.handleClient();
    delay(100);
  }

  //pattern = new update_red_1_led_test(200);
  //pattern = new update_red_8_led_test(100);
  //pattern = new update_minimal_clock();
  //pattern = new update_amplitude_basic();
  //pattern = new update_fft_test();
  pattern = new update_log_samples();
  //pattern = new update_blank();
}

void loop(void){
  if (!updating) {
    pattern->update();
  }
  server.handleClient();
} 
