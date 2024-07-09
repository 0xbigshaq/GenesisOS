#ifndef ASSERT_H
#define ASSERT_H

#include <assert.h>

void __assert_fail(const char* assertion, const char* file, unsigned int line, const char* function);

#endif // ASSERT_H