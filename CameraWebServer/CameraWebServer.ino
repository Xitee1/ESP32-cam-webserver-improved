#include "esp_camera.h"
#include <WiFi.h>

//
// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
//

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM

#include "camera_pins.h"

const char* ssid = "Wlan-HRM";
const char* password = "KaltesFeuer";

void startCameraServer();

// Static IP
IPAddress local_IP(192, 168, 2, 182);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 0, 0);

void setup() {
  // LEDs
  pinMode(4, OUTPUT);
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);

  // Serial connection
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  // Initialize the camera
  Serial.print("Initializing the camera module...");
  configInitCamera();
  Serial.println("Ok!");

  // Connect to WiFi
  if(!WiFi.config(local_IP, gateway, subnet))
    Serial.println("STA Failed to configure");
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to WiFi.");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(33, HIGH);
    delay(250);
    Serial.print(".");
    digitalWrite(33, LOW);
    delay(250);
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the Server
  startCameraServer();

  // Camera successfully started!
  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");

  // Blink LED
  digitalWrite(33, HIGH); // off
  delay(50);
  digitalWrite(33, LOW); // on
  delay(50);
  digitalWrite(33, HIGH); // off
  delay(50);
  digitalWrite(33, LOW); // on
  delay(50);
  digitalWrite(33, HIGH); // off
}
void loop() {
  // Autoreconnect WiFi
  delay(1000*10);
  if(WiFi.status() != WL_CONNECTED) {
    Serial.print("Lost WiFi connection. Reconnecting");
    WiFi.disconnect();
    WiFi.reconnect();
    while(WiFi.status() != WL_CONNECTED) {
      digitalWrite(33, LOW);
      delay(250);
      Serial.print(".");
      digitalWrite(33, HIGH);
      delay(250);
    }
    Serial.println("");
    Serial.println("WiFi connected!");
  }
}
void configInitCamera(){
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
  config.xclk_freq_hz = 5000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_SVGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
  config.jpeg_quality = 10; //10-63 lower number means higher quality
  config.fb_count = 2;
  #if defined(CAMERA_MODEL_ESP_EYE)
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
  #endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_SVGA);
  s->set_contrast(s, 1);       // -2 to 2
  s->set_saturation(s, -1);    // -2 to 2
}
