#ifndef FLAGS_STRINGS_H_
#define FLAGS_STRINGS_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct CharSlice {
  size_t Cap;
  char *Value;
} CharSlice;

#define CharSliceDeclare(name, cap)                                            \
  char __##name[cap];                                                          \
  CharSlice name = {.Cap = 0, .Value = __##name}

typedef bool(CharSkipper)(char c);

bool StringCaseEqualsWithLen(const char *a, size_t alen, const char *b,
                             size_t blen);
bool StringCaseEquals(const char *a, const char *b);

bool StringEqualsWithLen(const char *a, size_t alen, const char *b,
                         size_t blen);
bool StringEquals(const char *a, const char *b);
bool StringCopy(char *dest, size_t destLen, const char *src);
bool StringCopyWithLen(char *dest, size_t destLen, const char *src,
                       size_t srcLen);

bool CharIsNewline(char c);
bool CharIsNotNewline(char c);
bool CharIsBlank(char c);
bool CharIsNotBlank(char c);

bool StringIsEmpty(const char *c);
bool StringIsBlank(const char *c);
bool StringIsNotEmpty(const char *c);
bool StringIsNotBlank(const char *c);
bool StringIsEmptyWithLen(const char *c, size_t len);
bool StringIsBlankWithLen(const char *c, size_t len);
bool StringIsNotEmptyWithLen(const char *c, size_t len);
bool StringIsNotBlankWithLen(const char *c, size_t len);
bool StringIsSubstringOf(const char *a, size_t alen, const char *s,
                         size_t slen);

const char *StringSkipChar(const char *c, size_t len, CharSkipper skipper);
const char *StringSkipLine(const char *c, size_t len);
const char *StringSkipBlank(const char *c, size_t len);
const char *StringSkipNonBlank(const char *c, size_t len);
int64_t StringToInt64(const char *nptr, size_t len, const char **endptr,
                      int base);
uint64_t StringToUint64(const char *nptr, size_t len, const char **endptr,
                        int base);

#endif // FLAGS_STRINGS_H_
