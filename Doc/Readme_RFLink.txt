Please note that the RFLink Gateway is a freeware project.   
Stuntteam is not making money in any way.   
This means that there are no unlimited funds to purchase test devices,   
it also means the project has to rely on you, the user, to send debug data.  
  
If you want to contribute to this project, you can send a donation which is more than welcome (see www.nemcon.nl/blog2 donation button),   
or help with sending debug data of unsupported devices (you can even write and contribute plugins and/or code fixes),  
or donate the device that you would like to see supported.  
   
Right now we are looking for some older remotes and/or switches.  
Like for example: Impuls, Ikea Koppla, Powerfix, Blyss, Home Confort, Conrad, Kambrook, Everflourish, SelectRemote  
For the implementation of the planned 2.4Ghz support we could use some simple MySensor devices.   
For the implementation of the planned 868Mhz support we could use some devices as well.   
If you have anything that you do not use, send a mail to frankzirrone@gmail.com   
Thanks in advance!  

------------------------
R30: 
- New Device: Conrad 9771 Pool Thermometer
- New Device: SilverCrest Z31370-TX Doorbell
- New Device: Smartwares remote controls (among others: SH5-TDR-K 10.037.17) 
- New Device: Chuango Alarm devices Motion/Door/Window etc. (among others: CG-105S)
- New Device: Oregon Scientific NR868 PIR/night light
- New Device: Oregon Scientific MSR939 PIR
- New Device: Imagintronix Temperature/Soil humidity sensor
- New Device: Ikea Koppla
- New Device: Chacon (TR-502MSV, NR.RC402)
- Fixed: Arc protocol send
- Fixed: Impuls. Note: pair devices with the KAKU protocol, the remote is recognized separately. (Needs more tests!)
- Changed: Plugin 3 send method, combined routines
- Changed: HomeConfort was recognized as Impuls, now using GDR2 name
- Changed: HomeEasy remotes can deliver various signals, now skipping KAKU compatible signals and just reporting the HomeEasy code when both codes are transmitted
- Fixed: HomeEasy group on/off command was reversed for HE8xx devices, now correctly detects differences between HE3xx and HE8xx
- Fixed: HomeEasy was not able to control HE87x switches, changed the entire transmit routine
- Changed: stretched Xiron timing checks
- Changed: Various timing modifications (NewKaku/AC, Blyss) due to the new timing introduced at version R26
- Changed: Plugin 61, Chinese Alarm devices, reversed bits as it seemed to correspond better to bit settings, increased address range
- Fixed: Flamingo Smokedetector packet detection tightened up to prevent false positives
- Fixed: Corrected Conrad RSL command interpretation
- Added: Extended Nodo Slave support to support separate and combined sensors
- Added: Extended Nodo Slave support to support pulse meters
===
R29: 
- Fixed: AC/NewKaku high unit numbers were incorrect. 
         If you already have devices with high unit numbers in Domoticz, just throw them away and let them be recognized again
===
R28: 
- Fixed: FA20RF smoke detector transmit from Domoticz 
===
R27: 
- Added: OSV1 battery status 
- Fixed: OSV1 boundaries and removed some debug info 
- Fixed: Some plugins set an incorrect sampling rate divider value 
- Changed: AlectoV1 false positives filter was too agressive
===
R26:
- Added: QRFDEBUG command to do faster logging of undecoded data
- Added: VERSION command
- Added: Powerfix/Quigg switches
- Added: proper Lacrosse V3 WS7000 sensor support
- Changed: config file and plugin integration
- Changed: timeout and divider value
- Changed: Lacrosse V2 WS2300/WS3600 plugin number to get faster processing, changed various other parts as well
- Changed: Lacrosse V1 pulse duration checks
- Changed: various parts to improve speed
- Changed: Flamingo Smoke detector signal re-transmits from 8 to 10 times
- Added: Additional tests on Alecto V1 and Alecto V4 to filter out false positives
- Fixed: AC (NewKaku) protocol send for some device numbers
- Fixed: little bug in UPM code
- Fixed: Oregon wind speed reporting
- Fixed: Wind speed calculations
- Fixed: Wind direction reporting in all plugins
- Fixed: AlectoV3 humidity value displaying out of range values
- Fixed: OregonV1 decoding
===
R25:
- Fixed: Eurodomest address range check
- Fixed: Alecto V1 and V3 humidity handling
- Fixed: Lacrosse WS2300/WS3600 and labelled as LacrosseV2
===
R24: 
- Fixed: Flamingo Smoke Detector timings and device address usage
- Fixed: Timing for Nexa/Jula Anslut
===
R23: 
- Changed: Alecto V1 temperature data filtering
- Added: Alecto V1 battery status now shown for temperature sensors
===
R22: 
- Various additional tests and fixes after intensive tests
- Added: Home Confort send and recognition by Domoticz
===
R21: 
- Re-Activated PIR & Door/Window sensors (plugin 60/61) 
===
R20: 
- Switched to Arduino 1.6.5
===
R19: 
- Complete rewrite
- Added: Home Confort Smart Home - TEL-010
- Added: RGB LED Controller
- Added: RL-02 Digital Doorbell
- Added: Deltronic Doorbell
- Added: Sartano 2606 remote & switch
===
r18:
- Added Banggood SKU174397, Sako CH113, Homemart/Onemall FD030 and Blokker (Dake) 1730796 outdoor temperature sensor
- Tested Okay: Promax RSL366T, Profile PR-44N & PR-47N
- Fixed: LaCrosse humidity values are correctly displayed
- Fixed: Humidity values that originate from slave Nodos are correctly displayed
- Fixed: UPM/Esic insane temperature values are skipped
- Removed Xiron & Auriol debug data 
- Tightened pulse range on various protocols to prevent false positives
===
r17:
- Modified Oregon THGR228N code, 
- Modified Newkaku(AC) dim values, 
- Corrected support for KAKU door switches, 
- Fixed Nodo Slave sensors, 
- Improved speed and priorities so that group commands are properly transmitting
===
r16: 
- Fixed Aleco V1 temperature ID to match wind sensors
- Fixed HomeEasy transmit
- Added AC(NewKaku) dimmer support
===
r15:
- Improved large packet translation
===
r14: 
- Changed Motion sensors (60/61)
===
r13:
- Flamingo Smoke detector fix
- Added Xiron sensor support
===
r11/12:
- Mertik / Dru Send added
===
r10:
- Added Auriol Z32171A
===
r9:
- Fixed Kaku send with high device id's (P1 M1 etc)
===
r8:
- Improved descriptions
===
r7:
- Fixed Oregon RTGR328N ID and humidity format
- Fixed UPM/Esic humidity format
- Fixed Alecto humidity format
===
r6:
- Fixed Auriol V2 plugin
- Updated Auriol plugin
- Fixed Lacrosse Humidity
===
r1/2/3/4/5:
- Added X10 receive/transmit plugin
- Minor changes & improvements
===

Special thanks to:
Alex, Benoit, Bert, Christophe, Deennoo, Jelle, John, Jonas, Gerrit, Goran, Mark, Martinus, Maurice, Paul, Pim, Remco, Richard, Rob, Sebastien, Thibaut, William
and everyone who contributed with feedback, suggestions, debug data, tests etc.
