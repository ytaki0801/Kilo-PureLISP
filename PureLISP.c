// PureLISP.c: C version of https://github.com/ytaki0801/PureLISP.sh
//
// This code is Licensed under CC0.
// https://creativecommons.org/publicdomain/zero/1.0/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PureLISP.h"

// Basic functions for conscel operations:
// cons, car, cdr, eq, atom

node_t node(value_t value, enum NODE_TAG tag)
{
  node_t n = malloc(sizeof(node_t));
  n->value = value; n->tag = tag;
  return (n);
}

typedef struct _cons_t_ {
  node_t x, y;
} _cons_t, *cons_t;

#define str_to_node(s)  (node((value_t)(s), NODE_STRG))
#define node_to_str(s)  ((char *)(s->value))

#define n_strg(s)  (s->tag == NODE_STRG)
#define n_cons(s)  (s->tag == NODE_CONS)

int eq(node_t s1, node_t s2);
#define atom(s)   (eq(s, NULL) || n_strg(s))

node_t car(node_t s) {
  if (s == NULL || atom(s)) return NULL;
  else return ((cons_t)(s->value))->x;
}

node_t cdr(node_t s) {
  if (s == NULL || atom(s)) return NULL;
  else return ((cons_t)(s->value))->y;
}

node_t cons(node_t x, node_t y)
{
  cons_t c = malloc(sizeof(cons_t));
  c->x = x; c->y = y;
  node_t n = node((value_t)c, NODE_CONS);
  return (n);
}

int eq(node_t s1, node_t s2)
{
  if (s1 == NULL && s2 == NULL) return (1);
  else if (s1 == NULL || s2 == NULL) return (0);
  else if (n_cons(s1) || n_cons(s2)) return (0);
  else return (!strcmp(node_to_str(s1), node_to_str(s2)));
}

// S-expression input: s_lex and s_syn

int s_lex(const char *s, char* sl[])
{
  char sf[SSTR_MAX * 3];
  int i, j = 0;
  for (i = 0; i < strlen(s); i++) {
    switch (s[i]) {
      case '(':
      case ')':
      case '\'': sf[j++] = ' '; sf[j++] = s[i]; sf[j++] = ' '; break;
      case '\n': j++; break;
      default: sf[j++] = s[i];
    }
  }
  sf[j] = '\0';

  char *t;
  int len = 0;
  for (t = strtok(sf, " "); t != NULL; t = strtok(NULL, " ")) sl[len++] = t;
  sl[len] = NULL;

  return (len);
}

node_t s_syn(char *s[], int *pos)
{
  char *t = s[*pos];
  *pos = *pos - 1;

  if (t[0] == ')') {
    if (*pos <= 0) return NULL;
    node_t r = NULL;
    while (s[*pos][0] != '(') {
      if (s[*pos][0] == '.') {
        *pos = *pos - 1;
        r = cons(s_syn(s, pos), car(r));
      } else
        r = cons(s_syn(s, pos), r);
    }
    *pos = *pos - 1;
    if (*pos != -1 && s[*pos][0] == '\'') {
      *pos = *pos - 1;
      return cons(str_to_node("quote"), cons(r, NULL));
    } else
      return (r);
  } else {
    char *tr = malloc((strlen(t) + 1) * sizeof(*tr));
    sprintf(tr, "%s", t);
    node_t tn = str_to_node(tr);
    if (*pos != -1 && s[*pos][0] == '\'') {
      *pos = *pos - 1;
      return cons(str_to_node("quote"), cons(tn, NULL));
    } else
      return (tn);
  }
}

// S-expression output: s_string

char s_eval_retval[SSTR_MAX];
void s_string(node_t s);

void s_strcons(node_t s)
{
  s_string(car(s));
  node_t sd = cdr(s);
  if (sd == NULL) {
  } else if (n_strg(sd)) {
    strcat(s_eval_retval, " . ");
    strcat(s_eval_retval, node_to_str(sd));
  } else {
    strcat(s_eval_retval, " ");
    s_strcons(sd);
  }
}

void s_string(node_t s)
{
  if (s == NULL) {
    strcat(s_eval_retval, "nil");
  } else if (n_strg(s)) {
    strcat(s_eval_retval, node_to_str(s));
  } else {
    strcat(s_eval_retval, "(");
    s_strcons(s);
    strcat(s_eval_retval, ")");
  }
}

// The evaluator: s_eval and utility functions

node_t s_eval(node_t e, node_t a);
node_t GENV;

#define S_T    (str_to_node("t"))
#define S_NIL  (str_to_node("nil"))
#define S_CONS (str_to_node("cons"))
#define S_CAR  (str_to_node("car"))
#define S_CDR  (str_to_node("cdr"))
#define S_EQ   (str_to_node("eq"))
#define S_ATOM (str_to_node("atom"))

node_t caar(node_t x) { return car(car(x)); }
node_t cadr(node_t x) { return car(cdr(x)); }
node_t cdar(node_t x) { return cdr(car(x)); }
node_t cadar(node_t x) { return car(cdr(car(x))); }
node_t caddr(node_t x) { return car(cdr(cdr(x))); }
node_t cadddr(node_t x) { return car(cdr(cdr(cdr(x)))); }

node_t s_null(node_t x)
{
  if (eq(x, NULL)) return S_T; else return NULL;
}

node_t s_append(node_t x, node_t y)
{
  if (s_null(x)) return y;
  else return cons(car(x), s_append(cdr(x), y));
}

node_t s_pair(node_t x, node_t y)
{
  if (s_null(x) || s_null(y)) return NULL;
  else if (!atom(x) && !atom(y))
    return cons(cons(car(x), car(y)), s_pair(cdr(x), cdr(y)));
  else if (atom(x)) return cons(cons(x, y), NULL);
  else return NULL;
}

node_t s_assq(node_t k, node_t v)
{
  if (s_null(v)) return NULL;
  else if (eq(k, caar(v))) return cdar(v);
  else return s_assq(k, cdr(v));
}

#define S_LENGTH  (str_to_node("length"))

int length_r(node_t x, int c)
{
  if (s_null(x)) return c;
  else return length_r(cdr(x), c + 1);
}
node_t length(node_t x)
{
  char *tmp = malloc(SSTR_MAX * sizeof(*tmp));
  sprintf(tmp, "%d", length_r(x, 0));
  return str_to_node(tmp);
}

node_t s_cond(node_t c, node_t a)
{
  if (s_null(c)) return NULL;
  else if (eq(s_eval(caar(c), a), S_T))
    return cadar(c);
  else
    return s_cond(cdr(c), a);
}

node_t s_lookup(node_t t, node_t a)
{
  if      (eq(t, S_T))   return S_T;
  else if (eq(t, S_NIL)) return NULL;
  else if (eq(t, S_CONS) || eq(t, S_CAR)  || eq(t, S_CDR)
        || eq(t, S_EQ)   || eq(t, S_ATOM) || eq(t, S_LENGTH))
    return t;
  else {
    node_t r = s_assq(t, a);
    if (s_null(r)) {
      return s_assq(t, GENV);
    }
    else return r;
  }
}

node_t s_bool2node(int e)
{
  if (e) return S_T; else return NULL;
}

node_t s_apply(node_t f, node_t v)
{
  if      (eq(f, S_CONS))   return cons(car(v), cadr(v));
  else if (eq(f, S_CAR))    return car(car(v));
  else if (eq(f, S_CDR))    return cdr(car(v));
  else if (eq(f, S_EQ))     return s_bool2node(eq(car(v), cadr(v)));
  else if (eq(f, S_ATOM))   return s_bool2node(atom(car(v)));
  else if (eq(f, S_LENGTH)) return length(car(v));
  else return NULL;
}
 
#define S_QUOTE  (str_to_node("quote"))
#define S_COND   (str_to_node("cond"))
#define S_LAMBDA (str_to_node("lambda"))
#define S_MACRO  (str_to_node("macro"))
#define S_DEF    (str_to_node("def"))

node_t s_evals(node_t v, node_t a)
{
  if (s_null(v)) return NULL;
  else return cons(s_eval(car(v), a), s_evals(cdr(v), a));
}

node_t s_eval(node_t e, node_t a)
{
  while (1) {
    if (atom(e)) {
      return s_lookup(e, a);
    } else if (eq(car(e), S_QUOTE)) {
      node_t vals = cadr(e);
      return vals;
    } else if (eq(car(e), S_COND)) {
      node_t seqs = cdr(e);
      e = s_cond(seqs, a);
    } else if (eq(car(e), S_LAMBDA) || eq(car(e), S_MACRO)) {
      node_t name = car(e);
      node_t vars = cadr(e);
      node_t body = caddr(e);
      return cons(name, cons(vars, cons(body, cons(a, NULL))));
    } else if (eq(car(e), S_DEF)) {
      node_t name = cadr(e);
      node_t vals = caddr(e);
      GENV = cons(cons(name, s_eval(vals, a)), GENV);
      return name;
    } else {
      node_t efunc = s_eval(car(e), a);
      node_t fvals = s_evals(cdr(e), a);
      if (atom(efunc)) return s_apply(efunc, fvals);
      else {
        node_t lname = car(efunc);
        node_t lvars = cadr(efunc);
        node_t lbody = caddr(efunc);
        node_t lenvs = cadddr(efunc);
        node_t fenvs = a;
        if (eq(lname, S_MACRO)) fvals = cdr(e);
        e = lbody;
        if (s_null(lvars))
          a = lenvs;
        else if (atom(lvars))
          a = s_append(cons(cons(lvars, fvals), NULL), lenvs);
        else
          a = s_append(s_pair(lvars, fvals), lenvs);
        if (eq(lname, S_MACRO)) return s_eval(s_eval(e, a), fenvs);
      }
    }
  }
}

// Interface of eval_string to C

void s_eval_string(char *s)
{
  char *lr_s[SSTR_MAX];;
  int s_len;

  s_len = s_lex(s, lr_s) - 1;
  node_t rs = s_syn(lr_s, &s_len);
  node_t r = s_eval(rs, NULL);

  s_eval_retval[0] = '\0';
  s_string(r);
}

void s_init_purelisp(void)
{
  GENV = NULL;
}

#ifndef PURE_LISP_LIB
// Sample main to use s_eval_string

int main(int argc, char *argv[])
{
  int prompt = 1;

  if (argc == 2 && !(strcmp(argv[1], "-snl")))
    prompt = 0;
  s_init_purelisp();

  char in[SSTR_MAX];
  if (prompt) printf("S> ");
  while ((fgets(in, SSTR_MAX, stdin)) != NULL) {
    if (in[0] != '\n') {
      in[strlen(in) - 1] = '\0';
      s_eval_retval[0] = '\0';
      s_eval_string(in);
      printf("%s\n", s_eval_retval);
    }
    if (prompt) printf("S> ");
  }

  return (0);
}
#endif

