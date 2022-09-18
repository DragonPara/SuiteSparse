//------------------------------------------------------------------------------
// SuiteSparse_config/SuiteSparse_config.h: common utilites for SuiteSparse
//------------------------------------------------------------------------------

// SuiteSparse_config, Copyright (c) 2012-2022, Timothy A. Davis.
// All Rights Reserved.
// SPDX-License-Identifier: BSD-3-clause

//------------------------------------------------------------------------------

/* Configuration file for SuiteSparse: a Suite of Sparse matrix packages (AMD,
 * COLAMD, CCOLAMD, CAMD, CHOLMOD, UMFPACK, CXSparse, and others).
 *
 * SuiteSparse_config.h provides the definition of the long integer:
 * SuiteSparse_long.  For SuiteSparse v6.0.0 and later, this is now int64_t for
 * all compilers, and ANSI C11 is assumed.  In prior versions, its definition
 * depended on the C compiler, with the intent of ensuring it was a 64-bit
 * signed integer.
 *
 * This file also defines the SUITESPARSE_VERSION and related definitions.
 */

#ifndef SUITESPARSE_CONFIG_H
#define SUITESPARSE_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/* SuiteSparse-wide ANSI C11 #include files */
/* -------------------------------------------------------------------------- */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <ctype.h>

/* -------------------------------------------------------------------------- */
/* OpenMP */
/* -------------------------------------------------------------------------- */

#if defined ( _OPENMP )

    #include <omp.h>
    #define SUITESPARSE_OPENMP_MAX_THREADS       omp_get_max_threads ( )
    #define SUITESPARSE_OPENMP_GET_NUM_THREADS   omp_get_num_threads ( )
    #define SUITESPARSE_OPENMP_GET_WTIME         omp_get_wtime ( )
    #define SUITESPARSE_OPENMP_GET_THREAD_ID     omp_get_thread_num ( )

#else

    /* OpenMP not available */
    #define SUITESPARSE_OPENMP_MAX_THREADS       (1)
    #define SUITESPARSE_OPENMP_GET_NUM_THREADS   (1)
    #define SUITESPARSE_OPENMP_GET_WTIME         (0)
    #define SUITESPARSE_OPENMP_GET_THREAD_ID     (0)

#endif

/* -------------------------------------------------------------------------- */
/* MATLAB/Octave */
/* -------------------------------------------------------------------------- */

#if defined ( MATLAB_MEX_FILE )
#include "mex.h"
#include "matrix.h"
#endif

/* -------------------------------------------------------------------------- */
/* string handling macros */
/* -------------------------------------------------------------------------- */

/* SUITESPARSE_STR: convert the content of x into a string "x" */
#define SUITESPARSE_XSTR(x) SUITESPARSE_STR(x)
#define SUITESPARSE_STR(x) #x

/* -------------------------------------------------------------------------- */
/* determine which compiler is in use */
/* -------------------------------------------------------------------------- */

#if defined ( __NVCC__ )

    /* NVIDIA nvcc compiler */
    #define SUITESPARSE_COMPILER_NVCC    1
    #define SUITESPARSE_COMPILER_ICX     0
    #define SUITESPARSE_COMPILER_ICC     0
    #define SUITESPARSE_COMPILER_CLANG   0
    #define SUITESPARSE_COMPILER_GCC     0
    #define SUITESPARSE_COMPILER_MSC     0
    #define SUITESPARSE_COMPILER_XLC     0
    #define SUITESPARSE_COMPILER_MINGW   0

    #define SUITESPARSE_COMPILER_MAJOR __CUDACC_VER_MAJOR__
    #define SUITESPARSE_COMPILER_MINOR __CUDACC_VER_MINOR__
    #define SUITESPARSE_COMPILER_SUB   __CUDACC_VER_BUILD__
    #define SUITESPARSE_COMPILER_NAME  "nvcc"

#elif defined ( __INTEL_CLANG_COMPILER )

    /* Intel icx compiler, 2022.0.0 based on clang/llvm 14.0.0 */
    #define SUITESPARSE_COMPILER_NVCC    0
    #define SUITESPARSE_COMPILER_ICX     1
    #define SUITESPARSE_COMPILER_ICC     0
    #define SUITESPARSE_COMPILER_CLANG   0
    #define SUITESPARSE_COMPILER_GCC     0
    #define SUITESPARSE_COMPILER_MSC     0
    #define SUITESPARSE_COMPILER_XLC     0
    #define SUITESPARSE_COMPILER_MINGW   0

    #define SUITESPARSE_COMPILER_MAJOR __INTEL_CLANG_COMPILER
    #define SUITESPARSE_COMPILER_MINOR 0
    #define SUITESPARSE_COMPILER_SUB   0
    #define SUITESPARSE_COMPILER_NAME  __VERSION__

#elif defined ( __INTEL_COMPILER )

    /* Intel icc compiler: 2021.5.0 uses "gcc 7.5 mode" */
    #define SUITESPARSE_COMPILER_NVCC    0
    #define SUITESPARSE_COMPILER_ICX     0
    #define SUITESPARSE_COMPILER_ICC     1
    #define SUITESPARSE_COMPILER_CLANG   0
    #define SUITESPARSE_COMPILER_GCC     0
    #define SUITESPARSE_COMPILER_MSC     0
    #define SUITESPARSE_COMPILER_XLC     0
    #define SUITESPARSE_COMPILER_MINGW   0

    #define SUITESPARSE_COMPILER_MAJOR __INTEL_COMPILER
    #define SUITESPARSE_COMPILER_MINOR __INTEL_COMPILER_UPDATE
    #define SUITESPARSE_COMPILER_SUB   0
    #define SUITESPARSE_COMPILER_NAME  __VERSION__

#elif defined ( __clang__ )

    /* clang */
    #define SUITESPARSE_COMPILER_NVCC    0
    #define SUITESPARSE_COMPILER_ICX     0
    #define SUITESPARSE_COMPILER_ICC     0
    #define SUITESPARSE_COMPILER_CLANG   1
    #define SUITESPARSE_COMPILER_GCC     0
    #define SUITESPARSE_COMPILER_MSC     0
    #define SUITESPARSE_COMPILER_XLC     0
    #define SUITESPARSE_COMPILER_MINGW   0

    #define SUITESPARSE_COMPILER_MAJOR __clang_major__
    #define SUITESPARSE_COMPILER_MINOR __clang_minor__
    #define SUITESPARSE_COMPILER_SUB   __clang_patchlevel__
    #define SUITESPARSE_COMPILER_NAME  "clang " __clang_version__

#elif defined ( __xlC__ )

    /* xlc */
    #define SUITESPARSE_COMPILER_NVCC    0
    #define SUITESPARSE_COMPILER_ICX     0
    #define SUITESPARSE_COMPILER_ICC     0
    #define SUITESPARSE_COMPILER_CLANG   0
    #define SUITESPARSE_COMPILER_GCC     0
    #define SUITESPARSE_COMPILER_MSC     0
    #define SUITESPARSE_COMPILER_XLC     1
    #define SUITESPARSE_COMPILER_MINGW   0

    #define SUITESPARSE_COMPILER_MAJOR ( __xlC__ / 256 )
    #define SUITESPARSE_COMPILER_MINOR \
        ( __xlC__ - 256 * SUITESPARSE_COMPILER_MAJOR)
    #define SUITESPARSE_COMPILER_SUB   0
    #define SUITESPARSE_COMPILER_NAME  "IBM xlc " SUITESPARSE_XSTR (__xlC__)

#elif defined ( __GNUC__ )

    /* gcc */
    #define SUITESPARSE_COMPILER_NVCC    0
    #define SUITESPARSE_COMPILER_ICX     0
    #define SUITESPARSE_COMPILER_ICC     0
    #define SUITESPARSE_COMPILER_CLANG   0
    #define SUITESPARSE_COMPILER_GCC     1
    #define SUITESPARSE_COMPILER_MSC     0
    #define SUITESPARSE_COMPILER_XLC     0
    #define SUITESPARSE_COMPILER_MINGW   0

    #define SUITESPARSE_COMPILER_MAJOR __GNUC__
    #define SUITESPARSE_COMPILER_MINOR __GNUC_MINOR__
    #define SUITESPARSE_COMPILER_SUB   __GNUC_PATCHLEVEL__
    #define SUITESPARSE_COMPILER_NAME  "GNU gcc "   \
        SUITESPARSE_XSTR (__GNUC__) "."             \
        SUITESPARSE_XSTR (__GNUC_MINOR__) "."       \
        SUITESPARSE_XSTR (__GNUC_PATCHLEVEL__)

#elif defined ( _MSC_VER )

    /* Microsoft Visual Studio */
    #define SUITESPARSE_COMPILER_NVCC    0
    #define SUITESPARSE_COMPILER_ICX     0
    #define SUITESPARSE_COMPILER_ICC     0
    #define SUITESPARSE_COMPILER_CLANG   0
    #define SUITESPARSE_COMPILER_GCC     0
    #define SUITESPARSE_COMPILER_MSC     1
    #define SUITESPARSE_COMPILER_XLC     0
    #define SUITESPARSE_COMPILER_MINGW   0

    #define SUITESPARSE_COMPILER_MAJOR ( _MSC_VER / 100 )
    #define SUITESPARSE_COMPILER_MINOR \
        ( _MSC_VER - 100 * SUITESPARSE_COMPILER_MAJOR)
    #define SUITESPARSE_COMPILER_SUB   0
    #define SUITESPARSE_COMPILER_NAME  \
        "Microsoft Visual Studio " SUITESPARSE_XSTR (_MSC_VER)

#elif defined ( __MINGW32__ ) || defined ( __MINGW64__ )

    /* MinGW (32-bit or 64-bit) */
    #define SUITESPARSE_COMPILER_NVCC    0
    #define SUITESPARSE_COMPILER_ICX     0
    #define SUITESPARSE_COMPILER_ICC     0
    #define SUITESPARSE_COMPILER_CLANG   0
    #define SUITESPARSE_COMPILER_GCC     0
    #define SUITESPARSE_COMPILER_MSC     0
    #define SUITESPARSE_COMPILER_XLC     0
    #define SUITESPARSE_COMPILER_MINGW   1

    #if defined ( __MINGW32__ )
    #define SUITESPARSE_COMPILER_MAJOR ( __MINGW32_MAJOR_VERSION )
    #define SUITESPARSE_COMPILER_MINOR ( __MINGW32_MINOR_VERSION )
    #else
    #define SUITESPARSE_COMPILER_MAJOR ( __MINGW64_MAJOR_VERSION )
    #define SUITESPARSE_COMPILER_MINOR ( __MINGW64_MINOR_VERSION )
    #endif
    #define SUITESPARSE_COMPILER_SUB   0
    #define SUITESPARSE_COMPILER_NAME  "MinGW"

#else

    /* other compiler */
    #define SUITESPARSE_COMPILER_NVCC    0
    #define SUITESPARSE_COMPILER_ICX     0
    #define SUITESPARSE_COMPILER_ICC     0
    #define SUITESPARSE_COMPILER_CLANG   0
    #define SUITESPARSE_COMPILER_GCC     0
    #define SUITESPARSE_COMPILER_MSC     0
    #define SUITESPARSE_COMPILER_XLC     0
    #define SUITESPARSE_COMPILER_MINGW   0

    #define SUITESPARSE_COMPILER_MAJOR 0
    #define SUITESPARSE_COMPILER_MINOR 0
    #define SUITESPARSE_COMPILER_SUB   0
    #define SUITESPARSE_COMPILER_NAME  "other C compiler"

#endif

/* -------------------------------------------------------------------------- */
/* malloc.h: required include file for Microsoft Visual Studio */
/* -------------------------------------------------------------------------- */

#if SUITESPARSE_COMPILER_MSC
    #include <malloc.h>
#endif

/* -------------------------------------------------------------------------- */
/* Exporting/importing symbols for Microsoft Visual Studio */
/* -------------------------------------------------------------------------- */

#if SUITESPARSE_COMPILER_MSC
#ifdef SUITESPARSE_LIBRARY
/* compiling SuiteSparse itself, exporting symbols to user apps */
#define SUITESPARSE_PUBLIC extern __declspec ( dllexport )
#else
/* compiling the user application, importing symbols from SuiteSparse */
#define SUITESPARSE_PUBLIC extern __declspec ( dllimport )
#endif
#else
/* for other compilers */
#define SUITESPARSE_PUBLIC extern
#endif

/* -------------------------------------------------------------------------- */
/* determine the ANSI C version */
/* -------------------------------------------------------------------------- */

#ifdef __STDC_VERSION__
/* ANSI C17: 201710L */
/* ANSI C11: 201112L */
/* ANSI C99: 199901L */
/* ANSI C95: 199409L */
#define SUITESPARSE_STDC_VERSION __STDC_VERSION__
#else
/* assume ANSI C90 / C89 */
#define SUITESPARSE_STDC_VERSION 199001L
#endif

/* -------------------------------------------------------------------------- */
/* handle the restrict keyword */
/* -------------------------------------------------------------------------- */

#if defined ( __cplusplus )

    /* C++ does not have the "restrict" keyword */
    #define SUITESPARSE_RESTRICT

#elif SUITESPARSE_COMPILER_MSC

    /* MS Visual Studio */
    #define SUITESPARSE_RESTRICT __restrict

#elif SUITESPARSE_COMPILER_NCSS

    /* NVIDIA nvcc */
    #define SUITESPARSE_RESTRICT __restrict__

#elif SUITESPARSE_STDC_VERSION >= 199901L

    /* ANSI C99 or later */
    #define SUITESPARSE_RESTRICT restrict

#else

    /* ANSI C95 and earlier: no restrict keyword */
    #define SUITESPARSE_RESTRICT

#endif

/* ========================================================================== */
/* SuiteSparse_long */
/* ========================================================================== */

#undef  SuiteSparse_long
#define SuiteSparse_long int64_t
#define SuiteSparse_long_max INT64_MAX
#define SuiteSparse_long_idd PRId64
#define SuiteSparse_long_id "%" SuiteSparse_long_idd

/* ========================================================================== */
/* SuiteSparse_config parameters and functions */
/* ========================================================================== */

/* SuiteSparse-wide parameters are placed in this struct.  It is meant to be
   an extern, globally-accessible struct.  It is not meant to be updated
   frequently by multiple threads.  Rather, if an application needs to modify
   SuiteSparse_config, it should do it once at the beginning of the application,
   before multiple threads are launched.

   The intent of these function pointers is that they not be used in your
   application directly, except to assign them to the desired user-provided
   functions.  Rather, you should use the SuiteSparse_malloc/calloc, etc
   wrappers defined below to access them.
 */

struct SuiteSparse_config_struct
{
    void *(*malloc_func) (size_t) ;             /* pointer to malloc */
    void *(*calloc_func) (size_t, size_t) ;     /* pointer to calloc */
    void *(*realloc_func) (void *, size_t) ;    /* pointer to realloc */
    void (*free_func) (void *) ;                /* pointer to free */
    int (*printf_func) (const char *, ...) ;    /* pointer to printf */
    double (*hypot_func) (double, double) ;     /* pointer to hypot */
    int (*divcomplex_func) (double, double, double, double, double *, double *);
} ;

SUITESPARSE_PUBLIC struct SuiteSparse_config_struct SuiteSparse_config ;

SUITESPARSE_PUBLIC
void SuiteSparse_start ( void ) ;   /* called to start SuiteSparse */

SUITESPARSE_PUBLIC
void SuiteSparse_finish ( void ) ;  /* called to finish SuiteSparse */

SUITESPARSE_PUBLIC
void *SuiteSparse_malloc    /* pointer to allocated block of memory */
(
    size_t nitems,          /* number of items to malloc (>=1 is enforced) */
    size_t size_of_item     /* sizeof each item */
) ;

SUITESPARSE_PUBLIC
void *SuiteSparse_calloc    /* pointer to allocated block of memory */
(
    size_t nitems,          /* number of items to calloc (>=1 is enforced) */
    size_t size_of_item     /* sizeof each item */
) ;

SUITESPARSE_PUBLIC
void *SuiteSparse_realloc   /* pointer to reallocated block of memory, or
                               to original block if the realloc failed. */
(
    size_t nitems_new,      /* new number of items in the object */
    size_t nitems_old,      /* old number of items in the object */
    size_t size_of_item,    /* sizeof each item */
    void *p,                /* old object to reallocate */
    int *ok                 /* 1 if successful, 0 otherwise */
) ;

SUITESPARSE_PUBLIC
void *SuiteSparse_free      /* always returns NULL */
(
    void *p                 /* block to free */
) ;

SUITESPARSE_PUBLIC
void SuiteSparse_tic    /* start the timer */
(
    double tic [2]      /* output, contents undefined on input */
) ;

SUITESPARSE_PUBLIC
double SuiteSparse_toc  /* return time in seconds since last tic */
(
    double tic [2]      /* input: from last call to SuiteSparse_tic */
) ;

SUITESPARSE_PUBLIC
double SuiteSparse_time  /* returns current wall clock time in seconds */
(
    void
) ;

/* returns sqrt (x^2 + y^2), computed reliably */
SUITESPARSE_PUBLIC
double SuiteSparse_hypot (double x, double y) ;

/* complex division of c = a/b */
SUITESPARSE_PUBLIC
int SuiteSparse_divcomplex
(
    double ar, double ai,       /* real and imaginary parts of a */
    double br, double bi,       /* real and imaginary parts of b */
    double *cr, double *ci      /* real and imaginary parts of c */
) ;

/* determine which timer to use, if any */
#ifndef NTIMER
    #if defined ( _OPENMP )
        #define SUITESPARSE_TIMER_ENABLED
    #elif defined ( _POSIX_C_SOURCE )
        #if _POSIX_C_SOURCE >= 199309L
        #define SUITESPARSE_TIMER_ENABLED
        #endif
    #endif
#endif

/* SuiteSparse printf macro */
#define SUITESPARSE_PRINTF(params)                          \
{                                                           \
    if (SuiteSparse_config.printf_func != NULL)             \
    {                                                       \
        (void) (SuiteSparse_config.printf_func) params ;    \
    }                                                       \
}

/* ========================================================================== */
/* SuiteSparse version */
/* ========================================================================== */

/* SuiteSparse is not a package itself, but a collection of packages, some of
 * which must be used together (UMFPACK requires AMD, CHOLMOD requires AMD,
 * COLAMD, CAMD, and CCOLAMD, etc).  A version number is provided here for the
 * collection itself, which is also the version number of SuiteSparse_config.
 */

SUITESPARSE_PUBLIC
int SuiteSparse_version     /* returns SUITESPARSE_VERSION */
(
    /* output, not defined on input.  Not used if NULL.  Returns
       the three version codes in version [0..2]:
       version [0] is SUITESPARSE_MAIN_VERSION
       version [1] is SUITESPARSE_SUB_VERSION
       version [2] is SUITESPARSE_SUBSUB_VERSION
       */
    int version [3]
) ;

/* Versions prior to 4.2.0 do not have the above function.  The following
   code fragment will work with any version of SuiteSparse:

   #ifdef SUITESPARSE_HAS_VERSION_FUNCTION
   v = SuiteSparse_version (NULL) ;
   #else
   v = SUITESPARSE_VERSION ;
   #endif
*/

#define SUITESPARSE_HAS_VERSION_FUNCTION

#define SUITESPARSE_DATE "Oct 1, 2022"
#define SUITESPARSE_MAIN_VERSION    6
#define SUITESPARSE_SUB_VERSION     0
#define SUITESPARSE_SUBSUB_VERSION  0

#define SUITESPARSE_VER_CODE(main,sub) ((main) * 1000 + (sub))
#define SUITESPARSE_VERSION \
    SUITESPARSE_VER_CODE(SUITESPARSE_MAIN_VERSION,SUITESPARSE_SUB_VERSION)

#ifdef __cplusplus
}
#endif
#endif

