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
    installDriver();
};

#endif // GUARD_REGEXLITE_H_INCLUDE
