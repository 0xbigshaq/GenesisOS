#include "kernel/types.h"
#include "kernel/x86.h"
#include <stddef.h>

void* memset2(void *dst, int c, uint n)
{
  if ((int)dst%4 == 0 && n%4 == 0){
    c &= 0xFF;
    stosl(dst, (c<<24)|(c<<16)|(c<<8)|c, n/4);
  } else
    stosb(dst, c, n);
  return dst;
}

int memcmp(const void *v1, const void *v2, uint n)
{
  const uchar *s1, *s2;

  s1 = v1;
  s2 = v2;
  while(n-- > 0){
    if(*s1 != *s2)
      return *s1 - *s2;
    s1++, s2++;
  }

  return 0;
}

void* memmove(void *dst, const void *src, uint n)
{
  const char *s;
  char *d;

  s = src;
  d = dst;
  if(s < d && s + n > d){
    s += n;
    d += n;
    while(n-- > 0)
      *--d = *--s;
  } else
    while(n-- > 0)
      *d++ = *s++;

  return dst;
}

// memcpy exists to placate GCC.  Use memmove.
void* memcpy(void *dst, const void *src, uint n)
{
  return memmove(dst, src, n);
}

int strncmp(const char *p, const char *q, uint n)
{
  while(n > 0 && *p && *p == *q)
    n--, p++, q++;
  if(n == 0)
    return 0;
  return (uchar)*p - (uchar)*q;
}

char* strncpy(char *s, const char *t, int n)
{
  char *os;

  os = s;
  while(n-- > 0 && (*s++ = *t++) != 0)
    ;
  while(n-- > 0)
    *s++ = 0;
  return os;
}

int strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

char* strchr(const char* str, int c) {
    // Iterate over each character in the string
    while (*str != '\0') {
        // If the character matches, return a pointer to it
        if (*str == (char)c) {
            return (char*)str;
        }
        str++;
    }
    // If the character is '\0', check the null terminator
    if (c == '\0') {
        return (char*)str;
    }
    // Character not found, return NULL
    return NULL;
}

char *strcat(char *destination, const char *source)
{
    // Get the length of the destination string
    size_t dest_len = strlen(destination);
    // Iterate over each character in the source string
    for (size_t i = 0; source[i] != '\0'; i++) {
        // Append the character to the destination string
        destination[dest_len + i] = source[i];
    }
    // Append the null terminator to the destination string
    destination[dest_len + strlen(source)] = '\0';
    // Return the destination string
    return destination;
}

char *strrchr(char *s, int c) {
    char *last = NULL;
    while (*s) {
        if (*s == c) {
            last = s;
        }
        s++;
    }
    return last;
}