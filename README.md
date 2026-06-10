# ESP32MotionSensorRelay

ESP32 motion sensor with relay control, TM1637 display and Telegram notifications (FastBot).

## Project structure (PlatformIO)

- `src/main.cpp` — firmware source
- `include/config.h.example` — credentials template
- `platformio.ini` — PlatformIO configuration
- `old/` — original Arduino IDE project files

## Build

1. Copy `include/config.h.example` to `include/config.h` and fill in your Wi-Fi and Telegram bot credentials.
2. Build and upload:

```sh
pio run                # build
pio run -t upload      # upload
pio device monitor     # serial monitor (115200 baud)
```

The default board is `esp32dev` — change `board` in `platformio.ini` if you use a different ESP32 module.
