#include "parse.h"

#include <errno.h>
#include <string.h>

#include "strings.h"

bool ParseBool(bool *value, const char *s, size_t len) {
  int64_t parsed;

  if (StringCaseEqualsWithLen(s, len, "true", 4) ||
      StringCaseEqualsWithLen(s, len, "on", 2) ||
      StringCaseEqualsWithLen(s, len, "enabled", 7)) {
    *value = true;
    return true;

  } else if (StringCaseEqualsWithLen(s, len, "false", 5) ||
             StringCaseEqualsWithLen(s, len, "off", 3) ||
             StringCaseEqualsWithLen(s, len, "disabled", 8)) {
    *value = false;
    return true;

  } else if (ParseInt64(&parsed, s, len)) {
    *value = parsed != 0;
    return true;

  } else {
    return false;
  }
}

bool ParseString(char *value, size_t maxLen, const char *s, size_t len) {
  if (len >= maxLen) {
    return false;
  }

  strncpy(value, s, len); // NOLINT
  value[len] = '\0';
  return true;
}

bool ParseInt32(int32_t *value, const char *s, size_t len) {
  const char *endptr = NULL;
  int64_t parsed = StringToInt64(s, len, &endptr, 10);

  if (errno || ((size_t)(endptr - s + 1) < len && !CharIsBlank(*endptr))) {
    errno = 0;
    return false;

  } else if (parsed > INT32_MAX) {
    return false;

  } else if (parsed < INT32_MIN) {
    return false;

  } else {
    *value = (int32_t)(parsed);
    return true;
  }
}

bool ParseInt64(int64_t *value, const char *s, size_t len) {
  const char *endptr = NULL;
  int64_t parsed = StringToInt64(s, len, &endptr, 10);

  if (errno || ((size_t)(endptr - s + 1) < len && !CharIsBlank(*endptr))) {
    errno = 0;
    return false;

  } else if (parsed > INT64_MAX) {
    return false;

  } else if (parsed < INT64_MIN) {
    return false;

  } else {
    *value = (int64_t)(parsed);
    return true;
  }
}

bool ParseUint32(uint32_t *value, const char *s, size_t len) {
  const char *endptr = NULL;
  uint64_t parsed = StringToUint64(s, len, &endptr, 10);

  if (errno || ((size_t)(endptr - s + 1) < len && !CharIsBlank(*endptr))) {
    errno = 0;
    return false;

  } else if (parsed > UINT32_MAX) {
    return false;

  } else {
    *value = (int64_t)(parsed);
    return true;
  }
}

bool ParseUint64(uint64_t *value, const char *s, size_t len) {
  const char *endptr = NULL;
  uint64_t parsed = StringToUint64(s, len, &endptr, 10);

  if (errno || ((size_t)(endptr - s + 1) < len && !CharIsBlank(*endptr))) {
    errno = 0;
    return false;

  } else if (parsed > UINT64_MAX) {
    return false;

  } else {
    *value = (uint64_t)(parsed);
    return true;
  }
}

bool ParseFuncBool(void *value, size_t maxLen, const char *s, size_t len) {
  (void)(maxLen);
  return ParseBool((bool *)value, s, len);
}

bool ParseFuncString(void *value, size_t maxLen, const char *s, size_t len) {
  return ParseString((char *)value, maxLen, s, len);
}

bool ParseFuncInt32(void *value, size_t maxLen, const char *s, size_t len) {
  (void)(maxLen);
  return ParseInt32((int32_t *)value, s, len);
}

bool ParseFuncInt64(void *value, size_t maxLen, const char *s, size_t len) {
  (void)(maxLen);
  return ParseInt64((int64_t *)value, s, len);
}

bool ParseFuncUint32(void *value, size_t maxLen, const char *s, size_t len) {
  (void)(maxLen);
  return ParseUint32((uint32_t *)value, s, len);
}

bool ParseFuncUint64(void *value, size_t maxLen, const char *s, size_t len) {
  (void)(maxLen);
  return ParseUint64((uint64_t *)value, s, len);
}
