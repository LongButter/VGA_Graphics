# VGA_Graphics

A small collection of C programs for drawing graphics in **VGA Mode 13h** (320×200, 256 colours) on DOS. The files build on each other, starting with slow BIOS pixel plotting and working up to direct video memory writes and Bresenham-style line drawing.

## Contents

| File | What it does |
| --- | --- |
| `VGA_TEST.C` | Baseline. Plots pixels through the BIOS (`int 0x10`, function `0Ch`) and sweeps a red vertical line across the screen. Simple, but slow. |
| `VGA_EFFI.C` | Faster version of the same sweep. Writes straight to video memory at `0xA0000000` instead of calling the BIOS for every pixel. |
| `VGA_CONT.C` | Adds `fast_line()`, an integer-only Bresenham line routine, and draws a yellow, magenta, light red and green box outline. |
| `VGA_LINE.C` | Extends `fast_line()` to accept endpoints in either order. Draws the box plus a few diagonal test lines. |
| `BITMAP.C` | Cleaned-up `line()` routine used to fill the screen with a red and cyan column pattern. |

## How it works

**Entering graphics mode.** `init_vga()` calls BIOS interrupt `0x10` with `AH = 0x00` and `AL = 0x13` to switch to Mode 13h.

**Plotting a pixel.** There are two approaches in the repo:

1. *BIOS call* (`VGA_TEST.C`): `int 0x10` with `AH = 0x0C`, colour in `AL`, column in `CX`, row in `DX`.
2. *Direct memory write* (everything else): Mode 13h maps the screen to a linear buffer at segment `0xA000`, one byte per pixel, so a pixel's address is `y * 320 + x`.

```c
byte far *vga = (byte far *)0xA0000000L;
vga[SCREEN_WIDTH * y + x] = colour;
```

**Drawing lines.** `fast_line()` uses Bresenham's algorithm with only integer arithmetic. A decision variable `d` is updated each step to decide whether to move straight along x or step diagonally in y. Vertical lines (`dx == 0`) have their own branch.

**Colours.** The first 16 palette entries are named in an `enum`: `VGA_BLACK`, `VGA_BLUE`, `VGA_GREEN`, `VGA_CYAN`, `VGA_RED`, `VGA_MAGENTA`, `VGA_BROWN`, `VGA_LGRAY`, `VGA_DGRAY`, `VGA_LBLUE`, `VGA_LGREEN`, `VGA_LCYAN`, `VGA_LRED`, `VGA_LMAGENTA`, `VGA_YELLOW`, `VGA_WHITE`.

## Requirements

The code uses 16-bit DOS conventions (`far` pointers, `int86()`, `<dos.h>`, `<conio.h>`, `void main()`), so it targets an old DOS compiler such as **Turbo C / Borland C++**. It will not compile with modern GCC or Clang as-is.

You'll need:

- A DOS C compiler (Turbo C or Borland C++)
- A DOS environment, either real hardware or an emulator such as [DOSBox](https://www.dosbox.com/)

## Building and running

Using Turbo C under DOSBox:

1. Mount the project folder in DOSBox, e.g. `mount c C:\path\to\VGA_Graphics`, then `c:`.
2. Compile a file from the command line, for example:
   ```
   tcc VGA_LINE.C
   ```
   Or open it in the Turbo C IDE and press `Ctrl+F9` to build and run.
3. Run the executable:
   ```
   VGA_LINE.EXE
   ```

Most programs wait for a keypress (`getch()`) before exiting. Note that `VGA_TEST.C` and `VGA_EFFI.C` do not wait and will return to text mode immediately after drawing, so you may want to add a `getch();` at the end of `main()` to see the result.

## Known limitations

- `fast_line()` only steps `y` upward by one, so it is correct for lines with a slope between 0 and 1 (plus vertical lines). Steep or downward-sloping lines will not render correctly yet.
- `plot_pixel()` does no bounds checking, so coordinates outside 0–319 / 0–199 will write to the wrong memory.
- In `VGA_EFFI.C`, `SCREEN_HEIGHT` is defined as `320` and used as the row stride. It works, but the name is misleading, since the stride is the screen *width*.
- The programs do not restore text mode (`AL = 0x03`) on exit.

## Ideas for next steps

- Handle all octants in the line routine
- Add bounds checking to `plot_pixel()`
- Circles and filled shapes
- Custom palettes via the VGA DAC ports
- Restore text mode on exit
