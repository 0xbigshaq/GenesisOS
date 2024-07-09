// #ifndef NK_UTIL_H_
// #define NK_UTIL_H_

// #include "nuklear.h"

// #ifndef NK_MEMCPY
// #define NK_MEMCPY nk_memcopy
// NK_LIB void*
// nk_memcopy(void *dst0, const void *src0, nk_size length);
// #endif
// #ifndef NK_MEMSET
// #define NK_MEMSET nk_memset
// NK_LIB void
// nk_memset(void *ptr, int c0, nk_size size);
// #endif
// NK_LIB void
// nk_zero(void *ptr, nk_size size);
// NK_API int
// nk_strlen(const char *str);
// NK_API int
// nk_strtoi(const char *str, const char **endptr);
// NK_API double
// nk_strtod(const char *str, const char **endptr);
// NK_API float
// nk_strtof(const char *str, const char **endptr);
// NK_API int
// nk_stricmp(const char *s1, const char *s2);
// NK_API int
// nk_stricmpn(const char *s1, const char *s2, int n);

// NK_INTERN int
// nk_str_match_here(const char *regexp, const char *text);

// NK_INTERN int
// nk_str_match_star(int c, const char *regexp, const char *text);

// NK_API int
// nk_strfilter(const char *text, const char *regexp);



// NK_API int
// nk_strmatch_fuzzy_text(const char *str, int str_len,
//      const char *pattern, int *out_score);

// NK_API int
// nk_strmatch_fuzzy_string(char const *str, char const *pattern, int *out_score);

// NK_LIB int
// nk_string_float_limit(char *string, int prec);

// NK_INTERN void
// nk_strrev_ascii(char *s);

// NK_LIB char*
// nk_itoa(char *s, long n);


// #ifndef NK_DTOA
// #define NK_DTOA nk_dtoa
// NK_LIB char*
// nk_dtoa(char *s, double n);
// #endif

// #ifdef NK_INCLUDE_STANDARD_VARARGS
// #ifndef NK_INCLUDE_STANDARD_IO
// NK_INTERN int
// nk_vsnprintf(char *buf, int buf_size, const char *fmt, va_list args);
// #endif
// NK_LIB int
// nk_strfmt(char *buf, int buf_size, const char *fmt, va_list args);
// #endif


// NK_API nk_hash
// nk_murmur_hash(const void * key, int len, nk_hash seed);


// #ifdef NK_INCLUDE_STANDARD_IO
// NK_LIB char*
// nk_file_load(const char* path, nk_size* siz, const struct nk_allocator *alloc)
// #endif

// NK_LIB int
// nk_text_clamp(const struct nk_user_font *font, const char *text,
//      int text_len, float space, int *glyphs, float *text_width,
//      nk_rune *sep_list, int sep_count);


// NK_LIB struct nk_vec2
// nk_text_calculate_text_bounds(const struct nk_user_font *font,
//      const char *begin, int byte_len, float row_height, const char **remaining,
//      struct nk_vec2 *out_offset, int *glyphs, int op);

// #endif // NK_UTIL_H_