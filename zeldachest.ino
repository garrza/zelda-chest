#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include "pitches.h"

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);
Servo myservo;
byte targetUID[] = {0x04, 0x7C, 0x0A, 0x52, 0xB8, 0x48, 0x84}; // UID of the target RFID tag
int buzzerPin = 3;                                             // Pin connected to the piezo buzzer

void setup()
{
    Serial.begin(9600);
    Serial.println("RFID Servo Example");

    SPI.begin();
    rfid.PCD_Init();

    Serial.println("Waiting for an RFID card or tag during setup...");

    // Check for a readable RFID tag during setup
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
    {
        Serial.println("Found an RFID card or tag during setup!");
        Serial.print("UID Value: ");
        for (byte i = 0; i < rfid.uid.size; i++)
        {
            targetUID[i] = rfid.uid.uidByte[i]; // Set the detected tag's UID as the target UID
            Serial.print(targetUID[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");
    }

    myservo.attach(5); // Attach the servo to pin 3
    myservo.write(60); // Reset the servo position
    pinMode(buzzerPin, OUTPUT);
}

void loop()
{
    // Look for new RFID cards
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
    {
        Serial.println("Found an RFID card or tag!");
        Serial.print("UID Value: ");
        for (byte i = 0; i < rfid.uid.size; i++)
        {
            Serial.print(rfid.uid.uidByte[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");

        // Check if the detected RFID tag matches the target tag
        if (memcmp(rfid.uid.uidByte, targetUID, rfid.uid.size) == 0)
        {
            Serial.println("Matching tag found! Rotating the servo 45 degrees.");
            playZeldaChestSound1(); // Play the Legend of Zelda chest sound start
            myservo.write(145);     // Rotate the servo to 45 degrees
            playZeldaChestSound2(); // Play the Legend of Zelda chest sound start
            delay(1000);            // Wait for the servo to reach the position
            myservo.write(60);      // Reset the servo position
            delay(500);             // Delay before reading another tag
        }
    }
    rfid.PICC_HaltA();      // Halt PICC
    rfid.PCD_StopCrypto1(); // Stop encryption on PCD
}

int soundSplit = 6;

char notes[] = "gabygabyxzCDxzCDabywabywzCDEzCDEbywFCDEqywFGDEqi        azbC"; // a space represents a restsoundSplit
int length = sizeof(notes);                                                    // the number of notessoundSplit
int beats[] = {
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    2,
    3,
    3,
    16,
};
int tempo = 75;

void playTone(int tone, int duration)
{
    for (long i = 0; i < duration * 1000L; i += tone * 2)
    {
        digitalWrite(buzzerPin, HIGH);
        delayMicroseconds(tone);
        digitalWrite(buzzerPin, LOW);
        delayMicroseconds(tone);
    }
}

void playNote(char note, int duration)
{
    char names[] = {'c', 'd', 'e', 'f', 'g', 'x', 'a', 'z', 'b', 'C', 'y', 'D', 'w', 'E', 'F', 'q', 'G', 'i'};
    // c=C4, C = C5. These values have been tuned.
    int tones[] = {1898, 1690, 1500, 1420, 1265, 1194, 1126, 1063, 1001, 947, 893, 843, 795, 749, 710, 668, 630, 594};

    // play the tone corresponding to the note name
    for (int i = 0; i < 18; i++)
    {
        if (names[i] == note)
        {
            playTone(tones[i], duration);
        }
    }
}

void playZeldaChestSound1()
{
    for (int i = 0; i < length - soundSplit; i++)
    {
        if (notes[i] == ' ')
        {
            delay(beats[i] * tempo); // rest
        }
        else
        {
            playNote(notes[i], beats[i] * tempo);
        }

        // pause between notes
        delay(tempo / 2);
    }
}

void playZeldaChestSound2()
{
    for (int i = length - soundSplit; i < length; i++)
    {
        if (notes[i] == ' ')
        {
            delay(beats[i] * tempo); // rest
        }
        else
        {
            playNote(notes[i], beats[i] * tempo);
        }

        // pause between notes
        delay(tempo / 2);
    }
}
