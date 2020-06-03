# x16-Columns
A work-in-progress remake of the classic Sega game Columns on the Commander X16.

This game is written in C using the CC65 toolchain.  It uses Chris Love's [libX16](https://github.com/CJLove/libX16) library.

# Compiling x16-Columns
I use MSYS2/mingw-w64 to build the game.  The following instructions assume the cc65 toolchain is included in your PATH.

1. Build libX16
```
cd [path to x16-Columns]/submodules/libX16/src/
make
```
2. Build x16-Columns
```
cd [path to x16-Columns]
make
```
This will build the program in the "out" folder and copy the necessary .bin files.

3. Run x16-Columns
```
cd [path to x16-Columns]/out
[path to x16emu]/x16emu.exe -prg COLUMNS.PRG
```

# Playing x16-Columns
This game uses joystick controls.  On my system, the X16 emulator maps these to the keyboard arrow keys and the CTRL key for button 1.

The objective of Columns is to line up three or more of the same gem horizontally, vertically, or diagonally:
![In-game screenshot](https://github.com/kcowolf/x16-columns/blob/master/COLUMNS.GIF)

# License
x16-Columns is released under the MIT license.  libX16 is used under the Apache License 2.0.  For artwork, see gfx/credit.txt in the source code.

# Note
This is a fan-made remake.  It is not endorsed, produced, or affiliated with Sega or any of its subsidiaries.