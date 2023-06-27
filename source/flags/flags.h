#ifndef FLAGS_FLAGS_H_
#define FLAGS_FLAGS_H_

#include <stddef.h>
#include <stdint.h>

#include "parse.h"

typedef int FlagError;

#define Ok 0
#define FlagErrNoArg 1
#define FlagErrParse 2
#define FlagErrUnknownFlag 3
#define FlagErrMissingFlag 4
#define FlagErrUnknownCommand 5

typedef struct HelpItem {
  const char *Name;
  const char *Help;
} HelpItem;

typedef enum FlagType {
  FlagBool,
  FlagString,
  FlagInt32,
  FlagInt64,
  FlagUint32,
  FlagUint64,
} FlagType;

typedef struct FlagOption {
  enum FlagType Type;
  int NumArgs;
  ParseFunc ParseFunc;
  void *Value;
  size_t MaxLen;
  HelpItem Help;
} FlagOption;

typedef struct FlagOptions {
  size_t OptionsLen;
  FlagOption *Options;
} FlagOptions;

typedef struct FlagCommand {
  HelpItem Help;
} FlagCommand;

typedef struct FlagCommands {
  char *Value;
  FlagCommand *Commands;
  size_t MaxLen;
  size_t CommandsLen;
} FlagCommands;

typedef struct Flags {
  FlagOptions Options;
  FlagCommands Commands;
} Flags;

FlagOption FlagsNewBool(bool *value, const char *name, const char *help);
FlagOption FlagsNewString(char *value, size_t maxLen, const char *name,
                          const char *help);
FlagOption FlagsNewInt32(int32_t *value, const char *name, const char *help);
FlagOption FlagsNewInt64(int64_t *value, const char *name, const char *help);
FlagOption FlagsNewUint32(uint32_t *value, const char *name, const char *help);
FlagOption FlagsNewUint64(uint64_t *value, const char *name, const char *help);
FlagCommand FlagNewCommand(const char *name, const char *help);

#define FlagCommandsDeclare(var, output, outputLen, ...)                       \
  FlagCommand __##var[] = {__VA_ARGS__};                                       \
  FlagCommands var = {                                                         \
      .CommandsLen = sizeof(__##var) / sizeof(FlagCommand),                    \
      .Commands = __##var,                                                     \
      .Value = output,                                                         \
      .MaxLen = outputLen,                                                     \
  }

#define FlagOptionsDeclare(var, ...)                                           \
  FlagOption __##var[] = {__VA_ARGS__};                                        \
  FlagOptions var = {                                                          \
      .OptionsLen = sizeof(__##var) / sizeof(FlagOption),                      \
      .Options = __options,                                                    \
  }

Flags FlagsDefine(FlagOptions options, FlagCommands commands);
Flags FlagsDefineOnlyOptions(FlagOptions options);
Flags FlagsDefineOnlyCommands(FlagCommands cmds);

FlagError FlagsParse(int argc, char *argv[], Flags *flags, int *index);
void FlagsPrintError(int argc, char *argv[], FlagError error, int index);
void FlagsPrintHelp(const char *app, Flags *flags);

#endif // FLAGS_FLAGS_H_
