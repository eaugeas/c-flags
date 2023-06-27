#include <stdlib.h>

#include <flags/flags.h>
#include <flags/strings.h>

#include "asserts.h"
#include "runner.h"

static int Test_FlagsStringFlag(void) {
  size_t len = 16;
  char value[16] = "not set";
  int argc = 3;
  int index = -1;
  char *argv[] = {"test", "-string", "value"};
  FlagOptionsDeclare(options,
                     FlagsNewString(value, len, "string", "my string value"), );
  Flags flags = FlagsDefineOnlyOptions(options);

  FlagError err = FlagsParse(argc, argv, &flags, &index);

  AssertNotError(err);
  AssertTrue(StringEquals(value, "value"));
  return EXIT_SUCCESS;
}

static int Test_FlagsBoolFlag(void) {
  bool value = false;
  int argc = 2;
  int index = -1;
  char *argv[] = {"test", "-bool"};
  FlagOptionsDeclare(options, FlagsNewBool(&value, "bool", "my bool value"), );
  Flags flags = FlagsDefineOnlyOptions(options);

  FlagError err = FlagsParse(argc, argv, &flags, &index);

  AssertNotError(err);
  AssertTrue(value);
  return EXIT_SUCCESS;
}

static int Test_FlagsInt32Flag(void) {
  int32_t value = 0;
  int argc = 3;
  int index = -1;
  char *argv[] = {"prog", "-int32", "1"};
  FlagOptionsDeclare(options,
                     FlagsNewInt32(&value, "int32", "my int32 value"), );
  Flags flags = FlagsDefineOnlyOptions(options);

  FlagError err = FlagsParse(argc, argv, &flags, &index);

  AssertNotError(err);
  AssertEq(value, 1);
  return EXIT_SUCCESS;
}

static int Test_FlagsInt64Flag(void) {
  int64_t value = 0;
  int argc = 3;
  int index = -1;
  char *argv[] = {"prog", "-int64", "1"};
  FlagOptionsDeclare(options,
                     FlagsNewInt64(&value, "int64", "my int64 value"), );
  Flags flags = FlagsDefineOnlyOptions(options);

  FlagError err = FlagsParse(argc, argv, &flags, &index);

  AssertNotError(err);
  AssertEq(value, 1);
  return EXIT_SUCCESS;
}

static int Test_FlagsUint32Flag(void) {
  uint32_t value = 0;
  int argc = 3;
  int index = -1;
  char *argv[] = {"prog", "-uint32", "1"};
  FlagOptionsDeclare(options,
                     FlagsNewUint32(&value, "uint32", "my uint32 value"), );
  Flags flags = FlagsDefineOnlyOptions(options);

  FlagError err = FlagsParse(argc, argv, &flags, &index);

  AssertNotError(err);
  AssertEq(value, 1);
  return EXIT_SUCCESS;
}

static int Test_FlagsUint64Flag(void) {
  uint64_t value = 0;
  int argc = 3;
  int index = -1;
  char *argv[] = {"prog", "-uint64", "1"};
  FlagOptionsDeclare(options,
                     FlagsNewUint64(&value, "uint64", "my uint64 value"), );
  Flags flags = FlagsDefineOnlyOptions(options);

  FlagError err = FlagsParse(argc, argv, &flags, &index);

  AssertNotError(err);
  AssertEq(value, 1);
  return EXIT_SUCCESS;
}

static int Test_FlagsParseBoolAfterInt(void) {
  bool help = false;
  uint64_t value = 0;
  int argc = 4;
  int index = -1;
  char *argv[] = {"prog", "-uint64", "1", "-help"};
  FlagOptionsDeclare(options,
                     FlagsNewUint64(&value, "uint64", "my uint64 value"),
                     FlagsNewBool(&help, "help", "sets help"), );
  Flags flags = FlagsDefineOnlyOptions(options);

  FlagError err = FlagsParse(argc, argv, &flags, &index);

  AssertNotError(err);
  AssertEq(value, 1);
  AssertTrue(help);
  return EXIT_SUCCESS;
}

static int Test_FlagsMultiFlags(void) {
  char stringValue[16] = "not set";
  bool boolValue = false;
  int32_t int32Value = 0;
  int64_t int64Value = 0;
  uint32_t uint32Value = 0;
  uint64_t uint64Value = 0;
  int argc = 12;
  int index = -1;
  char *argv[] = {"test",   "-string", "value",   "-bool", "-int32",  "1",
                  "-int64", "1",       "-uint32", "1",     "-uint64", "1"};
  FlagOptionsDeclare(
      options, FlagsNewString(stringValue, 16, "string", "my string value"),
      FlagsNewBool(&boolValue, "bool", "my bool value"),
      FlagsNewInt32(&int32Value, "int32", "my int32 value"),
      FlagsNewInt64(&int64Value, "int64", "my int64 value"),
      FlagsNewUint32(&uint32Value, "uint32", "my uint32 value"),
      FlagsNewUint64(&uint64Value, "uint64", "my uint64 value"), );
  Flags flags = FlagsDefineOnlyOptions(options);

  FlagError err = FlagsParse(argc, argv, &flags, &index);
  AssertNotError(err);

  AssertStringEq(stringValue, "value");
  AssertEq(boolValue, true);
  AssertEq(int32Value, 1);
  AssertEq(int64Value, 1);
  AssertEq(uint32Value, 1u);
  AssertEq(uint64Value, 1u);
  return EXIT_SUCCESS;
}

static int Test_FlagsParseMissingArgs(void) {
  size_t len = 16;
  char value[16] = "not set";
  int argc = 2;
  int index = -1;
  char *argv[] = {"test", "-string"};
  FlagOptionsDeclare(options,
                     FlagsNewString(value, len, "string", "my string value"), );
  Flags flags = FlagsDefineOnlyOptions(options);

  FlagError err = FlagsParse(argc, argv, &flags, &index);

  AssertEq(err, FlagErrNoArg);
  return EXIT_SUCCESS;
}

static int Test_FlagsParseUnknown(void) {
  int argc = 3;
  int index = -1;
  char *argv[] = {"test", "-string", "value"};
  Flags flags = {.Options = {.OptionsLen = 0}, .Commands = {.CommandsLen = 0}};

  FlagError err = FlagsParse(argc, argv, &flags, &index);

  AssertEq(err, FlagErrUnknownFlag);
  return EXIT_SUCCESS;
}

static int Test_FlagsParseInvalidInt32(void) {
  int32_t value = 0;
  int argc = 3;
  int index = -1;
  char *argv[] = {"prog", "-int32", "string"};
  FlagOptionsDeclare(options,
                     FlagsNewInt32(&value, "int32", "my int32 value"), );
  Flags flags = FlagsDefineOnlyOptions(options);

  FlagError err = FlagsParse(argc, argv, &flags, &index);

  AssertEq(err, FlagErrParse);
  return EXIT_SUCCESS;
}

static int Test_FlagsCommandOnly(void) {
  char value[16] = "not set";
  char cmd[16] = "not set";
  int argc = 2;
  int index = -1;
  char *argv[] = {"test", "cmd1"};
  FlagCommandsDeclare(cmds, cmd, 16, FlagNewCommand("cmd1", "command one"),
                      FlagNewCommand("cmd2", "command two"));
  Flags flags = FlagsDefineOnlyCommands(cmds);

  FlagError err = FlagsParse(argc, argv, &flags, &index);

  AssertNotError(err);
  AssertTrue(StringEquals(cmd, "cmd1"));
  AssertTrue(StringEquals(value, "not set"));
  return EXIT_SUCCESS;
}

static int Test_FlagsCommandWithOption(void) {
  size_t len = 16;
  char value[16] = "not set";
  char cmd[16] = "not set";
  int argc = 4;
  int index = -1;
  char *argv[] = {"test", "-string", "value", "cmd1"};
  FlagCommandsDeclare(cmds, cmd, 16, FlagNewCommand("cmd1", "command one"),
                      FlagNewCommand("cmd2", "command two"));
  FlagOptionsDeclare(options,
                     FlagsNewString(value, len, "string", "my string value"));
  Flags flags = FlagsDefine(options, cmds);

  FlagError err = FlagsParse(argc, argv, &flags, &index);

  AssertNotError(err);
  AssertStringEq(value, "value");
  AssertStringEq(cmd, "cmd1");
  return EXIT_SUCCESS;
}

static int Test_FlagsCommandUnknown(void) {
  char cmd[16] = "not set";
  int argc = 2;
  int index = -1;
  char *argv[] = {"test", "cmd3"};
  FlagCommandsDeclare(cmds, cmd, 16, FlagNewCommand("cmd1", "command one"),
                      FlagNewCommand("cmd2", "command two"));
  Flags flags = FlagsDefineOnlyCommands(cmds);

  FlagError err = FlagsParse(argc, argv, &flags, &index);

  AssertError(err);
  AssertEq(err, FlagErrUnknownCommand);
  AssertStringEq(cmd, "not set");
  return EXIT_SUCCESS;
}

int main() {
  TestRun(Test_FlagsStringFlag);
  TestRun(Test_FlagsBoolFlag);
  TestRun(Test_FlagsParseBoolAfterInt);
  TestRun(Test_FlagsInt32Flag);
  TestRun(Test_FlagsInt64Flag);
  TestRun(Test_FlagsUint32Flag);
  TestRun(Test_FlagsUint64Flag);
  TestRun(Test_FlagsMultiFlags);
  TestRun(Test_FlagsParseMissingArgs);
  TestRun(Test_FlagsParseUnknown);
  TestRun(Test_FlagsParseInvalidInt32);
  TestRun(Test_FlagsCommandOnly);
  TestRun(Test_FlagsCommandWithOption);
  TestRun(Test_FlagsCommandUnknown);

  return EXIT_SUCCESS;
}
