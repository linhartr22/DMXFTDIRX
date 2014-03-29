/* DMX FTDI Receiver
 *
 * Receive DMX Commands from Artemis SBS.
 *
 * If you found this fun or interesting please make a small donation to my PayPal account 
 * at: https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=U48JEW8HSQFM2 
 * I have many more Arduino projects in mind and appreciate your support.
 *
 * No commercial use without prior consent.
 *
 */

// DMX Serial Library 
#include <DMXSerial.h>

// Artemis DMX Indexes (See %ProgramFiles(x86)%\Artemis\dat\DMXcommands.xml)
const int DMXLEDRedAIndex = 0 + 1;
const int DMXLEDGrnAIndex = 1 + 1;
const int DMXLEDBluAIndex = 2 + 1;
const int DMXLEDRedBIndex = 3 + 1;
const int DMXLEDGrnBIndex = 4 + 1;
const int DMXLEDBluBIndex = 5 + 1;
const int DMXRAIndex = 6 + 1;
 
// Common Anode RGB LED Output Pins
#define LEDRedAPin 9
#define LEDGrnAPin 10
#define LEDBluAPin 11
#define LEDRedBPin 3
#define LEDGrnBPin 5
#define LEDBluBPin 6

// LED Default PWM 
#define LEDRedDefPWM 5
#define LEDGrnDefPWM 5
#define LEDBluDefPWM 5

// Init LED PWM
int LEDRedAPWM, LEDGrnAPWM, LEDBluAPWM;
int LEDRedBPWM, LEDGrnBPWM, LEDBluBPWM;

// Red Alert Beacon Output Pin
#define RABeaconPin 7

// Init Red Alert Beacon
boolean RABeacon = false;

// setup() runs once after reset. 
void setup () {
  // Init DMX Serial
  DMXSerial.init(DMXReceiver);

/* 
  // Init DMX LED
  DMXSerial.write(DMXLEDRedAIndex, LEDRedDefPWM);
  DMXSerial.write(DMXLEDGrnAIndex, LEDGrnDefPWM);
  DMXSerial.write(DMXLEDBluAIndex, LEDBluDefPWM);
  DMXSerial.write(DMXLEDRedBIndex, LEDRedDefPWM);
  DMXSerial.write(DMXLEDGrnBIndex, LEDGrnDefPWM);
  DMXSerial.write(DMXLEDBluBIndex, LEDBluDefPWM);
  
  // Init DMX Red Alert Beacon
  DMXSerial.write(DMXRAIndex, int(RABeacon));
 */

  // Enable LED Output Pins
  pinMode(LEDRedAPin, OUTPUT);
  pinMode(LEDGrnAPin, OUTPUT);
  pinMode(LEDBluAPin, OUTPUT);
  pinMode(LEDRedBPin, OUTPUT);
  pinMode(LEDGrnBPin, OUTPUT);
  pinMode(LEDBluBPin, OUTPUT);
  
  // Enable Red Alert Beacon Output Pin
  pinMode(RABeaconPin, OUTPUT);
  
  // Test
  for (byte x = 0; x < 4; x++) {
    // Toggle Red Alert Beacon
    RABeacon = !RABeacon;
    digitalWrite(RABeaconPin, !RABeacon);

    // Cycle Colors
    for (byte y = 0; y < 4; y++) {
      switch (y) {
        case 0:
          // Red
          analogWrite(LEDRedAPin, 0);
          analogWrite(LEDGrnAPin, 255);
          analogWrite(LEDBluAPin, 255);
          analogWrite(LEDRedBPin, 0);
          analogWrite(LEDGrnBPin, 255);
          analogWrite(LEDBluBPin, 255);
          break;
        case 1:
          // Green
          analogWrite(LEDRedAPin, 255);
          analogWrite(LEDGrnAPin, 0);
          analogWrite(LEDBluAPin, 255);
          analogWrite(LEDRedBPin, 255);
          analogWrite(LEDGrnBPin, 0);
          analogWrite(LEDBluBPin, 255);
          break;
        case 2:
          // Blue
          analogWrite(LEDRedAPin, 255);
          analogWrite(LEDGrnAPin, 255);
          analogWrite(LEDBluAPin, 0);
          analogWrite(LEDRedBPin, 255);
          analogWrite(LEDGrnBPin, 255);
          analogWrite(LEDBluBPin, 0);
          break;
        case 3:
          // White
          analogWrite(LEDRedAPin, 0);
          analogWrite(LEDGrnAPin, 0);
          analogWrite(LEDBluAPin, 0);
          analogWrite(LEDRedBPin, 0);
          analogWrite(LEDGrnBPin, 0);
          analogWrite(LEDBluBPin, 0);
          break;
      }
      delay(1000); 
    }
  }
  // Red Alert Beacon Off
  RABeacon = false;
  digitalWrite(RABeaconPin, !RABeacon);
}
 
// Runs continuously after setup()
void loop() {
  // Get Last Packet Time
  unsigned long lastPacket = DMXSerial.noDataSince();

  // Time Since Last Packet < 5 seconds?
  if (lastPacket < 5000) {
    // Yes, Read DMX LED Values
    LEDRedAPWM = DMXSerial.read(DMXLEDRedAIndex);
    LEDGrnAPWM = DMXSerial.read(DMXLEDGrnAIndex);
    LEDBluAPWM = DMXSerial.read(DMXLEDBluAIndex);
    LEDRedBPWM = DMXSerial.read(DMXLEDRedBIndex);
    LEDGrnBPWM = DMXSerial.read(DMXLEDGrnBIndex);
    LEDBluBPWM = DMXSerial.read(DMXLEDBluBIndex);
    RABeacon = boolean(DMXSerial.read(DMXRAIndex));
  }
  else {
    // No, Revert to Default Color
    LEDRedAPWM = LEDRedDefPWM;
    LEDGrnAPWM = LEDGrnDefPWM;
    LEDBluAPWM = LEDBluDefPWM;
    LEDRedBPWM = LEDRedDefPWM;
    LEDGrnBPWM = LEDGrnDefPWM;
    LEDBluBPWM = LEDBluDefPWM;
    RABeacon = false;
  }

  // Refresh LED PWMs
  analogWrite(LEDRedAPin, 255 - LEDRedAPWM);
  analogWrite(LEDGrnAPin, 255 - LEDGrnAPWM);
  analogWrite(LEDBluAPin, 255 - LEDBluAPWM);
  analogWrite(LEDRedBPin, 255 - LEDRedBPWM);
  analogWrite(LEDGrnBPin, 255 - LEDGrnBPWM);
  analogWrite(LEDBluBPin, 255 - LEDBluBPWM);

  // Update Red Alert Beacon
  digitalWrite(RABeaconPin, !RABeacon);
}

