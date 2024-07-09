#include "kernel/gfx/gfx_math.h"
#include <stddef.h>
#include <math.h>

void swap(void* a, void* b, size_t size) {
    unsigned char* p = a, * q = b, tmp;
    for (size_t i = 0; i < size; i++) {
        tmp = p[i];
        p[i] = q[i];
        q[i] = tmp;
    }
}

int partition(void* base, size_t size, int low, int high, int (*compar)(const void*, const void*)) {
    unsigned char* pivot = (unsigned char*)base + high * size;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (compar((unsigned char*)base + j * size, pivot) <= 0) {
            i++;
            swap((unsigned char*)base + i * size, (unsigned char*)base + j * size, size);
        }
    }
    swap((unsigned char*)base + (i + 1) * size, (unsigned char*)base + high * size, size);
    return i + 1;
}

void quicksort(void* base, size_t size, int low, int high, int (*compar)(const void*, const void*)) {
    if (low < high) {
        int pi = partition(base, size, low, high, compar);
        quicksort(base, size, low, pi - 1, compar);
        quicksort(base, size, pi + 1, high, compar);
    }
}

void qsort(void* base, size_t num, size_t size, int (*compar)(const void*, const void*)) {
    quicksort(base, size, 0, num - 1, compar);
}

// sqrt implementation
double sqrt(double x) {
    if (x < 0) return -1; // Error for negative input
    double guess = x / 2.0;
    double epsilon = 0.00001;
    while ((guess * guess - x) > epsilon || (x - guess * guess) > epsilon) {
        guess = (guess + x / guess) / 2.0;
    }
    return guess;
}

// floor implementation
double floor(double x) {
    int xi = (int)x;
    return (x < xi) ? (xi - 1) : xi;
}

// ceil implementation
double ceil(double x) {
    int xi = (int)x;
    return (x > xi) ? (xi + 1) : xi;
}

// fabs implementation
double fabs(double x) {
    return (x < 0) ? -x : x;
}

// fmod implementation
double fmod(double x, double y) {
    return x - y * (int)(x / y);
}

// atan2 implementation
double atan(double x) {
    double result = 0.0;
    double term = x;
    int n = 1;
    while (fabs(term) > 0.0001) {
        result += term;
        term *= -x * x * (2 * n - 1) / (2 * n + 1);
        n++;
    }
    return result;
}

double atan2(double y, double x) {
    if (x > 0) return atan(y / x);
    if (x < 0 && y >= 0) return atan(y / x) + M_PI;
    if (x < 0 && y < 0) return atan(y / x) - M_PI;
    if (x == 0 && y > 0) return M_PI / 2;
    if (x == 0 && y < 0) return -M_PI / 2;
    return 0;
}

// acos implementation
double acos(double x) {
    if (x < -1.0 || x > 1.0) return -1; // Error for out-of-bounds input
    return atan2(sqrt(1 - x * x), x);
}

// cos implementation
double cos(double x) {
    double sum = 1.0;
    double term = 1.0;
    double x_squared = x * x;
    for (int n = 1; n < 10; n++) {
        term *= -x_squared / ((2 * n - 1) * (2 * n));
        sum += term;
    }
    return sum;
}

// abs implementation
int abs(int x) {
    return (x < 0) ? -x : x;
}

// pow implementation
double pow(double x, double y) {
    if (y == 0) return 1;
    if (y < 0) return 1 / pow(x, -y);
    double temp = pow(x, y / 2);
    if ((int)y % 2 == 0) {
        return temp * temp;
    } else {
        return x * temp * temp;
    }
}