#include <esp_now.h>
#include <WiFi.h>

// Alamat MAC Device 2 (Display/Game) → ganti dengan punyamu
uint8_t broadcastAddress[] = {0x8C, 0x4F, 0x00, 0xB8, 0xA3, 0x34};

// Pin tombol
const int buttonBluePin  = 7;
const int buttonGreenPin = 3;

// Data yang dikirim
typedef struct struct_message {
  char team[10];
} struct_message;

struct_message myData;

// Callback kirim data
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Kirim data: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "SUKSES" : "GAGAL");
}

void setup() {
  Serial.begin(115200);

  pinMode(buttonBluePin, INPUT_PULLUP);
  pinMode(buttonGreenPin, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error ESP-NOW init");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Gagal add peer");
    return;
  }
}

void loop() {
  if (digitalRead(buttonBluePin) == LOW) {
    strcpy(myData.team, "BLUE");
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    delay(150); // debounce sederhana
  }
  if (digitalRead(buttonGreenPin) == LOW) {
    strcpy(myData.team, "GREEN");
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    delay(150); // debounce sederhana
  }
}