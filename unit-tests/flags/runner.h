#ifndef RUNNER_H_
#define RUNNER_H_

#define TestRun(test)                                                          \
  do {                                                                         \
    int result = test();                                                       \
    if (result != EXIT_SUCCESS) {                                              \
      printf("testing %s failed\n", #test);                                    \
    }                                                                          \
  } while (0);

#endif // RUNNER_H_
