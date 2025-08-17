#include <FS.h>
#include <LittleFS.h>
#include <TFT_eSPI.h>
#include <JPEGDecoder.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);

  // Init TFT
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);

  // Init LittleFS
  if (!LittleFS.begin()) {
    Serial.println("Gagal mount LittleFS");
    while (1);
  }
}

void loop() {
  // Contoh memanggil animasi muka marah
  playGif("/muka_marah", 39, 50);
  playGif("/muka_salting", 39, 50);
  playGif("/muka_datar", 39, 50);

  // Kalau mau animasi lain tinggal panggil:
  // playGif("/muka_senyum", 25, 80);
}

void playGif(const char *folderName, int totalFrame, int delayMs) {
  char filename[50];
  
  for (int frame = 1; frame <= totalFrame; frame++) {
    sprintf(filename, "%s/%s_%d.jpg", folderName, folderName + 1, frame); 
    // folderName + 1 dipakai untuk ambil nama tanpa '/' di awal
    
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

    if (mcu_x + mcu_w <= max_x && mcu_y + mcu_h <= max_y) {
      tft.pushImage(mcu_x, mcu_y, mcu_w, mcu_h, pImg);
    } else {
      int clip_w = (mcu_x + mcu_w > max_x) ? max_x - mcu_x : mcu_w;
      int clip_h = (mcu_y + mcu_h > max_y) ? max_y - mcu_y : mcu_h;
      if (clip_w > 0 && clip_h > 0) {
        tft.pushImage(mcu_x, mcu_y, clip_w, clip_h, pImg);
      }
    }
  }
}
