This file is used to list all important information about the Chip8 architecture.
Please bear in mind that this document is a personal notepad. It is by no means an
official document that can be relied upon.

https://en.wikipedia.org/wiki/CHIP-8#Virtual_machine_description
http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

That being said, most of this information should be correct. Feel free to use it
as you see fit. Just bear in mind that I may have made some mistakes while writing this.

- Tahar Meijs

=================
=== REGISTERS ===
=================

The address register of the Chip8 is called "I" and is 16 bits wide.
The program counter, "PC", is 16 bits wide as well.

Chip8 has 16 registers:
- V0
- V1
- V2
- V3
- V4
- V5
- V6
- V7
- V8
- V9
- VA
- VB
- VC
- VD
- VE
- VF

The "VF" register is special, as it doubles as a flag:
- Addition              ==> carry flag
- Subtraction           ==> no borrow flag
- Rendering / drawing   ==> set upon pixel collision (see the graphics paragraph)

Therefore, using this register as a regular register should be avoided.

==============
=== MEMORY ===
==============

- The Chip8 has 4096 memory locations (0x1000), each 1 byte wide.
- The first 512 bytes (0x200) is where the Chip8 interpreter used to live.
  Nowadays, this memory is used for additional (font) information, as emulators
  usually run outside fo the 4k memory space.
- Because of this reserved memory for the interpreter, almost every program
  starts at location 0x200 (512 bytes into the memory block).
- The last 256 bytes in memory (0xF00 to 0xFFF) are reserved for display refresh.
- The 96 bytes below the display refresh reserved memory is reserved for
  call stack / internal use / other variables.

=============
=== STACK ===
=============

- The stack only stores subroutine return addresses.
- Originally, 48 bytes were allocated for the stack, allowing up to 24 levels of
  nesting to be stored. These days, the stack stores at least 16 levels.

==============
=== TIMERS ===
==============
- Chip8 has two timers, a delay timer and a sound timer. Both count down at a speed
  of 60 Hz and stop when they reach zero.
- Sound timer is used to play sound effects. A beep is played when the timer value is non-zero.
- Delay timer is used in games. The application has read / write access to this timer.

================
=== GRAPHICS ===
================

- Monochrone black / white display.
- Resolution of 64x32 pixels.
- Sprites are 8 pixels wide and 1 to 15 pixels tall.
- Sprite pixels are XOR'd with corresponding screen pixels.
  Sprite pixels that are set flip the color of the corresponding screen pixel, while
  unset sprite pixels do nothing. The carry flag (VF register) is set to 1 when any
  pixels are flipped from set to unset when drawing a sprite. Otherwise it is set to 0.
  This is used for collision detection.

=============
=== INPUT ===
=============
- Hexadecimal key input.
- Key values are in the range 0x0 to 0xF.
- Usually, keys 2 / 4 / 6 / 8 are used for directional input.
- Three OpCodes are used to detect input:
    - One skips an instruction when a specific key is pressed.
    - One skips an instruction when a specific key is released.
    - One waits for a key press and stores it in one of the data registers.

===============
=== OpCodes ===
===============

An OpCode is 2 bytes wide, this means that when reading from memory, we have
to combine two bytes into one word / short / 16-bit data structure.

Combining 0xA4 and 0x5F:
    1010 0100   0xA4
    0101 1111   0x5F

    Shifting 0xA4 8 to the left (<<) gives:
        1010 0100 0000 0000     0xA4 << 8
    
    Now apply bit-wise OR (|) to combine 0xA4 and 0x5F:
        1010 0100 0000 0000
        0000 0000 0101 1111
        ------------------- |
        1010 0100 0101 1111

Decoding OpCode 0x9C1B:
    - You can quickly narrow down the OpCode type by looking at the
      number that comes right after the "0x", e.g.: 9.
    
    Retrieving the left-most value of the OpCode can be done by
    using a bit-wise AND (&) operation:
        1001 1101 0001 1011     0x9C1B
        1111 0000 0000 0000     0xF000
        ------------------------------ &
        1001 0000 0000 0000     0x9000
    
    Using this information, we can conclude that the OpCode we are
    dealing with is "9XY0". This skips the next instruction if VX doesn't equal VY.
    (Usually the next instruction is a jump to skip a code block).

Decoding OpCode 0x8FE2:
    Get the first number after "0x":
        1000 1111 1110 0010     0x8FE2
        1111 0000 0000 0000     0xF000
        ------------------------------ &
        1000 0000 0000 0000     0x8000
    
    The result is an OpCode starting with the number 8 right after "0x".
    Looking at the OpCode table for the Chip8 processor, we can conclude that
    the OpCode we are looking for is one of the following codes:
    - 8XY0
    - 8XY1
    - 8XY2
    - 8XY3
    - 8XY4
    - 8XY5
    - 8XY6
    - 8XYE

    Masking out all numbers but the right-most number will give us the OpCode:
        1000 1111 1110 0010     0x8FE2
        0000 0000 0000 1111     0x000F
        ------------------------------ &
        0000 0000 0000 0010     0x0002
    
    Now we know that we were looking for OpCode "8XY2", which shifts VY left by
    one and copies the result to VX. VF is set to the value of the most significant
    bit of VY before the shift.