#ifndef PRINTS_H_
#define PRINTS_H_

#include <stdbool.h>
#include <stdint.h>

#define StringError "assertion error %s %s at line %d: "

void PrintErrBool(bool expected, bool received, const char *err);
void PrintErrInt(int64_t expected, int64_t received, const char *err);
void PrintErrUint(uint64_t expected, uint64_t received, const char *err);
void PrintErrString(const char *expected, const char *received,
                    const char *err);
void PrintErrPtr(void *expected, void *received, const char *err);
void PrintErr(const void *expected, const void *received, const char *err);

#define PrintErrorValue(expected, received, err)                               \
  _Generic((expected), bool: PrintErrBool, \
                                         int64_t: PrintErrInt,                            \
                                         uint64_t: PrintErrUint,                          \
                                         int: PrintErrInt,                                \
                                         unsigned int: PrintErrInt,                       \
                                         short unsigned int: PrintErrInt,                 \
                                         const char*: PrintErrString,                     \
                                         char*: PrintErrString,                           \
                                         unsigned char*: PrintErr,                        \
                                         void*: PrintErrPtr)(expected, received, err)

#define PrintErr2(received, expected, error, Cond)                             \
  do {                                                                         \
    fprintf(stderr, StringError #received " " #Cond " " #expected "\n",        \
            __FILE__, __func__, __LINE__);                                     \
    PrintErrorValue(expected, received, error);                                \
  } while (0)

#define PrintErr3(received, expected, n, error, Cond)                          \
  do {                                                                         \
    fprintf(stderr, StringError #received " " #Cond " " #expected " " #n "\n", \
            __FILE__, __func__, __LINE__);                                     \
    PrintErrorValue(expected, received, error);                                \
  } while (0)

#endif // PRINTS_H_
