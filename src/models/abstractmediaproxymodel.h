/*
 * Copyright 2016-2017 Matthieu Gallien <matthieu_gallien@yahoo.fr>
 * Copyright 2017 Alexander Stippich <a.stippich@gmx.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef ABSTRACTMEDIAPROXYMODEL_H
#define ABSTRACTMEDIAPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QRegularExpression>

#include "mediaplaylist.h"

class MediaPlayList;

class AbstractMediaProxyModel : public QSortFilterProxyModel
{

    Q_OBJECT

    Q_PROPERTY(QString filterText
               READ filterText
               WRITE setFilterText
               NOTIFY filterTextChanged)

    Q_PROPERTY(int filterRating
               READ filterRating
               WRITE setFilterRating
               NOTIFY filterRatingChanged)

    Q_PROPERTY(MediaPlayList* mediaPlayList
               READ mediaPlayList
               WRITE setMediaPlayList
               NOTIFY mediaPlayListChanged)


public:

    explicit AbstractMediaProxyModel(QObject *parent = nullptr);

    ~AbstractMediaProxyModel() override;

    QString filterText() const;

    int filterRating() const;

    MediaPlayList *mediaPlayList() const;

public Q_SLOTS:

    void setMediaPlayList(MediaPlayList *playList);

    void setFilterText(const QString &filterText);

    void setFilterRating(int filterRating);

    virtual void enqueueToPlayList() = 0;

    void replaceAndPlayOfPlayList();

Q_SIGNALS:

    void filterTextChanged(const QString &filterText);

    void filterRatingChanged(int filterRating);

    void mediaPlayListChanged(MediaPlayList *mediaPlayList);

protected:

    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override = 0;

    QString mFilterText;

    int mFilterRating = 0;

    QRegularExpression mFilterExpression;

    MediaPlayList* mMediaPlayList;

};

#endif // ABSTRACTMEDIAPROXYMODEL_H
