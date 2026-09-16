# RGBI VGA/HDMI + USB Keyboard → MT8816

Firmware for the **LEO V3 RP2040** board that converts ZX Spectrum RGBI video to VGA and HDMI/DVI and adds USB keyboard input through an **MT8816 analog switch matrix**.

This project is based on the original `zx-rgbi-to-vga-hdmi-PICOSDK` project and has been adapted for the LEO V3 hardware and the MT8816 keyboard matrix.

## Current functionality

### Video

- ZX Spectrum RGBI input.
- VGA output with the existing supported video modes.
- HDMI/DVI output with the existing supported video modes.
- Stable VGA video path on LEO V3.
- Scanline support where available.
- `NO SIGNAL` indication when RGBI input is not detected.

### USB keyboard → MT8816

- USB keyboard support using TinyUSB Host.
- USB HID keyboard events are translated to the ZX Spectrum keyboard layout.
- ZX Spectrum keyboard matrix is represented as **8 rows × 5 columns**.
- Matrix changes are sent directly to the **MT8816** analog switch.
- MT8816 address/control signals are driven directly by the RP2040 GPIOs.
- Keyboard output uses the required ZX Spectrum matrix columns (X0–X4).
- The previous CH446Q keyboard-matrix backend has been removed from the active firmware.

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

**Important:** GP8–GP15 remain dedicated to the VGA/DVI video output. GP16 is reserved for the onboard RGB LED. GP18/GP19 remain dedicated to I2C OSD.

### OSD

- Existing OSD functionality remains available.
- OSD uses the LEO V3 I2C interface:
  - SDA: GP18
  - SCL: GP19
- Previous Gotek/FlashFloppy hardware-control functionality has been removed from this firmware.
- The I2C OSD interface remains part of the project and is independent of the MT8816 keyboard matrix.
- LEO V3 is configured without physical OSD buttons (`NO_OSD_BUTTONS`).

### Configuration

- Settings are stored in flash.
- Serial configuration remains available where enabled by the firmware.
- ZX Spectrum capture-frequency presets remain available.

## Removed from this firmware

The following legacy hardware/control features are no longer part of the active LEO V3 firmware:

- CH446Q keyboard matrix control.
- EPM3256 keyboard/mouse interface.
- Gotek drive-selection hardware control.
- Gotek keyboard-control mode (F10).
- Gotek/FlashFloppy hardware configuration menus.
- ROM bank selection menu.
- RAM-size hardware configuration menu.
- Three physical OSD buttons on LEO V3.
- PS/2 keyboard support on LEO V3.
- USB mouse / Kempston mouse support.
- Legacy NMI/RESET switching through CH446Q/EPM3256.

## Firmware

The tested UF2 firmware is stored in:

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

**REUȘITA 1** is the current known-good integration checkpoint for the LEO V3 VGA path and the MT8816 firmware integration.
