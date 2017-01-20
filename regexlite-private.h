/**
 * @file tempile-private.h
 * @brief Declarations for TemPile class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_TEMPILE_PRIVATE_H_INCLUDE
#define GUARD_TEMPILE_PRIVATE_H_INCLUDE

#include <tempile/tempile-config.h>

#if 0
#    define TEMPILE_DEBUGM printf
#else
#    define TEMPILE_DEBUGM black_hole
#endif

#if 0
#    define TEMPILE_TRACE_ENTRY printf("TEMPILE ENTRY %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define TEMPILE_TRACE_ENTRY
#endif

#if 0
#    define TEMPILE_TRACE_EXIT printf("TEMPILE EXIT %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define TEMPILE_TRACE_EXIT
#endif


static inline void black_hole (...)
{}

#endif // GUARD_TEMPILE_PRIVATE_H_INCLUDE
