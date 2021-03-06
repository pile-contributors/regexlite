/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSql module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
** Original name of this file was qsqlcachedresult_p.h
****************************************************************************/

#ifndef QSQLCACHEDRESULT_P_H
#define QSQLCACHEDRESULT_P_H


#include <QSqlResult>

class QVariant;
template <typename T> class QVector;

class QSqlCachedResult: public QSqlResult
{
public:
    virtual ~QSqlCachedResult();

    typedef QVector<QVariant> ValueCache;

protected:

    QSqlCachedResult(
            const QSqlDriver * db);

    void
    init (
            int colCount);

    void
    cleanup();

    void
    clearValues();

    virtual bool gotoNext (
            ValueCache &values,
            int index) = 0;

    QVariant
    data (
            int i);

    bool isNull (
            int i);

    bool fetch (
            int i);

    bool
    fetchNext ();

    bool
    fetchPrevious ();

    bool
    fetchFirst ();

    bool
    fetchLast ();

    int
    colCount() const;

    ValueCache &
    cache();

    void
    virtual_hook (
            int id, void *data);

    void
    detachFromResultSet();

    void
    setNumericalPrecisionPolicy (
            QSql::NumericalPrecisionPolicy policy);


private:

    bool
    cacheNext();

    bool
    canSeek (int i) const;

    int
    cacheCount() const;

    void
    init (int count, bool fo);

    int
    nextIndex();

    void
    revertLast();

    QSqlCachedResult::ValueCache cache_;
    int row_cache_end_;
    int col_count_;
    bool forward_only_;
    bool atEnd;
};


#endif // QSQLCACHEDRESULT_P_H
