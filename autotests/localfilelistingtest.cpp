/*
 * Copyright 2015-2017 Matthieu Gallien <matthieu_gallien@yahoo.fr>
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

#include "databasetestdata.h"

#include "filescanning/localfilelisting.h"
#include "musicaudiotrack.h"

#include "config-upnp-qt.h"

#include <QObject>
#include <QUrl>
#include <QString>
#include <QHash>
#include <QVector>
#include <QThread>
#include <QMetaObject>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

#include <QDebug>

#include <QtTest>
#include <QTest>

#include <algorithm>

class LocalFileListingTests: public QObject, public DatabaseTestData
{
    Q_OBJECT

public:

    LocalFileListingTests(QObject *parent = nullptr) : QObject(parent)
    {
    }

private Q_SLOTS:

    void initTestCase()
    {
        qRegisterMetaType<QHash<qulonglong,int>>("QHash<qulonglong,int>");
        qRegisterMetaType<QHash<QString,QUrl>>("QHash<QString,QUrl>");
        qRegisterMetaType<QList<MusicAudioTrack>>("QList<MusicAudioTrack>");
        qRegisterMetaType<QVector<qlonglong>>("QVector<qlonglong>");
        qRegisterMetaType<QHash<qlonglong,int>>("QHash<qlonglong,int>");
        qRegisterMetaType<QList<QUrl>>("QList<QUrl>");
        qRegisterMetaType<NotificationItem>("NotificationItem");
    }

    void initialTestWithNoTrack()
    {
        LocalFileListing myListing;

        QSignalSpy tracksListSpy(&myListing, &LocalFileListing::tracksList);
        QSignalSpy removedTracksListSpy(&myListing, &LocalFileListing::removedTracksList);
        QSignalSpy rootPathChangedSpy(&myListing, &LocalFileListing::rootPathChanged);
        QSignalSpy errorWatchingFilesSpy(&myListing, &LocalFileListing::errorWatchingFiles);

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 0);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        myListing.init();

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 0);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        myListing.setRootPath(QStringLiteral("/directoryNotExist"));

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        QCOMPARE(myListing.rootPath(), QStringLiteral("/directoryNotExist"));

        myListing.refreshContent();

        if (errorWatchingFilesSpy.count()) {
            QEXPECT_FAIL("", "watching files for change is not working", Abort);
        }

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);
    }

    void initialTestWithTracks()
    {
        LocalFileListing myListing;

        QString musicPath = QStringLiteral(LOCAL_FILE_TESTS_SAMPLE_FILES_PATH) + QStringLiteral("/music");

        QSignalSpy tracksListSpy(&myListing, &LocalFileListing::tracksList);
        QSignalSpy removedTracksListSpy(&myListing, &LocalFileListing::removedTracksList);
        QSignalSpy rootPathChangedSpy(&myListing, &LocalFileListing::rootPathChanged);
        QSignalSpy errorWatchingFilesSpy(&myListing, &LocalFileListing::errorWatchingFiles);

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 0);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        myListing.init();

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 0);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        myListing.setRootPath(musicPath);

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        QCOMPARE(myListing.rootPath(), musicPath);

        myListing.refreshContent();

        QCOMPARE(tracksListSpy.count(), 2);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);

        if (errorWatchingFilesSpy.count()) {
            QEXPECT_FAIL("", "watching files for change is not working", Abort);
        }
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        const auto &firstNewTracksSignal = tracksListSpy.at(0);
        auto firstNewTracks = firstNewTracksSignal.at(0).value<QList<MusicAudioTrack>>();
        const auto &secondNewTracksSignal = tracksListSpy.at(1);
        auto secondNewTracks = secondNewTracksSignal.at(0).value<QList<MusicAudioTrack>>();
        auto newCovers = secondNewTracksSignal.at(1).value<QHash<QString, QUrl>>();

        QCOMPARE(firstNewTracks.count() + secondNewTracks.count(), 3);
        QCOMPARE(newCovers.count(), 3);
    }

    void addAndRemoveTracks()
    {
        LocalFileListing myListing;

        QString musicOriginPath = QStringLiteral(LOCAL_FILE_TESTS_SAMPLE_FILES_PATH) + QStringLiteral("/music");

        QString musicPath = QStringLiteral(LOCAL_FILE_TESTS_WORKING_PATH) + QStringLiteral("/music2/data/innerData");

        QString musicParentPath = QStringLiteral(LOCAL_FILE_TESTS_WORKING_PATH) + QStringLiteral("/music2");
        QDir musicParentDirectory(musicParentPath);
        QDir rootDirectory(QStringLiteral(LOCAL_FILE_TESTS_WORKING_PATH));

        musicParentDirectory.removeRecursively();
        rootDirectory.mkpath(QStringLiteral("music2/data/innerData"));

        QSignalSpy tracksListSpy(&myListing, &LocalFileListing::tracksList);
        QSignalSpy removedTracksListSpy(&myListing, &LocalFileListing::removedTracksList);
        QSignalSpy rootPathChangedSpy(&myListing, &LocalFileListing::rootPathChanged);
        QSignalSpy errorWatchingFilesSpy(&myListing, &LocalFileListing::errorWatchingFiles);

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 0);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        myListing.init();

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 0);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        myListing.setRootPath(musicParentPath);

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        QCOMPARE(myListing.rootPath(), musicParentPath);

        myListing.refreshContent();

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);

        if (errorWatchingFilesSpy.count()) {
            QEXPECT_FAIL("", "watching files for change is not working", Abort);
        }
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        QFile myTrack(musicOriginPath + QStringLiteral("/test.ogg"));
        myTrack.copy(musicPath + QStringLiteral("/test.ogg"));
        QFile myCover(musicOriginPath + QStringLiteral("/cover.jpg"));
        myCover.copy(musicPath + QStringLiteral("/cover.jpg"));

        QCOMPARE(tracksListSpy.wait(), true);

        QCOMPARE(tracksListSpy.count(), 1);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);

        if (errorWatchingFilesSpy.count()) {
            QEXPECT_FAIL("", "watching files for change is not working", Abort);
        }
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        auto newTracksSignal = tracksListSpy.at(0);
        auto newTracks = newTracksSignal.at(0).value<QList<MusicAudioTrack>>();
        auto newCovers = newTracksSignal.at(1).value<QHash<QString, QUrl>>();

        QCOMPARE(newTracks.count(), 1);
        QCOMPARE(newCovers.count(), 1);

        QString commandLine(QStringLiteral("rm -rf ") + musicPath);
        system(commandLine.toLatin1().data());

        QCOMPARE(removedTracksListSpy.wait(), true);

        QCOMPARE(tracksListSpy.count(), 1);
        QCOMPARE(removedTracksListSpy.count(), 1);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        auto removeSignal = removedTracksListSpy.at(0);
        auto removedTracks = removeSignal.at(0).value<QList<QUrl>>();
        QCOMPARE(removedTracks.isEmpty(), false);

        QCOMPARE(rootDirectory.mkpath(QStringLiteral("music2/data/innerData")), true);
        QCOMPARE(myTrack.copy(musicPath + QStringLiteral("/test.ogg")), true);
        QCOMPARE(myCover.copy(musicPath + QStringLiteral("/cover.jpg")), true);

        if (tracksListSpy.count() == 1) {
            QCOMPARE(tracksListSpy.wait(), true);
        }

        QCOMPARE(tracksListSpy.count(), 2);
        QCOMPARE(removedTracksListSpy.count(), 1);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        auto newTracksSignalLast = tracksListSpy.at(1);
        auto newTracksLast = newTracksSignalLast.at(0).value<QList<MusicAudioTrack>>();
        auto newCoversLast = newTracksSignalLast.at(1).value<QHash<QString, QUrl>>();

        QCOMPARE(newTracksLast.count(), 1);
        QCOMPARE(newCoversLast.count(), 1);
    }

    void addTracksAndRemoveDirectory()
    {
        LocalFileListing myListing;

        QString musicOriginPath = QStringLiteral(LOCAL_FILE_TESTS_SAMPLE_FILES_PATH) + QStringLiteral("/music");

        QString musicPath = QStringLiteral(LOCAL_FILE_TESTS_WORKING_PATH) + QStringLiteral("/music2/data/innerData");

        QString innerMusicPath = QStringLiteral(LOCAL_FILE_TESTS_WORKING_PATH) + QStringLiteral("/music2/data");

        QString musicParentPath = QStringLiteral(LOCAL_FILE_TESTS_WORKING_PATH) + QStringLiteral("/music2");
        QDir musicParentDirectory(musicParentPath);
        QDir rootDirectory(QStringLiteral(LOCAL_FILE_TESTS_WORKING_PATH));

        musicParentDirectory.removeRecursively();
        rootDirectory.mkpath(QStringLiteral("music2/data/innerData"));

        QSignalSpy tracksListSpy(&myListing, &LocalFileListing::tracksList);
        QSignalSpy removedTracksListSpy(&myListing, &LocalFileListing::removedTracksList);
        QSignalSpy rootPathChangedSpy(&myListing, &LocalFileListing::rootPathChanged);
        QSignalSpy errorWatchingFilesSpy(&myListing, &LocalFileListing::errorWatchingFiles);

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 0);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        myListing.init();

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 0);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        myListing.setRootPath(musicParentPath);

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        QCOMPARE(myListing.rootPath(), musicParentPath);

        myListing.refreshContent();

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);

        if (errorWatchingFilesSpy.count()) {
            QEXPECT_FAIL("", "watching files for change is not working", Abort);
        }
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        QFile myTrack(musicOriginPath + QStringLiteral("/test.ogg"));
        myTrack.copy(musicPath + QStringLiteral("/test.ogg"));
        QFile myCover(musicOriginPath + QStringLiteral("/cover.jpg"));
        myCover.copy(musicPath + QStringLiteral("/cover.jpg"));

        QCOMPARE(tracksListSpy.wait(), true);

        QCOMPARE(tracksListSpy.count(), 1);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);

        if (errorWatchingFilesSpy.count()) {
            QEXPECT_FAIL("", "watching files for change is not working", Abort);
        }
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        auto newTracksSignal = tracksListSpy.at(0);
        auto newTracks = newTracksSignal.at(0).value<QList<MusicAudioTrack>>();
        auto newCovers = newTracksSignal.at(1).value<QHash<QString, QUrl>>();

        QCOMPARE(newTracks.count(), 1);
        QCOMPARE(newCovers.count(), 1);

        QString commandLine(QStringLiteral("rm -rf ") + innerMusicPath);
        system(commandLine.toLatin1().data());

        QCOMPARE(removedTracksListSpy.wait(), true);

        QCOMPARE(tracksListSpy.count(), 1);
        QCOMPARE(removedTracksListSpy.count(), 1);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        auto removeSignal = removedTracksListSpy.at(0);
        auto removedTracks = removeSignal.at(0).value<QList<QUrl>>();
        QCOMPARE(removedTracks.isEmpty(), false);

        QCOMPARE(rootDirectory.mkpath(QStringLiteral("music2/data/innerData")), true);
        QCOMPARE(myTrack.copy(musicPath + QStringLiteral("/test.ogg")), true);
        QCOMPARE(myCover.copy(musicPath + QStringLiteral("/cover.jpg")), true);

        if (tracksListSpy.count() == 1) {
            QCOMPARE(tracksListSpy.wait(), true);
        }

        QCOMPARE(tracksListSpy.count(), 2);
        QCOMPARE(removedTracksListSpy.count(), 1);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        auto newTracksSignalLast = tracksListSpy.at(1);
        auto newTracksLast = newTracksSignalLast.at(0).value<QList<MusicAudioTrack>>();
        auto newCoversLast = newTracksSignalLast.at(1).value<QHash<QString, QUrl>>();

        QCOMPARE(newTracksLast.count(), 1);
        QCOMPARE(newCoversLast.count(), 1);
    }

    void addAndMoveTracks()
    {
        LocalFileListing myListing;

        QString musicOriginPath = QStringLiteral(LOCAL_FILE_TESTS_SAMPLE_FILES_PATH) + QStringLiteral("/music");

        QString musicPath = QStringLiteral(LOCAL_FILE_TESTS_WORKING_PATH) + QStringLiteral("/music2/data/innerData");
        QDir musicDirectory(musicPath);

        QString musicParentPath = QStringLiteral(LOCAL_FILE_TESTS_WORKING_PATH) + QStringLiteral("/music2");
        QDir musicParentDirectory(musicParentPath);

        QString musicFriendPath = QStringLiteral(LOCAL_FILE_TESTS_WORKING_PATH) + QStringLiteral("/music3");
        QDir musicFriendDirectory(musicFriendPath);

        QCOMPARE(musicFriendDirectory.removeRecursively(), true);
        QCOMPARE(musicParentDirectory.removeRecursively(), true);

        musicFriendDirectory.mkpath(musicFriendPath);
        musicDirectory.mkpath(musicPath);

        QSignalSpy tracksListSpy(&myListing, &LocalFileListing::tracksList);
        QSignalSpy removedTracksListSpy(&myListing, &LocalFileListing::removedTracksList);
        QSignalSpy modifiedTracksListSpy(&myListing, &LocalFileListing::modifyTracksList);
        QSignalSpy rootPathChangedSpy(&myListing, &LocalFileListing::rootPathChanged);
        QSignalSpy errorWatchingFilesSpy(&myListing, &LocalFileListing::errorWatchingFiles);

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(modifiedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 0);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        myListing.init();

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(modifiedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 0);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        myListing.setRootPath(musicParentPath);

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(modifiedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        QCOMPARE(myListing.rootPath(), musicParentPath);

        myListing.refreshContent();

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(modifiedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);

        if (errorWatchingFilesSpy.count()) {
            QEXPECT_FAIL("", "watching files for change is not working", Abort);
        }
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        QFile myTrack(musicOriginPath + QStringLiteral("/test.ogg"));
        myTrack.copy(musicPath + QStringLiteral("/test.ogg"));
        QFile myCover(musicOriginPath + QStringLiteral("/cover.jpg"));
        myCover.copy(musicPath + QStringLiteral("/cover.jpg"));

        QCOMPARE(tracksListSpy.wait(), true);

        QCOMPARE(tracksListSpy.count(), 1);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(modifiedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);

        if (errorWatchingFilesSpy.count()) {
            QEXPECT_FAIL("", "watching files for change is not working", Abort);
        }
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        auto newTracksSignal = tracksListSpy.at(0);
        auto newTracks = newTracksSignal.at(0).value<QList<MusicAudioTrack>>();
        auto newCovers = newTracksSignal.at(1).value<QHash<QString, QUrl>>();

        QCOMPARE(newTracks.count(), 1);
        QCOMPARE(newCovers.count(), 1);

        QString commandLine(QStringLiteral("mv ") + musicPath + QStringLiteral(" ") + musicFriendPath);
        system(commandLine.toLatin1().data());

        QCOMPARE(removedTracksListSpy.wait(), true);

        QCOMPARE(tracksListSpy.count(), 1);
        QCOMPARE(removedTracksListSpy.count(), 1);
        QCOMPARE(modifiedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        auto removeSignal = removedTracksListSpy.at(0);
        auto removedTracks = removeSignal.at(0).value<QList<QUrl>>();
        QCOMPARE(removedTracks.isEmpty(), false);

        QCOMPARE(musicFriendDirectory.mkpath(musicFriendPath), true);
        QCOMPARE(musicDirectory.mkpath(musicPath), true);
        QCOMPARE(myTrack.copy(musicPath + QStringLiteral("/test.ogg")), true);
        QCOMPARE(myCover.copy(musicPath + QStringLiteral("/cover.jpg")), true);

        if (tracksListSpy.count() == 1) {
            QCOMPARE(tracksListSpy.wait(), true);
        }

        QCOMPARE(tracksListSpy.count(), 2);
        QCOMPARE(removedTracksListSpy.count(), 1);
        QCOMPARE(modifiedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        auto newTracksSignalLast = tracksListSpy.at(1);
        auto newTracksLast = newTracksSignalLast.at(0).value<QList<MusicAudioTrack>>();
        auto newCoversLast = newTracksSignalLast.at(1).value<QHash<QString, QUrl>>();

        QCOMPARE(newTracksLast.count(), 1);
        QCOMPARE(newCoversLast.count(), 1);
    }

    void restoreRemovedTracks()
    {
        LocalFileListing myListing;

        QSignalSpy tracksListSpy(&myListing, &LocalFileListing::tracksList);
        QSignalSpy removedTracksListSpy(&myListing, &LocalFileListing::removedTracksList);
        QSignalSpy modifiedTracksListSpy(&myListing, &LocalFileListing::modifyTracksList);
        QSignalSpy rootPathChangedSpy(&myListing, &LocalFileListing::rootPathChanged);
        QSignalSpy indexingStartedSpy(&myListing, &LocalFileListing::indexingStarted);
        QSignalSpy indexingFinishedSpy(&myListing, &LocalFileListing::indexingFinished);
        QSignalSpy newNotificationSpy(&myListing, &LocalFileListing::newNotification);
        QSignalSpy closeNotificationSpy(&myListing, &LocalFileListing::closeNotification);
        QSignalSpy askRestoredTracksSpy(&myListing, &LocalFileListing::askRestoredTracks);
        QSignalSpy errorWatchingFilesSpy(&myListing, &LocalFileListing::errorWatchingFiles);

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(modifiedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 0);
        QCOMPARE(indexingStartedSpy.count(), 0);
        QCOMPARE(indexingFinishedSpy.count(), 0);
        QCOMPARE(newNotificationSpy.count(), 0);
        QCOMPARE(closeNotificationSpy.count(), 0);
        QCOMPARE(askRestoredTracksSpy.count(), 0);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        myListing.setRootPath(QStringLiteral("/does/not/exists"));

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(modifiedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(indexingStartedSpy.count(), 0);
        QCOMPARE(indexingFinishedSpy.count(), 0);
        QCOMPARE(newNotificationSpy.count(), 0);
        QCOMPARE(closeNotificationSpy.count(), 0);
        QCOMPARE(askRestoredTracksSpy.count(), 0);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        myListing.init();

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 0);
        QCOMPARE(modifiedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(indexingStartedSpy.count(), 0);
        QCOMPARE(indexingFinishedSpy.count(), 0);
        QCOMPARE(newNotificationSpy.count(), 0);
        QCOMPARE(closeNotificationSpy.count(), 0);
        QCOMPARE(askRestoredTracksSpy.count(), 1);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        myListing.restoredTracks(QStringLiteral("/does/not/exists"),
            {{QUrl::fromLocalFile(QStringLiteral("/removed/files1")), QDateTime::fromMSecsSinceEpoch(1)},
             {QUrl::fromLocalFile(QStringLiteral("/removed/files2")), QDateTime::fromMSecsSinceEpoch(2)}});

        QCOMPARE(tracksListSpy.count(), 0);
        QCOMPARE(removedTracksListSpy.count(), 1);
        QCOMPARE(modifiedTracksListSpy.count(), 0);
        QCOMPARE(rootPathChangedSpy.count(), 1);
        QCOMPARE(indexingStartedSpy.count(), 1);
        QCOMPARE(indexingFinishedSpy.count(), 1);
        QCOMPARE(newNotificationSpy.count(), 0);
        QCOMPARE(closeNotificationSpy.count(), 0);
        QCOMPARE(askRestoredTracksSpy.count(), 1);
        QCOMPARE(errorWatchingFilesSpy.count(), 0);

        auto removedTracksSignal = removedTracksListSpy.at(0);

        QCOMPARE(removedTracksSignal.count(), 1);

        auto removedTracks = removedTracksSignal.at(0).value<QList<QUrl>>();

        QCOMPARE(removedTracks.count(), 2);

        std::sort(removedTracks.begin(), removedTracks.end());

        QCOMPARE(removedTracks[0], QUrl::fromLocalFile(QStringLiteral("/removed/files1")));
        QCOMPARE(removedTracks[1], QUrl::fromLocalFile(QStringLiteral("/removed/files2")));
    }
};

QTEST_GUILESS_MAIN(LocalFileListingTests)


#include "localfilelistingtest.moc"
