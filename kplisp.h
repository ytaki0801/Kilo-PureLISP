#ifndef KPLISP_H
#define KPLISP_H

#define KILO_PURE_LISP_VERSION "0.0.1"

void evalLastSExp(void);
void matchParentheses(void);
void initPureLISP();

extern node_t GENV;

#endif

