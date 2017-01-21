/**
 * @file regexlite.cc
 * @brief Definitions for regexlite class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "regexlite.h"
#include "regexlite-private.h"
#include <QRegExp>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QSqlDriverCreator>

#include "sqlite/sqlite3.h"
#include "drvsqlite.h"

/**
 * @class RegexLite
 *
 *
 */

static QLatin1String drv_name ("QSQLITE");

/* ------------------------------------------------------------------------- */
RegexLite::RegexLite() :
    QSqlDriverPlugin()
{
    REGEXLITE_TRACE_ENTRY;
    REGEXLITE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
RegexLite::~RegexLite()
{
    REGEXLITE_TRACE_ENTRY;
    REGEXLITE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QSqlDriver* RegexLite::create (const QString &name)
{
    if (name == drv_name) {
        QSQLiteEnhancedDriver* driver = new QSQLiteEnhancedDriver();
        return driver;
    }
    return 0;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void RegexLite::installDriver ()
{
    REGEXLITE_TRACE_ENTRY;
    QSqlDatabase::registerSqlDriver(
        drv_name,
        new QSqlDriverCreator<QSQLiteEnhancedDriver>);
    REGEXLITE_TRACE_EXIT;
}
/* ========================================================================= */


#ifdef OBSOLETE_REGEXLITE
/* ------------------------------------------------------------------------- */
static void qtregexp (sqlite3_context* ctx, int argc, sqlite3_value** argv)
{
    REGEXLITE_TRACE_ENTRY;
    QRegExp regex;
    QString str1((const char*)sqlite3_value_text(argv[0]));
    QString str2((const char*)sqlite3_value_text(argv[1]));

    regex.setPattern(str1);
    regex.setCaseSensitivity(Qt::CaseInsensitive);

    bool b = str2.contains(regex);

    if (b) {
        sqlite3_result_int(ctx, 1);
    } else {
        sqlite3_result_int(ctx, 0);
    }
    REGEXLITE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
static void regexp (sqlite3_context* ctx, int /*argc*/, sqlite3_value* argv[])
{
    REGEXLITE_TRACE_ENTRY;

    // Get arguments and check their values
    QRegExp arg1((const char*)sqlite3_value_text(argv[0]));
    QString arg2((const char*)sqlite3_value_text(argv[1]));

    if (!arg1.isValid () || arg2.isNull ()) {
        return sqlite3_result_error (ctx, "invalid operand", -1);
    }

    // Set the pattern matching syntax to a Perl-like one.
    // This is the default in Qt 4.x but Qt 5
    // changes this to a greedy one (QRegExp::RegExp2).
    // To make sure the behaviour of our application
    // doesn't change depending on the build environment,
    // we make sure to always set the same pattern
    // matching syntax.
    arg1.setPatternSyntax (QRegExp::RegExp);

    // Perform the actual matching and return the result.
    // Note that Qt's QRegExp returns -1 if the regex
    // doesn't match and the position in the string otherwise;
    // SQLite expects a 0 for not found and a 1 for found.
    sqlite3_result_int (ctx, arg1.indexIn(arg2) >= 0 ? 1 : 0);
    REGEXLITE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
// sqlite3 *sldb;
// sqlite3_open(":memory:", &sldb);
// sqlite3_create_function(sldb, "regexp", 2, SQLITE_UTF8, NULL, &qtregexp, NULL, NULL);
static bool installHandler (QSqlDriver * driver)
{
    return true;
    REGEXLITE_TRACE_ENTRY;

    bool b_ret = false;
    if (driver == NULL) {
        QSqlDatabase db = QSqlDatabase::addDatabase ("QSQLITE", "conn1");
        db.setDatabaseName(":memory:");
        if (db.open ()) {
            driver = db.driver ();
            db.close();
        } else {
            qDebug() << "Cannot open database; driver probably not installed";
        }
    }

    QVariant v = driver->handle ();
    if (v.isValid() && qstrcmp(v.typeName(), "sqlite3*")==0) {
        sqlite3 *db_handle = *static_cast<sqlite3 **>(v.data());
        if (db_handle != 0) { // check that it is not NULL
            sqlite3_initialize();
            sqlite3 *p; //without this there is a crash.
            int result = sqlite3_open( ":memory:", &p );
            if (result == SQLITE_OK) {
                sqlite3_close(p);
                //call any API you need on handle.

                // This shows that the database handle is generally valid:
                sqlite3_create_function (db_handle, "regexp", 2, SQLITE_UTF8 | SQLITE_DETERMINISTIC, NULL, &qtregexp, NULL, NULL);
                qDebug() << "regexp handle added.";

                b_ret = true;

            } else {
                qDebug() << "Cannot open in-memory database.";
            }

        } else {
            qDebug() << "NULL database handle.";
        }
    } else {
        qDebug() << "I got no clue.";
    }

    REGEXLITE_TRACE_EXIT;
    return b_ret;
}
/* ========================================================================= */

#endif // OBSOLETE_REGEXLITE
