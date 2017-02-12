// - - - - -
// DmxSerial - A hardware supported interface to DMX.
// 
// Copyright (c) 2016 by Ashley Daminato
// This work is licensed under a TBD license
// 
// Documentation and samples for DMXSerial.h are available at http://www.mathertel.de/Arduino
// 25.07.2011 creation of the DmxSerial library.
// 10.09.2011 fully control the serial hardware register
//            without using the Arduino Serial (HardwareSerial) class to avoid ISR implementation conflicts.
// 01.12.2011 include file and extension changed to work with the Arduino 1.0 environment
// 28.12.2011 changed to channels 1..3 (RGB) for compatibility with the DmxSerialSend sample.
// 10.05.2012 added some lines to loop to show how to fall back to a default color when no data was received since some time.
// - - - - -

#include <DMXSerial.h>
#include <NeoPixelBus.h>

// Since we are useing a 32U device (ProMicro), the default serial port is used for USB, have to use an alternate serial port.
#define DMX_USE_PORT1 TRUE

const uint16_t PixelCount = 5; // this example assumes 4 pixels
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin

#define RedDefaultLevel   100
#define GreenDefaultLevel 0
#define BlueDefaultLevel  0

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
RgbColor thisColor;

void setup () {
    DMXSerial.init(DMXReceiver);

    // set some default values, first number corresponds to the DMX Channel. Second number is the value.
    DMXSerial.write(1, RedDefaultLevel);
    DMXSerial.write(2, GreenDefaultLevel);
    DMXSerial.write(3, BlueDefaultLevel);


    strip.Begin();
    strip.Show();

}


void loop() {
    // Calculate how long no data backet was received
    unsigned long lastPacket = DMXSerial.noDataSince();

    if (lastPacket < 10000) {
        thisColor = RgbColor(DMXSerial.read(1), DMXSerial.read(2), DMXSerial.read(3) );
    } else {
        thisColor = RgbColor(RedDefaultLevel, GreenDefaultLevel, BlueDefaultLevel);
    }

    for ( int thisPin = 0; thisPin < PixelCount; thisPin++) {  
        strip.SetPixelColor(thisPin, thisColor);
    }

    strip.Show();

}

// End.
