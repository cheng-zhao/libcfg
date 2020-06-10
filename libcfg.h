/*******************************************************************************
* libcfg.h: this file is part of the libcfg library.

* libcfg: C library for parsing command line option and configuration files.

* Github repository:
        https://github.com/cheng-zhao/libcfg

* Copyright (c) 2019 Cheng Zhao <zhaocheng03@gmail.com>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*******************************************************************************/

#ifndef _LIBCFG_H_
#define _LIBCFG_H_

#include <stdio.h>
#include <stdbool.h>

/******************************************************************************
  Definitions for data types.
******************************************************************************/

typedef enum {
  CFG_DTYPE_NULL,
  CFG_DTYPE_BOOL,
  CFG_DTYPE_CHAR,
  CFG_DTYPE_INT,
  CFG_DTYPE_LONG,
  CFG_DTYPE_FLT,
  CFG_DTYPE_DBL,
  CFG_DTYPE_STR,
  CFG_ARRAY_BOOL,
  CFG_ARRAY_CHAR,
  CFG_ARRAY_INT,
  CFG_ARRAY_LONG,
  CFG_ARRAY_FLT,
  CFG_ARRAY_DBL,
  CFG_ARRAY_STR
} cfg_dtype_t;

#define CFG_DTYPE_INVALID(x)    ((x) < CFG_DTYPE_BOOL || (x) > CFG_ARRAY_STR)
#define CFG_DTYPE_IS_ARRAY(x)   ((x) >= CFG_ARRAY_BOOL && (x) <= CFG_ARRAY_STR)

/******************************************************************************
  Definitions for string lengths.
******************************************************************************/
#define CFG_MAX_NAME_LEN        128
#define CFG_MAX_LOPT_LEN        128
#define CFG_MAX_FILENAME_LEN    1024

/******************************************************************************
  Definitions for the formats.
******************************************************************************/
#define CFG_SYM_EQUAL           '='
#define CFG_SYM_ARRAY_START     '['
#define CFG_SYM_ARRAY_END       ']'
#define CFG_SYM_ARRAY_SEP       ','
#define CFG_SYM_COMMENT         '#'
#define CFG_SYM_NEWLINE         '\\'

#define CFG_CMD_FLAG            '-'
#define CFG_CMD_ASSIGN          '='


/******************************************************************************
  Definition of data types and structures.
******************************************************************************/

/* Main entry for all configuration parameters and command line functions. */
typedef struct {
  int npar;             /* number of verified configuration parameters  */
  int nfunc;            /* number of verified command line functions    */
  void *params;         /* data structure for storing parameters        */
  void *funcs;          /* data structure for storing function pointers */
  void *error;          /* data structure for storing error messages    */
} cfg_t;

/* Interface for registering configuration parameters. */
typedef struct {
  int opt;                      /* short command line option            */
  char *lopt;                   /* long command line option             */
  char *name;                   /* name of the parameter                */
  cfg_dtype_t dtype;            /* data type of the parameter           */
  void *var;                    /* variable for the retrieved value     */
} cfg_param_t;

/* Interface for registering command line functions. */
typedef struct {
  int opt;                      /* short command line option            */
  char *lopt;                   /* long command line option             */
  void (*func) (void *);        /* pointer to the function              */
  void *args;                   /* pointer to the arguments             */
} cfg_func_t;


/******************************************************************************
  Definition of functions.
******************************************************************************/

cfg_t *cfg_init(void);

int cfg_set_params(cfg_t *, const cfg_param_t *, const int);

int cfg_set_funcs(cfg_t *, const cfg_func_t *, const int);

int cfg_read_opts(cfg_t *, const int, char *const *, const int, int *);

int cfg_read_file(cfg_t *, const char *, const int);

bool cfg_is_set(const cfg_t *, const void *);

int cfg_get_size(const cfg_t *, const void *);

void cfg_destroy(cfg_t *);

void cfg_perror(const cfg_t *, FILE *, const char *);

void cfg_pwarn(cfg_t *, FILE *, const char *);

#endif
