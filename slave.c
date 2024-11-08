#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

#define SS_PIN 53   // SS Pin pada Arduino untuk RC522
#define RST_PIN 5   // RST Pin pada Arduino untuk RC522

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Inisialisasi MFRC522
SoftwareSerial serial(2, 3);       // RX, TX untuk komunikasi serial

void setup() {
  SPI.begin();            // Inisialisasi SPI bus
  mfrc522.PCD_Init();     // Inisialisasi MFRC522
  serial.begin(9600);     // Komunikasi serial dengan baudrate 9600 ke perangkat lain
  Serial.begin(9600);     // Komunikasi serial untuk Serial Monitor
}

void loop() {
  // Periksa apakah ada kartu RFID yang terdeteksi
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Ambil UID kartu RFID
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");  // Tambahkan "0" jika diperlukan
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();  // Ubah UID menjadi huruf besar

    // Kirim UID ke perangkat lain dan tampilkan di Serial Monitor
    serial.println(uid);
    Serial.println("ID Kartu RFID: " + uid);

    // Periksa UID dan kirim sinyal sesuai kondisi
    if (uid == "1334780F") {
      serial.println("1"); // Kirim sinyal '1' jika UID sesuai
    } else {
      serial.println("2"); // Kirim sinyal '2' jika UID lain terdeteksi
    }

    mfrc522.PICC_HaltA(); // Hentikan komunikasi dengan kartu
  } else {
    // Jika tidak ada kartu RFID yang terdeteksi, kirim sinyal "0"
    serial.println("0");
  }

  delay(1000); // Jeda sebelum membaca kartu RFID lagi
}
