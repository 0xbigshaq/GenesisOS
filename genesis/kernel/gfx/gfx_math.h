/**
 * @file gfx_math.h
 * @brief Math helper functions for the graphics library. 
 */
#ifndef _GFX_MATH_H
#define _GFX_MATH_H

#include <stddef.h>

void swap(void* a, void* b, size_t size);
int partition(void* base, size_t size, int low, int high, int (*compar)(const void*, const void*));
void quicksort(void* base, size_t size, int low, int high, int (*compar)(const void*, const void*));
void qsort(void* base, size_t num, size_t size, int (*compar)(const void*, const void*));
double sqrt(double x);
double floor(double x);
double ceil(double x);
double fabs(double x);
double fmod(double x, double y);
double atan(double x);
double atan2(double y, double x);
double acos(double x);
double cos(double x);
int abs(int x);
double pow(double x, double y);

#endif // _GFX_MATH_H