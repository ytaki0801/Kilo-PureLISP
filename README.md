# Kilo with C version of PureLISP.sh

This software is [Kilo Texteditor](https://github.com/antirez/kilo) with a Pure LISP interpreter, C version of [PureLISP.sh](https://github.com/ytaki0801/PureLISP.sh), including operations of last S-expression evaluating and parenthese matching.

The source code to compile Kilo is supposed to be from a tutorial site, [Build Your Own Text Editor](https://viewsourcecode.org/snaptoken/kilo/) Step 130, which is copied and arranged as `kilo-BYOTE130.c` and `kilo-BYOTE130.h` in this repository.

## How to Compile and Use

Just type `make` to compile in this repository, on a standard POSIX system including C compiler and library with Make, as well as `cc -DPURE_LISP_LIB -o kplisp kplisp.c kilo-BYOTE130.c PureLISP.c`

Type `./kplisp <filename>` to use. Parentheses matching is done automatically, Ctrl-e is to eval-last-sexp, [Ctrl-s is to save and Ctrl-q is to quit](https://github.com/antirez/kilo).

<!-- ![demo](https://github.com/ytaki0801/kilo-lisp/blob/master/kilo-lisp.gif) -->

For the `PureLISP.sh` specification, see [PureLISP.sh repository](https://github.com/ytaki0801/PureLISP.sh) and `sample.plsh`.

## TODO

* Full implemention of eval error handling in the interpreter.

## License

* Kilo: [BSD 2-Clause "Simplified" License](https://github.com/snaptoken/kilo-src/blob/master/LICENSE)

* Source codes in this repository except Kilo: [Public Domain by CC0, Creative Commons Zero v1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/)

