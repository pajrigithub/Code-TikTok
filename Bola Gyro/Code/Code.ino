#include <Wire.h>
#include <TFT_eSPI.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

TFT_eSPI tft = TFT_eSPI();  
Adafruit_MPU6050 mpu;

// Posisi bola (pakai float biar halus)
float ballX = 120;
float ballY = 120;
int radius = 20;

// Simpan nilai offset (kalibrasi awal)
float offsetX = 0;
float offsetY = 0;

// Simpan posisi bola lama (buat hapus tanpa clear full screen)
int lastX, lastY;

void setup() {
  Serial.begin(115200);

  // Init TFT
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  // Init MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(1000);

  // Kalibrasi awal → ambil nilai rata-rata saat diam
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  offsetX = a.acceleration.x;
  offsetY = a.acceleration.y;

  // Gambar bola awal
  tft.fillCircle((int)ballX, (int)ballY, radius, TFT_GREEN);
  lastX = ballX;
  lastY = ballY;
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Ambil nilai yang sudah dikalibrasi
  float ax = a.acceleration.x - offsetX;
  float ay = a.acceleration.y - offsetY;

  // Update posisi bola (lebih responsif → kalikan lebih besar)
  ballX += (-ax * 2.5);   // arah kiri/kanan (dibalik)
  ballY += (-ay * 2.5);   // arah atas/bawah

  // Batasi agar bola tidak keluar layar
  if (ballX < radius) ballX = radius;
  if (ballX > 240 - radius) ballX = 240 - radius;
  if (ballY < radius) ballY = radius;
  if (ballY > 240 - radius) ballY = 240 - radius;

  // Hapus bola lama (gambar lingkaran hitam di posisi lama)
  tft.fillCircle(lastX, lastY, radius, TFT_BLACK);

  // Gambar bola baru
  tft.fillCircle((int)ballX, (int)ballY, radius, TFT_GREEN);

  // Simpan posisi terakhir
  lastX = ballX;
  lastY = ballY;

  delay(15); // refresh cepat biar smooth
}
