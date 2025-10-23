#include <FS.h>
#include <LittleFS.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <JPEGDecoder.h>

// --- PIN KONFIGURASI ---
#define TFT_CS    -1   // tidak digunakan (karena SPI hardware)
#define TFT_RST   0    // reset di pin 0
#define TFT_DC    1    // data/command di pin 1
#define TFT_SDA   6    // SDA (MOSI)
#define TFT_SCL   4    // SCL (SCK)

#define SCR_WD    240
#define SCR_HT    240

// --- OBJEK TFT ---
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi display
  tft.init(SCR_WD, SCR_HT, SPI_MODE3);
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLACK);

  // Inisialisasi LittleFS
  if (!LittleFS.begin()) {
    Serial.println("Gagal mount LittleFS");
    while (1);
  }

  delay(100);
}

void loop() {
  // Main loop untuk memainkan animasi
  playGif("/muka", 55, 5);
  // Contoh lain:
  // playGif("/senyum", 30, 70);
}

// --- FUNGSI PEMUTAR GIF FRAME BY FRAME ---
void playGif(const char *folderName, int totalFrame, int delayMs) {
  char filename[64];

  for (int frame = 1; frame <= totalFrame; frame++) {
    sprintf(filename, "%s/frame_%d.jpg", folderName, frame);
    
    File jpgFile = LittleFS.open(filename, "r");
    if (!jpgFile) {
      Serial.printf("Gagal buka file %s\n", filename);
      continue;
    }

    if (JpegDec.decodeFsFile(jpgFile)) {
      renderJPEG(0, 0);
    } else {
      Serial.printf("Gagal decode %s\n", filename);
    }

    jpgFile.close();
    delay(delayMs);
  }
}

void renderJPEG(int xpos, int ypos) {
  uint16_t* pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = tft.width();
  uint32_t max_y = tft.height();

  while (JpegDec.read()) {
    pImg = JpegDec.pImage;
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    int drawWidth = (mcu_x + mcu_w <= max_x) ? mcu_w : max_x - mcu_x;
    int drawHeight = (mcu_y + mcu_h <= max_y) ? mcu_h : max_y - mcu_y;

    if (drawWidth > 0 && drawHeight > 0) {
      tft.drawRGBBitmap(mcu_x, mcu_y, pImg, drawWidth, drawHeight);
    }
  }
}
