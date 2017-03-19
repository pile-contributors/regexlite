/**
 * @file regexlite.h
 * @brief Declarations for regexlite class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_REGEXLITE_H_INCLUDE
#define GUARD_REGEXLITE_H_INCLUDE

#include <regexlite/regexlite-config.h>

#include <QSqlDriverPlugin>

//! This is an Sql driver factory that will create an enhanced sqlite driver.
class REGEXLITE_EXPORT RegexLite : public QSqlDriverPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QSqlDriverFactoryInterface" FILE "regexlite.json")

public:

    typedef void(*sqliteEntryPoint)(void);

public:

    //! Default constructor.
    RegexLite ();

    //! Destructor.
    virtual ~RegexLite();

    //! Required by the QSqlDriverPlugin interface.
    QSqlDriver*
    create (
            const QString &) Q_DECL_OVERRIDE;

    //! Might be used if you don't want to use Qt plugin mechanism.
    static void
    installDriver ();

    //! The entry point for the sqlite3.
    static sqliteEntryPoint
    getEntryPoint ();

    //! Autoregister this extension with each new database
    //! (when not using the plugin).
    //! @warning this is n untested, paralel implementation; currently the
    //! code in sqlite.c contains the function to use.
    static bool
    autoregister ();

};

#endif // GUARD_REGEXLITE_H_INCLUDE
