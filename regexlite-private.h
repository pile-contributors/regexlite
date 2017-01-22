/**
 * @file regexlite-private.h
 * @brief Declarations for regexlite class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_REGEXLITE_PRIVATE_H_INCLUDE
#define GUARD_REGEXLITE_PRIVATE_H_INCLUDE

#include <regexlite/regexlite-config.h>

#if 0
#    define REGEXLITE_DEBUGM printf
#else
#    define REGEXLITE_DEBUGM black_hole
#endif

#if 0
#    define REGEXLITE_TRACE_ENTRY printf("regexlite ENTRY %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define REGEXLITE_TRACE_ENTRY
#endif

#if 0
#    define REGEXLITE_TRACE_EXIT printf("regexlite EXIT %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define REGEXLITE_TRACE_EXIT
#endif

#ifndef BLACK_HOLE
static inline void black_hole (...)
{}
#define BLACK_HOLE black_hole
#endif

#endif // GUARD_REGEXLITE_PRIVATE_H_INCLUDE
