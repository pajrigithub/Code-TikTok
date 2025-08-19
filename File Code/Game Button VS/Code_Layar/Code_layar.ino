#include <esp_now.h>
#include <WiFi.h>
#include <TFT_eSPI.h>  // Library ST7789
#include <SPI.h>

// === TFT setup ===
TFT_eSPI tft = TFT_eSPI();  // 240x240 ST7789

// Skor
int blueScore = 0;
int greenScore = 0;
int maxScore = 15; // target poin untuk menang

bool gameOver = false;  // Flag penanda kalau sudah ada pemenang

// Data diterima
typedef struct struct_message {
  char team[10];
} struct_message;

struct_message incomingData;

// === Callback setelah data diterima ===
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingDataRaw, int len) {
  if (gameOver) return; // kalau sudah ada pemenang, jangan tambah skor lagi

  memcpy(&incomingData, incomingDataRaw, sizeof(incomingData));

  if (strcmp(incomingData.team, "BLUE") == 0) {
    if (blueScore < maxScore) blueScore++;
  }
  if (strcmp(incomingData.team, "GREEN") == 0) {
    if (greenScore < maxScore) greenScore++;
  }

  drawBars();
}

// === Setup ===
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error ESP-NOW init");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  // Init TFT
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  // Tampilan awal
  drawBars();
}

// === Loop ===
void loop() {
  // tidak ada yang perlu di-loop, semua event dari ESP-NOW
}

// === Fungsi gambar bar dan skor ===
void drawBars() {
  if (gameOver) return; // kalau game sudah selesai, jangan gambar bar lagi

  // Hitung panjang bar sesuai skor
  int barWidthBlue  = map(blueScore, 0, maxScore, 0, 240);
  int barWidthGreen = map(greenScore, 0, maxScore, 0, 240);

  // Bersihkan area bar dulu
  tft.fillRect(0, 60, 240, 40, TFT_BLACK);   // area Blue
  tft.fillRect(0, 140, 240, 40, TFT_BLACK);  // area Green

  // Bar Blue (atas)
  tft.fillRect(0, 60, barWidthBlue, 40, TFT_BLUE);

  // Bar Green (bawah)
  tft.fillRect(0, 140, barWidthGreen, 40, TFT_GREEN);

  // Tulis skor
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.setCursor(10, 20);
  tft.printf("Blue: %d", blueScore);

  tft.setCursor(10, 200);
  tft.printf("Green: %d", greenScore);

  // === Cek kondisi menang ===
  if (blueScore >= maxScore) {
    showWinner("BLUE");
  } else if (greenScore >= maxScore) {
    showWinner("GREEN");
  }
}

// === Fungsi tampilkan pemenang ===
void showWinner(const char *team) {
  gameOver = true;  // tandai permainan selesai

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  tft.setCursor(40, 100);
  tft.printf("WINNER: %s", team);
}