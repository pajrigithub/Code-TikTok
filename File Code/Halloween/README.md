# 📌 Setup Adafruit ST7789 (ESP32) + LittleFS (Arduino IDE 2.x)

Proyek ini menggunakan library **[Adafruit ST7789](https://github.com/adafruit/Adafruit-ST7735-Library)** untuk menghubungkan **ESP32** dengan display **TFT ST7789** (240x240), serta **LittleFS** untuk menyimpan data di flash ESP32.  

---

## ⚙️ 1. Instalasi Library

Pastikan library berikut sudah terinstal melalui **Library Manager** di Arduino IDE:

- **Adafruit ST7735 and ST7789 Library**
- **Adafruit GFX Library**
- **LittleFS for ESP32**

Buka menu:
```
Sketch > Include Library > Manage Libraries
```

Cari dan install semua library di atas.

---

## ⚙️ 2. Konfigurasi Pin ESP32 ke TFT ST7789

Berikut contoh konfigurasi pin yang digunakan:

| ESP32 GPIO | TFT ST7789 | Keterangan         |
|------------|------------|--------------------|
| GPIO 26    | SDA / MOSI | Jalur data (MOSI) |
| GPIO 25    | SCL / SCK  | Jalur clock (SCLK)|
| GPIO 2     | DC         | Data / Command    |
| GPIO 4     | RST        | Reset layar       |
| GND        | CS         | Dipasang ke GND (tidak dipakai) |
| 3.3V       | VCC        | Power 3.3V        |
| GND        | GND        | Ground            |

> ⚠️ **Catatan:** ST7789 tidak menggunakan MISO.

---

## 🧠 3. Contoh Inisialisasi di Sketch

```cpp
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_MOSI 26
#define TFT_SCLK 25
#define TFT_DC   2
#define TFT_RST  4
#define TFT_CS   -1  // Tidak digunakan

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  tft.init(240, 240);  // Resolusi layar
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Adafruit ST7789 Ready!");
}

void loop() {
}
```

---

## 📂 4. Setup LittleFS di Arduino IDE 2.x (ESP32)

### 1. Install library LittleFS ESP32
- Buka menu: **Sketch > Include Library > Manage Libraries**  
- Cari **"LittleFS"** dan install **[ESP32 LittleFS Data Upload](https://github.com/lorol/LITTLEFS)**.

### 2. Tambahkan Plugin "ESP32 LittleFS Data Upload"
- Download dari GitHub: [ESP32FS-Plugin](https://github.com/lorol/arduino-esp32fs-plugin)  
- Extract ke folder:  

  ```
  C:\Users\<Username>\AppData\Local\Arduino15\packages\esp32\tools\
  ```
- Restart Arduino IDE 2.x.  
- Akan muncul menu baru:  
  **Tools > ESP32 Data Upload**

### 3. Buat folder `data/` dalam sketch project  
Contoh struktur:

```
ProjectName/
 ├── ProjectName.ino
 └── data/
     └──image.jpg
```

### 4. Upload file ke flash ESP32  
- Pilih board ESP32 yang sesuai.  
- Tekan Ctrl + Shift + P
- Ketik: upload littlefs
- Pilih: Upload LittleFS to ESP32
- Tunggu hingga proses selesai.

---

## 🚀 Cara Pakai

1. Install semua library yang diperlukan.  
2. Hubungkan pin sesuai tabel di atas.  
3. Buat folder `data/` bila ingin menyimpan file di LittleFS.  
4. Upload file ke flash ESP32.  
5. Upload sketch Arduino kamu.  
6. Layar ST7789 dan LittleFS siap digunakan ✅
