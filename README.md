nand2tetris
===========

The nand2tetris software projects. I chose not to include the the hdl files I
created in the first 5 chapters since they can be trivial at times and they can
only be used with the simulator provided by nand2tetris anyway.

I am however including the assembler and the rest of the projects here because
they're no longer so specific to the book and much more complicated.

Assembler
=========
This is the first ANSI C program I've written, I'm used to C++ but I decided to
learn C and write my assembler in it. It runs in two passes, first building the
symbol table and then in the second pass replacing the symbols with constants
and actually assembling it.
