# RGBI VGA/HDMI + Tastatură USB → MT8816

## Despre proiect

Proiect firmware pentru placa **LEO V3 RP2040**, care convertește semnalul video ZX Spectrum RGBI către VGA și HDMI/DVI și adaugă intrare de la tastatură USB printr-o matrice de comutatoare analogice **MT8816**.

## Proiectele originale și creditele

Acest proiect pornește de la proiectele originale pentru conversia video ZX RGBI:

- **Alex EKB / ZX_RGBI2VGA-HDMI** — proiectul original hardware și software pentru conversia ZX Spectrum RGBI către VGA/HDMI.
- **Osemenyuk / zx-rgbi-to-vga-hdmi** — implementarea upstream din care a fost derivată și adaptată versiunea pentru Pico SDK.
- **RGBI_TO_VGA** — proiect de conversie RGBI asociat, realizat de tchv71.
- **RGBtoHDMI** — proiect de conversie RGB către semnal digital realizat de hoglet67, folosit ca referință pentru partea de intrare video.

Proiectul original pentru Pico SDK a oferit baza pentru convertorul video, inclusiv calea VGA/HDMI, infrastructura pentru timpii video și arhitectura OSD.

## Ce am făcut noi — LEO V3 + MT8816

Adaptarea hardware și modificările firmware pentru placa **LEO V3 RP2040** includ:

- Adaptarea firmware-ului pentru configurația GPIO a plăcii LEO V3.
- Păstrarea căii video funcționale **VGA și HDMI/DVI**.
- Înlocuirea backend-ului anterior **CH446Q** cu control direct al circuitului **MT8816**.
- Implementarea traseului **tastatură USB → matrice ZX Spectrum 8×5 → MT8816**.
- Control direct din GPIO-urile RP2040 pentru adresa, datele, STROBE, CS și RESET ale MT8816.
- Eliminarea interfeței legacy **EPM3256** pentru tastatură/mouse.
- Eliminarea controlului hardware legacy pentru **Gotek** și a meniurilor Hardware Configuration asociate.
- Eliminarea configurației hardware pentru ROM bank și dimensiunea RAM.
- Dezactivarea tastaturii PS/2 și a mouse-ului USB pentru configurația LEO V3.
- Păstrarea interfeței **I2C pentru OSD** pe GP18/GP19.
- Adăugarea firmware-ului UF2 testat în `firmware/`.
- Adăugarea documentației GPIO LEO V3 + MT8816 în `docs/`.

## Funcționalitatea actuală

### Video

- Intrare ZX Spectrum RGBI.
- Ieșire VGA cu modurile video suportate existente.
- Ieșire HDMI/DVI cu modurile video suportate existente.
- Cale VGA stabilă pe LEO V3.
- Suport pentru scanlines acolo unde este disponibil.
- Indicație `NO SIGNAL` când intrarea RGBI nu este detectată.

### Tastatură USB → MT8816

- Tastatură USB prin TinyUSB Host.
- Evenimentele HID sunt convertite în layout-ul tastaturii ZX Spectrum.
- Matricea ZX Spectrum este reprezentată ca **8 rânduri × 5 coloane**.
- Modificările matricei sunt transmise direct către **MT8816**.
- Semnalele de adresă și control sunt comandate direct de GPIO-urile RP2040.
- Sunt utilizate coloanele X0–X4 ale matricei ZX Spectrum.

## Alocarea GPIO pentru MT8816 — LEO V3

| Semnal MT8816 | RP2040 GPIO |
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

### GPIO rezervate / existente

- **GP8–GP15:** ieșire video VGA/DVI — rămân dedicate video.
- **GP16:** LED RGB onboard WS2812 — rezervat.
- **GP18:** I2C SDA — OSD.
- **GP19:** I2C SCL — OSD.
- **GP29:** neutilizat în această configurație; PS/2 este dezactivat.

## OSD

- Funcționalitatea OSD existentă rămâne disponibilă.
- OSD folosește I2C:
  - SDA: **GP18**
  - SCL: **GP19**
- Funcționalitatea legacy de control/configurare hardware Gotek a fost eliminată.
- Interfața I2C pentru OSD este independentă de matricea MT8816.
- LEO V3 este configurat cu `NO_OSD_BUTTONS` pentru configurația fără butoane fizice OSD.

## Funcții eliminate

- Controlul matricei de tastatură prin CH446Q.
- Interfața EPM3256 pentru tastatură/mouse.
- Controlul hardware pentru selectarea unității Gotek.
- Modul de control Gotek din tastatură.
- Meniurile Hardware Configuration pentru Gotek/FlashFloppy.
- Meniul hardware pentru selectarea ROM bank.
- Meniul hardware pentru configurarea dimensiunii RAM.
- Suportul pentru tastatură PS/2 pe LEO V3.
- Mouse USB / mouse Kempston.
- Comutarea legacy NMI/RESET prin CH446Q/EPM3256.
- Butoanele fizice OSD pe LEO V3.

## Firmware

Firmware-ul UF2 testat se află în:

`firmware/ZX_RGBI_TO_VGA_HDMI.uf2`

## Documentație

- [LEO V3 MT8816 GPIO](LEO_V3_MT8816_GPIO_FINAL.pdf)
- [Ghidul meniului OSD](OSD_MENU_GUIDE.md)
- [Timpi VGA](VGA_TIMINGS.md)
- [Ghidul tastaturii](KEYBOARD_GUIDE.md)

## Compilare

Proiectul folosește **Raspberry Pi Pico SDK** și este configurat pentru placa **LEO V3**.

```bash
mkdir -p build
cd build
cmake -G Ninja .. -DBOARD=BOARD_LEO_V3
ninja
```

Firmware-ul rezultat este:

`build/ZX_RGBI_TO_VGA_HDMI.uf2`

## REUȘITA 1

**REUȘITA 1** este checkpoint-ul actual cunoscut ca funcțional pentru calea video VGA pe LEO V3 și integrarea firmware-ului MT8816.
