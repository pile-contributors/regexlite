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
** Original name of this file was qsqlcachedresult.cpp
****************************************************************************/

#include "qsqlcachedresult_p.h"

#include <qvariant.h>
#include <qdatetime.h>
#include <qvector.h>


/*
   QSqlCachedResult is a convenience class for databases that only allow
   forward only fetching. It will cache_ all the results so we can iterate
   backwards over the results again.

   All you need to do is to inherit from QSqlCachedResult and reimplement
   gotoNext(). gotoNext() will have a reference to the internal cache_ and
   will give you an index where you can start filling in your data. Special
   case: If the user actually wants a forward-only query, idx will be -1
   to indicate that we are not interested in the actual values.
*/

static const uint initial_cache__size = 128;


void QSqlCachedResult::init (int count, bool fo)
{
    Q_ASSERT(count);
    cleanup();
    forward_only_ = fo;
    col_count_ = count;
    if (fo) {
        cache_.resize(count);
        row_cache_end_ = count;
    } else {
        cache_.resize(initial_cache__size * count);
    }
}

int QSqlCachedResult::nextIndex()
{
    if (forward_only_)
        return 0;
    int newIdx = row_cache_end_;
    if (newIdx + col_count_ > cache_.size())
        cache_.resize(qMin(cache_.size() * 2, cache_.size() + 10000));
    row_cache_end_ += col_count_;

    return newIdx;
}

bool QSqlCachedResult::canSeek (int i) const
{
    if (forward_only_ || i < 0)
        return false;
    return row_cache_end_ >= (i + 1) * col_count_;
}

void QSqlCachedResult::revertLast ()
{
    if (forward_only_)
        return;
    row_cache_end_ -= col_count_;
}

int QSqlCachedResult::cacheCount() const
{
    Q_ASSERT(!forward_only_);
    Q_ASSERT(col_count_);
    return row_cache_end_ / col_count_;
}

QSqlCachedResult::QSqlCachedResult (const QSqlDriver * db):
    QSqlResult (db),
    row_cache_end_(0),
    col_count_(0),
    forward_only_(false),
    atEnd(false)
{
}

QSqlCachedResult::~QSqlCachedResult()
{
}

void QSqlCachedResult::init (int col_count_)
{
    init(col_count_, isForwardOnly());
}

bool QSqlCachedResult::fetch (int i)
{
    if ((!isActive()) || (i < 0))
        return false;
    if (at() == i)
        return true;
    if (forward_only_) {
        // speed hack - do not copy values if not needed
        if (at() > i || at() == QSql::AfterLastRow)
            return false;
        while(at() < i - 1) {
            if (!gotoNext(cache_, -1))
                return false;
            setAt(at() + 1);
        }
        if (!gotoNext (cache_, 0))
            return false;
        setAt(at() + 1);
        return true;
    }
    if (canSeek(i)) {
        setAt(i);
        return true;
    }
    if (row_cache_end_ > 0)
        setAt(cacheCount());
    while (at() < i + 1) {
        if (!cacheNext()) {
            if (canSeek(i))
                break;
            return false;
        }
    }
    setAt(i);

    return true;
}

bool QSqlCachedResult::fetchNext()
{
    if (canSeek(at() + 1)) {
        setAt(at() + 1);
        return true;
    }
    return cacheNext();
}

bool QSqlCachedResult::fetchPrevious()
{
    return fetch(at() - 1);
}

bool QSqlCachedResult::fetchFirst()
{
    if (forward_only_ && at() != QSql::BeforeFirstRow) {
        return false;
    }
    if (canSeek(0)) {
        setAt(0);
        return true;
    }
    return cacheNext();
}

bool QSqlCachedResult::fetchLast()
{
    if (atEnd) {
        if (forward_only_)
            return false;
        else
            return fetch(cacheCount() - 1);
    }

    int i = at();
    while (fetchNext())
        ++i; /* brute force */
    if (forward_only_ && at() == QSql::AfterLastRow) {
        setAt(i);
        return true;
    } else {
        return fetch(i);
    }
}

QVariant QSqlCachedResult::data(int i)
{
    int idx = forward_only_ ? i : at() * col_count_ + i;
    if (i >= col_count_ || i < 0 || at() < 0 || idx >= row_cache_end_)
        return QVariant();

    return cache_.at(idx);
}

bool QSqlCachedResult::isNull(int i)
{
    int idx = forward_only_ ? i : at() * col_count_ + i;
    if (i >= col_count_ || i < 0 || at() < 0 || idx >= row_cache_end_)
        return true;

    return cache_.at(idx).isNull();
}

void QSqlCachedResult::cleanup()
{
    setAt(QSql::BeforeFirstRow);
    setActive(false);
    cache_.clear();
    forward_only_ = false;
    atEnd = false;
    col_count_ = 0;
    row_cache_end_ = 0;
}

void QSqlCachedResult::clearValues()
{
    setAt(QSql::BeforeFirstRow);
    row_cache_end_ = 0;
    atEnd = false;
}

bool QSqlCachedResult::cacheNext()
{
    if (atEnd)
        return false;

    if(isForwardOnly()) {
        cache_.clear();
        cache_.resize(col_count_);
    }

    if (!gotoNext(cache_, nextIndex())) {
        revertLast();
        atEnd = true;
        return false;
    }
    setAt(at() + 1);
    return true;
}

int QSqlCachedResult::colCount() const
{
    return col_count_;
}

QSqlCachedResult::ValueCache &QSqlCachedResult::cache()
{
    return cache_;
}

void QSqlCachedResult::virtual_hook(int id, void *data)
{
    QSqlResult::virtual_hook(id, data);
}

void QSqlCachedResult::detachFromResultSet()
{
    cleanup();
}

void QSqlCachedResult::setNumericalPrecisionPolicy (
        QSql::NumericalPrecisionPolicy policy)
{
    QSqlResult::setNumericalPrecisionPolicy(policy);
    cleanup();
}

