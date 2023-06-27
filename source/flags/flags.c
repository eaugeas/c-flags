#include "flags.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"

const int TabCharLen = 8;

static FlagError FlagParse(FlagOption *option, int argc, char **argv,
                           int *cargc) {
  if (option->NumArgs == 0) {
    if (option->Type != FlagBool) {
      abort();
    }

    const bool ok = option->ParseFunc(option->Value, 0, "true", 4);
    if (!ok) {
      abort();
    }

    return Ok;
  }

  if (option->NumArgs > argc) {
    return FlagErrNoArg;
  }

  if (option->NumArgs != 1) {
    abort();
  }

  const bool ok = option->ParseFunc(option->Value, option->MaxLen, argv[0],
                                    strlen(argv[0]));
  if (!ok) {
    return FlagErrParse;
  }

  *cargc += option->NumArgs;
  return Ok;
}

FlagError FlagsParseNextFlag(int argc, char **argv, Flags *flags, int *cargc) {
  if (argv[0][0] != '-') {
    // only flags should be passed to this function
    abort();
  }

  FlagOption *option = NULL;
  for (size_t i = 0; i < flags->Options.OptionsLen; i++) {
    if (StringEquals(argv[0] + 1, flags->Options.Options[i].Help.Name)) {
      option = &flags->Options.Options[i];
    }
  }

  if (!option) {
    return FlagErrUnknownFlag;
  }

  *cargc += 1;
  return FlagParse(option, argc - 1, argv + 1, cargc);
}

FlagError FlagsParseCommand(int argc, char **argv, Flags *flags, int *cargc) {
  (void)argc;
  if (argv[0][0] == '-') {
    // only commands should be passed to this function
    abort();
  }

  FlagCommand *cmd = NULL;
  for (size_t i = 0; i < flags->Commands.CommandsLen; i++) {
    FlagCommand *cmdTmp = &flags->Commands.Commands[i];
    if (StringEquals(argv[0], cmdTmp->Help.Name)) {
      cmd = cmdTmp;
    }
  }

  if (!cmd) {
    return FlagErrUnknownCommand;
  }

  *cargc += 0;
  bool ok = ParseFuncString(flags->Commands.Value, flags->Commands.MaxLen,
                            *argv, strlen(argv[0]));
  if (!ok) {
    return FlagErrParse;
  }

  return Ok;
}

FlagError FlagsParseNext(int argc, char **argv, Flags *flags, int *cargc) {
  if (argv[0] == NULL || argv[0][0] == '\0') {
    abort();
  }

  if (argv[0][0] == '-') {
    return FlagsParseNextFlag(argc, argv, flags, cargc);

  } else {
    return FlagsParseCommand(argc, argv, flags, cargc);
  }
}

FlagError FlagsParse(int argc, char *argv[], Flags *flags, int *index) {
  FlagError err = Ok;

  for (int i = 1, cargc = i, nargc = argc - cargc;
       i < argc && !err && nargc > 0 && cargc > 0; i += cargc, nargc -= cargc) {
    char **nargv = argv + i;
    *index = i;
    cargc = 0;
    err = FlagsParseNext(nargc, nargv, flags, &cargc);
  }

  return err;
}

static const char *FlagErrorToString(FlagError err) {
  switch (err) {
  case Ok:
    return "ok";
  case FlagErrNoArg:
    return "option expected argument";
  case FlagErrParse:
    return "failed to parse argument for option";
  case FlagErrUnknownFlag:
    return "unknown option provided";
  case FlagErrMissingFlag:
    return "option expected";
  case FlagErrUnknownCommand:
    return "unknown command provided";
  default:
    return "argument parser found unknown error";
  }
}

void FlagsPrintError(int argc, char *argv[], FlagError err, int index) {
  if (err) {
    fprintf(stderr, "%s: error: %s", argv[0], FlagErrorToString(err));
    if (index >= 0 && index < argc) {
      fprintf(stderr, " `%s`", argv[index]);
    }
    fprintf(stderr, "\n");
  }
}

size_t ComputeTabsTaken(size_t len) { return (len / TabCharLen) + 1; }

void PrintHelpItems(HelpItem *items, size_t len, const char *prefix) {
  size_t maxLen = 0;
  for (size_t i = 0; i < len; i++) {
    const HelpItem *item = &items[i];
    const size_t itemLen = strlen(item->Name);
    const size_t totalLen = TabCharLen + 1 + itemLen;
    if (totalLen > maxLen) {
      maxLen = totalLen;
    }
  }

  size_t maxTabsTaken = ComputeTabsTaken(maxLen);
  char tabsString[4];
  for (size_t i = 0; i < len; i++) {
    memset(tabsString, 0, 4); // NOLINT

    const HelpItem *item = &items[i];
    const size_t flagLen = strlen(item->Name);
    const size_t totalLen = TabCharLen + 1 + flagLen;
    const size_t tabsTaken = ComputeTabsTaken(totalLen);
    size_t tabsAdded = maxTabsTaken - tabsTaken + 1;

    if (tabsAdded > 4) {
      tabsAdded = 4;
    }

    for (int j = 0; j < (int)tabsAdded; j++) {
      tabsString[j] = '\t';
    }

    fprintf(stderr, "%s%s%s%s\n", prefix, item->Name, tabsString, item->Help);
  }

  fprintf(stderr, "\n");
}

void FlagsPrintHelp(const char *app, Flags *flags) {
  fprintf(stderr, "\n");
  if (flags->Commands.CommandsLen > 0) {
    fprintf(stderr, "USAGE:\t%s [OPTIONS] COMMAND\n", app);

  } else {
    fprintf(stderr, "USAGE:\t%s [OPTIONS]\n", app);
  }
  fprintf(stderr, "\n");

  if (flags->Options.OptionsLen > 0) {
    fprintf(stderr, "OPTIONS:\n");
    HelpItem flagsHelp[flags->Options.OptionsLen];
    for (size_t i = 0; i < flags->Options.OptionsLen; i++) {
      flagsHelp[i] = flags->Options.Options[i].Help;
    }
    PrintHelpItems(flagsHelp, flags->Options.OptionsLen, "\t-");
  }

  if (flags->Commands.CommandsLen > 0) {
    fprintf(stderr, "COMMANDS:\n");
    HelpItem commandsHelp[flags->Commands.CommandsLen];
    for (size_t i = 0; i < flags->Commands.CommandsLen; i++) {
      commandsHelp[i] = flags->Commands.Commands[i].Help;
    }
    PrintHelpItems(commandsHelp, flags->Commands.CommandsLen, "\t");
  }
}

FlagOption FlagsNewBool(bool *value, const char *name, const char *help) {
  FlagOption option = {.Type = FlagBool,
                       .NumArgs = 0,
                       .ParseFunc = &ParseFuncBool,
                       .Help = {.Name = name, .Help = help},
                       .Value = value,
                       .MaxLen = 0};
  return option;
}

FlagOption FlagsNewString(char *value, size_t maxLen, const char *name,
                          const char *help) {
  FlagOption option = {.Type = FlagString,
                       .NumArgs = 1,
                       .ParseFunc = &ParseFuncString,
                       .Help = {.Name = name, .Help = help},
                       .Value = value,
                       .MaxLen = maxLen};
  return option;
}

FlagOption FlagsNewInt32(int32_t *value, const char *name, const char *help) {
  FlagOption option = {.Type = FlagInt32,
                       .NumArgs = 1,
                       .ParseFunc = &ParseFuncInt32,
                       .Help = {.Name = name, .Help = help},
                       .Value = value,
                       .MaxLen = 0};
  return option;
}

FlagOption FlagsNewInt64(int64_t *value, const char *name, const char *help) {
  FlagOption option = {.Type = FlagInt64,
                       .NumArgs = 1,
                       .ParseFunc = &ParseFuncInt64,
                       .Help = {.Name = name, .Help = help},
                       .Value = value,
                       .MaxLen = 0};
  return option;
}

FlagOption FlagsNewUint32(uint32_t *value, const char *name, const char *help) {
  FlagOption option = {.Type = FlagUint32,
                       .NumArgs = 1,
                       .ParseFunc = &ParseFuncUint32,
                       .Help = {.Name = name, .Help = help},
                       .Value = value,
                       .MaxLen = 0};
  return option;
}

FlagOption FlagsNewUint64(uint64_t *value, const char *name, const char *help) {
  FlagOption option = {.Type = FlagUint64,
                       .NumArgs = 1,
                       .ParseFunc = &ParseFuncUint64,
                       .Help = {.Name = name, .Help = help},
                       .Value = value,
                       .MaxLen = 0};
  return option;
}

FlagCommand FlagNewCommand(const char *name, const char *help) {
  FlagCommand cmd = {.Help = {.Name = name, .Help = help}};
  return cmd;
}

Flags FlagsDefine(FlagOptions options, FlagCommands commands) {
  Flags flags = {.Options = options, .Commands = commands};
  return flags;
}

Flags FlagsDefineOnlyOptions(FlagOptions options) {
  FlagCommands cmds = {.CommandsLen = 0};
  return FlagsDefine(options, cmds);
}

Flags FlagsDefineOnlyCommands(FlagCommands cmds) {
  FlagOptions options = {.OptionsLen = 0};
  return FlagsDefine(options, cmds);
}
