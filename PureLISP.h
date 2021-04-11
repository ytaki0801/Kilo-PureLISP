#ifndef PURE_LISP_H
#define PURE_LISP_H

// PureLISP.c: C version of https://github.com/ytaki0801/PureLISP.sh
//
// This code is Licensed under CC0.
// https://creativecommons.org/publicdomain/zero/1.0/

#include <stdint.h>

#define SSTR_MAX 4096

typedef uintptr_t value_t;
enum NODE_TAG { NODE_STRG, NODE_CONS };

typedef struct _node_t_ {
  value_t value;
  enum NODE_TAG tag;
} _node_t, *node_t;

void s_eval_string(char *s);
void s_init_purelisp(void);
extern char s_eval_retval[];

#endif

