#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial serial(2, 3);
const int buzzerPin = 9;
const int relayPin = 7;
const int led1Pin = 5;  // Pin untuk LED1
const int led2Pin = 6;  // Pin untuk LED2

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat LCD I2C: 0x27, 16 kolom, 2 baris

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  serial.begin(9600);

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  displayCenteredText("SAFETY BOX", "kelompok 14");
}

void loop() {
  if (serial.available() > 0) {
    char receivedChar = serial.read();
    handleReceivedChar(receivedChar);
  }
}

void handleReceivedChar(char receivedChar) {
  switch (receivedChar) {
    case '1':
      activateDevice1();
      break;
    case '2':
      activateDevice3();
      break;
  }
}

void activateDevice1() {
  digitalWrite(relayPin, HIGH);
  digitalWrite(led1Pin, HIGH);  // Menyalakan LED1 untuk akses diterima
  digitalWrite(led2Pin, LOW);   // Memastikan LED2 mati
  displayCenteredText("Access Granted", "[SAFETY BOX]");
  singAcceptedMelody();
  delay(2000);                  // Menunggu 2 detik
  digitalWrite(led1Pin, LOW);   // Mematikan LED1 setelah 2 detik
  digitalWrite(relayPin, LOW);  // Mematikan relay
  displayCenteredText("SAFETY BOX", "kelompok 14");
}

void activateDevice3() {
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(relayPin, LOW);
  digitalWrite(led1Pin, LOW);   // Memastikan LED1 mati
  digitalWrite(led2Pin, HIGH);  // Menyalakan LED2 untuk akses ditolak
  displayCenteredText("Access Denied", "[SAFETY BOX]");
  singDeniedMelody();
  delay(2000);                  // Menunggu 2 detik
  digitalWrite(led2Pin, LOW);   // Mematikan LED2 setelah 2 detik
  noTone(buzzerPin);            // Mematikan buzzer
  displayCenteredText("SAFETY BOX", "kelompok 14");
}

void singAcceptedMelody() {
  tone(buzzerPin, 784);
  delay(300);
  tone(buzzerPin, 880);
  delay(300);
  tone(buzzerPin, 987);
  delay(300);
  tone(buzzerPin, 1046);
  delay(300);
  noTone(buzzerPin);
}

void singDeniedMelody() {
  tone(buzzerPin, 392);
  delay(300);
  tone(buzzerPin, 330);
  delay(300);
  tone(buzzerPin, 294);
  delay(300);
  tone(buzzerPin, 262);
  delay(300);
  noTone(buzzerPin);
}

void displayCenteredText(const char* line1, const char* line2) {
  lcd.clear();

  // Menampilkan teks pada baris pertama
  int len1 = strlen(line1);
  int pos1 = (16 - len1) / 2;  // Menghitung posisi awal untuk baris pertama
  lcd.setCursor(pos1, 0);      // Mengatur kursor ke posisi tengah pada baris pertama
  lcd.print(line1);

  // Menampilkan teks pada baris kedua
  int len2 = strlen(line2);
  int pos2 = (16 - len2) / 2;  // Menghitung posisi awal untuk baris kedua
  lcd.setCursor(pos2, 1);      // Mengatur kursor ke posisi tengah pada baris kedua
  lcd.print(line2);
}
