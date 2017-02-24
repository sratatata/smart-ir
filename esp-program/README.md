# About
ESP firmware.
More details soon.

# Installation:

1. Use WifiManager to set-up wifi connection.
2. Change ip in [platformio.ini](./platformio.ini) to your esp device IP.
3. Use platformio to upload program and file system.

## PlatformIO upload:

1. Program: ```pio run --target upload```
2. FS: ```pio run --target uploadfs```

## Dependencies:

* [WifiManager](https://github.com/tzapu/WiFiManager)
