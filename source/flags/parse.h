#ifndef VALUES_PARSE_H_
#define VALUES_PARSE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool ParseString(char *value, size_t maxLen, const char *s, size_t len);
bool ParseBool(bool *value, const char *s, size_t len);
bool ParseInt32(int32_t *value, const char *s, size_t len);
bool ParseInt64(int64_t *value, const char *s, size_t len);
bool ParseUint32(uint32_t *value, const char *s, size_t len);
bool ParseUint64(uint64_t *value, const char *s, size_t len);

typedef bool (*ParseFunc)(void *value, size_t maxLen, const char *s,
                          size_t len);

bool ParseFuncBool(void *value, size_t maxLen, const char *s, size_t len);

bool ParseFuncString(void *value, size_t maxLen, const char *s, size_t len);

bool ParseFuncInt32(void *value, size_t maxLen, const char *s, size_t len);

bool ParseFuncInt64(void *value, size_t maxLen, const char *s, size_t len);

bool ParseFuncUint32(void *value, size_t maxLen, const char *s, size_t len);

bool ParseFuncUint64(void *value, size_t maxLen, const char *s, size_t len);

#endif // VALUES_PARSE_H_
