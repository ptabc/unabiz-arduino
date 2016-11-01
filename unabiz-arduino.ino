//  Send sample SIGFOX messages with Arduino shield based on Radiocrafts RC1692HP-SIG.
//  Based on https://github.com/Snootlab/Akeru

#include "SIGFOX.h"

//  Create the SIGFOX library. Default to pin D4 for transmit, pin D5 for receive.
Radiocrafts transceiver;

void setup()
{
  //  Initialize console serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println(F("Demo sketch for SIGFOX transceiver library :)"));

  transceiver.echoOn();  //  Comment this line to hide the echoing of commands.
  //  Check whether the SIGFOX module is functioning.
  if (!transceiver.begin())
  {
    Serial.println(F("Error: SIGFOX Module KO!"));
    for(;;) {}  //  Loop forever because we can't continue.
  }
}

void loop()
{
  String result = "";
  //  Enter command mode.  TODO: Confirm response = '>'
  Serial.println(F("\nEntering command mode (expecting '>')..."));
  transceiver.enterCommandMode();

  //  Disable emulation mode.
  Serial.println(F("\nDisabling emulation mode..."));
  transceiver.disableEmulator(result);

  //  Check whether emulator is used for transmission.
  Serial.println(F("\nChecking emulation mode (expecting 0)...")); int emulator = 0;
  transceiver.getEmulator(emulator);

  //  Get network mode for transmission.  Should return network mode = 0 for uplink only, no downlink.
  Serial.println(F("\nGetting network mode (expecting 0)..."));
  transceiver.getParameter(0x3b, result);

  //  Get baud rate.  Should return baud rate = 5 for 19200 bps.
  Serial.println(F("\nGetting baud rate (expecting 5)..."));
  transceiver.getParameter(0x30, result);

  //  Set the frequency of SIGFOX module to SG/TW.
  Serial.println(F("\nSetting frequency..."));  result = "";
  transceiver.setFrequencySG(result);
  Serial.print(F("Set frequency result = "));  Serial.println(result);

  //  Get and display the frequency used by the SIGFOX module.  Should return 3 for RCZ4 (SG/TW).
  Serial.println(F("\nGetting frequency (expecting 3)..."));  String frequency = "";
  transceiver.getFrequency(frequency);
  Serial.print(F("Frequency (expecting 3) = "));  Serial.println(frequency);

  //  Read module temperature.
  Serial.println(F("\nGetting temperature..."));  int temperature = 0;
  if (transceiver.getTemperature(temperature))
  {
    Serial.print(F("Temperature = "));  Serial.print(temperature);  Serial.println(F(" C"));
  }
  else
  {
    Serial.println(F("Temperature KO"));
  }

  //  Read module supply voltage.
  Serial.println(F("\nGetting voltage..."));  float voltage = 0.0;
  if (transceiver.getVoltage(voltage))
  {
    Serial.print(F("Supply voltage = "));  Serial.print(voltage);  Serial.println(F(" V"));
  }
  else
  {
    Serial.println(F("Supply voltage KO"));
  }

  //  Read SIGFOX ID and PAC from module.
  Serial.println(F("\nGetting SIGFOX ID..."));  String id = "", pac = "";
  if (transceiver.getID(id, pac))
  {
    Serial.print(F("SIGFOX ID = "));  Serial.println(id);
    Serial.print(F("PAC = "));  Serial.println(pac);
  }
  else
  {
    Serial.println(F("ID KO"));
  }

#if NOTUSED
  //  Read module hardware version.
  Serial.println(F("\nGetting hardware "));  String hardware = "";
  if (transceiver.getHardware(hardware))
  {
    Serial.print(F("Hardware version = "));  Serial.println(hardware);
  }
  else
  {
    Serial.println(F("Hardware version KO"));
  }

  //  Read module firmware version.
  Serial.println(F("\nGetting firmware "));  String firmware = "";
  if (transceiver.getFirmware(firmware))
  {
    Serial.print(F("Firmware version = "));  Serial.println(firmware);
  }
  else
  {
    Serial.println(F("Firmware version KO"));
  }
#endif  //  NOTUSED

  //  Read power.
  Serial.println(F("\nGetting power..."));  int power = 0;
  if (transceiver.getPower(power))
  {
    Serial.print(F("Power level = "));  Serial.print(power);  Serial.println(F(" dB"));
  }
  else
  {
    Serial.println(F("Power level KO"));
  }

  //  Exit command mode.
  transceiver.exitCommandMode();

  //  Send temperature and voltage as a SIGFOX message.  Convert to hexadecimal before sending.
  String temp = transceiver.toHex(temperature);
  String volt = transceiver.toHex(voltage);
  String msg = temp + volt;

  //  Send 10 times.
  for (int i = 0; i < 10; i++) {
    Serial.println(F("\nSending payload "));
    if (transceiver.sendPayload(msg))
    {
      Serial.println(F("Message sent !"));
    }
    else
    {
      Serial.println(F("Message not sent !"));
    }
    delay(6000);
  }

  //  End of tests.  Loop forever.
  for(;;) {}
}

/*
Demo sketch for SIGFOX transceiver library :)
Radiocrafts.echoOn

Entering command mode (expecting '>')...
Radiocrafts.sendCommand: 00
>> 00 [3e]

Radiocrafts.sendCommand: response:
Radiocrafts.enterCommandMode: OK

Disabling emulation mode...
Radiocrafts.sendCommand: 4d2800
>> 4d 28 00 [3e]

Radiocrafts.sendCommand: response:
Radiocrafts.sendCommand: ff
>> ff [3e]

Radiocrafts.sendCommand: response:

Checking emulation mode (expecting 0)...
Radiocrafts.getParameter: address=0x28
Radiocrafts.sendCommand: 5928
>> 59 28 [3e]

Radiocrafts.sendCommand: response:
Radiocrafts.getParameter: address=0x28 returned

Getting network mode (expecting 0)...
Radiocrafts.getParameter: address=0x3b
Radiocrafts.sendCommand: 593b
>> 59 3b [3e]

Radiocrafts.sendCommand: response:
Radiocrafts.getParameter: address=0x3b returned

Getting baud rate (expecting 5)...
Radiocrafts.getParameter: address=0x30
Radiocrafts.sendCommand: 5930
>> 59 30 [3e]

Radiocrafts.sendCommand: response:
Radiocrafts.getParameter: address=0x30 returned

Setting frequency...
Radiocrafts.setFrequencySG
Radiocrafts.sendCommand: 4d0003
>> 4d 00 03 [3e]

Radiocrafts.sendCommand: response:
Radiocrafts.sendCommand: ff
>> ff [3e]

Radiocrafts.sendCommand: response:
Set frequency result =

Getting frequency (expecting 3)...
Radiocrafts.sendCommand: 5900
>> 59 00 [3e]

Radiocrafts.sendCommand: response:
Frequency (expecting 3) =

Getting temperature...
Radiocrafts.sendCommand: 55
>> 55 [a2] [3e]

Radiocrafts.sendCommand: response: a2
Radiocrafts.getTemperature: returned 97
Temperature = 97 C

Getting voltage...
Radiocrafts.sendCommand: 56
>> 56 [73] [3e]

Radiocrafts.sendCommand: response: 73
Radiocrafts.getVoltage: returned 55.00
Supply voltage = 55.00 V

Getting SIGFOX ID...
Radiocrafts.sendCommand: 39
>> 39 [68] [86] [1c] [00] [95] [22] [6f] [82] [2b] [cd] [7d] [3c] [3e]

Radiocrafts.sendCommand: response: 68861c0095226f822bcd7d3c
Radiocrafts.getID: returned id=001c8668, pac=95226f822bcd7d3c
SIGFOX ID = 001c8668
PAC = 95226f822bcd7d3c

Getting power...
Radiocrafts.getParameter: address=0x01
Radiocrafts.sendCommand: 5901
>> 59 01 [3e]

Radiocrafts.sendCommand: response:
Radiocrafts.getParameter: address=0x01 returned
Radiocrafts.getPower: returned 0
Power level = 0 dB
Radiocrafts.sendCommand: 58
>> 58

Radiocrafts.sendCommand: Error: No response

Sending payload
Radiocrafts.sendPayload: 610000005c42
Radiocrafts.sendCommand: 06610000005c42
>> 06 61 00 00 00 5c 42

Radiocrafts.sendCommand: Error: No response
Message not sent !

Sending payload
Radiocrafts.sendPayload: 610000005c42
Radiocrafts.sendCommand: 06610000005c42
>> 06 61 00 00 00 5c 42

Radiocrafts.sendCommand: Error: No response
Message not sent !

Sending payload
Radiocrafts.sendPayload: 610000005c42
Radiocrafts.sendCommand: 06610000005c42
>> 06 61 00 00 00 5c 42

Radiocrafts.sendCommand: Error: No response
Message not sent !

Sending payload
Radiocrafts.sendPayload: 610000005c42
Radiocrafts.sendCommand: 06610000005c42
>> 06 61 00 00 00 5c 42

Radiocrafts.sendCommand: Error: No response
Message not sent !

Sending payload
Radiocrafts.sendPayload: 610000005c42
Radiocrafts.sendCommand: 06610000005c42
>> 06 61 00 00 00 5c 42

Radiocrafts.sendCommand: Error: No response
Message not sent !

Sending payload
Radiocrafts.sendPayload: 610000005c42
Radiocrafts.sendCommand: 06610000005c42
>> 06 61 00 00 00 5c 42

Radiocrafts.sendCommand: Error: No response
Message not sent !

Sending payload
Radiocrafts.sendPayload: 610000005c42
Radiocrafts.sendCommand: 06610000005c42
>> 06 61 00 00 00 5c 42

Radiocrafts.sendCommand: Error: No response
Message not sent !

Sending payload
Radiocrafts.sendPayload: 610000005c42
Radiocrafts.sendCommand: 06610000005c42
>> 06 61 00 00 00 5c 42

Radiocrafts.sendCommand: Error: No response
Message not sent !

Sending payload
Radiocrafts.sendPayload: 610000005c42
Radiocrafts.sendCommand: 06610000005c42
>> 06 61 00 00 00 5c 42

Radiocrafts.sendCommand: Error: No response
Message not sent !
*/
