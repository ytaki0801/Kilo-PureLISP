# Kilo with C version of PureLISP.sh

This software is [Kilo texteditor](https://github.com/antirez/kilo) with a Pure LISP interpreter, C version of [PureLISP.sh](https://github.com/ytaki0801/PureLISP.sh), including operations of last S-expression evaluating and parenthese matching.

The source code to compile Kilo is supposed to be from a tutorial site, [Build Your Own Text Editor](https://viewsourcecode.org/snaptoken/kilo/) Step 130, which is copied and arranged as `kilo-BYOTE130.c` and `kilo-BYOTE130.h` in this repository.

## How to Compile and Use

* Docker image: `docker run --rm -it ytaki0801/kplisp` for amd64, arm64/v8 and arm/v7

Just type `make` to compile in this repository, on a standard POSIX system including C compiler and library with Make, as well as `cc -DPURE_LISP_LIB -o kplisp kplisp.c kilo-BYOTE130.c PureLISP.c`

Type `./kplisp <filename>` to use. Parentheses matching is done automatically, Ctrl-e is to eval-last-sexp, [Ctrl-s is to save and Ctrl-q is to quit](https://github.com/antirez/kilo).

![demo](https://github.com/ytaki0801/Kilo-PureLISP/blob/master/Kilo-PureLISP_demo.gif)

For the `PureLISP.sh` specification, see [PureLISP.sh repository](https://github.com/ytaki0801/PureLISP.sh) and `sample.plsh`.

## PureLISP.c

You can also compile `PureLISP.c` with `PureLISP.h` alone to use in a simple one-line-input REPL like the following. It is written in standard C specification only so enable to compile by not only GCC or Clang but also C compiler of Visual Studio Community or [Tiny C Compiler](https://bellard.org/tcc/).

```
$ cc PureLISP.c
$ ./a.out
S> (def reduce (lambda (f L i) (cond ((eq L nil) i) (t (f (car L) (reduce f (cdr L) i))))))
reduce
S> (reduce cons '(a b c) '(d e f g))
(a b c d e f g)
S> [Type Ctrl-d or Ctrl-c to exit]
```

## TODO

* Full implemention of eval error handling in the interpreter.

## License

* Kilo: [BSD 2-Clause "Simplified" License](https://github.com/snaptoken/kilo-src/blob/master/LICENSE)

* Source codes in this repository except Kilo: [Public Domain by CC0, Creative Commons Zero v1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/)

