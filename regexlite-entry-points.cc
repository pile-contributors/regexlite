/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/**
 * @file regexlite-entry-points.cc
 * @brief Definitions for ReSqliteUn class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <sqlite/sqlite3ext.h>
static SQLITE_EXTENSION_INIT1

#include "regexlite.h"
#include "regexlite-private.h"

#include <assert.h>
#include <QString>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define REGEX_FUNCTION_NAME "regexp"

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  SQLITE Entry Points   -------------------------------------------------- */



extern "C" {

/* ------------------------------------------------------------------------- */
//! Implementation of the `regexp` function.
static void epoint_regexp (
            sqlite3_context *context, int argc, sqlite3_value **argv)
{
    REGEXLITE_TRACE_ENTRY;
    bool b_ret = false;
    for (;;) {

        if (argc != 0) {
            sqlite3_result_error (
                        context,
                        REGEX_FUNCTION_NAME " expects two arguments",
                        -1);
            break;
        }

        // Get arguments and check their values
        QRegExp arg1((const char*)sqlite3_value_text(argv[0]));
        QString arg2((const char*)sqlite3_value_text(argv[1]));

        if (!arg1.isValid () || arg2.isNull ()) {
            sqlite3_result_error (context, "invalid operand", -1);
            break;
        }

        // Set the pattern matching syntax to a Perl-like one.
        // This is the default in Qt 4.x but Qt 5
        // changes this to a greedy one (QRegExp::RegExp2).
        // To make sure the behaviour of our application
        // doesn't change depending on the build environment,
        // we make sure to always set the same pattern
        // matching syntax.
        arg1.setPatternSyntax (QRegExp::RegExp);
        arg1.setCaseSensitivity (Qt::CaseInsensitive);

        // Perform the actual matching and return the result.
        // Note that Qt's QRegExp returns -1 if the regex
        // doesn't match and the position in the string otherwise;
        // SQLite expects a 0 for not found and a 1 for found.
        sqlite3_result_int (context, arg1.indexIn(arg2) >= 0 ? 1 : 0);

        b_ret = true;
        break;
    }
    REGEXLITE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
//! The entry point used by sqlite.
REGEXLITE_EXPORT int sqlite3_regexlite_init (
            sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    REGEXLITE_TRACE_ENTRY;
    int rc = SQLITE_ERROR;
    for (;;) {
        SQLITE_EXTENSION_INIT2(pApi);

        rc = sqlite3_create_function_v2 (
                    db,
                    /* zFunctionName */ REGEX_FUNCTION_NAME,
                    /* nArg */ -1,
                    /* eTextRep */ SQLITE_UTF8 | SQLITE_DETERMINISTIC,
                    /* pApp */ NULL,
                    /* xFunc */ epoint_regexp,
                    /* xStep */ NULL,
                    /* xFinal */ NULL,
                    /* xDestroy */ NULL);
        if (rc != SQLITE_OK) {
            *pzErrMsg = sqlite3_mprintf (
                        "Failed to register function " REGEX_FUNCTION_NAME ": %1",
                        sqlite3_errmsg(db));
            break;
        }

        rc = SQLITE_OK;
        break;
    }
    REGEXLITE_TRACE_EXIT;
    return rc;
}
/* ========================================================================= */

} // extern "C"

/* ------------------------------------------------------------------------- */
RegexLite::sqliteEntryPoint RegexLite::getEntryPoint ()
{
    return (void(*)(void))sqlite3_regexlite_init;
}
/* ========================================================================= */

/*  SQLITE Entry Points   ================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
