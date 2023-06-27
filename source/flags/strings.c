#include "strings.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <strings.h>

bool StringCaseEqualsWithLen(const char *a, size_t alen, const char *b,
                             size_t blen) {
  return alen == blen && strncasecmp(a, b, alen) == 0;
}

bool StringCaseEquals(const char *a, const char *b) {
  while (tolower(*a) == tolower(*b) && *a != '\0' && *b != '\0') {
    a++;
    b++;
  }

  return *a == '\0' && *b == '\0';
}

bool StringEqualsWithLen(const char *a, size_t alen, const char *b,
                         size_t blen) {
  return alen == blen && strncmp(a, b, alen) == 0;
}

bool StringEquals(const char *a, const char *b) {
  while (*a == *b && *a != '\0' && *b != '\0') {
    a++;
    b++;
  }
  return *a == '\0' && *b == '\0';
}

bool StringCopy(char *dest, size_t destLen, const char *src) {
  return StringCopyWithLen(dest, destLen, src, strlen(src));
}

bool StringCopyWithLen(char *dest, size_t destLen, const char *src,
                       size_t sourceLen) {
  if (destLen < sourceLen) {
    return false;
  }

  strncpy(dest, src, sourceLen); // NOLINT
  dest[sourceLen] = '\0';
  return true;
}

bool CharIsNewline(char c) { return c == '\n'; }

bool CharIsNotNewline(char c) { return c != '\n'; }

bool CharIsBlank(char c) { return isspace(c); }

bool CharIsNotBlank(char c) { return !isspace(c); }

bool StringIsEmpty(const char *c) { return c == NULL || *c == '\0'; }

bool StringIsBlank(const char *c) {
  return StringIsBlankWithLen(c, c == NULL ? 0 : strlen(c));
}

bool StringIsNotEmpty(const char *c) { return c == NULL || *c == '\0'; }

bool StringIsNotBlank(const char *c) {
  return StringIsBlankWithLen(c, c == NULL ? 0 : strlen(c));
}

bool StringIsEmptyWithLen(const char *c, size_t len) {
  (void)(c);
  return len == 0;
}

bool StringIsBlankWithLen(const char *c, size_t len) {
  return StringSkipBlank(c, len) == c + len;
}

bool StringIsNotEmptyWithLen(const char *c, size_t len) {
  return !StringIsEmptyWithLen(c, len);
}

bool StringIsNotBlankWithLen(const char *c, size_t len) {
  return !StringIsBlankWithLen(c, len);
}

const char *StringSkipChar(const char *c, size_t len, CharSkipper skipper) {
  size_t i = 0;
  while (i < len && skipper(*c)) {
    c++;
    i++;
  }
  return c;
}

const char *StringSkipLine(const char *c, size_t len) {
  return StringSkipChar(c, len, CharIsNotNewline);
}

const char *StringSkipBlank(const char *c, size_t len) {
  return StringSkipChar(c, len, CharIsBlank);
}

const char *StringSkipNonBlank(const char *c, size_t len) {
  return StringSkipChar(c, len, CharIsNotBlank);
}

bool StringIsSubstringOf(const char *a, size_t alen, const char *s,
                         size_t slen) {
  if (alen == 0) {
    return true;
  }

  if (slen < alen) {
    return false;
  }

  size_t i = 0;
  bool match = false;

  while (i < slen) {
    if (s[i] == a[0]) {
      size_t j = 0;

      while (i < slen && j < alen && s[i] == a[j]) {
        i++;
        j++;
      }

      if (j == alen) {
        match = true;
        break;
      }

    } else {
      i++;
    }
  }

  return match;
}

int64_t StringToInt64(const char *nptr, size_t len, const char **endptr,
                      int base) {
  const char *s = nptr;
  uint64_t acc;
  int c;
  uint64_t cutoff;
  int neg = 0, any, cutlim;

  errno = 0;

  /*
   * Skip white space and pick up leading +/- sign if any.
   * If base is 0, allow 0x for hex and 0 for octal, else
   * assume decimal; if base is already 16, allow 0x.
   */
  do {
    c = *s++;
    len--;
  } while (len > 0 && isspace(c));

  if (len > 0) {
    if (c == '-') {
      neg = 1;
      c = *s++;
      len--;
    } else if (c == '+') {
      c = *s++;
      len--;
    }

    if (len > 1 && (base == 0 || base == 16) && c == '0' &&
        (*s == 'x' || *s == 'X')) {
      c = s[1];
      s += 2;
      len -= 2;
      base = 16;
    }
  }

  if (base == 0) {
    base = c == '0' ? 8 : 10;
  }

  /*
   * Compute the cutoff value between legal numbers and illegal
   * numbers.  That is the largest legal value, divided by the
   * base.  An input number that is greater than this value, if
   * followed by a legal input character, is too big.  One that
   * is equal to this value may be valid or not; the limit
   * between valid and invalid numbers is then based on the last
   * digit.  For instance, if the range for longs is
   * [-2147483648..2147483647] and the input base is 10,
   * cutoff will be set to 214748364 and cutlim to either
   * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
   * a value > 214748364, or equal but the next digit is > 7 (or 8),
   * the number is too big, and we will return a range error.
   *
   * Set any if any `digits' consumed; make it negative to indicate
   * overflow.
   */
  cutoff = neg ? -(uint64_t)LONG_MIN : LONG_MAX;
  cutlim = cutoff % (uint64_t)base;
  cutoff /= (uint64_t)base;
  for (acc = 0, any = 0;; c = *s++, len--) {
    if (isdigit(c)) {
      c -= '0';

    } else if (isalpha(c)) {
      c -= isupper(c) ? 'A' - 10 : 'a' - 10;

    } else {
      break;
    }

    if (c >= base) {
      break;
    }

    if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)) {
      any = -1;

    } else {
      any = 1;
      acc *= base;
      acc += c;
    }

    if (len == 0) {
      break;
    }
  }

  if (any < 0) {
    acc = neg ? LONG_MIN : LONG_MAX;
    errno = ERANGE;
  } else if (neg) {
    acc = -acc;
  }

  if (endptr != 0) {
    *endptr = any ? s - 1 : nptr;
  }

  return (acc);
}

uint64_t StringToUint64(const char *nptr, size_t len, const char **endptr,
                        int base) {
  const char *s = nptr;
  uint64_t acc;
  int c;
  uint64_t cutoff;
  int neg = 0, any, cutlim;

  errno = 0;

  /*
   * See strtol for comments as to the logic used.
   */
  do {
    c = *s++;
    len--;
  } while (len > 0 && isspace(c));

  if (len > 0) {
    if (c == '-') {
      neg = 1;
      c = *s++;
      len--;
    } else if (c == '+') {
      c = *s++;
      len--;
    }
    if (len > 1 && (base == 0 || base == 16) && c == '0' &&
        (*s == 'x' || *s == 'X')) {
      c = s[1];
      s += 2;
      len -= 2;
      base = 16;
    }
  }

  if (base == 0) {
    base = c == '0' ? 8 : 10;
  }

  cutoff = (uint64_t)ULONG_MAX / (uint64_t)base;
  cutlim = (uint64_t)ULONG_MAX % (uint64_t)base;
  for (acc = 0, any = 0;; c = *s++, len--) {
    if (isdigit(c)) {
      c -= '0';
    } else if (isalpha(c)) {
      c -= isupper(c) ? 'A' - 10 : 'a' - 10;
    } else {
      break;
    }

    if (c >= base) {
      break;
    }

    if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)) {
      any = -1;

    } else {
      any = 1;
      acc *= base;
      acc += c;
    }

    if (len == 0) {
      break;
    }
  }
  if (any < 0) {
    acc = ULONG_MAX;
    errno = ERANGE;
  } else if (neg) {
    acc = -acc;
  }

  if (endptr != 0)
    *endptr = any ? s - 1 : nptr;
  return (acc);
}
