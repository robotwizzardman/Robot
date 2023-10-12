
#define CAMERA_MODEL_AI_THINKER

#include "esp_camera.h"
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <string.h>
#include "camera_pins.h" // This must come after camera model has been selected

const char* ssid     = "DESKTOP-PPPP2NJ-0037";     // input your wifi name
const char* password = "hotspot-123";   // input your wifi passwords

//const char* ssid     = "hostpot_giganta";     // input your wifi name
//const char* password = "bezimienny";   // input your wifi passwords
const char* identity = "I_am_left_eye";

WebSocketsServer webSocket = WebSocketsServer(80);
 
// Called when receiving any WebSocket message
void onWebSocketEvent(uint8_t num,
                      WStype_t type,
                      uint8_t * payload,
                      size_t length) {

  // Figure out the type of WebSocket event
  switch(type) {
// Client has disconnected
//    case WStype_DISCONNECTED:
//      Serial.printf("[%u] Disconnected!\n", num);
//      break;

    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
//        Serial.printf("[%u] Connection from ", num);
//        Serial.println(ip.toString());
      }
      break;

    // Echo text message back to client
    case WStype_TEXT:
      if (String((char*)payload) == "identify yourself")
      {
        Serial.println("I was asked to identify myself");
        webSocket.sendTXT(num, identity);
        //webSocket.sendTXT(num, payload);
      }
      else
      if (String((char*)payload) == "capture")
      {
        Serial.println("Capture Command Received - capturing frame");

        camera_fb_t * fb = NULL; // tworzy pointer do bufora
        //fb = NULL; //
        fb = esp_camera_fb_get(); // wykonuje zdjęcie i zapisuje do bufora
        size_t fbsize = fb->len; // liczy rozmiar bufora    
        webSocket.sendBIN(num, fb->buf, fbsize); // wysyła websocketem dane
        esp_camera_fb_return(fb); // zwalnia pamięć w kamerze
      } else
      {
        webSocket.sendTXT(num, payload);
      }
      break;

    // For everything else: do nothing
    case WStype_BIN:
     // Serial.printf("[%u] get binary length: %u\n", num, length);
     // hexdump(payload, length);

      // send message to client
      // webSocket.sendBIN(num, payload, length);
     // break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("Connecting...");

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;
  config.frame_size = FRAMESIZE_QVGA; //FRAMESIZE_96X96; //FRAMESIZE_QQVGA; //FRAMESIZE_SVGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  //config.fb_location = CAMERA_FB_IN_DRAM;
  //}

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Msg: Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);

  char* NAZWA = "lewe_oko";
  WiFi.setHostname(NAZWA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.printf(".");
  }
  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

void loop() {
  // Look for and handle WebSocket data
  webSocket.loop();
}
