#ifndef ASSERTS_H_
#define ASSERTS_H_

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prints.h"

#define CondEq(a, b) ((a) == (b))
#define CondNotEq(a, b) (!(CondEq((a), (b))))
#define CondMemEq(a, b, n) (memcmp((a), (b), (n)) == 0)
#define CondMemNotEq(a, b, n) (!(CondMemEq((a), (b), (n))))
#define CondStringEq(a, b)                                                     \
  (strlen(a) == strlen(b) && strncmp((a), (b), (strlen(a))) == 0)
#define CondTrue(a) CondEq(!!(a), 1)
#define CondFalse(a) CondEq(!(a), 1)
#define CondNull(a) CondEq(a, NULL)

#define AssertCond1(a, b, error, Cond)                                         \
  do {                                                                         \
    if (!Cond(a)) {                                                            \
      PrintErr2(a, b, error, Cond);                                            \
      return EXIT_FAILURE;                                                     \
    }                                                                          \
  } while (0)

#define AssertCond2(a, b, error, Cond)                                         \
  do {                                                                         \
    if (!Cond(a, b)) {                                                         \
      PrintErr2(a, b, error, Cond);                                            \
      return EXIT_FAILURE;                                                     \
    }                                                                          \
  } while (0)

#define AssertCond3(a, b, n, error, Cond)                                      \
  do {                                                                         \
    if (!Cond(a, b, n)) {                                                      \
      PrintErr3(a, b, n, error, Cond);                                         \
      return EXIT_FAILURE;                                                     \
    }                                                                          \
  } while (0)

#define ExpectCond1(a, b, err, Cond)                                           \
  do {                                                                         \
    if (!Cond(a)) {                                                            \
      PrintErr2(a, b, err, Cond);                                              \
      return err;                                                              \
    }                                                                          \
  } while (0)

#define ExpectCond2(a, b, err, Cond)                                           \
  do {                                                                         \
    if (!Cond(a, b)) {                                                         \
      PrintErr2(a, b, err, Cond);                                              \
      return err;                                                              \
    }                                                                          \
  } while (0)

#define ExpectCond3(a, b, n, err, Cond)                                        \
  do {                                                                         \
    if (!Cond(a, b, n)) {                                                      \
      PrintErr3(a, b, n, err, Cond);                                           \
      return err;                                                              \
    }                                                                          \
  } while (0)

#define AssertMemEq(a, b, l)                                                   \
  AssertCond3((a), (b), (l), "memory not equal", CondMemEq)
#define AssertStringEq(a, b)                                                   \
  AssertCond2((a), (b), "strings not equal", CondStringEq)
#define AssertMemNotEq(a, b, l)                                                \
  AssertCond3((a), (b), (l), "memory equal", CondMemNotEq)
#define AssertEq(a, b) AssertCond2((a), (b), "not equal", CondEq)
#define AssertNotEq(a, b) AssertCond2((a), (b), "equal", CondNotEq)
#define AssertTrue(a) AssertCond1((a), 1, "value is false", CondTrue)
#define AssertFalse(a) AssertCond1((a), 0, "value is true", CondFalse)
#define AssertNull(a) AssertCond1((a), NULL, "value is not null", CondFalse)
#define AssertError(a) AssertCond1((a), 0, "value is not an error", CondTrue)
#define AssertOk(a) AssertCond1((a), (char *)NULL, "not Ok", CondFalse)
#define AssertNotError(a) AssertCond1((a), 0, "value is an error", CondFalse)

#define ExpectMemEq(a, b, l, err) ExpectCond3((a), (b), (l), (err), CondMemEq)
#define ExpectStringEq(a, b, err) ExpectCond2((a), (b), (err), CondStringEq)
#define ExpectMemNotEq(a, b, l, err)                                           \
  ExpectCond3((a), (b), (l), (err), CondMemNotEq)
#define ExpectEq(a, b, err) ExpectCond2((a), (b), (err), CondEq)
#define ExpectNotEq(a, b, err) ExpectCond2((a), (b), (err), CondNotEq)
#define ExpectTrue(a, err) ExpectCond1((a), 1, (err), CondTrue)
#define ExpectFalse(a, err) ExpectCond1((a), 0, (err), CondFalse)
#define ExpectNull(err, msg) ExpectCond1((err), NULL, msg, CondFalse)
#define ExpectError(err, msg) ExpectCond1((err), "error", msg, CondTrue)
#define ExpectOk(err, msg) ExpectCond1((err), (char *)NULL, msg, CondFalse)
#define ExpectNotError(err, msg)                                               \
  ExpectCond1((err), (char *)NULL, msg, CondFalse)

#endif // ASSERTS_H_
