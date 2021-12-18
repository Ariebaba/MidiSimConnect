# MidiSimConnect
Use your midi device as a keyboard/joystick for Microsoft Flightsimulator 2020

![MidiSimConnect](https://github.com/Ariebaba/MidiSimConnect/blob/main/Github/MidiSimConnect.jpg)

### Dependencies ###

You need to have Microsoft Flightsim 2020 installed and also have the SDK installed.

I used Qt Creator to program this app. You don't need to have Qt installed to run it, but I think you need to have Visual Studio installed. I need more time for testing.

This program uses the MobiFlight Wasm Module. You can get it by installing MobiFlight

https://www.mobiflight.com/en/index.html

### Setup ###

If you want to compile the program yourself, I used Qt Creator with Qmake and Visual Studio 2019 as the compiler.

You can also download the zip file in the executable folder, extract the file and run MidiSimConnect.exe 

In the future I want to see if I can make it a bit more VS independent.

### Usage ###

If you run the program for the first time, the program will create a folder called config. In this folder it will create two files; config.cfg and program.ide.

Managing config files is hopefully self explanatory. 

To assign keys

![Assign](https://github.com/Ariebaba/MidiSimConnect/blob/main/Github/AssignMidiToSimCommands.jpg)

Press and hold (or release) and click Change Assignment. The 'Assign midid to sim commands' window pops up. Midi keys are expressed in bytes. Check the bytes and fill in the MSFS Command. You can find the commands on the following website

https://hubhop.mobiflight.com/#/

Press assign to link the button or encoder to the sim command. This is also the way to unsign links.

Keep the program running in the background while flying.

This is the way I setup up my midi device

![MidiDevice](https://github.com/Ariebaba/MidiSimConnect/blob/main/Github/AkaiMPKMiniIIIProgramEditor.jpg)

### Feedback ###

Any feedback is appreciated. Support MobiFlight.. they did the heavy lifting!
