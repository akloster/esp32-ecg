# ESP32 ECG server

I created this project for my talk "Heartbeats for Hackers" at the Europython 2021. This directory contains a PlatformIO
project to upload to an ESP32. I used a board with an OLED Display and a LI-Ion battery holder, which can be uploaded to
by specifying the platform  as "lolin32", even though it technically does not even look like a clone of the "Wemos Lolin32"
board. If you change this setting in .platformio.ini, you should be able to use other ESP32 boards easily.

The program will wait for an incoming TCP/IP connection on port 3333, then it will poll an AD1105 ADC over I2C (0x48, ADDR
pin pulled low) 1000 times a second and send its data over the TCP/IP socket as raw binary buffers.

The hardware setup is realtively easy. You need one AD1105 and one AD8323 breakout boards. Both need to be connected to
GND. Make sure you use the +3.3V (often labeled VCC on the ESP32 boards) for the "V" or positive Voltage connection, not
the 5V. If you connect 5V, it may damage the data pins. ESP32 pin 22 is SDA and 21 is SCL, both need to be wired to the
respective pins on the AD1105.

It somewhat works on a breadboard. During my presentation I had a prototyping board where I still used female headers to
be able to change out the breakout boards, but I think this is the main reason why sometimes it worked and sometimes it didn't.

After the presentation I soldered them directly to a prototyping board and now it works a lot better.

I included "esp32_holder.amf/.stl/.FCStd" as a 3D model you can print if you want to prop up the board a little. You can also
easily change the parameters in the FreeCAD file.

This program currently doesn't use the builtin display. The display also uses I2C, and it would interfere. There are ways around
this, especially because the ESP32 can use two I2C busses, but I haven't gotten it to work yet...

## Danger!

Use this code at your own risk! Connecting electrodes to your body is potentially dangerous, so this is only a project for
those who know how not to connect the wrong pins to some dangerous voltage. Unless something catastrophic happens, a 3.3V
connection should not be harmful.

For added protection, only use battery power when the electrodes are connected, don't use the USB connection. This also means
less interference from the powerline in your computer.

You shouldn't use this code for medical diagnosis. This is prohibited in most jurisdictions, as such devices are usually
regulated and much more sophisticated. That being said, the ECG signal recorded by this setup can be quite good. The main issue
may be to translate back the ADC values to actual ECG voltage levels. This would certainly be necessary for proper ECG
interpretation.

Enjoy!
