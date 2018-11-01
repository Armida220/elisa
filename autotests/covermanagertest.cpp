/*
 * Copyright 2018 Alexander Stippich <a.stippich@gmx.net>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "filescanning/covermanager.h"
#include "musicaudiotrack.h"
#include "config-upnp-qt.h"

#include <QObject>
#include <QList>
#include <QUrl>
#include <QDateTime>
#include <QTime>

#include <QDebug>

#include <QtTest>

class CoverManagerTests: public QObject
{
    Q_OBJECT

public:

    QUrl createTrackUrl(QString subpath) {
        return QUrl::fromLocalFile(QStringLiteral(LOCAL_FILE_TESTS_SAMPLE_FILES_PATH) + QStringLiteral("/music") + subpath);
    }

    QList<MusicAudioTrack> mTestTracks = {
        {true, QString(), QString(), QStringLiteral("title"), QStringLiteral("artist1"),
         QStringLiteral("album1"), QStringLiteral("artist1"), 1, 1, QTime(),
         createTrackUrl(QStringLiteral("/artist1/album1/not_existing.ogg")),
         QDateTime(), QUrl(), 0, 0, QString(), QString(), QString()},
        {true, QString(), QString(), QStringLiteral("title"), QStringLiteral("artist1"),
         QStringLiteral("album2"), QStringLiteral("artist1"), 1, 1, QTime(),
         createTrackUrl(QStringLiteral("/artist1/album2/not_existing.ogg")),
         QDateTime(), QUrl(), 0, 0, QString(), QString(), QString()},
        {true, QString(), QString(), QStringLiteral("title"), QStringLiteral("artist1"),
         QStringLiteral("album3"), QStringLiteral("artist1"), 1, 1, QTime(),
         createTrackUrl(QStringLiteral("/artist1/album3/not_existing.ogg")),
         QDateTime(), QUrl(), 0, 0, QString(), QString(), QString()},
        {true, QString(), QString(), QStringLiteral("title"), QStringLiteral("artist2"),
         QStringLiteral("album1"), QStringLiteral("artist2"), 1, 1, QTime(),
         createTrackUrl(QStringLiteral("/artist2/album1/not_existing.ogg")),
         QDateTime(), QUrl(), 0, 0, QString(), QString(), QString()},
        {true, QString(), QString(), QStringLiteral("title"), QStringLiteral("artist2"),
         QStringLiteral("album2"), QStringLiteral("artist2"), 1, 1, QTime(),
         createTrackUrl(QStringLiteral("/artist2/album2/not_existing.ogg")),
         QDateTime(), QUrl(), 0, 0, QString(), QString(), QString()},
        {true, QString(), QString(), QStringLiteral("title"), QStringLiteral("artist2"),
         QStringLiteral("album3"), QStringLiteral("artist2"), 1, 1, QTime(),
         createTrackUrl(QStringLiteral("/artist2/album3/not_existing.ogg")),
         QDateTime(), QUrl(), 0, 0, QString(), QString(), QString()},
        {true, QString(), QString(), QStringLiteral("title"), QStringLiteral("artist3"),
         QStringLiteral("album1"), QStringLiteral("artist3"), 1, 1, QTime(),
         createTrackUrl(QStringLiteral("/artist3/album1/not_existing.ogg")),
         QDateTime(), QUrl(), 0, 0, QString(), QString(), QString()},
        {true, QString(), QString(), QStringLiteral("title"), QStringLiteral("artist3"),
         QStringLiteral("album2"), QStringLiteral("artist3"), 1, 1, QTime(),
         createTrackUrl(QStringLiteral("/artist3/album2/not_existing.ogg")),
         QDateTime(), QUrl(), 0, 0, QString(), QString(), QString()},
        {true, QString(), QString(), QStringLiteral("title"), QStringLiteral("artist3"),
         QStringLiteral("album3"), QStringLiteral("artist3"), 1, 1, QTime(),
         createTrackUrl(QStringLiteral("/artist3/album3/not_existing.ogg")),
         QDateTime(), QUrl(), 0, 0, QString(), QString(), QString()},
    };

private Q_SLOTS:

    void initTestCase()
    {

    }

    void test()
    {
        CoverManager coverManager;
        QVERIFY(!coverManager.findAlbumCover(mTestTracks.at(0)).isEmpty());
        QVERIFY(!coverManager.findAlbumCover(mTestTracks.at(1)).isEmpty());
        QVERIFY(!coverManager.findAlbumCover(mTestTracks.at(2)).isEmpty());
        QVERIFY(!coverManager.findAlbumCover(mTestTracks.at(3)).isEmpty());
        QVERIFY(!coverManager.findAlbumCover(mTestTracks.at(4)).isEmpty());
        QVERIFY(!coverManager.findAlbumCover(mTestTracks.at(5)).isEmpty());
        QVERIFY(!coverManager.findAlbumCover(mTestTracks.at(6)).isEmpty());
        QVERIFY(!coverManager.findAlbumCover(mTestTracks.at(7)).isEmpty());
        QVERIFY(!coverManager.findAlbumCover(mTestTracks.at(8)).isEmpty());
    }
};

QTEST_GUILESS_MAIN(CoverManagerTests)


#include "covermanagertest.moc"
