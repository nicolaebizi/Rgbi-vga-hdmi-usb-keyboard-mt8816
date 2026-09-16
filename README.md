# RGBI VGA/HDMI + USB Keyboard → MT8816

A LEO V3 RP2040 firmware project for converting ZX Spectrum RGBI video to VGA and HDMI/DVI, with USB keyboard input routed through an MT8816 analog switch matrix.

## Original projects and credits

This project builds on the work of the original ZX RGBI video projects:

- **Alex EKB / ZX_RGBI2VGA-HDMI** — original ZX Spectrum RGBI-to-VGA/HDMI hardware and software project.
- **Osemenyuk / zx-rgbi-to-vga-hdmi** — the upstream implementation from which the Pico SDK version was derived and adapted.
- **RGBI_TO_VGA** — related RGBI video conversion work by tchv71.
- **RGBtoHDMI** — RGB-to-digital conversion work by hoglet67, used as a reference in the video-input portion of the project.

The original Pico SDK project provided the foundation for the video converter, including the VGA/HDMI video path, timing infrastructure and OSD architecture.

## Our work — LEO V3 + MT8816

This repository documents the subsequent hardware adaptation and firmware work for the **LEO V3 RP2040** board.

The main changes are:

- Adapted the firmware for the **LEO V3 RP2040** GPIO layout.
- Preserved the working **VGA and HDMI/DVI video output path**.
- Replaced the previous **CH446Q keyboard-matrix backend** with direct **MT8816** control.
- Added **USB keyboard → ZX Spectrum 8×5 keyboard matrix → MT8816** handling.
- Added direct RP2040 GPIO control for the MT8816 address, data, strobe, chip-select and reset signals.
- Removed the legacy **EPM3256** keyboard/mouse interface from the active firmware.
- Removed legacy **Gotek drive-selection hardware control** and the associated Hardware Configuration menus.
- Removed legacy ROM-bank and RAM-size hardware configuration from the LEO V3 firmware.
- Disabled PS/2 keyboard and USB mouse functionality for this LEO V3 configuration.
- Kept the existing **I2C OSD interface** on GP18/GP19.
- Added a tested firmware UF2 under `firmware/`.
- Added the LEO V3 / MT8816 GPIO documentation under `docs/`.

## Current functionality

### Video

- ZX Spectrum RGBI input.
- VGA output using the existing supported video modes.
- HDMI/DVI output using the existing supported video modes.
- Stable VGA video path on LEO V3.
- Scanline support where available.
- `NO SIGNAL` indication when RGBI input is not detected.

### USB keyboard → MT8816

- USB keyboard support using TinyUSB Host.
- USB HID keyboard events are translated to the ZX Spectrum keyboard layout.
- ZX Spectrum keyboard matrix is represented as **8 rows × 5 columns**.
- Matrix changes are sent directly to the **MT8816** analog switch.
- MT8816 address/control signals are driven directly by the RP2040 GPIOs.
- Keyboard output uses the ZX Spectrum matrix columns X0–X4.

### MT8816 GPIO assignment — LEO V3

| MT8816 signal | RP2040 GPIO |
|---|---:|
| AX0 | GP7 |
| AX1 | GP28 |
| AX2 | GP17 |
| AX3 | GP20 |
| AY0 | GP21 |
| AY1 | GP22 |
| AY2 | GP23 |
| DATA | GP24 |
| STROBE | GP25 |
| CS | GP26 |
| RESET | GP27 |

**Reserved / existing LEO V3 functions:**

- **GP8–GP15:** VGA/DVI video output — must remain dedicated to video.
- **GP16:** onboard WS2812 RGB LED — reserved.
- **GP18:** I2C SDA — OSD.
- **GP19:** I2C SCL — OSD.
- **GP29:** unused in this configuration; PS/2 is disabled.

### OSD

- Existing OSD functionality remains available.
- I2C OSD uses:
  - SDA: **GP18**
  - SCL: **GP19**
- The legacy Gotek hardware-control/configuration functionality is removed from this firmware.
- The I2C OSD interface remains available and is independent of the MT8816 keyboard matrix.
- LEO V3 uses `NO_OSD_BUTTONS` for the physical-button configuration.

## Removed from the active LEO V3 firmware

The following legacy hardware/control functions are not part of this version:

- CH446Q keyboard matrix control.
- EPM3256 keyboard/mouse interface.
- Gotek drive-selection hardware control.
- Gotek keyboard-control mode.
- Gotek/FlashFloppy hardware configuration menus.
- ROM bank selection hardware menu.
- RAM-size hardware configuration menu.
- PS/2 keyboard support on LEO V3.
- USB mouse / Kempston mouse support.
- Legacy NMI/RESET switching through CH446Q/EPM3256.
- Physical OSD buttons on LEO V3.

## Firmware

The tested firmware is stored in:

`firmware/ZX_RGBI_TO_VGA_HDMI.uf2`

## Documentation

- [LEO V3 MT8816 GPIO](docs/LEO_V3_MT8816_GPIO_FINAL.pdf)
- [OSD Menu Guide](docs/OSD_MENU_GUIDE.md)
- [VGA Timings](docs/VGA_TIMINGS.md)
- [Keyboard Guide](docs/KEYBOARD_GUIDE.md)

## Build

The project uses the **Raspberry Pi Pico SDK** and is configured for the **LEO V3** board.

Example build flow:

```bash
mkdir -p build
cd build
cmake -G Ninja .. -DBOARD=BOARD_LEO_V3
ninja
```

The resulting firmware is generated as:

`build/ZX_RGBI_TO_VGA_HDMI.uf2`

## Checkpoint

**REUȘITA 1** is the current known-good checkpoint for the LEO V3 VGA video path and the MT8816 firmware integration.
