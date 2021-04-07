#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "kilo-BYOTE130.h"
#include "PureLISP.h"

#define KILO_PURE_LISP_VERSION "0.0.1"

int *lastSExp() {
  int cx = E.cx, cy = E.cy, p_depth = 0, *retc;
  erow *r = &E.row[cy];
  char t;

  do {
    if (cx <= 0) {
      if (cy <= 0) break;
      else { cy--; r = &E.row[cy]; cx = r->size; }
    }
    t = r->chars[cx - 1];
    switch (t) {
      case ')': p_depth++; cx--; break;
      case '(': p_depth--; cx--; break;
      default:
        do { cx--; t = r->chars[cx - 1]; }
        while (t != ')' && t != '(' && t != ' ' && t != '\'' && cx > 0);
        break;
    }
  } while (p_depth > 0);

  if (p_depth == 0) {
    if ((retc = (int *)malloc(sizeof(int))) == NULL) exit(1);
    retc[0] = cx; retc[1] = cy;
    return retc;
  } else return (int *)(NULL);
}

void matchParentheses() {
  int *retc;
  if ((retc = lastSExp()) != NULL) {
    int current_cx = E.cx, current_cy = E.cy;
    E.cx = retc[0]; E.cy = retc[1];
    editorRefreshScreen();
    usleep(300000);
    E.cx = current_cx; E.cy = current_cy;
    free(retc);
  }
}

char *sExpSetChar(char *buf, int size, char t) {
  char *tmp;
  if ((tmp = (char *)realloc(buf, size)) == NULL) exit(1);
  tmp[size - 2] = t; tmp[size - 1] = '\0';
  return tmp;
}

void evalLastSExp() {
  int *retc;

  if ((retc = lastSExp()) == NULL) {
    editorSetStatusMessage("eval-last-sexp scan error");
    return;
  }

  char *se_buf, t;
  int cx = E.cx, cy = E.cy, se_buf_size;
  erow *r = &E.row[cy];

  if ((se_buf = (char *)malloc(1)) == NULL) exit(1);
  se_buf[0] = '\0'; se_buf_size = 1;

  while (!(cx == retc[0] && cy == retc[1])) {
    if (cx <= 0) {
      if (cy <= 0) break;
      else {
        cy--; r = &E.row[cy]; cx = r->size;
        se_buf = sExpSetChar(se_buf, ++se_buf_size, ' ');
      }
    }
    t = r->chars[cx - 1];
    se_buf = sExpSetChar(se_buf, ++se_buf_size, t); cx--;
  }

  for (int i = 0, j = strlen(se_buf) - 1;
    i < strlen(se_buf) / 2; i++, j--) {
    t = se_buf[i]; se_buf[i] = se_buf[j]; se_buf[j] = t;
  }

  if (r->chars[cx - 1] == '\'') {
    char *tmp = se_buf;
    char *se_bufq = (char *)malloc(strlen(se_buf) + 9);
    sprintf(se_bufq, "(quote %s)", se_buf);
    se_buf = se_bufq; free(tmp);
  }

  s_eval_string(se_buf);
  editorSetStatusMessage(s_eval_retval);

  free(se_buf);
}

