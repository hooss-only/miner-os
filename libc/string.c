#include "string.h"

void int_to_ascii(int n, char str[]) {
  int i, sign;
  if ((sign = n) < 0) n = -n;
  i = 0;
  do {
    str[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);

  if (sign < 0) str[i++] = '-';
  str[i] = '\0';
}

void reverse(char s[]) {
  int c, j, i;
  for (i=0, j = strlen(s)-1; i<j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

int strlen(char s[]) {
  int i=0;
  while (s[i] != 0) ++i;
  return i;
}

int strcmp(char* s1, char* s2) {
  int i;
  for (i=0; s1[i] == s2[i]; i++) {
    if (s1[i] == 0) return 0;
  }
  return s1[i] - s2[i];
}

void backspace(char* str) {
  int l = strlen(str);
  str[l-1] = 0;
}

void append(char* str, char c) {
  int l = strlen(str);
  str[l] = c;
  str[l+1] = 0;
}
