# 📌 Setup TFT_eSPI untuk ST7789 (ESP32) + LittleFS (Arduino IDE 2.x)

Proyek ini menggunakan library **[TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)** untuk menghubungkan **ESP32** dengan display **TFT ST7789** (240x240), serta **LittleFS** untuk menyimpan data di flash ESP32.  

---

## ⚙️ 1. Konfigurasi `User_Setup_Select.h`

Buka file:

```
Arduino/libraries/TFT_eSPI/User_Setup_Select.h
```

Aktifkan setup untuk **ST7789** dengan cara uncomment baris berikut:

```cpp
#include <User_Setups/Setup24_ST7789.h>
```

Pastikan setup lain tetap dalam kondisi `//` (comment).

---

## ⚙️ 2. Konfigurasi Pin di `Setup24_ST7789.h`

Lokasi file:

```
Arduino/libraries/TFT_eSPI/User_Setups/Setup24_ST7789.h
```

Isi default yang digunakan:

```cpp
#define TFT_MISO -1    // Tidak digunakan (ST7789 hanya pakai MOSI)
#define TFT_MOSI 26    // SDA
#define TFT_SCLK 25    // SCL
#define TFT_CS   -1    // Tidak digunakan (ST7789 SPI tanpa CS)
#define TFT_DC    2    // Data / Command
#define TFT_RST   4    // Reset
```

---

## 📌 3. Konfigurasi Pin ESP32 ke TFT ST7789

| ESP32 GPIO | TFT ST7789 | Keterangan         |
|------------|------------|--------------------|
| GPIO 26    | SDA        | Jalur data (MOSI) |
| GPIO 25    | SCL        | Jalur clock (SCLK)|
| GND        | CS         | Ikat ke GND (tidak dipakai) |
| GPIO 2     | DC         | Data / Command    |
| GPIO 4     | RST        | Reset layar       |
| 3.3V       | VCC        | Power 3.3V        |
| GND        | GND        | Ground            |

> ⚠️ **Catatan**: ST7789 tidak menggunakan MISO, jadi pin ini bisa diabaikan.

---

## 📂 Struktur Folder

```
Arduino/
 └── libraries/
     └── TFT_eSPI/
         ├── User_Setup_Select.h     <-- pilih setup di sini
         └── User_Setups/
             └── Setup24_ST7789.h    <-- konfigurasi pin ST7789
```

---

## 🗂️ 4. Setup LittleFS di Arduino IDE 2.x (ESP32)

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
     ├── image.jpg
     └── config.txt
```

### 4. Upload file ke flash ESP32  
- Pilih board ESP32 yang sesuai.  
- Tekan Ctrl + Shift + P
- Ketik: upload littlefs
- Pilih: Upload LittleFS to Pico/ESP8266/ESP32
- Tunggu hingga proses selesai.

```

---

## 🚀 Cara Pakai

1. Install library **TFT_eSPI** & **LittleFS** di Arduino IDE 2.x.  
2. Edit `User_Setup_Select.h` dan aktifkan `Setup24_ST7789.h`.  
3. Sesuaikan pin di `Setup24_ST7789.h`.  
4. Buat folder `data/` bila ingin menyimpan file di LittleFS.  
5. Upload file ke flash ESP32 
6. Upload sketch Arduino kamu.  
7. TFT ST7789 & LittleFS siap digunakan ✅  
