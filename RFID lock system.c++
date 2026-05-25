#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

Servo lockServo;

String authorizedUID = "C3 F6 97 F7";

void setup() {

  Serial.begin(9600);

  SPI.begin();
  rfid.PCD_Init();

  lockServo.attach(3);
  lockServo.write(0);

  Serial.println("Scan RFID Card");
}

void loop() {

  if (!rfid.PICC_IsNewCardPresent())
    return;

  if (!rfid.PICC_ReadCardSerial())
    return;

  String cardUID = "";

  for (byte i = 0; i < rfid.uid.size; i++) {

    if (rfid.uid.uidByte[i] < 0x10)
      cardUID += "0";

    cardUID += String(rfid.uid.uidByte[i], HEX);

    if (i != rfid.uid.size - 1)
      cardUID += " ";
  }

  cardUID.toUpperCase();

  Serial.print("UID: ");
  Serial.println(cardUID);

  if (cardUID == authorizedUID) {

    Serial.println("Access Granted");

    lockServo.write(90);

    delay(3000);

    lockServo.write(0);

  } else {

    Serial.println("Access Denied");
  }

  rfid.PICC_HaltA();
}