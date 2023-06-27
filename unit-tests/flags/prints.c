#include <inttypes.h>
#include <stdio.h>

#include "prints.h"

void PrintErrBool(bool expected, bool received, const char *err) {
  fprintf(stderr, "expected: %d, received: %d, err: \"%s\"\n", expected,
          received, err);
}

void PrintErrInt(int64_t expected, int64_t received, const char *err) {
  fprintf(stderr, "expected: %" PRId64 ", received: %" PRId64 ", err: \"%s\"\n",
          expected, received, err);
}

void PrintErrUint(uint64_t expected, uint64_t received, const char *err) {
  fprintf(stderr, "expected: %" PRIu64 ", received: %" PRIu64 ", err: \"%s\"\n",
          expected, received, err);
}

void PrintErrString(const char *expected, const char *received,
                    const char *err) {
  fprintf(stderr, "expected: %s, received: %s, err: \"%s\"\n", expected,
          received, err);
}

void PrintErrPtr(void *expected, void *received, const char *err) {
  fprintf(stderr, "expected: %p, received: %p, err: \"%s\"\n", expected,
          received, err);
}

void PrintErr(const void *expected, const void *received, const char *err) {
  (void)expected;
  (void)received;
  fprintf(stderr, "%s\n", err);
}
