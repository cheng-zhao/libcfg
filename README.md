# libcfg

![GitHub](https://img.shields.io/github/license/cheng-zhao/libcfg.svg)
![Codacy grade](https://img.shields.io/codacy/grade/aa80ae437f8a4aaba942d922500a5e36.svg)

## Table of Contents

-   [Introduction](#introduction)
-   [Compilation and linking](#compilation-and-linking)
-   [Getting started](#getting-started)
    -   [Initialisation](#initialisation)
    -   [Parameter registration](#parameter-registration)
    -   [Function registration](#function-registration)
    -   [Parsing command line options](#parsing-command-line-options)
    -   [Parsing configuration file](#parsing-configuration-file)
    -   [Result validation](#result-validation)
    -   [Releasing memory](#releasing-memory)
    -   [Error handling](#error-handling)
    -   [Examples](#examples)

## Introduction

This is a simple library written in C, for parsing command line options and plain configuration files. It can be used to retrieve variables and arrays into runtime memory, or call functions indicated by command line flags.

This library is compliant with the ISO C99 standard, and relies only on the C standard library. It is written by Cheng Zhao (&#36213;&#25104;), and is distributed under the [MIT license](LICENSE.txt).

<sub>[\[TOC\]](#table-of-contents)</sub>

## Compilation and linking

Since this library is tiny and portable, it is recommended to compile the only two source files &mdash; `libcfg.h` and `libcfg.c` &mdash; along with your own program. To this end, one only needs to include the header `libcfg.h` in the source file for parsing configurations:

```c
#include "libcfg.h"
```

<sub>[\[TOC\]](#table-of-contents)</sub>

## Getting started

### Initialisation

A `cfg_t` type structure has to be initialised in the first place, for storing all the configuration information. This can be done using the function `cfg_init`, e.g

```c
cfg_t *cfg = cfg_init();
```

This function returns the `NULL` pointer on error.

<sub>[\[TOC\]](#table-of-contents)</sub>

### Parameter registration

To retrieve variables and arrays from command line options or configuration files, they need to be registered as configuration parameters, which are represented by the `cfg_param_t` type structure:

```c
typedef struct {
  int opt;                      /* short command line option            */
  char *lopt;                   /* long command line option             */
  char *name;                   /* name of the parameter                */
  cfg_dtype_t dtype;            /* data type of the parameter           */
  void *var;                    /* variable for the retrieved value     */
} cfg_param_t;
```

The format of the attributes are:
-   `opt`: a single case-sensitive letter or `0`;
-   `lopt`: a string composed of characters with graphical representations (ensured by [isgraph](https://en.cppreference.com/w/c/string/byte/isgraph)), or a `NULL` pointer;
-   `name`: a string composed of case-sensitive letters, digits, and the underscore character, and starting with either a letter or an underscore;
-   `dtype`: a pre-defined data type indicator;
-   `var`: pointer to the address of the variable/array for holding the retrieved value, and no memory allocation is needed.

In particular, if `opt` is set to `0`, or `lopt` is set to `NULL`, the value will not be retrieved from short or long command line options, respectively. For safety consideration, the length of `lopt` and `name` must be smaller than the pre-defined [`CFG_MAX_LOPT_LEN`](define.h#L65) and [`CFG_MAX_NAME_LEN`](define.h#L65) values respectively.

The supported data types and their indicators are listed below:

| Data type                              | Indicator        | Native C type |
|----------------------------------------|------------------|---------------|
| Boolean variable                       | `CFG_DTYPE_BOOL` | `bool`        |
| Character variable                     | `CFG_DTYPE_CHAR` | `char`        |
| Integer variable                       | `CFG_DTYPE_INT`  | `int`         |
| Long integer variable.                 | `CFG_DTYPE_LONG` | `long`        |
| Single-precision floating-point number | `CFG_DTYPE_FLT`  | `float`       |
| Double-precision floating-point number | `CFG_DTYPE_DBL`  | `double`      |
| String variable                        | `CFG_DTYPE_STR`  | `char *`      |
| Boolean array                          | `CFG_ARRAY_BOOL` | `bool *`      |
| Character array                        | `CFG_ARRAY_CHAR` | `char *`      |
| Integer array                          | `CFG_ARRAY_INT`  | `int *`       |
| Long integer array                     | `CFG_ARRAY_LONG` | `long *`      |
| Single-precision floating-point array  | `CFG_ARRAY_FLT`  | `float *`     |
| Double-precision floating-point array  | `CFG_ARRAY_DBL`  | `double *`    |
| String array                           | `CFG_ARRAY_STR`  | `char **`     |

Once the configuration parameters are set, they can be registered using the function

```c
int cfg_set_params(cfg_t *cfg, const cfg_param_t *params, const int npar);
```

Here, `cfg` indicates the structure for storing all configuration information, or the entry for the registration. `params` denotes the address of the configuration parameter structure, and `npar` indicates the number of parameters to be registered at once. This function returns `0` on success, and a non-zero integer on error.

Note that the `cfg_param_t` type structure for parameter registration cannot be deconstructed until the command line options and configuration files containing this parameter are parsed (see [Parsing command line options](#parsing-command-line-options) and [Parsing configuration file](#parsing-configuration-file)),

<sub>[\[TOC\]](#table-of-contents)</sub>

### Function registration

Functions that can be called with command line flags must be declared with the prototype

```c
void func(void *args);
```

Similar to configuration parameters, functions can also be registered in the form of a structure:

```c
typedef struct {
  int opt;                      /* short command line option            */
  char *lopt;                   /* long command line option             */
  void (*func) (void *);        /* pointer to the function              */
  void *args;                   /* pointer to the arguments             */
} cfg_func_t;
```

The `opt` and `lopt` variables are the short and long command line option for calling this function, respectively. And at least one of them has to be set, i.e., a case-sensitive letter for `opt`, or a string composed of graphical characters for `lopt`. Again, the length of `lopt` must be smaller than the pre-defined [`CFG_MAX_LOPT_LEN`](define.h#L65) limit. The pointers `func` and `args` are the address of the function to be called, and the corresponding arguments, respectively.

The functions can then be registered using

```c
int cfg_set_funcs(cfg_t *cfg, const cfg_func_t *funcs, const int nfunc);
```

Here, `cfg` indicates the entry for the registration, `funcs` denotes the address to the structure holding the registration information of functions, and `nfunc` indicates the number of functions to be registered at once. This function returns `0` on success, and a non-zero integer on error.

Note that the `cfg_func_t` type structure for function registration cannot be deconstructed until the command line options are parsed (see [Parsing command line options](#parsing-command-line-options)).

As an example, a typical demand for calling functions via command line is to print the usage of a program, when there is the `-h` or `--help` flag. In this case, the help function and the corresponding structure can be defined as

```c
void help(void *arg) {
  printf("Display the help messages.\n");
  exit(0);
}
```
```c
const cfg_func_t help_func = {'h', "help", help, NULL};
```

<sub>[\[TOC\]](#table-of-contents)</sub>

### Parsing command line options

Command line options are passed to the `main` function at program startup, as the `argc` and `argv` parameters. These two parameters can be used by the function `cfg_read_opts` for parsing the options, and retrieving parameter values or calling functions. This function is defined as

```c
int cfg_read_opts(cfg_t *cfg, const int argc, char *const *argv, const int priority, int *optidx);
```

It returns `0` on success, and a non-zero integer on error. And the arguments are:
-   `cfg`: the structure for all configuration information;
-   `argc`: the number of command line arguments, obtained from `main`;
-   `argv`: the command line argument list, obtained from `main`;
-   `priority`: the priority of values retrieved from command line, must be positive;
-   `optidx`: the index of the argument list at which the parser is terminated.

In particular, `priority` decides which value to take if the parameter is defined in multiple sources, say, both in a configuration file and command line options. For instance, if a variable has already been set in a configuration file with a lower `priority` value than the one passed to `cfg_read_opts`, then its value will be overwritten by the one obtained from command line.

The supported formats of command line options are listed below:

| Description        | Format                                      | Example  | Note                                                                                                                                      |
|--------------------|---------------------------------------------|----------|-------------------------------------------------------------------------------------------------------------------------------------------|
| Short option       | `-OPT VALUE`<br />or<br />`-OPT=VALUE`      | `-n=10`  | `OPT` must be a letter;<br />`VALUE` is optional.                                                                                         |
| Long option        | `--LOPT VALUE` <br/>or<br/> `--LOPT=VALUE`  | `--help` | `LOPT` is a string with graphical characters,<br />with length smaller than [`CFG_MAX_LOPT_LEN`](define.h#L65);<br />`VALUE` is optional. |
| Option terminator | `--`                                        |          | It terminates option scanning.                                                                                                             |

Note that the `-` and `=` symbols in the formats are customisable. They are actually defined as [`CFG_CMD_FLAG`](define.h#L79) and [`CFG_CMD_ASSIGN`](define.h#L80) in `define.h`, respectively.

All command line arguments satisfying the above formats are interpreted as options, otherwise they are treated as values. And values can only be omitted for boolean type variables &mdash; which implies `true` &mdash; or function calls. If the value contains space or special characters that are reserved by the environment, then it should be enclosed by pairs of single or double quotation marks. Besides, values that may be confused with options (such as `-x`) are recommended to be passed with the assignment symbol `=`.

Furthermore, if the `--` option is found, then the option scanning will be terminated, and the current index of the argument list is reported as `optidx`. Therefore, when calling the program, non-option parameters should always be passed after all the options. And when `optidx` is equal to `argc`, it means that all command line arguments are parsed.

<sub>[\[TOC\]](#table-of-contents)</sub>

### Parsing configuration file

Plain text files can be parsed using the function

```c
int cfg_read_file(cfg_t *cfg, const char *filename, const int priority);
```

The argument `cfg` indicates the structure for storing all configuration information, `filename` denotes the name of the input file, and `priority` defines the priority of values read from this file. This function returns `0` on success, and a non-zero integer on error.

By default the format of a valid configuration file has to be

```nginx
# This is a comment.
name_variable = value                # inline comment
name_array = [ element1, element2 ]  # entry for an array
```

Here, `name_variable`, `name_array`, and `name_long_array` indicate the registered name of configuration parameters (see [Parameter registration](#parameter-registration)), and `value`, `element1`, and `element2` are the values to be loaded into memory.

In particular, scalar type definitions can be parsed as arrays with a single element. And by default array type definitions with multiple elements have to be enclosed by a pair of brackets `[]`. In addition, multiple-line definitions are only allowed for arrays, and the line break symbol `\` can only be placed after the array element separator `,`. These symbols, including `[`, `]`, `\`, `,`, as well as the comment indicator `#`, are customisable in [define.h](define.h#L70). And if a value or an element of an array contains special characters, the full value or element has to be enclosed by a pair of single or double quotation marks.

<sub>[\[TOC\]](#table-of-contents)</sub>

### Result validation

The functions `cfg_read_opts` and `cfg_read_file` extract the parameter value from command line options and configuration files respectively. The value is then converted to the given data type, and passed to the address of the variable specified at registration.

To verify whether a variable or an array is set correctly, one can use the function

```c
bool cfg_is_set(const cfg_t *cfg, const void *var);
```

It returns `true` if the variable or array values are set by the functions `cfg_read_opts` or `cfg_read_file`, and `false` if they are untouched. Here, `var` has to be the variable/array address specified at registration.

Moreover, the number of array elements read by the parser can be reported by the function

```c
int cfg_get_size(const cfg_t *cfg, const void *var);
```

It returns `0` if the array is not set. So it may not be necessary to verify arrays using `cfg_is_set`. Note that the array is allocated with precisely the number of elements reported by this function, so the indices for accessing array elements must be smaller than this number.

Once the variable or array is verified successfully, it can then be used directly in the rest parts of the program.

<sub>[\[TOC\]](#table-of-contents)</sub>

### Releasing memory

Once all the variables and arrays are retrieved and verified, the `cfg_t` type structure for storing all the configuration information can be deconstructed by the function

```c
void cfg_destroy(cfg_t *cfg);
```

After calling this function, the values of the variables and arrays are still accessible, but the size of arrays cannot be obtained using the `cfg_get_size` function anymore.

In addition, since the memory of arrays and string variables are allocated by this library, it is the user's responsibility to free them using the standard `free` function. In particular, since string arrays are represented two-dimensional character arrays, the pointers to both the string array and its first element have to be freed, e.g.

```c
char **str;             /* declaration of the string array */

/* parameter registration and retriving */

free(*str);             /* free the first element of the array */
free(str);              /* free the array itself */
```

<sub>[\[TOC\]](#table-of-contents)</sub>

### Error handling

Errors can be caught by checking the return values of some of the functions, such as `cfg_init`, `cfg_set_params`, `cfg_read_opts`, etc. And once the `cfg_init` is executed successfully, error messages can be printed using the function

```c
void cfg_perror(const cfg_t *cfg, FILE *stream, const char *msg);
```

It outputs the string indicated by `msg`, followed by a colon and a space, and then followed by the error message produced by this library, as well as a newline character `\n`. The results are written to `stream`, which is typically `stderr`.

Unexpected issues that are not critical enough to stop the program are treated as warnings. They cannot be handled by the return values of functions, but one can check warning messages using the function

```c
void cfg_pwarn(cfg_t *cfg, FILE *stream, const char *msg);
```

This function is similar to `cfg_perror`. Note that there can be multiple warning messages, and once a warning message is printed, it is automatically removed from the message pool. 

<sub>[\[TOC\]](#table-of-contents)</sub>

### Examples

An examples for the usage of this library is provided in the [example](example) folder.

It registers variables and arrays for all the supported data types, as well as two functions to be called via command line. Command line options and the configuration file [`input.conf`](example/input.conf) are then parsed. The variables and arrays are printed if they are set correctly.

<sub>[\[TOC\]](#table-of-contents)</sub>

