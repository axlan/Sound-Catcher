/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WifiUDP.h>

#include "Patterns.h"
#include "LedController.h"
#include "LogBuffer.h"
#include "WifiConfig.h"

bool updating = false;

ESP8266WebServer server(80);
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

PatternBase* pattern;
int sel_pattern_idx;

void setup(void){
  
  LedController::init();
  LogBuffer::Clear();

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(SSID, WIFI_PASS);
  if(WiFi.waitForConnectResult() == WL_CONNECTED){
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
    server.on("/ctrl", HTTP_GET, [](){
      
      updating = false;
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      String html_buf;
      bool new_pattern = false;

    
      if (server.hasArg("patternlist")){ //Check if body received
        sel_pattern_idx = server.arg("patternlist").toInt();
        delete pattern;
        pattern = PatternBase::get_pattern(sel_pattern_idx);
        new_pattern = true;
      }

      html_buf+=PatternBase::get_patterns()[sel_pattern_idx];
      html_buf+="<br>";
      html_buf+="<select name=\"patternlist\" form=\"patternform\">";
      for (int i = 0; i < PatternBase::num_patterns(); i++) {
        html_buf+="<option value=";
        html_buf+= i;
        html_buf+=">";
        html_buf+=PatternBase::get_patterns()[i];
        html_buf+="</option>";
      }
      html_buf+="</select>";
      html_buf+="<form action=\"/ctrl\" id=\"patternform\"><input type=\"submit\"></form>";

      int num_vals = pattern->get_vals()->size();
      if (num_vals > 0) {
        html_buf+="<form action=\"/ctrl\" id=\"settingform\">";
        
        for (int i = 0; i < num_vals; i++) {

          Value* val = pattern->get_vals()->at(i);

          html_buf+=pattern->get_names()->at(i);
          html_buf+="<input type=\"";

          String param_name = "val";
          param_name += i;
          if (server.hasArg(param_name)){ //Check if body received
            val->set_val(server.arg(param_name).c_str());
          } else if (!new_pattern && val->get_type()==Value::BOOL) {
            val->set_bool(false);
          }

          switch(val->get_type()) {
            case Value::INT: html_buf+="number"; break;
            case Value::BOOL: html_buf+="checkbox"; break;
            case Value::FLOAT: html_buf+="number\" step=\"any"; break;
          }
          html_buf += "\" name=\"val";
          html_buf += i;
          html_buf +="\" value=\"";

          switch(val->get_type()) {
            case Value::INT: html_buf+=val->get_int(); break;
            case Value::BOOL: 
              html_buf+="1\"";
              html_buf+=(val->get_bool())?" checked ":"";
              html_buf+="\"";
            break;
            case Value::FLOAT: html_buf += val->get_float(); break;
          }
        
          html_buf += "\"><br>";
        }
        html_buf+="<input type=\"submit\"></form>";
      }

      server.send(200, "text/html", html_buf);
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
  
    Serial.printf("Ready!\n");
  } else {
    Serial.println("WiFi Failed");
  }
  //check for 10
  for(int i = 0; i < 30; i++) {
    server.handleClient();
    delay(100);
  }

  sel_pattern_idx = 2;
  pattern = PatternBase::get_pattern(sel_pattern_idx);
  //pattern = new update_red_8_led_test(100);
  //pattern = new update_minimal_clock();
  //pattern = new update_amplitude_basic();
  //pattern = new update_fft_test();
  //pattern = new update_log_samples();
  //pattern = new update_blank();
}

void loop(void){
  if (!updating) {
    pattern->update();
  }
  server.handleClient();
} 
