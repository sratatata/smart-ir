# About

I've couple of IR remotes for my 5.1 audio system. Worst case is with TV and DVD.
Harman-Kardon remote for my audio set is so hard to use, that needs buttons to be pushed to make volume adjustments.

The idea behind this project is to simplify usage, and create some macros to operate system easier.

## Currently done:

* WifiManager added
* File System server for html + js is implemented
* OTA firmware uploads added.

## Todo:

* IR signals emmitting (IRLib2 is not working with esp8266)
* CSS design for web page
* JSON export for Android and Tizen applications.
* Circuit design
* Printable case design

If you are interested with similiar projects, please find: [Smart-Power](https://github.com/sratatata/smart-power).

1. Use WifiManager to set-up wifi connection.

# Installation:
2. Change ip in [platformio.ini](./platformio.ini) to your esp device IP.
3. Use platformio to upload program and file system.

## PlatformIO upload:

1. Program: ```pio run --target upload```
2. FS: ```pio run --target uploadfs```

## Dependencies:

* [WifiManager](https://github.com/tzapu/WiFiManager)
