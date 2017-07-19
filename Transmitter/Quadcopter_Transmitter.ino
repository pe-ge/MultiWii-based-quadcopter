#include <SPI.h>
#include <RF24.h>

#define SERIAL_DEBUG true

RF24 radio(9,10);
const uint64_t writingPipe = 0xF0F0F0F0E1LL;

struct packet {
  uint8_t throttle;
  uint8_t yaw;
  uint8_t pitch;
  uint8_t roll;
} packet;

uint16_t pitch_mid = 537; // higher means more backward
uint16_t roll_mid = 492; // higher means more right

void setup() {
  radio.begin();
  radio.openWritingPipe(writingPipe);
  radio.enableDynamicPayloads();
  radio.setAutoAck(false) ;
  //radio.setDataRate(RF24_250KBPS);
  //radio.setPALevel(RF24_PA_MAX);
  //radio.setChannel(108);
  radio.powerUp();

  if (SERIAL_DEBUG) Serial.begin(57600);
}

void loop() {
  // 16 bit version
  /*packet.throttle = 1023 - analogRead(3);
  packet.yaw      = 1023 - analogRead(0);

  uint16_t pitch = analogRead(1);
  if (pitch < pitch_mid) {
    packet.pitch = map(pitch, 0, pitch_mid, 0, 512);
  } else {
    packet.pitch = map(pitch, pitch_mid, 1023, 512, 1023);
  }

  uint16_t roll = analogRead(2);
  if (roll < roll_mid) {
    packet.roll = map(roll, 0, roll_mid, 1023, 512);
  } else {
    packet.roll = map(roll, roll_mid, 1023, 512, 0);
  }*/

  packet.throttle = map(analogRead(3), 0, 1023, 255, 0);
  packet.yaw      = map(analogRead(0), 0, 1023, 255, 0);
  packet.pitch = map(analogRead(1), 0, 1023, 0, 255);
  packet.roll = map(analogRead(2), 0, 1023, 255, 0);

  radio.write(&packet, sizeof(packet));

  if (SERIAL_DEBUG) {
    Serial.print(packet.throttle);
    Serial.print(" ");
    Serial.print(packet.yaw);
    Serial.print(" ");
    Serial.print(packet.pitch);
    Serial.print(" ");
    Serial.print(packet.roll);
    Serial.print(" ");
    Serial.print(analogRead(1));
    Serial.print(" ");
    Serial.print(analogRead(2));
    Serial.println();
  }
}
