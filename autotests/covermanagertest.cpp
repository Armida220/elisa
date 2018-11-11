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
        return QUrl::fromLocalFile(QStringLiteral(LOCAL_FILE_TESTS_SAMPLE_FILES_PATH) + QStringLiteral("/cover_art") + subpath);
    }

    QList<MusicAudioTrack> mTestTracksForDirectory = {
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
        {true, QString(), QString(), QStringLiteral("title"), QStringLiteral("artist3"),
         QStringLiteral("album4"), QStringLiteral("artist3"), 1, 1, QTime(),
         createTrackUrl(QStringLiteral("/artist3/album_folder/not_existing.ogg")),
         QDateTime(), QUrl(), 0, 0, QString(), QString(), QString()},
        {true, QString(), QString(), QStringLiteral("title"), QStringLiteral("artist3"),
         QStringLiteral("albumXX"), QStringLiteral("artist3"), 1, 1, QTime(),
         createTrackUrl(QStringLiteral("/artist3/specific_directory_for_album_name/not_existing.ogg")),
         QDateTime(), QUrl(), 0, 0, QString(), QString(), QString()},
    };

    QList<MusicAudioTrack> mTestTracksForMetaData = {
        {true, QString(), QString(), QStringLiteral("title"), QStringLiteral("artist"),
         QStringLiteral("album"), QStringLiteral("artist"), 1, 1, QTime(),
         createTrackUrl(QStringLiteral("/artist4/test.ogg")),
         QDateTime(), QUrl(), 0, 0, QString(), QString(), QString()},
        {true, QString(), QString(), QStringLiteral("title"), QStringLiteral("artist"),
         QStringLiteral("album"), QStringLiteral("artist"), 1, 1, QTime(),
         createTrackUrl(QStringLiteral("/artist4/test.flac")),
         QDateTime(), QUrl(), 0, 0, QString(), QString(), QString()},
        {true, QString(), QString(), QStringLiteral("title"), QStringLiteral("artist"),
         QStringLiteral("album"), QStringLiteral("artist"), 1, 1, QTime(),
         createTrackUrl(QStringLiteral("/artist4/test.mp3")),
         QDateTime(), QUrl(), 0, 0, QString(), QString(), QString()},
    };

private Q_SLOTS:

    void initTestCase()
    {

    }

    void test_find_in_directory()
    {
        CoverManager coverManager;
        QVERIFY(!coverManager.findAlbumCoverInDirectory(mTestTracksForDirectory.at(0)).isEmpty());
        QVERIFY(!coverManager.findAlbumCoverInDirectory(mTestTracksForDirectory.at(1)).isEmpty());
        QVERIFY(!coverManager.findAlbumCoverInDirectory(mTestTracksForDirectory.at(2)).isEmpty());
        QVERIFY(!coverManager.findAlbumCoverInDirectory(mTestTracksForDirectory.at(3)).isEmpty());
        QVERIFY(!coverManager.findAlbumCoverInDirectory(mTestTracksForDirectory.at(4)).isEmpty());
        QVERIFY(!coverManager.findAlbumCoverInDirectory(mTestTracksForDirectory.at(5)).isEmpty());
        QVERIFY(!coverManager.findAlbumCoverInDirectory(mTestTracksForDirectory.at(6)).isEmpty());
        QVERIFY(!coverManager.findAlbumCoverInDirectory(mTestTracksForDirectory.at(7)).isEmpty());
        QVERIFY(!coverManager.findAlbumCoverInDirectory(mTestTracksForDirectory.at(8)).isEmpty());
        QVERIFY(!coverManager.findAlbumCoverInDirectory(mTestTracksForDirectory.at(9)).isEmpty());
        QVERIFY(!coverManager.findAlbumCoverInDirectory(mTestTracksForDirectory.at(10)).isEmpty());
    }

    void load_from_metadata()
    {
        CoverManager coverManager;
        QVERIFY(!coverManager.loadAlbumCoverFromMetaData(mTestTracksForMetaData.at(0)).isEmpty());
        QVERIFY(!coverManager.loadAlbumCoverFromMetaData(mTestTracksForMetaData.at(1)).isEmpty());
        QVERIFY(!coverManager.loadAlbumCoverFromMetaData(mTestTracksForMetaData.at(2)).isEmpty());
    }

    void benchmark_in_directory()
    {
        QBENCHMARK {
            test_find_in_directory();
        }
    }
    void benchmark_from_metadata()
    {
        QBENCHMARK {
            load_from_metadata();
        }
    }

};

QTEST_GUILESS_MAIN(CoverManagerTests)


#include "covermanagertest.moc"
