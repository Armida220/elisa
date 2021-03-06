/*
 * Copyright 2016-2017 Matthieu Gallien <matthieu_gallien@yahoo.fr>
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

#include "databaseinterface.h"

#include "musicaudiotrack.h"

#include <KI18n/KLocalizedString>

#include <QCoreApplication>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QDateTime>
#include <QMutex>
#include <QVariant>
#include <QAtomicInt>
#include <QDebug>

#include <algorithm>

class DatabaseInterfacePrivate
{
public:

    DatabaseInterfacePrivate(const QSqlDatabase &tracksDatabase)
        : mTracksDatabase(tracksDatabase), mSelectAlbumQuery(mTracksDatabase),
          mSelectTrackQuery(mTracksDatabase), mSelectAlbumIdFromTitleQuery(mTracksDatabase),
          mInsertAlbumQuery(mTracksDatabase), mSelectTrackIdFromTitleAlbumIdArtistQuery(mTracksDatabase),
          mInsertTrackQuery(mTracksDatabase), mSelectTracksFromArtist(mTracksDatabase),
          mSelectTrackFromIdQuery(mTracksDatabase), mSelectCountAlbumsForArtistQuery(mTracksDatabase),
          mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery(mTracksDatabase), mSelectAllAlbumsQuery(mTracksDatabase),
          mSelectAllAlbumsFromArtistQuery(mTracksDatabase), mSelectAllArtistsQuery(mTracksDatabase),
          mInsertArtistsQuery(mTracksDatabase), mSelectArtistByNameQuery(mTracksDatabase),
          mSelectArtistQuery(mTracksDatabase), mUpdateTrackStatistics(mTracksDatabase),
          mRemoveTrackQuery(mTracksDatabase), mRemoveAlbumQuery(mTracksDatabase),
          mRemoveArtistQuery(mTracksDatabase), mSelectAllTracksQuery(mTracksDatabase),
          mInsertTrackMapping(mTracksDatabase), mUpdateTrackFirstPlayStatistics(mTracksDatabase),
          mInsertMusicSource(mTracksDatabase), mSelectMusicSource(mTracksDatabase),
          mUpdateTrackMapping(mTracksDatabase),
          mSelectTracksMapping(mTracksDatabase), mSelectTracksMappingPriority(mTracksDatabase),
          mUpdateAlbumArtUriFromAlbumIdQuery(mTracksDatabase), mSelectTracksMappingPriorityByTrackId(mTracksDatabase),
          mSelectAllTrackFilesFromSourceQuery(mTracksDatabase),  mSelectAlbumIdsFromArtist(mTracksDatabase),
          mRemoveTracksMappingFromSource(mTracksDatabase), mRemoveTracksMapping(mTracksDatabase),
          mSelectTracksWithoutMappingQuery(mTracksDatabase), mSelectAlbumIdFromTitleAndArtistQuery(mTracksDatabase),
          mSelectAlbumIdFromTitleWithoutArtistQuery(mTracksDatabase),
          mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery(mTracksDatabase), mSelectAlbumArtUriFromAlbumIdQuery(mTracksDatabase),
          mInsertComposerQuery(mTracksDatabase), mSelectComposerByNameQuery(mTracksDatabase),
          mSelectComposerQuery(mTracksDatabase), mInsertLyricistQuery(mTracksDatabase),
          mSelectLyricistByNameQuery(mTracksDatabase), mSelectLyricistQuery(mTracksDatabase),
          mInsertGenreQuery(mTracksDatabase), mSelectGenreByNameQuery(mTracksDatabase),
          mSelectGenreQuery(mTracksDatabase), mSelectAllTracksShortQuery(mTracksDatabase),
          mSelectAllAlbumsShortQuery(mTracksDatabase), mSelectAllComposersQuery(mTracksDatabase),
          mSelectAllLyricistsQuery(mTracksDatabase), mSelectCountAlbumsForComposerQuery(mTracksDatabase),
          mSelectCountAlbumsForLyricistQuery(mTracksDatabase), mSelectAllGenresQuery(mTracksDatabase),
          mSelectGenreForArtistQuery(mTracksDatabase), mSelectGenreForAlbumQuery(mTracksDatabase),
          mUpdateTrackQuery(mTracksDatabase), mUpdateAlbumArtistQuery(mTracksDatabase),
          mUpdateAlbumArtistInTracksQuery(mTracksDatabase), mQueryMaximumTrackIdQuery(mTracksDatabase),
          mQueryMaximumAlbumIdQuery(mTracksDatabase), mQueryMaximumArtistIdQuery(mTracksDatabase),
          mQueryMaximumLyricistIdQuery(mTracksDatabase), mQueryMaximumComposerIdQuery(mTracksDatabase),
          mQueryMaximumGenreIdQuery(mTracksDatabase), mSelectAllArtistsWithGenreFilterQuery(mTracksDatabase),
          mSelectAllAlbumsShortWithGenreArtistFilterQuery(mTracksDatabase), mSelectAllAlbumsShortWithArtistFilterQuery(mTracksDatabase),
          mSelectAllRecentlyPlayedTracksQuery(mTracksDatabase), mSelectAllFrequentlyPlayedTracksQuery(mTracksDatabase)
    {
    }

    QSqlDatabase mTracksDatabase;

    QSqlQuery mSelectAlbumQuery;

    QSqlQuery mSelectTrackQuery;

    QSqlQuery mSelectAlbumIdFromTitleQuery;

    QSqlQuery mInsertAlbumQuery;

    QSqlQuery mSelectTrackIdFromTitleAlbumIdArtistQuery;

    QSqlQuery mInsertTrackQuery;

    QSqlQuery mSelectTracksFromArtist;

    QSqlQuery mSelectTrackFromIdQuery;

    QSqlQuery mSelectCountAlbumsForArtistQuery;

    QSqlQuery mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery;

    QSqlQuery mSelectAllAlbumsQuery;

    QSqlQuery mSelectAllAlbumsFromArtistQuery;

    QSqlQuery mSelectAllArtistsQuery;

    QSqlQuery mInsertArtistsQuery;

    QSqlQuery mSelectArtistByNameQuery;

    QSqlQuery mSelectArtistQuery;

    QSqlQuery mUpdateTrackStatistics;

    QSqlQuery mRemoveTrackQuery;

    QSqlQuery mRemoveAlbumQuery;

    QSqlQuery mRemoveArtistQuery;

    QSqlQuery mSelectAllTracksQuery;

    QSqlQuery mInsertTrackMapping;

    QSqlQuery mUpdateTrackFirstPlayStatistics;

    QSqlQuery mInsertMusicSource;

    QSqlQuery mSelectMusicSource;

    QSqlQuery mUpdateTrackMapping;

    QSqlQuery mSelectTracksMapping;

    QSqlQuery mSelectTracksMappingPriority;

    QSqlQuery mUpdateAlbumArtUriFromAlbumIdQuery;

    QSqlQuery mSelectTracksMappingPriorityByTrackId;

    QSqlQuery mSelectAllTrackFilesFromSourceQuery;

    QSqlQuery mSelectAlbumIdsFromArtist;

    QSqlQuery mRemoveTracksMappingFromSource;

    QSqlQuery mRemoveTracksMapping;

    QSqlQuery mSelectTracksWithoutMappingQuery;

    QSqlQuery mSelectAlbumIdFromTitleAndArtistQuery;

    QSqlQuery mSelectAlbumIdFromTitleWithoutArtistQuery;

    QSqlQuery mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery;

    QSqlQuery mSelectAlbumArtUriFromAlbumIdQuery;

    QSqlQuery mInsertComposerQuery;

    QSqlQuery mSelectComposerByNameQuery;

    QSqlQuery mSelectComposerQuery;

    QSqlQuery mInsertLyricistQuery;

    QSqlQuery mSelectLyricistByNameQuery;

    QSqlQuery mSelectLyricistQuery;

    QSqlQuery mInsertGenreQuery;

    QSqlQuery mSelectGenreByNameQuery;

    QSqlQuery mSelectGenreQuery;

    QSqlQuery mSelectAllTracksShortQuery;

    QSqlQuery mSelectAllAlbumsShortQuery;

    QSqlQuery mSelectAllComposersQuery;

    QSqlQuery mSelectAllLyricistsQuery;

    QSqlQuery mSelectCountAlbumsForComposerQuery;

    QSqlQuery mSelectCountAlbumsForLyricistQuery;

    QSqlQuery mSelectAllGenresQuery;

    QSqlQuery mSelectGenreForArtistQuery;

    QSqlQuery mSelectGenreForAlbumQuery;

    QSqlQuery mUpdateTrackQuery;

    QSqlQuery mUpdateAlbumArtistQuery;

    QSqlQuery mUpdateAlbumArtistInTracksQuery;

    QSqlQuery mQueryMaximumTrackIdQuery;

    QSqlQuery mQueryMaximumAlbumIdQuery;

    QSqlQuery mQueryMaximumArtistIdQuery;

    QSqlQuery mQueryMaximumLyricistIdQuery;

    QSqlQuery mQueryMaximumComposerIdQuery;

    QSqlQuery mQueryMaximumGenreIdQuery;

    QSqlQuery mSelectAllArtistsWithGenreFilterQuery;

    QSqlQuery mSelectAllAlbumsShortWithGenreArtistFilterQuery;

    QSqlQuery mSelectAllAlbumsShortWithArtistFilterQuery;

    QSqlQuery mSelectAllRecentlyPlayedTracksQuery;

    QSqlQuery mSelectAllFrequentlyPlayedTracksQuery;

    QSet<qulonglong> mModifiedTrackIds;

    QSet<qulonglong> mModifiedAlbumIds;

    QSet<qulonglong> mModifiedArtistIds;

    QSet<qulonglong> mInsertedTracks;

    QSet<qulonglong> mInsertedAlbums;

    QSet<qulonglong> mInsertedArtists;

    qulonglong mAlbumId = 1;

    qulonglong mArtistId = 1;

    qulonglong mComposerId = 1;

    qulonglong mLyricistId = 1;

    qulonglong mGenreId = 1;

    qulonglong mTrackId = 1;

    qulonglong mDiscoverId = 1;

    QAtomicInt mStopRequest = 0;

    bool mInitFinished = false;

};

DatabaseInterface::DatabaseInterface(QObject *parent) : QObject(parent), d(nullptr)
{
}

DatabaseInterface::~DatabaseInterface()
{
    if (d) {
        d->mTracksDatabase.close();
    }
}

void DatabaseInterface::init(const QString &dbName, const QString &databaseFileName)
{
    qInfo() << QCoreApplication::libraryPaths();
    QSqlDatabase tracksDatabase = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), dbName);

    if (!databaseFileName.isEmpty()) {
        tracksDatabase.setDatabaseName(QStringLiteral("file:") + databaseFileName);
    } else {
        tracksDatabase.setDatabaseName(QStringLiteral("file:memdb1?mode=memory"));
    }
    tracksDatabase.setConnectOptions(QStringLiteral("foreign_keys = ON;locking_mode = EXCLUSIVE;QSQLITE_OPEN_URI;QSQLITE_BUSY_TIMEOUT=500000"));

    auto result = tracksDatabase.open();
    if (result) {
        qDebug() << "database open";
    } else {
        qDebug() << "database not open";
    }
    qDebug() << "DatabaseInterface::init" << (tracksDatabase.driver()->hasFeature(QSqlDriver::Transactions) ? "yes" : "no");

    tracksDatabase.exec(QStringLiteral("PRAGMA foreign_keys = ON;"));

    d = std::make_unique<DatabaseInterfacePrivate>(tracksDatabase);

    initDatabase();
    initRequest();

    if (!databaseFileName.isEmpty()) {
        reloadExistingDatabase();
    }
}

qulonglong DatabaseInterface::albumIdFromTitleAndArtist(const QString &title, const QString &artist)
{
    auto result = qulonglong{0};

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalAlbumIdFromTitleAndArtist(title, artist);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

DatabaseInterface::ListTrackDataType DatabaseInterface::allTracksData()
{
    auto result = ListTrackDataType{};

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalAllTracksPartialData();

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

DatabaseInterface::ListTrackDataType DatabaseInterface::recentlyPlayedTracksData(int count)
{
    auto result = ListTrackDataType{};

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalRecentlyPlayedTracksData(count);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

DatabaseInterface::ListTrackDataType DatabaseInterface::frequentlyPlayedTracksData(int count)
{
    auto result = ListTrackDataType{};

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalFrequentlyPlayedTracksData(count);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

DatabaseInterface::ListAlbumDataType DatabaseInterface::allAlbumsData()
{
    auto result = ListAlbumDataType{};

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalAllAlbumsPartialData(d->mSelectAllAlbumsShortQuery);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

DatabaseInterface::ListAlbumDataType DatabaseInterface::allAlbumsDataByGenreAndArtist(const QString &genre, const QString &artist)
{
    auto result = ListAlbumDataType{};

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    d->mSelectAllAlbumsShortWithGenreArtistFilterQuery.bindValue(QStringLiteral(":artistFilter"), artist);
    d->mSelectAllAlbumsShortWithGenreArtistFilterQuery.bindValue(QStringLiteral(":genreFilter"), genre);

    result = internalAllAlbumsPartialData(d->mSelectAllAlbumsShortWithGenreArtistFilterQuery);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

DatabaseInterface::ListAlbumDataType DatabaseInterface::allAlbumsDataByArtist(const QString &artist)
{
    auto result = ListAlbumDataType{};

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    d->mSelectAllAlbumsShortWithArtistFilterQuery.bindValue(QStringLiteral(":artistFilter"), artist);

    result = internalAllAlbumsPartialData(d->mSelectAllAlbumsShortWithArtistFilterQuery);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

DatabaseInterface::AlbumDataType DatabaseInterface::albumDataFromDatabaseId(qulonglong id)
{
    auto result = DatabaseInterface::AlbumDataType{};

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalOneAlbumPartialData(id);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

DatabaseInterface::ListTrackDataType DatabaseInterface::albumData(qulonglong databaseId)
{
    auto result = ListTrackDataType{};

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    d->mSelectTrackQuery.bindValue(QStringLiteral(":albumId"), databaseId);

    auto queryResult = d->mSelectTrackQuery.exec();

    if (!queryResult || !d->mSelectTrackQuery.isSelect() || !d->mSelectTrackQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::albumData" << d->mSelectTrackQuery.lastQuery();
        qDebug() << "DatabaseInterface::albumData" << d->mSelectTrackQuery.boundValues();
        qDebug() << "DatabaseInterface::albumData" << d->mSelectTrackQuery.lastError();
    }

    while (d->mSelectTrackQuery.next()) {
        const auto &currentRecord = d->mSelectTrackQuery.record();

        result.push_back(buildTrackDataFromDatabaseRecord(currentRecord));
    }

    d->mSelectTrackQuery.finish();

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

DatabaseInterface::ListArtistDataType DatabaseInterface::allArtistsData()
{
    auto result = ListArtistDataType{};

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalAllArtistsPartialData(d->mSelectAllArtistsQuery);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

DatabaseInterface::ListArtistDataType DatabaseInterface::allArtistsDataByGenre(const QString &genre)
{
    qDebug() << "DatabaseInterface::allArtistsDataByGenre" << genre;

    auto result = ListArtistDataType{};

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    d->mSelectAllArtistsWithGenreFilterQuery.bindValue(QStringLiteral(":genreFilter"), genre);

    result = internalAllArtistsPartialData(d->mSelectAllArtistsWithGenreFilterQuery);

    qDebug() << "DatabaseInterface::allArtistsDataByGenre" << result.count();

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

DatabaseInterface::ListGenreDataType DatabaseInterface::allGenresData()
{
    auto result = ListGenreDataType{};

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalAllGenresPartialData();

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

DatabaseInterface::DataType DatabaseInterface::oneData(ElisaUtils::PlayListEntryType aType, qulonglong databaseId)
{
    auto result = DataType{};

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    switch (aType)
    {
    case ElisaUtils::Artist:
        result = internalOneArtistPartialData(databaseId);
        break;
    case ElisaUtils::Album:
        result = internalOneAlbumPartialData(databaseId);
        break;
    case ElisaUtils::Track:
        result = internalOneTrackPartialData(databaseId);
        break;
    case ElisaUtils::Genre:
        result = internalOneGenrePartialData(databaseId);
        break;
    case ElisaUtils::Composer:
        result = internalOneComposerPartialData(databaseId);
        break;
    case ElisaUtils::Lyricist:
        result = internalOneLyricistPartialData(databaseId);
        break;
    case ElisaUtils::Unknown:
    case ElisaUtils::FileName:
        break;
    };

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

DatabaseInterface::ListTrackDataType DatabaseInterface::tracksDataFromAuthor(const QString &ArtistName)
{
    auto allTracks = ListTrackDataType{};

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return allTracks;
    }

    allTracks = internalTracksFromAuthor(ArtistName);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return allTracks;
    }

    return allTracks;
}

DatabaseInterface::TrackDataType DatabaseInterface::trackDataFromDatabaseId(qulonglong id)
{
    auto result = TrackDataType();

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalOneTrackPartialData(id);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

qulonglong DatabaseInterface::trackIdFromTitleAlbumTrackDiscNumber(const QString &title, const QString &artist, const QString &album,
                                                                   int trackNumber, int discNumber)
{
    auto result = qulonglong(0);

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalTrackIdFromTitleAlbumTracDiscNumber(title, artist, album, trackNumber, discNumber);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

qulonglong DatabaseInterface::trackIdFromFileName(const QUrl &fileName)
{
    auto result = qulonglong(0);

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalTrackIdFromFileName(fileName);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

void DatabaseInterface::applicationAboutToQuit()
{
    d->mStopRequest = 1;
}

void DatabaseInterface::removeAllTracksFromSource(const QString &sourceName)
{
    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    initChangesTrackers();

    auto sourceId = internalSourceIdFromName(sourceName);

    auto allFileNames = internalAllFileNameFromSource(sourceId);

    internalRemoveTracksList(allFileNames, sourceId);

    if (!d->mInsertedArtists.isEmpty()) {
        ListArtistDataType newArtists;
        for (auto artistId : qAsConst(d->mInsertedArtists)) {
            newArtists.push_back({{DatabaseIdRole, artistId}});
        }
        Q_EMIT artistsAdded(newArtists);
    }

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return;
    }
}

void DatabaseInterface::askRestoredTracks(const QString &musicSource)
{
    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    auto result = internalAllFileNameFromSource(internalSourceIdFromName(musicSource));

    Q_EMIT restoredTracks(musicSource, result);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return;
    }
}

void DatabaseInterface::trackHasStartedPlaying(const QUrl &fileName, const QDateTime &time)
{
    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    auto trackId = internalTrackIdFromFileName(fileName);
    if (trackId != 0) {
        updateTrackStatistics(trackId, time);
    }

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return;
    }
}

void DatabaseInterface::initChangesTrackers()
{
    d->mModifiedTrackIds.clear();
    d->mModifiedAlbumIds.clear();
    d->mModifiedArtistIds.clear();
    d->mInsertedTracks.clear();
    d->mInsertedAlbums.clear();
    d->mInsertedArtists.clear();
}

void DatabaseInterface::recordModifiedTrack(qulonglong trackId)
{
    d->mModifiedTrackIds.insert(trackId);
}

void DatabaseInterface::recordModifiedAlbum(qulonglong albumId)
{
    d->mModifiedAlbumIds.insert(albumId);
}

void DatabaseInterface::insertTracksList(const QList<MusicAudioTrack> &tracks, const QHash<QString, QUrl> &covers, const QString &musicSource)
{
    if (d->mStopRequest == 1) {
        return;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    initChangesTrackers();

    for(const auto &oneTrack : tracks) {
        d->mSelectTracksMapping.bindValue(QStringLiteral(":fileName"), oneTrack.resourceURI());

        auto result = d->mSelectTracksMapping.exec();

        if (!result || !d->mSelectTracksMapping.isSelect() || !d->mSelectTracksMapping.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::insertTracksList" << d->mSelectTracksMapping.lastQuery();
            qDebug() << "DatabaseInterface::insertTracksList" << d->mSelectTracksMapping.boundValues();
            qDebug() << "DatabaseInterface::insertTracksList" << d->mSelectTracksMapping.lastError();

            d->mSelectTracksMapping.finish();

            rollBackTransaction();
            return;
        }

        bool isNewTrack = !d->mSelectTracksMapping.next();

        if (isNewTrack) {
            insertTrackOrigin(oneTrack.resourceURI(), oneTrack.fileModificationTime(), insertMusicSource(musicSource));
        } else if (!d->mSelectTracksMapping.record().value(0).isNull() && d->mSelectTracksMapping.record().value(0).toULongLong() != 0) {
            updateTrackOrigin(d->mSelectTracksMapping.record().value(0).toULongLong(), oneTrack.resourceURI(), oneTrack.fileModificationTime());
        } else {
            continue;
        }

        d->mSelectTracksMapping.finish();

        const auto insertedTrackId = internalInsertTrack(oneTrack, covers, 0,
                                                         (isNewTrack ? TrackFileInsertType::NewTrackFileInsert : TrackFileInsertType::ModifiedTrackFileInsert));

        if (isNewTrack && insertedTrackId != 0) {
            d->mInsertedTracks.insert(insertedTrackId);
        }

        if (d->mStopRequest == 1) {
            transactionResult = finishTransaction();
            if (!transactionResult) {
                return;
            }
            return;
        }
    }

    if (!d->mInsertedArtists.isEmpty()) {
        ListArtistDataType newArtists;

        for (auto artistId : qAsConst(d->mInsertedArtists)) {
            newArtists.push_back({{DatabaseIdRole, artistId}});
        }
        qInfo() << "artistsAdded" << newArtists.size();
        Q_EMIT artistsAdded(newArtists);
    }

    if (!d->mInsertedAlbums.isEmpty()) {
        ListAlbumDataType newAlbums;

        for (auto albumId : qAsConst(d->mInsertedAlbums)) {
            d->mModifiedAlbumIds.remove(albumId);
            newAlbums.push_back(internalOneAlbumPartialData(albumId));
        }

        qInfo() << "albumsAdded" << newAlbums.size();
        Q_EMIT albumsAdded(newAlbums);
    }

    for (auto albumId : qAsConst(d->mModifiedAlbumIds)) {
        Q_EMIT albumModified({{DatabaseIdRole, albumId}}, albumId);
    }

    if (!d->mInsertedTracks.isEmpty()) {
        ListTrackDataType newTracks;

        for (auto trackId : qAsConst(d->mInsertedTracks)) {
            newTracks.push_back(internalOneTrackPartialData(trackId));
            d->mModifiedTrackIds.remove(trackId);
        }

        qInfo() << "tracksAdded" << newTracks.size();
        Q_EMIT tracksAdded(newTracks);
    }

    for (auto trackId : qAsConst(d->mModifiedTrackIds)) {
        Q_EMIT trackModified(internalOneTrackPartialData(trackId));
    }

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return;
    }
}

void DatabaseInterface::removeTracksList(const QList<QUrl> &removedTracks)
{
    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    initChangesTrackers();

    internalRemoveTracksList(removedTracks);

    if (!d->mInsertedArtists.isEmpty()) {
        ListArtistDataType newArtists;
        for (auto artistId : qAsConst(d->mInsertedArtists)) {
            newArtists.push_back({{DatabaseIdRole, artistId}});
        }
        Q_EMIT artistsAdded(newArtists);
    }

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return;
    }
}

void DatabaseInterface::modifyTracksList(const QList<MusicAudioTrack> &modifiedTracks, const QHash<QString, QUrl> &covers,
                                         const QString &musicSource)
{
    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    initChangesTrackers();

    for (const auto &oneModifiedTrack : modifiedTracks) {
        if (oneModifiedTrack.albumArtist().isEmpty()) {
            continue;
        }

        bool modifyExistingTrack = internalTrackFromDatabaseId(oneModifiedTrack.databaseId()).isValid() ||
                (internalTrackIdFromFileName(oneModifiedTrack.resourceURI()) != 0);

        auto originTrackId = oneModifiedTrack.databaseId();
        if (!originTrackId) {
            originTrackId = internalTrackIdFromFileName(oneModifiedTrack.resourceURI());
        }

        if (!modifyExistingTrack) {
            insertTrackOrigin(oneModifiedTrack.resourceURI(), oneModifiedTrack.fileModificationTime(), insertMusicSource(musicSource));
        } else {
            updateTrackOrigin(originTrackId, oneModifiedTrack.resourceURI(), oneModifiedTrack.fileModificationTime());
        }

        const auto insertedTrackId = internalInsertTrack(oneModifiedTrack, covers, (modifyExistingTrack ? originTrackId : 0),
                                                         (modifyExistingTrack ? TrackFileInsertType::ModifiedTrackFileInsert : TrackFileInsertType::NewTrackFileInsert));

        if (!modifyExistingTrack && insertedTrackId != 0) {
            d->mInsertedTracks.insert(insertedTrackId);
        }
    }

    if (!d->mInsertedArtists.isEmpty()) {
        ListArtistDataType newArtists;

        for (auto artistId : qAsConst(d->mInsertedArtists)) {
            newArtists.push_back({{DatabaseIdRole, artistId}});
        }

        Q_EMIT artistsAdded(newArtists);
    }

    if (!d->mInsertedAlbums.isEmpty()) {
        ListAlbumDataType newAlbums;

        for (auto albumId : qAsConst(d->mInsertedAlbums)) {
            d->mModifiedAlbumIds.remove(albumId);
            newAlbums.push_back(internalOneAlbumPartialData(albumId));
        }

        Q_EMIT albumsAdded(newAlbums);
    }

    for (auto albumId : qAsConst(d->mModifiedAlbumIds)) {
        Q_EMIT albumModified({{DatabaseIdRole, albumId}}, albumId);
    }

    if (!d->mInsertedTracks.isEmpty()) {
        ListTrackDataType newTracks;

        for (auto trackId : qAsConst(d->mInsertedTracks)) {
            newTracks.push_back(internalOneTrackPartialData(trackId));
            d->mModifiedTrackIds.remove(trackId);
        }

        Q_EMIT tracksAdded(newTracks);
    }

    for (auto trackId : qAsConst(d->mModifiedTrackIds)) {
        Q_EMIT trackModified(internalOneTrackPartialData(trackId));
    }

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return;
    }
}

bool DatabaseInterface::startTransaction() const
{
    auto result = false;

    auto transactionResult = d->mTracksDatabase.transaction();
    if (!transactionResult) {
        qDebug() << "transaction failed" << d->mTracksDatabase.lastError() << d->mTracksDatabase.lastError().driverText();

        return result;
    }

    result = true;

    return result;
}

bool DatabaseInterface::finishTransaction() const
{
    auto result = false;

    auto transactionResult = d->mTracksDatabase.commit();

    if (!transactionResult) {
        qDebug() << "commit failed" << d->mTracksDatabase.lastError() << d->mTracksDatabase.lastError().nativeErrorCode();

        return result;
    }

    result = true;

    return result;
}

bool DatabaseInterface::rollBackTransaction() const
{
    auto result = false;

    auto transactionResult = d->mTracksDatabase.rollback();

    if (!transactionResult) {
        qDebug() << "commit failed" << d->mTracksDatabase.lastError() << d->mTracksDatabase.lastError().nativeErrorCode();

        return result;
    }

    result = true;

    return result;
}

void DatabaseInterface::initDatabase()
{
    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    auto listTables = d->mTracksDatabase.tables();

    if (!listTables.contains(QStringLiteral("DatabaseVersionV9"))) {
        auto oldTables = QStringList{
                QStringLiteral("DatabaseVersionV2"),
                QStringLiteral("DatabaseVersionV3"),
                QStringLiteral("DatabaseVersionV4"),
                QStringLiteral("DatabaseVersionV5"),
                QStringLiteral("DatabaseVersionV6"),
                QStringLiteral("DatabaseVersionV7"),
                QStringLiteral("DatabaseVersionV8"),
                QStringLiteral("AlbumsArtists"),
                QStringLiteral("TracksArtists"),
                QStringLiteral("TracksMapping"),
                QStringLiteral("Tracks"),
                QStringLiteral("Composer"),
                QStringLiteral("Genre"),
                QStringLiteral("Lyricist"),
                QStringLiteral("Albums"),
                QStringLiteral("DiscoverSource"),
                QStringLiteral("Artists"),};
        for (const auto &oneTable : oldTables) {
            if (listTables.indexOf(oneTable) == -1) {
                continue;
            }

            QSqlQuery createSchemaQuery(d->mTracksDatabase);

            auto result = createSchemaQuery.exec(QStringLiteral("DROP TABLE ") + oneTable);

            if (!result) {
                qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
                qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

                Q_EMIT databaseError();
            }
        }

        listTables = d->mTracksDatabase.tables();
    }

    if (!listTables.contains(QStringLiteral("DatabaseVersionV9"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `DatabaseVersionV9` (`Version` INTEGER PRIMARY KEY NOT NULL)"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    if (!listTables.contains(QStringLiteral("DiscoverSource"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `DiscoverSource` (`ID` INTEGER PRIMARY KEY NOT NULL, "
                                                                   "`Name` VARCHAR(55) NOT NULL, "
                                                                   "UNIQUE (`Name`))"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    if (!listTables.contains(QStringLiteral("Artists"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `Artists` (`ID` INTEGER PRIMARY KEY NOT NULL, "
                                                                   "`Name` VARCHAR(55) NOT NULL, "
                                                                   "UNIQUE (`Name`))"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    if (!listTables.contains(QStringLiteral("Composer"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `Composer` (`ID` INTEGER PRIMARY KEY NOT NULL, "
                                                                   "`Name` VARCHAR(55) NOT NULL, "
                                                                   "UNIQUE (`Name`))"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    if (!listTables.contains(QStringLiteral("Genre"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `Genre` (`ID` INTEGER PRIMARY KEY NOT NULL, "
                                                                   "`Name` VARCHAR(85) NOT NULL, "
                                                                   "UNIQUE (`Name`))"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    if (!listTables.contains(QStringLiteral("Lyricist"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `Lyricist` (`ID` INTEGER PRIMARY KEY NOT NULL, "
                                                                   "`Name` VARCHAR(55) NOT NULL, "
                                                                   "UNIQUE (`Name`))"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    if (!listTables.contains(QStringLiteral("Albums"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `Albums` ("
                                                                   "`ID` INTEGER PRIMARY KEY NOT NULL, "
                                                                   "`Title` VARCHAR(55) NOT NULL, "
                                                                   "`ArtistName` VARCHAR(55), "
                                                                   "`AlbumPath` VARCHAR(255) NOT NULL, "
                                                                   "`CoverFileName` VARCHAR(255) NOT NULL, "
                                                                   "`AlbumInternalID` VARCHAR(55), "
                                                                   "UNIQUE (`Title`, `ArtistName`, `AlbumPath`), "
                                                                   "CONSTRAINT fk_artists FOREIGN KEY (`ArtistName`) REFERENCES `Artists`(`Name`) "
                                                                   "ON DELETE CASCADE)"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();
        }
    }

    if (!listTables.contains(QStringLiteral("Tracks"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `Tracks` ("
                                                                   "`ID` INTEGER PRIMARY KEY NOT NULL, "
                                                                   "`Title` VARCHAR(85) NOT NULL, "
                                                                   "`ArtistName` VARCHAR(55), "
                                                                   "`AlbumTitle` VARCHAR(55), "
                                                                   "`AlbumArtistName` VARCHAR(55), "
                                                                   "`AlbumPath` VARCHAR(255), "
                                                                   "`TrackNumber` INTEGER DEFAULT -1, "
                                                                   "`DiscNumber` INTEGER DEFAULT -1, "
                                                                   "`Duration` INTEGER NOT NULL, "
                                                                   "`Rating` INTEGER NOT NULL DEFAULT 0, "
                                                                   "`Genre` VARCHAR(55), "
                                                                   "`Composer` VARCHAR(55), "
                                                                   "`Lyricist` VARCHAR(55), "
                                                                   "`Comment` VARCHAR(255) DEFAULT '', "
                                                                   "`Year` INTEGER DEFAULT 0, "
                                                                   "`Channels` INTEGER DEFAULT -1, "
                                                                   "`BitRate` INTEGER DEFAULT -1, "
                                                                   "`SampleRate` INTEGER DEFAULT -1, "
                                                                   "`HasEmbeddedCover` BOOLEAN NOT NULL, "
                                                                   "`ImportDate` INTEGER NOT NULL, "
                                                                   "`FirstPlayDate` INTEGER, "
                                                                   "`LastPlayDate` INTEGER, "
                                                                   "`PlayCounter` INTEGER NOT NULL, "
                                                                   "UNIQUE ("
                                                                   "`Title`, `AlbumTitle`, `AlbumArtistName`, "
                                                                   "`AlbumPath`, `TrackNumber`, `DiscNumber`"
                                                                   "), "
                                                                   "CONSTRAINT fk_artist FOREIGN KEY (`ArtistName`) REFERENCES `Artists`(`Name`), "
                                                                   "CONSTRAINT fk_tracks_composer FOREIGN KEY (`Composer`) REFERENCES `Composer`(`Name`), "
                                                                   "CONSTRAINT fk_tracks_lyricist FOREIGN KEY (`Lyricist`) REFERENCES `Lyricist`(`Name`), "
                                                                   "CONSTRAINT fk_tracks_genre FOREIGN KEY (`Genre`) REFERENCES `Genre`(`Name`), "
                                                                   "CONSTRAINT fk_tracks_album FOREIGN KEY ("
                                                                   "`AlbumTitle`, `AlbumArtistName`, `AlbumPath`)"
                                                                   "REFERENCES `Albums`(`Title`, `ArtistName`, `AlbumPath`))"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    if (!listTables.contains(QStringLiteral("TracksMapping"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `TracksMapping` ("
                                                                   "`TrackID` INTEGER NULL, "
                                                                   "`DiscoverID` INTEGER NOT NULL, "
                                                                   "`FileName` VARCHAR(255) NOT NULL, "
                                                                   "`Priority` INTEGER NOT NULL, "
                                                                   "`FileModifiedTime` DATETIME NOT NULL, "
                                                                   "PRIMARY KEY (`FileName`), "
                                                                   "CONSTRAINT TracksUnique UNIQUE (`TrackID`, `Priority`), "
                                                                   "CONSTRAINT fk_tracksmapping_trackID FOREIGN KEY (`TrackID`) REFERENCES `Tracks`(`ID`) ON DELETE CASCADE, "
                                                                   "CONSTRAINT fk_tracksmapping_discoverID FOREIGN KEY (`DiscoverID`) REFERENCES `DiscoverSource`(`ID`))"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();
        }
    }

    {
        QSqlQuery createTrackIndex(d->mTracksDatabase);

        const auto &result = createTrackIndex.exec(QStringLiteral("CREATE INDEX "
                                                                  "IF NOT EXISTS "
                                                                  "`TitleAlbumsIndex` ON `Albums` "
                                                                  "(`Title`)"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        QSqlQuery createTrackIndex(d->mTracksDatabase);

        const auto &result = createTrackIndex.exec(QStringLiteral("CREATE INDEX "
                                                                  "IF NOT EXISTS "
                                                                  "`ArtistNameAlbumsIndex` ON `Albums` "
                                                                  "(`ArtistName`)"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        QSqlQuery createTrackIndex(d->mTracksDatabase);

        const auto &result = createTrackIndex.exec(QStringLiteral("CREATE INDEX "
                                                                  "IF NOT EXISTS "
                                                                  "`TracksAlbumIndex` ON `Tracks` "
                                                                  "(`AlbumTitle`, `AlbumArtistName`, `AlbumPath`)"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        QSqlQuery createTrackIndex(d->mTracksDatabase);

        const auto &result = createTrackIndex.exec(QStringLiteral("CREATE INDEX "
                                                                  "IF NOT EXISTS "
                                                                  "`ArtistNameIndex` ON `Tracks` "
                                                                  "(`ArtistName`)"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        QSqlQuery createTrackIndex(d->mTracksDatabase);

        const auto &result = createTrackIndex.exec(QStringLiteral("CREATE INDEX "
                                                                  "IF NOT EXISTS "
                                                                  "`AlbumArtistNameIndex` ON `Tracks` "
                                                                  "(`AlbumArtistName`)"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        QSqlQuery createTrackIndex(d->mTracksDatabase);

        const auto &result = createTrackIndex.exec(QStringLiteral("CREATE INDEX "
                                                                  "IF NOT EXISTS "
                                                                  "`TracksFileNameIndex` ON `TracksMapping` "
                                                                  "(`FileName`)"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastError();

            Q_EMIT databaseError();
        }
    }

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return;
    }
}

void DatabaseInterface::initRequest()
{
    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    {
        auto selectAlbumQueryText = QStringLiteral("SELECT "
                                                   "album.`ID`, "
                                                   "album.`Title`, "
                                                   "album.`AlbumInternalID`, "
                                                   "album.`ArtistName`, "
                                                   "album.`AlbumPath`, "
                                                   "album.`CoverFileName`, "
                                                   "("
                                                   "SELECT "
                                                   "COUNT(*) "
                                                   "FROM "
                                                   "`Tracks` tracks3 "
                                                   "WHERE "
                                                   "tracks3.`AlbumTitle` = album.`Title` AND "
                                                   "(tracks3.`AlbumArtistName` = album.`ArtistName` OR "
                                                   "(tracks3.`AlbumArtistName` IS NULL AND "
                                                   "album.`ArtistName` IS NULL"
                                                   ")"
                                                   ") AND "
                                                   "tracks3.`AlbumPath` = album.`AlbumPath` "
                                                   ") as `TracksCount`, "
                                                   "("
                                                   "SELECT "
                                                   "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                   "FROM "
                                                   "`Tracks` tracks2 "
                                                   "WHERE "
                                                   "tracks2.`AlbumTitle` = album.`Title` AND "
                                                   "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                   "(tracks2.`AlbumArtistName` IS NULL AND "
                                                   "album.`ArtistName` IS NULL"
                                                   ")"
                                                   ") AND "
                                                   "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                   ") as `IsSingleDiscAlbum`, "
                                                   "GROUP_CONCAT(tracks.`ArtistName`, ', ') as AllArtists, "
                                                   "MAX(tracks.`Rating`) as HighestRating, "
                                                   "GROUP_CONCAT(genres.`Name`, ', ') as AllGenres "
                                                   "FROM "
                                                   "`Albums` album LEFT JOIN "
                                                   "`Tracks` tracks ON "
                                                   "tracks.`AlbumTitle` = album.`Title` AND "
                                                   "("
                                                   "tracks.`AlbumArtistName` = album.`ArtistName` OR "
                                                   "("
                                                   "tracks.`AlbumArtistName` IS NULL AND "
                                                   "album.`ArtistName` IS NULL"
                                                   ")"
                                                   ") AND "
                                                   "tracks.`AlbumPath` = album.`AlbumPath`"
                                                   "LEFT JOIN "
                                                   "`Genre` genres ON tracks.`Genre` = genres.`Name` "
                                                   "WHERE "
                                                   "album.`ID` = :albumId "
                                                   "GROUP BY album.`ID`");

        auto result = prepareQuery(d->mSelectAlbumQuery, selectAlbumQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllAlbumsText = QStringLiteral("SELECT "
                                                  "album.`ID`, "
                                                  "album.`Title`, "
                                                  "album.`AlbumInternalID`, "
                                                  "album.`ArtistName`, "
                                                  "album.`AlbumPath`, "
                                                  "album.`CoverFileName`, "
                                                  "("
                                                  "SELECT "
                                                  "COUNT(*) "
                                                  "FROM "
                                                  "`Tracks` tracks "
                                                  "WHERE "
                                                  "tracks.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks.`AlbumArtistName` = album.`ArtistName` OR "
                                                  "(tracks.`AlbumArtistName` IS NULL AND "
                                                  "album.`ArtistName` IS NULL"
                                                  ")"
                                                  ") AND "
                                                  "tracks.`AlbumPath` = album.`AlbumPath` "
                                                  ") as `TracksCount`, "
                                                  "("
                                                  "SELECT "
                                                  "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                  "FROM "
                                                  "`Tracks` tracks2 "
                                                  "WHERE "
                                                  "tracks2.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                  "(tracks2.`AlbumArtistName` IS NULL AND "
                                                  "album.`ArtistName` IS NULL"
                                                  ")"
                                                  ") AND "
                                                  "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                  ") as `IsSingleDiscAlbum` "
                                                  "FROM `Albums` album "
                                                  "ORDER BY album.`Title` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllAlbumsQuery, selectAllAlbumsText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllGenresText = QStringLiteral("SELECT "
                                                  "genre.`ID`, "
                                                  "genre.`Name` "
                                                  "FROM `Genre` genre "
                                                  "ORDER BY genre.`Name` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllGenresQuery, selectAllGenresText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllAlbumsText = QStringLiteral("SELECT "
                                                  "album.`ID`, "
                                                  "album.`Title`, "
                                                  "album.`ArtistName` as SecondaryText, "
                                                  "album.`CoverFileName`, "
                                                  "album.`ArtistName`, "
                                                  "GROUP_CONCAT(tracks.`ArtistName`, ', ') as AllArtists, "
                                                  "MAX(tracks.`Rating`) as HighestRating, "
                                                  "GROUP_CONCAT(genres.`Name`, ', ') as AllGenres, "
                                                  "("
                                                  "SELECT "
                                                  "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                  "FROM "
                                                  "`Tracks` tracks2 "
                                                  "WHERE "
                                                  "tracks2.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                  "(tracks2.`AlbumArtistName` IS NULL AND "
                                                  "album.`ArtistName` IS NULL"
                                                  ")"
                                                  ") AND "
                                                  "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                  ") as `IsSingleDiscAlbum` "
                                                  "FROM "
                                                  "`Albums` album, "
                                                  "`Tracks` tracks LEFT JOIN "
                                                  "`Genre` genres ON tracks.`Genre` = genres.`Name` "
                                                  "WHERE "
                                                  "tracks.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks.`AlbumArtistName` = album.`ArtistName` OR "
                                                  "(tracks.`AlbumArtistName` IS NULL AND "
                                                  "album.`ArtistName` IS NULL"
                                                  ")"
                                                  ") AND "
                                                  "tracks.`AlbumPath` = album.`AlbumPath` "
                                                  "GROUP BY album.`ID`, album.`Title`, album.`AlbumPath` "
                                                  "ORDER BY album.`Title` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllAlbumsShortQuery, selectAllAlbumsText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsShortQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsShortQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllAlbumsText = QStringLiteral("SELECT "
                                                  "album.`ID`, "
                                                  "album.`Title`, "
                                                  "album.`ArtistName` as SecondaryText, "
                                                  "album.`CoverFileName`, "
                                                  "album.`ArtistName`, "
                                                  "GROUP_CONCAT(tracks.`ArtistName`, ', ') as AllArtists, "
                                                  "MAX(tracks.`Rating`) as HighestRating, "
                                                  "GROUP_CONCAT(genres.`Name`, ', ') as AllGenres "
                                                  "FROM "
                                                  "`Albums` album, "
                                                  "`Tracks` tracks LEFT JOIN "
                                                  "`Genre` genres ON tracks.`Genre` = genres.`Name` "
                                                  "WHERE "
                                                  "tracks.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks.`AlbumArtistName` = album.`ArtistName` OR "
                                                  "(tracks.`AlbumArtistName` IS NULL AND "
                                                  "album.`ArtistName` IS NULL"
                                                  ")"
                                                  ") AND "
                                                  "tracks.`AlbumPath` = album.`AlbumPath` AND "
                                                  "EXISTS ("
                                                  "  SELECT tracks2.`Genre` "
                                                  "  FROM "
                                                  "  `Tracks` tracks2, "
                                                  "  `Genre` genre2 "
                                                  "  WHERE "
                                                  "  tracks2.`AlbumTitle` = album.`Title` AND "
                                                  "  tracks2.`AlbumArtistName` = album.`ArtistName` AND "
                                                  "  tracks2.`Genre` = genre2.`Name` AND "
                                                  "  genre2.`Name` = :genreFilter AND "
                                                  "  (tracks2.`ArtistName` = :artistFilter OR tracks2.`AlbumArtistName` = :artistFilter) "
                                                  ") "
                                                  "GROUP BY album.`ID`, album.`Title`, album.`AlbumPath` "
                                                  "ORDER BY album.`Title` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllAlbumsShortWithGenreArtistFilterQuery, selectAllAlbumsText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsShortWithGenreArtistFilterQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsShortWithGenreArtistFilterQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllAlbumsText = QStringLiteral("SELECT "
                                                  "album.`ID`, "
                                                  "album.`Title`, "
                                                  "album.`ArtistName` as SecondaryText, "
                                                  "album.`CoverFileName`, "
                                                  "album.`ArtistName`, "
                                                  "GROUP_CONCAT(tracks.`ArtistName`, ', ') as AllArtists, "
                                                  "MAX(tracks.`Rating`) as HighestRating, "
                                                  "GROUP_CONCAT(genres.`Name`, ', ') as AllGenres "
                                                  "FROM "
                                                  "`Albums` album, "
                                                  "`Tracks` tracks LEFT JOIN "
                                                  "`Genre` genres ON tracks.`Genre` = genres.`Name` "
                                                  "WHERE "
                                                  "tracks.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks.`AlbumArtistName` = album.`ArtistName` OR "
                                                  "(tracks.`AlbumArtistName` IS NULL AND "
                                                  "album.`ArtistName` IS NULL"
                                                  ")"
                                                  ") AND "
                                                  "tracks.`AlbumPath` = album.`AlbumPath` AND "
                                                  "EXISTS ("
                                                  "  SELECT tracks2.`Genre` "
                                                  "  FROM "
                                                  "  `Tracks` tracks2 "
                                                  "  WHERE "
                                                  "  tracks2.`AlbumTitle` = album.`Title` AND "
                                                  "  tracks2.`AlbumArtistName` = album.`ArtistName` AND "
                                                  "  (tracks2.`ArtistName` = :artistFilter OR tracks2.`AlbumArtistName` = :artistFilter) "
                                                  ") "
                                                  "GROUP BY album.`ID`, album.`Title`, album.`AlbumPath` "
                                                  "ORDER BY album.`Title` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllAlbumsShortWithArtistFilterQuery, selectAllAlbumsText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsShortWithArtistFilterQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsShortWithArtistFilterQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllArtistsWithFilterText = QStringLiteral("SELECT artists.`ID`, "
                                                             "artists.`Name`, "
                                                             "GROUP_CONCAT(genres.`Name`, ', ') as AllGenres "
                                                             "FROM `Artists` artists  LEFT JOIN "
                                                             "`Tracks` tracks ON artists.`Name` = tracks.`ArtistName` LEFT JOIN "
                                                             "`Genre` genres ON tracks.`Genre` = genres.`Name` "
                                                             "GROUP BY artists.`ID` "
                                                             "ORDER BY artists.`Name` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllArtistsQuery, selectAllArtistsWithFilterText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllArtistsQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllArtistsQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllArtistsWithGenreFilterText = QStringLiteral("SELECT artists.`ID`, "
                                                                  "artists.`Name`, "
                                                                  "GROUP_CONCAT(genres.`Name`, ', ') as AllGenres "
                                                                  "FROM `Artists` artists  LEFT JOIN "
                                                                  "`Tracks` tracks ON (tracks.`ArtistName` = artists.`Name` OR tracks.`AlbumArtistName` = artists.`Name`) LEFT JOIN "
                                                                  "`Genre` genres ON tracks.`Genre` = genres.`Name` "
                                                                  "WHERE "
                                                                  "EXISTS ("
                                                                  "  SELECT tracks2.`Genre` "
                                                                  "  FROM "
                                                                  "  `Tracks` tracks2, "
                                                                  "  `Genre` genre2 "
                                                                  "  WHERE "
                                                                  "  (tracks2.`ArtistName` = artists.`Name` OR tracks2.`AlbumArtistName` = artists.`Name`) AND "
                                                                  "  tracks2.`Genre` = genre2.`Name` AND "
                                                                  "  genre2.`Name` = :genreFilter "
                                                                  ") "
                                                                  "GROUP BY artists.`ID` "
                                                                  "ORDER BY artists.`Name` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllArtistsWithGenreFilterQuery, selectAllArtistsWithGenreFilterText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllArtistsWithGenreFilterQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllArtistsWithGenreFilterQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllComposersWithFilterText = QStringLiteral("SELECT `ID`, "
                                                               "`Name` "
                                                               "FROM `Artists` "
                                                               "ORDER BY `Name` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllComposersQuery, selectAllComposersWithFilterText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllComposersQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllComposersQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllLyricistsWithFilterText = QStringLiteral("SELECT `ID`, "
                                                               "`Name` "
                                                               "FROM `Lyricist` "
                                                               "ORDER BY `Name` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllLyricistsQuery, selectAllLyricistsWithFilterText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllLyricistsQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllLyricistsQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllTracksText = QStringLiteral("SELECT "
                                                  "tracks.`ID`, "
                                                  "tracks.`Title`, "
                                                  "album.`ID`, "
                                                  "tracks.`ArtistName`, "
                                                  "tracks.`AlbumArtistName`, "
                                                  "tracksMapping.`FileName`, "
                                                  "tracksMapping.`FileModifiedTime`, "
                                                  "tracks.`TrackNumber`, "
                                                  "tracks.`DiscNumber`, "
                                                  "tracks.`Duration`, "
                                                  "tracks.`AlbumTitle`, "
                                                  "tracks.`Rating`, "
                                                  "album.`CoverFileName`, "
                                                  "("
                                                  "SELECT "
                                                  "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                  "FROM "
                                                  "`Tracks` tracks2 "
                                                  "WHERE "
                                                  "tracks2.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                  "(tracks2.`AlbumArtistName` IS NULL AND "
                                                  "album.`ArtistName` IS NULL"
                                                  ")"
                                                  ") AND "
                                                  "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                  ") as `IsSingleDiscAlbum`, "
                                                  "trackGenre.`Name`, "
                                                  "trackComposer.`Name`, "
                                                  "trackLyricist.`Name`, "
                                                  "tracks.`Comment`, "
                                                  "tracks.`Year`, "
                                                  "tracks.`Channels`, "
                                                  "tracks.`BitRate`, "
                                                  "tracks.`SampleRate`, "
                                                  "tracks.`HasEmbeddedCover`, "
                                                  "tracks.`ImportDate`, "
                                                  "tracks.`FirstPlayDate`, "
                                                  "tracks.`LastPlayDate`, "
                                                  "tracks.`PlayCounter`, "
                                                  "tracks.`PlayCounter` / (strftime('%s', 'now') - tracks.`FirstPlayDate`) as PlayFrequency "
                                                  "FROM "
                                                  "`Tracks` tracks, "
                                                  "`TracksMapping` tracksMapping "
                                                  "LEFT JOIN "
                                                  "`Albums` album "
                                                  "ON "
                                                  "tracks.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                  "tracks.`AlbumPath` = album.`AlbumPath` "
                                                  "LEFT JOIN `Genre` trackGenre ON trackGenre.`Name` = tracks.`Genre` "
                                                  "LEFT JOIN `Composer` trackComposer ON trackComposer.`Name` = tracks.`Composer` "
                                                  "LEFT JOIN `Lyricist` trackLyricist ON trackLyricist.`Name` = tracks.`Lyricist` "
                                                  "WHERE "
                                                  "tracksMapping.`TrackID` = tracks.`ID` AND "
                                                  "tracksMapping.`Priority` = (SELECT MIN(`Priority`) FROM `TracksMapping` WHERE `TrackID` = tracks.`ID`)");

        auto result = prepareQuery(d->mSelectAllTracksQuery, selectAllTracksText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllTracksQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllTracksQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllTracksText = QStringLiteral("SELECT "
                                                  "tracks.`ID`, "
                                                  "tracks.`Title`, "
                                                  "album.`ID`, "
                                                  "tracks.`ArtistName`, "
                                                  "tracks.`AlbumArtistName`, "
                                                  "tracksMapping.`FileName`, "
                                                  "tracksMapping.`FileModifiedTime`, "
                                                  "tracks.`TrackNumber`, "
                                                  "tracks.`DiscNumber`, "
                                                  "tracks.`Duration`, "
                                                  "tracks.`AlbumTitle`, "
                                                  "tracks.`Rating`, "
                                                  "album.`CoverFileName`, "
                                                  "("
                                                  "SELECT "
                                                  "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                  "FROM "
                                                  "`Tracks` tracks2 "
                                                  "WHERE "
                                                  "tracks2.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                  "(tracks2.`AlbumArtistName` IS NULL AND "
                                                  "album.`ArtistName` IS NULL"
                                                  ")"
                                                  ") AND "
                                                  "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                  ") as `IsSingleDiscAlbum`, "
                                                  "trackGenre.`Name`, "
                                                  "trackComposer.`Name`, "
                                                  "trackLyricist.`Name`, "
                                                  "tracks.`Comment`, "
                                                  "tracks.`Year`, "
                                                  "tracks.`Channels`, "
                                                  "tracks.`BitRate`, "
                                                  "tracks.`SampleRate`, "
                                                  "tracks.`HasEmbeddedCover`, "
                                                  "tracks.`ImportDate`, "
                                                  "tracks.`FirstPlayDate`, "
                                                  "tracks.`LastPlayDate`, "
                                                  "tracks.`PlayCounter`, "
                                                  "tracks.`PlayCounter` / (strftime('%s', 'now') - tracks.`FirstPlayDate`) as PlayFrequency "
                                                  "FROM "
                                                  "`Tracks` tracks, "
                                                  "`TracksMapping` tracksMapping "
                                                  "LEFT JOIN "
                                                  "`Albums` album "
                                                  "ON "
                                                  "tracks.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                  "tracks.`AlbumPath` = album.`AlbumPath` "
                                                  "LEFT JOIN `Genre` trackGenre ON trackGenre.`Name` = tracks.`Genre` "
                                                  "LEFT JOIN `Composer` trackComposer ON trackComposer.`Name` = tracks.`Composer` "
                                                  "LEFT JOIN `Lyricist` trackLyricist ON trackLyricist.`Name` = tracks.`Lyricist` "
                                                  "WHERE "
                                                  "tracksMapping.`TrackID` = tracks.`ID` AND "
                                                  "tracks.`PlayCounter` > 0 AND "
                                                  "tracksMapping.`Priority` = (SELECT MIN(`Priority`) FROM `TracksMapping` WHERE `TrackID` = tracks.`ID`) "
                                                  "ORDER BY tracks.`LastPlayDate` DESC "
                                                  "LIMIT :maximumResults");

        auto result = prepareQuery(d->mSelectAllRecentlyPlayedTracksQuery, selectAllTracksText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllRecentlyPlayedTracksQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllRecentlyPlayedTracksQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllTracksText = QStringLiteral("SELECT "
                                                  "tracks.`ID`, "
                                                  "tracks.`Title`, "
                                                  "album.`ID`, "
                                                  "tracks.`ArtistName`, "
                                                  "tracks.`AlbumArtistName`, "
                                                  "tracksMapping.`FileName`, "
                                                  "tracksMapping.`FileModifiedTime`, "
                                                  "tracks.`TrackNumber`, "
                                                  "tracks.`DiscNumber`, "
                                                  "tracks.`Duration`, "
                                                  "tracks.`AlbumTitle`, "
                                                  "tracks.`Rating`, "
                                                  "album.`CoverFileName`, "
                                                  "("
                                                  "SELECT "
                                                  "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                  "FROM "
                                                  "`Tracks` tracks2 "
                                                  "WHERE "
                                                  "tracks2.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                  "(tracks2.`AlbumArtistName` IS NULL AND "
                                                  "album.`ArtistName` IS NULL"
                                                  ")"
                                                  ") AND "
                                                  "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                  ") as `IsSingleDiscAlbum`, "
                                                  "trackGenre.`Name`, "
                                                  "trackComposer.`Name`, "
                                                  "trackLyricist.`Name`, "
                                                  "tracks.`Comment`, "
                                                  "tracks.`Year`, "
                                                  "tracks.`Channels`, "
                                                  "tracks.`BitRate`, "
                                                  "tracks.`SampleRate`, "
                                                  "tracks.`HasEmbeddedCover`, "
                                                  "tracks.`ImportDate`, "
                                                  "tracks.`FirstPlayDate`, "
                                                  "tracks.`LastPlayDate`, "
                                                  "tracks.`PlayCounter`, "
                                                  "tracks.`PlayCounter` / (strftime('%s', 'now') - tracks.`FirstPlayDate`) as PlayFrequency "
                                                  "FROM "
                                                  "`Tracks` tracks, "
                                                  "`TracksMapping` tracksMapping "
                                                  "LEFT JOIN "
                                                  "`Albums` album "
                                                  "ON "
                                                  "tracks.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                  "tracks.`AlbumPath` = album.`AlbumPath` "
                                                  "LEFT JOIN `Genre` trackGenre ON trackGenre.`Name` = tracks.`Genre` "
                                                  "LEFT JOIN `Composer` trackComposer ON trackComposer.`Name` = tracks.`Composer` "
                                                  "LEFT JOIN `Lyricist` trackLyricist ON trackLyricist.`Name` = tracks.`Lyricist` "
                                                  "WHERE "
                                                  "tracksMapping.`TrackID` = tracks.`ID` AND "
                                                  "tracks.`PlayCounter` > 0 AND "
                                                  "tracksMapping.`Priority` = (SELECT MIN(`Priority`) FROM `TracksMapping` WHERE `TrackID` = tracks.`ID`) "
                                                  "ORDER BY tracks.`PlayCounter` / (strftime('%s', 'now') - tracks.`FirstPlayDate`) DESC "
                                                  "LIMIT :maximumResults");

        auto result = prepareQuery(d->mSelectAllFrequentlyPlayedTracksQuery, selectAllTracksText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllFrequentlyPlayedTracksQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllFrequentlyPlayedTracksQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllTracksShortText = QStringLiteral("SELECT "
                                                       "tracks.`ID`, "
                                                       "tracks.`Title`, "
                                                       "tracks.`ArtistName`, "
                                                       "tracks.`AlbumTitle`, "
                                                       "tracks.`AlbumArtistName`, "
                                                       "tracks.`Duration`, "
                                                       "album.`CoverFileName`, "
                                                       "tracks.`TrackNumber`, "
                                                       "tracks.`DiscNumber`, "
                                                       "tracks.`Rating` "
                                                       "FROM "
                                                       "`Tracks` tracks "
                                                       "LEFT JOIN "
                                                       "`Albums` album "
                                                       "ON "
                                                       "tracks.`AlbumTitle` = album.`Title` AND "
                                                       "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                       "tracks.`AlbumPath` = album.`AlbumPath` "
                                                       "");

        auto result = prepareQuery(d->mSelectAllTracksShortQuery, selectAllTracksShortText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllTracksShortQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllTracksShortQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectArtistByNameText = QStringLiteral("SELECT `ID`, "
                                                     "`Name` "
                                                     "FROM `Artists` "
                                                     "WHERE "
                                                     "`Name` = :name");

        auto result = prepareQuery(d->mSelectArtistByNameQuery, selectArtistByNameText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectArtistByNameQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectArtistByNameQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectComposerByNameText = QStringLiteral("SELECT `ID`, "
                                                       "`Name` "
                                                       "FROM `Composer` "
                                                       "WHERE "
                                                       "`Name` = :name");

        auto result = prepareQuery(d->mSelectComposerByNameQuery, selectComposerByNameText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectComposerByNameQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectComposerByNameQuery.lastError();
        }
    }

    {
        auto selectLyricistByNameText = QStringLiteral("SELECT `ID`, "
                                                       "`Name` "
                                                       "FROM `Lyricist` "
                                                       "WHERE "
                                                       "`Name` = :name");

        auto result = prepareQuery(d->mSelectLyricistByNameQuery, selectLyricistByNameText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectLyricistByNameQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectLyricistByNameQuery.lastError();
        }
    }

    {
        auto selectGenreByNameText = QStringLiteral("SELECT `ID`, "
                                                    "`Name` "
                                                    "FROM `Genre` "
                                                    "WHERE "
                                                    "`Name` = :name");

        auto result = prepareQuery(d->mSelectGenreByNameQuery, selectGenreByNameText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreByNameQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreByNameQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto insertArtistsText = QStringLiteral("INSERT INTO `Artists` (`ID`, `Name`) "
                                                "VALUES (:artistId, :name)");

        auto result = prepareQuery(d->mInsertArtistsQuery, insertArtistsText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertArtistsQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertArtistsQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto insertGenreText = QStringLiteral("INSERT INTO `Genre` (`ID`, `Name`) "
                                              "VALUES (:genreId, :name)");

        auto result = prepareQuery(d->mInsertGenreQuery, insertGenreText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertGenreQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertGenreQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto insertComposerText = QStringLiteral("INSERT INTO `Composer` (`ID`, `Name`) "
                                                 "VALUES (:composerId, :name)");

        auto result = prepareQuery(d->mInsertComposerQuery, insertComposerText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertComposerQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertComposerQuery.lastError();
        }
    }

    {
        auto insertLyricistText = QStringLiteral("INSERT INTO `Lyricist` (`ID`, `Name`) "
                                                 "VALUES (:lyricistId, :name)");

        auto result = prepareQuery(d->mInsertLyricistQuery, insertLyricistText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertLyricistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertLyricistQuery.lastError();
        }
    }

    {
        auto selectTrackQueryText = QStringLiteral("SELECT "
                                                   "tracks.`ID`, "
                                                   "tracks.`Title`, "
                                                   "album.`ID`, "
                                                   "tracks.`ArtistName`, "
                                                   "tracks.`AlbumArtistName`, "
                                                   "tracksMapping.`FileName`, "
                                                   "tracksMapping.`FileModifiedTime`, "
                                                   "tracks.`TrackNumber`, "
                                                   "tracks.`DiscNumber`, "
                                                   "tracks.`Duration`, "
                                                   "tracks.`AlbumTitle`, "
                                                   "tracks.`Rating`, "
                                                   "album.`CoverFileName`, "
                                                   "("
                                                   "SELECT "
                                                   "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                   "FROM "
                                                   "`Tracks` tracks2 "
                                                   "WHERE "
                                                   "tracks2.`AlbumTitle` = album.`Title` AND "
                                                   "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                   "(tracks2.`AlbumArtistName` IS NULL AND "
                                                   "album.`ArtistName` IS NULL"
                                                   ")"
                                                   ") AND "
                                                   "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                   ") as `IsSingleDiscAlbum`, "
                                                   "trackGenre.`Name`, "
                                                   "trackComposer.`Name`, "
                                                   "trackLyricist.`Name`, "
                                                   "tracks.`Comment`, "
                                                   "tracks.`Year`, "
                                                   "tracks.`Channels`, "
                                                   "tracks.`BitRate`, "
                                                   "tracks.`SampleRate`, "
                                                   "tracks.`HasEmbeddedCover`, "
                                                   "tracks.`ImportDate`, "
                                                   "tracks.`FirstPlayDate`, "
                                                   "tracks.`LastPlayDate`, "
                                                   "tracks.`PlayCounter`, "
                                                   "tracks.`PlayCounter` / (strftime('%s', 'now') - tracks.`FirstPlayDate`) as PlayFrequency "
                                                   "FROM "
                                                   "`Tracks` tracks, "
                                                   "`TracksMapping` tracksMapping "
                                                   "LEFT JOIN "
                                                   "`Albums` album "
                                                   "ON "
                                                   "tracks.`AlbumTitle` = album.`Title` AND "
                                                   "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                   "tracks.`AlbumPath` = album.`AlbumPath` "
                                                   "LEFT JOIN `Composer` trackComposer ON trackComposer.`Name` = tracks.`Composer` "
                                                   "LEFT JOIN `Lyricist` trackLyricist ON trackLyricist.`Name` = tracks.`Lyricist` "
                                                   "LEFT JOIN `Genre` trackGenre ON trackGenre.`Name` = tracks.`Genre` "
                                                   "WHERE "
                                                   "tracksMapping.`TrackID` = tracks.`ID` AND "
                                                   "album.`ID` = :albumId AND "
                                                   "tracksMapping.`Priority` = (SELECT MIN(`Priority`) FROM `TracksMapping` WHERE `TrackID` = tracks.`ID`) "
                                                   "ORDER BY tracks.`DiscNumber` ASC, "
                                                   "tracks.`TrackNumber` ASC");

        auto result = prepareQuery(d->mSelectTrackQuery, selectTrackQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackQuery.lastError();

            Q_EMIT databaseError();
        }
    }
    {
        auto selectTrackFromIdQueryText = QStringLiteral("SELECT "
                                                         "tracks.`Id`, "
                                                         "tracks.`Title`, "
                                                         "album.`ID`, "
                                                         "tracks.`ArtistName`, "
                                                         "tracks.`AlbumArtistName`, "
                                                         "tracksMapping.`FileName`, "
                                                         "tracksMapping.`FileModifiedTime`, "
                                                         "tracks.`TrackNumber`, "
                                                         "tracks.`DiscNumber`, "
                                                         "tracks.`Duration`, "
                                                         "tracks.`AlbumTitle`, "
                                                         "tracks.`Rating`, "
                                                         "album.`CoverFileName`, "
                                                         "("
                                                         "SELECT "
                                                         "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                         "FROM "
                                                         "`Tracks` tracks2 "
                                                         "WHERE "
                                                         "tracks2.`AlbumTitle` = album.`Title` AND "
                                                         "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                         "(tracks2.`AlbumArtistName` IS NULL AND "
                                                         "album.`ArtistName` IS NULL"
                                                         ")"
                                                         ") AND "
                                                         "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                         ") as `IsSingleDiscAlbum`, "
                                                         "trackGenre.`Name`, "
                                                         "trackComposer.`Name`, "
                                                         "trackLyricist.`Name`, "
                                                         "tracks.`Comment`, "
                                                         "tracks.`Year`, "
                                                         "tracks.`Channels`, "
                                                         "tracks.`BitRate`, "
                                                         "tracks.`SampleRate`, "
                                                         "tracks.`HasEmbeddedCover`, "
                                                         "tracks.`ImportDate`, "
                                                         "tracks.`FirstPlayDate`, "
                                                         "tracks.`LastPlayDate`, "
                                                         "tracks.`PlayCounter`, "
                                                         "tracks.`PlayCounter` / (strftime('%s', 'now') - tracks.`FirstPlayDate`) as PlayFrequency "
                                                         "FROM "
                                                         "`Tracks` tracks, "
                                                         "`TracksMapping` tracksMapping "
                                                         "LEFT JOIN "
                                                         "`Albums` album "
                                                         "ON "
                                                         "tracks.`AlbumTitle` = album.`Title` AND "
                                                         "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                         "tracks.`AlbumPath` = album.`AlbumPath` "
                                                         "LEFT JOIN `Composer` trackComposer ON trackComposer.`Name` = tracks.`Composer` "
                                                         "LEFT JOIN `Lyricist` trackLyricist ON trackLyricist.`Name` = tracks.`Lyricist` "
                                                         "LEFT JOIN `Genre` trackGenre ON trackGenre.`Name` = tracks.`Genre` "
                                                         "WHERE "
                                                         "tracks.`ID` = :trackId AND "
                                                         "tracksMapping.`TrackID` = tracks.`ID` AND "
                                                         "tracksMapping.`Priority` = (SELECT MIN(`Priority`) FROM `TracksMapping` WHERE `TrackID` = tracks.`ID`)");

        auto result = prepareQuery(d->mSelectTrackFromIdQuery, selectTrackFromIdQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackFromIdQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackFromIdQuery.lastError();

            Q_EMIT databaseError();
        }
    }
    {
        auto selectCountAlbumsQueryText = QStringLiteral("SELECT count(*) "
                                                         "FROM `Albums` album "
                                                         "WHERE album.`ArtistName` = :artistName ");

        const auto result = prepareQuery(d->mSelectCountAlbumsForArtistQuery, selectCountAlbumsQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectCountAlbumsForArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectCountAlbumsForArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectGenreForArtistQueryText = QStringLiteral("SELECT DISTINCT trackGenre.`Name` "
                                                            "FROM "
                                                            "`Tracks` tracks "
                                                            "LEFT JOIN "
                                                            "`Albums` album "
                                                            "ON "
                                                            "tracks.`AlbumTitle` = album.`Title` AND "
                                                            "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                            "tracks.`AlbumPath` = album.`AlbumPath` "
                                                            "LEFT JOIN `Genre` trackGenre ON trackGenre.`Name` = tracks.`Genre` "
                                                            "WHERE "
                                                            "album.`ArtistName` = :artistName");

        const auto result = prepareQuery(d->mSelectGenreForArtistQuery, selectGenreForArtistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreForArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreForArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectGenreForAlbumQueryText = QStringLiteral("SELECT DISTINCT trackGenre.`Name` "
                                                           "FROM "
                                                           "`Tracks` tracks "
                                                           "LEFT JOIN "
                                                           "`Albums` album "
                                                           "ON "
                                                           "tracks.`AlbumTitle` = album.`Title` AND "
                                                           "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                           "tracks.`AlbumPath` = album.`AlbumPath` "
                                                           "LEFT JOIN `Genre` trackGenre ON trackGenre.`Name` = tracks.`Genre` "
                                                           "WHERE "
                                                           "album.`ID` = :albumId");

        const auto result = prepareQuery(d->mSelectGenreForAlbumQuery, selectGenreForAlbumQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreForAlbumQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreForAlbumQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectCountAlbumsQueryText = QStringLiteral("SELECT distinct count(album.`ID`) "
                                                         "FROM "
                                                         "`Tracks` tracks, "
                                                         "`Albums` album "
                                                         "LEFT JOIN `Composer` albumComposer ON albumComposer.`Name` = tracks.`Composer` "
                                                         "WHERE "
                                                         "(tracks.`AlbumTitle` = album.`Title` OR tracks.`AlbumTitle` IS NULL ) AND "
                                                         "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                         "(tracks.`AlbumPath` = album.`AlbumPath` OR tracks.`AlbumPath` IS NULL ) AND "
                                                         "albumComposer.`Name` = :artistName");

        const auto result = prepareQuery(d->mSelectCountAlbumsForComposerQuery, selectCountAlbumsQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectCountAlbumsForComposerQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectCountAlbumsForComposerQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectCountAlbumsQueryText = QStringLiteral("SELECT distinct count(album.`ID`) "
                                                         "FROM "
                                                         "`Tracks` tracks, "
                                                         "`Albums` album "
                                                         "LEFT JOIN `Lyricist` albumLyricist ON albumLyricist.`Name` = tracks.`Lyricist` "
                                                         "WHERE "
                                                         "(tracks.`AlbumTitle` = album.`Title` OR tracks.`AlbumTitle` IS NULL ) AND "
                                                         "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                         "(tracks.`AlbumPath` = album.`AlbumPath` OR tracks.`AlbumPath` IS NULL ) AND "
                                                         "albumLyricist.`Name` = :artistName");

        const auto result = prepareQuery(d->mSelectCountAlbumsForLyricistQuery, selectCountAlbumsQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectCountAlbumsForLyricistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectCountAlbumsForLyricistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAlbumIdFromTitleQueryText = QStringLiteral("SELECT "
                                                              "album.`ID` "
                                                              "FROM "
                                                              "`Albums` album "
                                                              "WHERE "
                                                              "album.`ArtistName` = :artistName AND "
                                                              "album.`Title` = :title");

        auto result = prepareQuery(d->mSelectAlbumIdFromTitleQuery, selectAlbumIdFromTitleQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdFromTitleQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdFromTitleQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAlbumIdFromTitleAndArtistQueryText = QStringLiteral("SELECT "
                                                                       "album.`ID` "
                                                                       "FROM "
                                                                       "`Albums` album "
                                                                       "WHERE "
                                                                       "album.`ArtistName` = :artistName AND "
                                                                       "album.`Title` = :title AND "
                                                                       "album.`AlbumPath` = :albumPath");

        auto result = prepareQuery(d->mSelectAlbumIdFromTitleAndArtistQuery, selectAlbumIdFromTitleAndArtistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdFromTitleAndArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdFromTitleAndArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAlbumIdFromTitleWithoutArtistQueryText = QStringLiteral("SELECT "
                                                                           "album.`ID` "
                                                                           "FROM "
                                                                           "`Albums` album "
                                                                           "WHERE "
                                                                           "album.`AlbumPath` = :albumPath AND "
                                                                           "album.`Title` = :title AND "
                                                                           "album.`ArtistName` IS NULL");

        auto result = prepareQuery(d->mSelectAlbumIdFromTitleWithoutArtistQuery, selectAlbumIdFromTitleWithoutArtistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto insertAlbumQueryText = QStringLiteral("INSERT INTO `Albums` "
                                                   "(`ID`, "
                                                   "`Title`, "
                                                   "`ArtistName`, "
                                                   "`AlbumPath`, "
                                                   "`CoverFileName`) "
                                                   "VALUES "
                                                   "(:albumId, "
                                                   ":title, "
                                                   ":albumArtist, "
                                                   ":albumPath, "
                                                   ":coverFileName)");

        auto result = prepareQuery(d->mInsertAlbumQuery, insertAlbumQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertAlbumQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertAlbumQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto insertTrackMappingQueryText = QStringLiteral("INSERT INTO "
                                                          "`TracksMapping` "
                                                          "(`FileName`, "
                                                          "`DiscoverID`, "
                                                          "`Priority`, "
                                                          "`FileModifiedTime`) "
                                                          "VALUES (:fileName, :discoverId, :priority, :mtime)");

        auto result = prepareQuery(d->mInsertTrackMapping, insertTrackMappingQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertTrackMapping.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertTrackMapping.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto initialUpdateTracksValidityQueryText = QStringLiteral("UPDATE `TracksMapping` "
                                                                   "SET "
                                                                   "`TrackID` = :trackId, "
                                                                   "`Priority` = :priority, "
                                                                   "`FileModifiedTime` = :mtime "
                                                                   "WHERE `FileName` = :fileName");

        auto result = prepareQuery(d->mUpdateTrackMapping, initialUpdateTracksValidityQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateTrackMapping.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateTrackMapping.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto removeTracksMappingFromSourceQueryText = QStringLiteral("DELETE FROM `TracksMapping` "
                                                                     "WHERE `FileName` = :fileName AND `DiscoverID` = :sourceId");

        auto result = prepareQuery(d->mRemoveTracksMappingFromSource, removeTracksMappingFromSourceQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveTracksMappingFromSource.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveTracksMappingFromSource.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto removeTracksMappingQueryText = QStringLiteral("DELETE FROM `TracksMapping` "
                                                           "WHERE `FileName` = :fileName");

        auto result = prepareQuery(d->mRemoveTracksMapping, removeTracksMappingQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveTracksMapping.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveTracksMapping.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTracksWithoutMappingQueryText = QStringLiteral("SELECT "
                                                                  "tracks.`Id`, "
                                                                  "tracks.`Title`, "
                                                                  "album.`ID`, "
                                                                  "tracks.`ArtistName`, "
                                                                  "tracks.`AlbumArtistName`, "
                                                                  "\"\" as FileName, "
                                                                  "NULL as FileModifiedTime, "
                                                                  "tracks.`TrackNumber`, "
                                                                  "tracks.`DiscNumber`, "
                                                                  "tracks.`Duration`, "
                                                                  "tracks.`AlbumTitle`, "
                                                                  "tracks.`Rating`, "
                                                                  "album.`CoverFileName`, "
                                                                  "("
                                                                  "SELECT "
                                                                  "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                                  "FROM "
                                                                  "`Tracks` tracks2 "
                                                                  "WHERE "
                                                                  "tracks2.`AlbumTitle` = album.`Title` AND "
                                                                  "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                                  "(tracks2.`AlbumArtistName` IS NULL AND "
                                                                  "album.`ArtistName` IS NULL"
                                                                  ")"
                                                                  ") AND "
                                                                  "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                                  ") as `IsSingleDiscAlbum`, "
                                                                  "trackGenre.`Name`, "
                                                                  "trackComposer.`Name`, "
                                                                  "trackLyricist.`Name`, "
                                                                  "tracks.`Comment`, "
                                                                  "tracks.`Year`, "
                                                                  "tracks.`Channels`, "
                                                                  "tracks.`BitRate`, "
                                                                  "tracks.`SampleRate`, "
                                                                  "tracks.`HasEmbeddedCover`, "
                                                                  "tracks.`ImportDate`, "
                                                                  "tracks.`FirstPlayDate`, "
                                                                  "tracks.`LastPlayDate`, "
                                                                  "tracks.`PlayCounter`, "
                                                                  "tracks.`PlayCounter` / (strftime('%s', 'now') - tracks.`FirstPlayDate`) as PlayFrequency "
                                                                  "FROM "
                                                                  "`Tracks` tracks "
                                                                  "LEFT JOIN "
                                                                  "`Albums` album "
                                                                  "ON "
                                                                  "tracks.`AlbumTitle` = album.`Title` AND "
                                                                  "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                                  "tracks.`AlbumPath` = album.`AlbumPath` "
                                                                  "LEFT JOIN `Composer` trackComposer ON trackComposer.`Name` = tracks.`Composer` "
                                                                  "LEFT JOIN `Lyricist` trackLyricist ON trackLyricist.`Name` = tracks.`Lyricist` "
                                                                  "LEFT JOIN `Genre` trackGenre ON trackGenre.`Name` = tracks.`Genre` "
                                                                  "WHERE "
                                                                  "tracks.`ID` NOT IN (SELECT tracksMapping2.`TrackID` FROM `TracksMapping` tracksMapping2)");

        auto result = prepareQuery(d->mSelectTracksWithoutMappingQuery, selectTracksWithoutMappingQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksWithoutMappingQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksWithoutMappingQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTracksMappingQueryText = QStringLiteral("SELECT "
                                                           "`TrackID`, "
                                                           "`FileName`, "
                                                           "`DiscoverID`, "
                                                           "`Priority`, "
                                                           "`FileModifiedTime` "
                                                           "FROM "
                                                           "`TracksMapping` "
                                                           "WHERE "
                                                           "`FileName` = :fileName");

        auto result = prepareQuery(d->mSelectTracksMapping, selectTracksMappingQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksMapping.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksMapping.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTracksMappingPriorityQueryText = QStringLiteral("SELECT "
                                                                   "`Priority` "
                                                                   "FROM "
                                                                   "`TracksMapping` "
                                                                   "WHERE "
                                                                   "`TrackID` = :trackId AND "
                                                                   "`FileName` = :fileName");

        auto result = prepareQuery(d->mSelectTracksMappingPriority, selectTracksMappingPriorityQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksMappingPriority.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksMappingPriority.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTracksMappingPriorityQueryByTrackIdText = QStringLiteral("SELECT "
                                                                            "MAX(`Priority`) "
                                                                            "FROM "
                                                                            "`TracksMapping` "
                                                                            "WHERE "
                                                                            "`TrackID` = :trackId");

        auto result = prepareQuery(d->mSelectTracksMappingPriorityByTrackId, selectTracksMappingPriorityQueryByTrackIdText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksMappingPriorityByTrackId.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksMappingPriorityByTrackId.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllTrackFilesFromSourceQueryText = QStringLiteral("SELECT "
                                                                     "tracksMapping.`FileName`, "
                                                                     "tracksMapping.`FileModifiedTime` "
                                                                     "FROM "
                                                                     "`TracksMapping` tracksMapping "
                                                                     "WHERE "
                                                                     "tracksMapping.`DiscoverID` = :discoverId");

        auto result = prepareQuery(d->mSelectAllTrackFilesFromSourceQuery, selectAllTrackFilesFromSourceQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllTrackFilesFromSourceQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllTrackFilesFromSourceQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto insertMusicSourceQueryText = QStringLiteral("INSERT OR IGNORE INTO `DiscoverSource` (`ID`, `Name`) "
                                                         "VALUES (:discoverId, :name)");

        auto result = prepareQuery(d->mInsertMusicSource, insertMusicSourceQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertMusicSource.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertMusicSource.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectMusicSourceQueryText = QStringLiteral("SELECT `ID` FROM `DiscoverSource` WHERE `Name` = :name");

        auto result = prepareQuery(d->mSelectMusicSource, selectMusicSourceQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectMusicSource.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectMusicSource.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTrackQueryText = QStringLiteral("SELECT "
                                                   "tracks.`ID`,  tracksMapping.`FileName` "
                                                   "FROM "
                                                   "`Tracks` tracks, "
                                                   "`Albums` album, "
                                                   "`TracksMapping` tracksMapping "
                                                   "WHERE "
                                                   "tracks.`Title` = :title AND "
                                                   "album.`ID` = :album AND "
                                                   "(tracks.`AlbumTitle` = album.`Title` OR tracks.`AlbumTitle` IS NULL ) AND "
                                                   "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                   "(tracks.`AlbumPath` = album.`AlbumPath` OR tracks.`AlbumPath` IS NULL ) AND "
                                                   "tracks.`ArtistName` = :artist AND "
                                                   "tracksMapping.`TrackID` = tracks.`ID` AND "
                                                   "tracksMapping.`Priority` = (SELECT MIN(`Priority`) FROM `TracksMapping` WHERE `TrackID` = tracks.`ID`)");

        auto result = prepareQuery(d->mSelectTrackIdFromTitleAlbumIdArtistQuery, selectTrackQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackIdFromTitleAlbumIdArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackIdFromTitleAlbumIdArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto insertTrackQueryText = QStringLiteral("INSERT INTO `Tracks` "
                                                   "("
                                                   "`ID`, "
                                                   "`Title`, "
                                                   "`ArtistName`, "
                                                   "`AlbumTitle`, "
                                                   "`AlbumArtistName`, "
                                                   "`AlbumPath`, "
                                                   "`Genre`, "
                                                   "`Composer`, "
                                                   "`Lyricist`, "
                                                   "`Comment`, "
                                                   "`TrackNumber`, "
                                                   "`DiscNumber`, "
                                                   "`Channels`, "
                                                   "`BitRate`, "
                                                   "`SampleRate`, "
                                                   "`Year`,  "
                                                   "`Duration`, "
                                                   "`Rating`, "
                                                   "`HasEmbeddedCover`, "
                                                   "`ImportDate`, "
                                                   "`PlayCounter`) "
                                                   "VALUES "
                                                   "("
                                                   ":trackId, "
                                                   ":title, "
                                                   ":artistName, "
                                                   ":albumTitle, "
                                                   ":albumArtistName, "
                                                   ":albumPath, "
                                                   ":genre, "
                                                   ":composer, "
                                                   ":lyricist, "
                                                   ":comment, "
                                                   ":trackNumber, "
                                                   ":discNumber, "
                                                   ":channels, "
                                                   ":bitRate, "
                                                   ":sampleRate, "
                                                   ":year, "
                                                   ":trackDuration, "
                                                   ":trackRating, "
                                                   ":hasEmbeddedCover, "
                                                   ":importDate, "
                                                   "0)");

        auto result = prepareQuery(d->mInsertTrackQuery, insertTrackQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertTrackQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertTrackQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto updateTrackQueryText = QStringLiteral("UPDATE `Tracks` "
                                                   "SET "
                                                   "`Title` = :title, "
                                                   "`ArtistName` = :artistName, "
                                                   "`AlbumTitle` = :albumTitle, "
                                                   "`AlbumArtistName` = :albumArtistName, "
                                                   "`AlbumPath` = :albumPath, "
                                                   "`Genre` = :genre, "
                                                   "`Composer` = :composer, "
                                                   "`Lyricist` = :lyricist, "
                                                   "`Comment` = :comment, "
                                                   "`TrackNumber` = :trackNumber, "
                                                   "`DiscNumber` = :discNumber, "
                                                   "`Channels` = :channels, "
                                                   "`BitRate` = :bitRate, "
                                                   "`SampleRate` = :sampleRate, "
                                                   "`Year` = :year, "
                                                   " `Duration` = :trackDuration, "
                                                   "`Rating` = :trackRating "
                                                   "WHERE "
                                                   "`ID` = :trackId");

        auto result = prepareQuery(d->mUpdateTrackQuery, updateTrackQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateTrackQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateTrackQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto updateAlbumArtistQueryText = QStringLiteral("UPDATE `Albums` "
                                                         "SET "
                                                         "`ArtistName` = :artistName "
                                                         "WHERE "
                                                         "`ID` = :albumId");

        auto result = prepareQuery(d->mUpdateAlbumArtistQuery, updateAlbumArtistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateAlbumArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateAlbumArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto updateAlbumArtistInTracksQueryText = QStringLiteral("UPDATE `Tracks` "
                                                                 "SET "
                                                                 "`AlbumArtistName` = :artistName "
                                                                 "WHERE "
                                                                 "`AlbumTitle` = :albumTitle AND "
                                                                 "`AlbumPath` = :albumPath AND "
                                                                 "`AlbumArtistName` IS NULL");

        auto result = prepareQuery(d->mUpdateAlbumArtistInTracksQuery, updateAlbumArtistInTracksQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateAlbumArtistInTracksQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateAlbumArtistInTracksQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto queryMaximumTrackIdQueryText = QStringLiteral("SELECT MAX(tracks.`ID`)"
                                                           "FROM "
                                                           "`Tracks` tracks");

        auto result = prepareQuery(d->mQueryMaximumTrackIdQuery, queryMaximumTrackIdQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mQueryMaximumTrackIdQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mQueryMaximumTrackIdQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto queryMaximumAlbumIdQueryText = QStringLiteral("SELECT MAX(albums.`ID`)"
                                                           "FROM "
                                                           "`Albums` albums");

        auto result = prepareQuery(d->mQueryMaximumAlbumIdQuery, queryMaximumAlbumIdQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mQueryMaximumAlbumIdQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mQueryMaximumAlbumIdQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto queryMaximumArtistIdQueryText = QStringLiteral("SELECT MAX(artists.`ID`)"
                                                            "FROM "
                                                            "`Artists` artists");

        auto result = prepareQuery(d->mQueryMaximumArtistIdQuery, queryMaximumArtistIdQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mQueryMaximumArtistIdQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mQueryMaximumArtistIdQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto queryMaximumLyricistIdQueryText = QStringLiteral("SELECT MAX(lyricists.`ID`)"
                                                              "FROM "
                                                              "`Lyricist` lyricists");

        auto result = prepareQuery(d->mQueryMaximumLyricistIdQuery, queryMaximumLyricistIdQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mQueryMaximumLyricistIdQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mQueryMaximumLyricistIdQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto queryMaximumComposerIdQueryText = QStringLiteral("SELECT MAX(composers.`ID`)"
                                                              "FROM "
                                                              "`Composer` composers");

        auto result = prepareQuery(d->mQueryMaximumComposerIdQuery, queryMaximumComposerIdQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mQueryMaximumComposerIdQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mQueryMaximumComposerIdQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto queryMaximumGenreIdQueryText = QStringLiteral("SELECT MAX(genres.`ID`)"
                                                           "FROM "
                                                           "`Genre` genres");

        auto result = prepareQuery(d->mQueryMaximumGenreIdQuery, queryMaximumGenreIdQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mQueryMaximumGenreIdQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mQueryMaximumGenreIdQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTrackQueryText = QStringLiteral("SELECT "
                                                   "tracks.ID "
                                                   "FROM "
                                                   "`Tracks` tracks "
                                                   "WHERE "
                                                   "tracks.`Title` = :title AND "
                                                   "tracks.`AlbumTitle` = :album AND "
                                                   "tracks.`TrackNumber` = :trackNumber AND "
                                                   "tracks.`DiscNumber` = :discNumber AND "
                                                   "tracks.`ArtistName` = :artist");

        auto result = prepareQuery(d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery, selectTrackQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTrackQueryText = QStringLiteral("SELECT "
                                                   "tracks.ID "
                                                   "FROM "
                                                   "`Tracks` tracks, "
                                                   "`Albums` albums "
                                                   "WHERE "
                                                   "tracks.`Title` = :title AND "
                                                   "tracks.`ArtistName` = :trackArtist AND "
                                                   "tracks.`AlbumTitle` = :album AND "
                                                   "(tracks.`AlbumArtistName` = :albumArtist OR tracks.`AlbumArtistName` IS NULL) AND "
                                                   "tracks.`AlbumPath` = :albumPath AND "
                                                   "tracks.`TrackNumber` = :trackNumber");

        auto result = prepareQuery(d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery, selectTrackQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAlbumArtUriFromAlbumIdQueryText = QStringLiteral("SELECT `CoverFileName`"
                                                                    "FROM "
                                                                    "`Albums` "
                                                                    "WHERE "
                                                                    "`ID` = :albumId");

        auto result = prepareQuery(d->mSelectAlbumArtUriFromAlbumIdQuery, selectAlbumArtUriFromAlbumIdQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumArtUriFromAlbumIdQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumArtUriFromAlbumIdQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto updateAlbumArtUriFromAlbumIdQueryText = QStringLiteral("UPDATE `Albums` "
                                                                    "SET `CoverFileName` = :coverFileName "
                                                                    "WHERE "
                                                                    "`ID` = :albumId");

        auto result = prepareQuery(d->mUpdateAlbumArtUriFromAlbumIdQuery, updateAlbumArtUriFromAlbumIdQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateAlbumArtUriFromAlbumIdQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateAlbumArtUriFromAlbumIdQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTracksFromArtistQueryText = QStringLiteral("SELECT "
                                                              "tracks.`ID`, "
                                                              "tracks.`Title`, "
                                                              "album.`ID`, "
                                                              "tracks.`ArtistName`, "
                                                              "tracks.`AlbumArtistName`, "
                                                              "tracksMapping.`FileName`, "
                                                              "tracksMapping.`FileModifiedTime`, "
                                                              "tracks.`TrackNumber`, "
                                                              "tracks.`DiscNumber`, "
                                                              "tracks.`Duration`, "
                                                              "tracks.`AlbumTitle`, "
                                                              "tracks.`Rating`, "
                                                              "album.`CoverFileName`, "
                                                              "("
                                                              "SELECT "
                                                              "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                              "FROM "
                                                              "`Tracks` tracks2 "
                                                              "WHERE "
                                                              "tracks2.`AlbumTitle` = album.`Title` AND "
                                                              "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                              "(tracks2.`AlbumArtistName` IS NULL AND "
                                                              "album.`ArtistName` IS NULL"
                                                              ")"
                                                              ") AND "
                                                              "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                              ") as `IsSingleDiscAlbum`, "
                                                              "trackGenre.`Name`, "
                                                              "trackComposer.`Name`, "
                                                              "trackLyricist.`Name`, "
                                                              "tracks.`Comment`, "
                                                              "tracks.`Year`, "
                                                              "tracks.`Channels`, "
                                                              "tracks.`BitRate`, "
                                                              "tracks.`SampleRate`, "
                                                              "tracks.`HasEmbeddedCover`, "
                                                              "tracks.`ImportDate`, "
                                                              "tracks.`FirstPlayDate`, "
                                                              "tracks.`LastPlayDate`, "
                                                              "tracks.`PlayCounter`, "
                                                              "tracks.`PlayCounter` / (strftime('%s', 'now') - tracks.`FirstPlayDate`) as PlayFrequency "
                                                              "FROM "
                                                              "`Tracks` tracks, "
                                                              "`TracksMapping` tracksMapping "
                                                              "LEFT JOIN "
                                                              "`Albums` album "
                                                              "ON "
                                                              "tracks.`AlbumTitle` = album.`Title` AND "
                                                              "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                              "tracks.`AlbumPath` = album.`AlbumPath` "
                                                              "LEFT JOIN `Composer` trackComposer ON trackComposer.`Name` = tracks.`Composer` "
                                                              "LEFT JOIN `Lyricist` trackLyricist ON trackLyricist.`Name` = tracks.`Lyricist` "
                                                              "LEFT JOIN `Genre` trackGenre ON trackGenre.`Name` = tracks.`Genre` "
                                                              "WHERE "
                                                              "tracks.`ArtistName` = :artistName AND "
                                                              "tracksMapping.`TrackID` = tracks.`ID` AND "
                                                              "tracksMapping.`Priority` = (SELECT MIN(`Priority`) FROM `TracksMapping` WHERE `TrackID` = tracks.`ID`) "
                                                              "ORDER BY tracks.`Title` ASC, "
                                                              "album.`Title` ASC");

        auto result = prepareQuery(d->mSelectTracksFromArtist, selectTracksFromArtistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksFromArtist.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksFromArtist.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAlbumIdsFromArtistQueryText = QStringLiteral("SELECT "
                                                                "album.`ID` "
                                                                "FROM "
                                                                "`Albums` album "
                                                                "WHERE "
                                                                "album.`ArtistName` = :artistName");

        auto result = prepareQuery(d->mSelectAlbumIdsFromArtist, selectAlbumIdsFromArtistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdsFromArtist.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdsFromArtist.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectArtistQueryText = QStringLiteral("SELECT `ID`, "
                                                    "`Name` "
                                                    "FROM `Artists` "
                                                    "WHERE "
                                                    "`ID` = :artistId");

        auto result = prepareQuery(d->mSelectArtistQuery, selectArtistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto updateTrackStatisticsQueryText = QStringLiteral("UPDATE `Tracks` "
                                                             "SET "
                                                             "`LastPlayDate` = :playDate, "
                                                             "`PlayCounter` = `PlayCounter` + 1 "
                                                             "WHERE "
                                                             "`ID` = :trackId");

        auto result = prepareQuery(d->mUpdateTrackStatistics, updateTrackStatisticsQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateTrackStatistics.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateTrackStatistics.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto updateTrackFirstPlayStatisticsQueryText = QStringLiteral("UPDATE `Tracks` "
                                                                      "SET "
                                                                      "`FirstPlayDate` = :playDate "
                                                                      "WHERE "
                                                                      "`ID` = :trackId AND "
                                                                      "`FirstPlayDate` IS NULL");

        auto result = prepareQuery(d->mUpdateTrackFirstPlayStatistics, updateTrackFirstPlayStatisticsQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateTrackFirstPlayStatistics.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateTrackFirstPlayStatistics.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectGenreQueryText = QStringLiteral("SELECT `ID`, "
                                                   "`Name` "
                                                   "FROM `Genre` "
                                                   "WHERE "
                                                   "`ID` = :genreId");

        auto result = prepareQuery(d->mSelectGenreQuery, selectGenreQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectComposerQueryText = QStringLiteral("SELECT `ID`, "
                                                      "`Name` "
                                                      "FROM `Composer` "
                                                      "WHERE "
                                                      "`ID` = :composerId");

        auto result = prepareQuery(d->mSelectComposerQuery, selectComposerQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectComposerQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectComposerQuery.lastError();
        }
    }

    {
        auto selectLyricistQueryText = QStringLiteral("SELECT `ID`, "
                                                      "`Name` "
                                                      "FROM `Lyricist` "
                                                      "WHERE "
                                                      "`ID` = :lyricistId");

        auto result = prepareQuery(d->mSelectLyricistQuery, selectLyricistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectLyricistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectLyricistQuery.lastError();
        }
    }

    {
        auto removeTrackQueryText = QStringLiteral("DELETE FROM `Tracks` "
                                                   "WHERE "
                                                   "`ID` = :trackId");

        auto result = prepareQuery(d->mRemoveTrackQuery, removeTrackQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveTrackQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveTrackQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto removeAlbumQueryText = QStringLiteral("DELETE FROM `Albums` "
                                                   "WHERE "
                                                   "`ID` = :albumId");

        auto result = prepareQuery(d->mRemoveAlbumQuery, removeAlbumQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveAlbumQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveAlbumQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto removeAlbumQueryText = QStringLiteral("DELETE FROM `Artists` "
                                                   "WHERE "
                                                   "`ID` = :artistId");

        auto result = prepareQuery(d->mRemoveArtistQuery, removeAlbumQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    finishTransaction();

    d->mInitFinished = true;
    Q_EMIT requestsInitDone();
}

qulonglong DatabaseInterface::insertAlbum(const QString &title, const QString &albumArtist, const QString &trackArtist,
                                          const QString &trackPath, const QUrl &albumArtURI)
{
    auto result = qulonglong(0);

    if (title.isEmpty()) {
        return result;
    }

    if (!albumArtist.isEmpty() || !trackArtist.isEmpty()) {
        d->mSelectAlbumIdFromTitleAndArtistQuery.bindValue(QStringLiteral(":title"), title);
        d->mSelectAlbumIdFromTitleAndArtistQuery.bindValue(QStringLiteral(":albumPath"), trackPath);
        if (!albumArtist.isEmpty()) {
            d->mSelectAlbumIdFromTitleAndArtistQuery.bindValue(QStringLiteral(":artistName"), albumArtist);
        } else {
            d->mSelectAlbumIdFromTitleAndArtistQuery.bindValue(QStringLiteral(":artistName"), trackArtist);
        }

        auto queryResult = d->mSelectAlbumIdFromTitleAndArtistQuery.exec();

        if (!queryResult || !d->mSelectAlbumIdFromTitleAndArtistQuery.isSelect() || !d->mSelectAlbumIdFromTitleAndArtistQuery.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::insertAlbum" << d->mSelectAlbumIdFromTitleAndArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::insertAlbum" << d->mSelectAlbumIdFromTitleAndArtistQuery.boundValues();
            qDebug() << "DatabaseInterface::insertAlbum" << d->mSelectAlbumIdFromTitleAndArtistQuery.lastError();

            d->mSelectAlbumIdFromTitleAndArtistQuery.finish();

            return result;
        }

        if (d->mSelectAlbumIdFromTitleAndArtistQuery.next()) {
            result = d->mSelectAlbumIdFromTitleAndArtistQuery.record().value(0).toULongLong();

            d->mSelectAlbumIdFromTitleAndArtistQuery.finish();

            return result;
        }

        d->mSelectAlbumIdFromTitleAndArtistQuery.finish();
    }

    if (result == 0) {
        d->mSelectAlbumIdFromTitleWithoutArtistQuery.bindValue(QStringLiteral(":title"), title);
        d->mSelectAlbumIdFromTitleWithoutArtistQuery.bindValue(QStringLiteral(":albumPath"), trackPath);

        auto queryResult = d->mSelectAlbumIdFromTitleWithoutArtistQuery.exec();

        if (!queryResult || !d->mSelectAlbumIdFromTitleWithoutArtistQuery.isSelect() || !d->mSelectAlbumIdFromTitleWithoutArtistQuery.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::insertAlbum" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::insertAlbum" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.boundValues();
            qDebug() << "DatabaseInterface::insertAlbum" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.lastError();

            d->mSelectAlbumIdFromTitleWithoutArtistQuery.finish();

            return result;
        }

        if (d->mSelectAlbumIdFromTitleWithoutArtistQuery.next()) {
            result = d->mSelectAlbumIdFromTitleWithoutArtistQuery.record().value(0).toULongLong();

            d->mSelectAlbumIdFromTitleWithoutArtistQuery.finish();

            return result;
        }

        d->mSelectAlbumIdFromTitleWithoutArtistQuery.finish();
    }

    d->mInsertAlbumQuery.bindValue(QStringLiteral(":albumId"), d->mAlbumId);
    d->mInsertAlbumQuery.bindValue(QStringLiteral(":title"), title);
    if (!albumArtist.isEmpty()) {
        insertArtist(albumArtist);
        d->mInsertAlbumQuery.bindValue(QStringLiteral(":albumArtist"), albumArtist);
    } else {
        d->mInsertAlbumQuery.bindValue(QStringLiteral(":albumArtist"), {});
    }
    d->mInsertAlbumQuery.bindValue(QStringLiteral(":albumPath"), trackPath);
    d->mInsertAlbumQuery.bindValue(QStringLiteral(":coverFileName"), albumArtURI);

    auto queryResult = d->mInsertAlbumQuery.exec();

    if (!queryResult || !d->mInsertAlbumQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertAlbum" << d->mInsertAlbumQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertAlbum" << d->mInsertAlbumQuery.boundValues();
        qDebug() << "DatabaseInterface::insertAlbum" << d->mInsertAlbumQuery.lastError();

        d->mInsertAlbumQuery.finish();

        return result;
    }

    result = d->mAlbumId;

    d->mInsertAlbumQuery.finish();

    ++d->mAlbumId;

    d->mInsertedAlbums.insert(result);

    return result;
}

bool DatabaseInterface::updateAlbumFromId(qulonglong albumId, const QUrl &albumArtUri,
                                          const MusicAudioTrack &currentTrack, const QString &albumPath)
{
    auto modifiedAlbum = false;
    modifiedAlbum = true;

    if (!albumArtUri.isValid()) {
        return modifiedAlbum;
    }

    auto storedAlbumArtUri = internalAlbumArtUriFromAlbumId(albumId);

    if (!storedAlbumArtUri.isValid() || storedAlbumArtUri != albumArtUri) {
        d->mUpdateAlbumArtUriFromAlbumIdQuery.bindValue(QStringLiteral(":albumId"), albumId);
        d->mUpdateAlbumArtUriFromAlbumIdQuery.bindValue(QStringLiteral(":coverFileName"), albumArtUri);

        auto result = d->mUpdateAlbumArtUriFromAlbumIdQuery.exec();

        if (!result || !d->mUpdateAlbumArtUriFromAlbumIdQuery.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::updateAlbumFromId" << d->mUpdateAlbumArtUriFromAlbumIdQuery.lastQuery();
            qDebug() << "DatabaseInterface::updateAlbumFromId" << d->mUpdateAlbumArtUriFromAlbumIdQuery.boundValues();
            qDebug() << "DatabaseInterface::updateAlbumFromId" << d->mUpdateAlbumArtUriFromAlbumIdQuery.lastError();

            d->mUpdateAlbumArtUriFromAlbumIdQuery.finish();

            return modifiedAlbum;
        }

        d->mUpdateAlbumArtUriFromAlbumIdQuery.finish();

        modifiedAlbum = true;
    }

    if (!isValidArtist(albumId) && currentTrack.isValidAlbumArtist()) {
        updateAlbumArtist(albumId, currentTrack.albumName(), albumPath, currentTrack.albumArtist());

        modifiedAlbum = true;
    }

    return modifiedAlbum;
}

qulonglong DatabaseInterface::insertArtist(const QString &name)
{
    auto result = qulonglong(0);

    if (name.isEmpty()) {
        return result;
    }

    d->mSelectArtistByNameQuery.bindValue(QStringLiteral(":name"), name);

    auto queryResult = d->mSelectArtistByNameQuery.exec();

    if (!queryResult || !d->mSelectArtistByNameQuery.isSelect() || !d->mSelectArtistByNameQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectArtistByNameQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectArtistByNameQuery.boundValues();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectArtistByNameQuery.lastError();

        d->mSelectArtistByNameQuery.finish();

        return result;
    }

    if (d->mSelectArtistByNameQuery.next()) {
        result = d->mSelectArtistByNameQuery.record().value(0).toULongLong();

        d->mSelectArtistByNameQuery.finish();

        return result;
    }

    d->mSelectArtistByNameQuery.finish();

    d->mInsertArtistsQuery.bindValue(QStringLiteral(":artistId"), d->mArtistId);
    d->mInsertArtistsQuery.bindValue(QStringLiteral(":name"), name);

    queryResult = d->mInsertArtistsQuery.exec();

    if (!queryResult || !d->mInsertArtistsQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertArtist" << d->mInsertArtistsQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertArtist" << d->mInsertArtistsQuery.boundValues();
        qDebug() << "DatabaseInterface::insertArtist" << d->mInsertArtistsQuery.lastError();

        d->mInsertArtistsQuery.finish();

        return result;
    }

    result = d->mArtistId;

    ++d->mArtistId;

    d->mInsertedArtists.insert(result);

    d->mInsertArtistsQuery.finish();

    return result;
}

qulonglong DatabaseInterface::insertComposer(const QString &name)
{
    auto result = qulonglong(0);

    if (name.isEmpty()) {
        return result;
    }

    d->mSelectComposerByNameQuery.bindValue(QStringLiteral(":name"), name);

    auto queryResult = d->mSelectComposerByNameQuery.exec();

    if (!queryResult || !d->mSelectComposerByNameQuery.isSelect() || !d->mSelectComposerByNameQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertComposer" << d->mSelectComposerByNameQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertComposer" << d->mSelectComposerByNameQuery.boundValues();
        qDebug() << "DatabaseInterface::insertComposer" << d->mSelectComposerByNameQuery.lastError();

        d->mSelectComposerByNameQuery.finish();

        return result;
    }


    if (d->mSelectComposerByNameQuery.next()) {
        result = d->mSelectComposerByNameQuery.record().value(0).toULongLong();

        d->mSelectComposerByNameQuery.finish();

        return result;
    }

    d->mSelectComposerByNameQuery.finish();

    d->mInsertComposerQuery.bindValue(QStringLiteral(":composerId"), d->mComposerId);
    d->mInsertComposerQuery.bindValue(QStringLiteral(":name"), name);

    queryResult = d->mInsertComposerQuery.exec();

    if (!queryResult || !d->mInsertComposerQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertComposer" << d->mInsertComposerQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertComposer" << d->mInsertComposerQuery.boundValues();
        qDebug() << "DatabaseInterface::insertComposer" << d->mInsertComposerQuery.lastError();

        d->mInsertComposerQuery.finish();

        return result;
    }

    result = d->mComposerId;

    ++d->mComposerId;

    d->mInsertComposerQuery.finish();

    Q_EMIT composersAdded(internalAllComposersPartialData());

    return result;
}

qulonglong DatabaseInterface::insertGenre(const QString &name)
{
    auto result = qulonglong(0);

    if (name.isEmpty()) {
        return result;
    }

    d->mSelectGenreByNameQuery.bindValue(QStringLiteral(":name"), name);

    auto queryResult = d->mSelectGenreByNameQuery.exec();

    if (!queryResult || !d->mSelectGenreByNameQuery.isSelect() || !d->mSelectGenreByNameQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertGenre" << d->mSelectGenreByNameQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertGenre" << d->mSelectGenreByNameQuery.boundValues();
        qDebug() << "DatabaseInterface::insertGenre" << d->mSelectGenreByNameQuery.lastError();

        d->mSelectGenreByNameQuery.finish();

        return result;
    }

    if (d->mSelectGenreByNameQuery.next()) {
        result = d->mSelectGenreByNameQuery.record().value(0).toULongLong();

        d->mSelectGenreByNameQuery.finish();

        return result;
    }

    d->mSelectGenreByNameQuery.finish();

    d->mInsertGenreQuery.bindValue(QStringLiteral(":genreId"), d->mGenreId);
    d->mInsertGenreQuery.bindValue(QStringLiteral(":name"), name);

    queryResult = d->mInsertGenreQuery.exec();

    if (!queryResult || !d->mInsertGenreQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertGenre" << d->mInsertGenreQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertGenre" << d->mInsertGenreQuery.boundValues();
        qDebug() << "DatabaseInterface::insertGenre" << d->mInsertGenreQuery.lastError();

        d->mInsertGenreQuery.finish();

        return result;
    }

    result = d->mGenreId;

    ++d->mGenreId;

    d->mInsertGenreQuery.finish();

    Q_EMIT genresAdded({{{DatabaseIdRole, result}}});

    return result;
}

void DatabaseInterface::insertTrackOrigin(const QUrl &fileNameURI, const QDateTime &fileModifiedTime,
                                          qulonglong discoverId)
{
    d->mInsertTrackMapping.bindValue(QStringLiteral(":discoverId"), discoverId);
    d->mInsertTrackMapping.bindValue(QStringLiteral(":fileName"), fileNameURI);
    d->mInsertTrackMapping.bindValue(QStringLiteral(":priority"), 1);
    d->mInsertTrackMapping.bindValue(QStringLiteral(":mtime"), fileModifiedTime);

    auto queryResult = d->mInsertTrackMapping.exec();

    if (!queryResult || !d->mInsertTrackMapping.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertArtist" << d->mInsertTrackMapping.lastQuery();
        qDebug() << "DatabaseInterface::insertArtist" << d->mInsertTrackMapping.boundValues();
        qDebug() << "DatabaseInterface::insertArtist" << d->mInsertTrackMapping.lastError();

        d->mInsertTrackMapping.finish();

        return;
    }

    d->mInsertTrackMapping.finish();
}

void DatabaseInterface::updateTrackOrigin(qulonglong trackId, const QUrl &fileName, const QDateTime &fileModifiedTime)
{
    d->mUpdateTrackMapping.bindValue(QStringLiteral(":trackId"), trackId);
    d->mUpdateTrackMapping.bindValue(QStringLiteral(":fileName"), fileName);
    d->mUpdateTrackMapping.bindValue(QStringLiteral(":priority"), computeTrackPriority(trackId, fileName));
    d->mUpdateTrackMapping.bindValue(QStringLiteral(":mtime"), fileModifiedTime);

    auto queryResult = d->mUpdateTrackMapping.exec();

    if (!queryResult || !d->mUpdateTrackMapping.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::updateTrackOrigin" << d->mUpdateTrackMapping.lastQuery();
        qDebug() << "DatabaseInterface::updateTrackOrigin" << d->mUpdateTrackMapping.boundValues();
        qDebug() << "DatabaseInterface::updateTrackOrigin" << d->mUpdateTrackMapping.lastError();

        d->mUpdateTrackMapping.finish();

        return;
    }

    d->mInsertTrackMapping.finish();
}

int DatabaseInterface::computeTrackPriority(qulonglong trackId, const QUrl &fileName)
{
    auto result = int(1);

    if (!d) {
        return result;
    }

    d->mSelectTracksMappingPriority.bindValue(QStringLiteral(":trackId"), trackId);
    d->mSelectTracksMappingPriority.bindValue(QStringLiteral(":fileName"), fileName);

    auto queryResult = d->mSelectTracksMappingPriority.exec();

    if (!queryResult || !d->mSelectTracksMappingPriority.isSelect() || !d->mSelectTracksMappingPriority.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMappingPriority.lastQuery();
        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMappingPriority.boundValues();
        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMappingPriority.lastError();

        d->mSelectTracksMappingPriority.finish();

        return result;
    }

    if (d->mSelectTracksMappingPriority.next()) {
        result = d->mSelectTracksMappingPriority.record().value(0).toInt();

        d->mSelectTracksMappingPriority.finish();

        return result;
    }

    d->mSelectTracksMappingPriority.finish();

    d->mSelectTracksMappingPriorityByTrackId.bindValue(QStringLiteral(":trackId"), trackId);

    queryResult = d->mSelectTracksMappingPriorityByTrackId.exec();

    if (!queryResult || !d->mSelectTracksMappingPriorityByTrackId.isSelect() || !d->mSelectTracksMappingPriorityByTrackId.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMappingPriorityByTrackId.lastQuery();
        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMappingPriorityByTrackId.boundValues();
        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMappingPriorityByTrackId.lastError();

        d->mSelectTracksMappingPriorityByTrackId.finish();

        return result;
    }

    if (d->mSelectTracksMappingPriorityByTrackId.next()) {
        result = d->mSelectTracksMappingPriorityByTrackId.record().value(0).toInt() + 1;
    }

    d->mSelectTracksMappingPriorityByTrackId.finish();

    return result;
}

qulonglong DatabaseInterface::internalInsertTrack(const MusicAudioTrack &oneTrack, const QHash<QString, QUrl> &covers,
                                                  qulonglong originTrackId, TrackFileInsertType insertType)
{
    qulonglong resultId = 0;

    if (oneTrack.title().isEmpty()) {
        return resultId;
    }

    auto oldTrack = oneTrack;

    QUrl::FormattingOptions currentOptions = QUrl::PreferLocalFile |
            QUrl::RemoveAuthority | QUrl::RemoveFilename | QUrl::RemoveFragment |
            QUrl::RemovePassword | QUrl::RemovePort | QUrl::RemoveQuery |
            QUrl::RemoveScheme | QUrl::RemoveUserInfo;

    const auto &trackPath = oneTrack.resourceURI().toString(currentOptions);

    auto albumId = insertAlbum(oneTrack.albumName(), (oneTrack.isValidAlbumArtist() ? oneTrack.albumArtist() : QString()),
                               oneTrack.artist(), trackPath, covers[oneTrack.resourceURI().toString()]);

    auto otherTrackId = getDuplicateTrackIdFromTitleAlbumTrackDiscNumber(oneTrack.title(), oneTrack.artist(), oneTrack.albumName(),
                                                                         oneTrack.albumArtist(), trackPath, oneTrack.trackNumber(),
                                                                         oneTrack.discNumber());
    bool isModifiedTrack = (otherTrackId != 0) || (insertType == TrackFileInsertType::ModifiedTrackFileInsert);
    bool isSameTrack = false;

    qulonglong oldAlbumId = 0;

    if (isModifiedTrack) {
        if (otherTrackId == 0) {
            otherTrackId = internalTrackIdFromFileName(oneTrack.resourceURI());
        }

        originTrackId = otherTrackId;

        oldTrack = internalTrackFromDatabaseId(originTrackId);

        isSameTrack = (oldTrack.title() == oneTrack.title());
        isSameTrack = isSameTrack && (oldTrack.albumName() == oneTrack.albumName());
        isSameTrack = isSameTrack && (oldTrack.artist() == oneTrack.artist());
        isSameTrack = isSameTrack && (oldTrack.albumArtist() == oneTrack.albumArtist());
        isSameTrack = isSameTrack && (oldTrack.trackNumber() == oneTrack.trackNumber());
        isSameTrack = isSameTrack && (oldTrack.discNumber() == oneTrack.discNumber());
        isSameTrack = isSameTrack && (oldTrack.duration() == oneTrack.duration());
        isSameTrack = isSameTrack && (oldTrack.rating() == oneTrack.rating());
        isSameTrack = isSameTrack && (oldTrack.resourceURI() == oneTrack.resourceURI());
        isSameTrack = isSameTrack && (oldTrack.genre() == oneTrack.genre());
        isSameTrack = isSameTrack && (oldTrack.composer() == oneTrack.composer());
        isSameTrack = isSameTrack && (oldTrack.lyricist() == oneTrack.lyricist());
        isSameTrack = isSameTrack && (oldTrack.comment() == oneTrack.comment());
        isSameTrack = isSameTrack && (oldTrack.year() == oneTrack.year());
        isSameTrack = isSameTrack && (oldTrack.channels() == oneTrack.channels());
        isSameTrack = isSameTrack && (oldTrack.bitRate() == oneTrack.bitRate());
        isSameTrack = isSameTrack && (oldTrack.sampleRate() == oneTrack.sampleRate());

        oldAlbumId = internalAlbumIdFromTitleAndArtist(oldTrack.albumName(), oldTrack.albumArtist());

        if (!isSameTrack) {
            auto newTrack = oneTrack;
            newTrack.setDatabaseId(oldTrack.databaseId());
            updateTrackInDatabase(newTrack, trackPath);
            updateTrackOrigin(newTrack.databaseId(), oneTrack.resourceURI(), oneTrack.fileModificationTime());
            updateAlbumFromId(albumId, oneTrack.albumCover(), oneTrack, trackPath);

            recordModifiedTrack(originTrackId);
            if (albumId != 0) {
                recordModifiedAlbum(albumId);
            }
            if (oldAlbumId != 0) {
                auto tracksCount = fetchTrackIds(oldAlbumId).count();

                if (tracksCount) {
                    recordModifiedAlbum(oldAlbumId);
                } else {
                    removeAlbumInDatabase(oldAlbumId);
                    Q_EMIT albumRemoved(oldAlbumId);
                }
            }

            isSameTrack = true;
        }
    } else {
        originTrackId = d->mTrackId;
    }

    resultId = originTrackId;

    const auto &albumData = internalOneAlbumPartialData(albumId);

    if (!isSameTrack) {
        d->mInsertTrackQuery.bindValue(QStringLiteral(":trackId"), originTrackId);
        d->mInsertTrackQuery.bindValue(QStringLiteral(":title"), oneTrack.title());
        insertArtist(oneTrack.artist());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":artistName"), oneTrack.artist());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":albumTitle"), albumData[AlbumDataType::key_type::TitleRole]);
        d->mInsertTrackQuery.bindValue(QStringLiteral(":albumArtistName"), albumData[AlbumDataType::key_type::ArtistRole]);
        d->mInsertTrackQuery.bindValue(QStringLiteral(":albumPath"), trackPath);
        d->mInsertTrackQuery.bindValue(QStringLiteral(":trackNumber"), oneTrack.trackNumber());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":discNumber"), oneTrack.discNumber());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":trackDuration"), QVariant::fromValue<qlonglong>(oneTrack.duration().msecsSinceStartOfDay()));
        d->mInsertTrackQuery.bindValue(QStringLiteral(":trackRating"), oneTrack.rating());
        if (insertGenre(oneTrack.genre()) != 0) {
            d->mInsertTrackQuery.bindValue(QStringLiteral(":genre"), oneTrack.genre());
        } else {
            d->mInsertTrackQuery.bindValue(QStringLiteral(":genre"), {});
        }
        if (insertComposer(oneTrack.composer()) != 0) {
            d->mInsertTrackQuery.bindValue(QStringLiteral(":composer"), oneTrack.composer());
        } else {
            d->mInsertTrackQuery.bindValue(QStringLiteral(":composer"), {});
        }
        if (insertLyricist(oneTrack.lyricist()) != 0) {
            d->mInsertTrackQuery.bindValue(QStringLiteral(":lyricist"), oneTrack.lyricist());
        } else {
            d->mInsertTrackQuery.bindValue(QStringLiteral(":lyricist"), {});
        }
        d->mInsertTrackQuery.bindValue(QStringLiteral(":comment"), oneTrack.comment());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":year"), oneTrack.year());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":channels"), oneTrack.channels());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":bitRate"), oneTrack.bitRate());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":sampleRate"), oneTrack.sampleRate());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":hasEmbeddedCover"), oneTrack.hasEmbeddedCover());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":importDate"), QDateTime::currentDateTime().toMSecsSinceEpoch());

        auto result = d->mInsertTrackQuery.exec();

        if (result && d->mInsertTrackQuery.isActive()) {
            d->mInsertTrackQuery.finish();

            if (!isModifiedTrack) {
                ++d->mTrackId;
            }

            updateTrackOrigin(originTrackId, oneTrack.resourceURI(), oneTrack.fileModificationTime());

            if (isModifiedTrack) {
                recordModifiedTrack(originTrackId);
                if (albumId != 0) {
                    recordModifiedAlbum(albumId);
                }
                if (oldAlbumId != 0) {
                    recordModifiedAlbum(oldAlbumId);
                }
            }

            if (albumId != 0) {
                if (updateAlbumFromId(albumId, covers[oneTrack.resourceURI().toString()], oneTrack, trackPath)) {
                    auto modifiedTracks = fetchTrackIds(albumId);
                    for (auto oneModifiedTrack : modifiedTracks) {
                        if (oneModifiedTrack != resultId) {
                            recordModifiedTrack(oneModifiedTrack);
                        }
                    }
                }
                recordModifiedAlbum(albumId);
            }
        } else {
            d->mInsertTrackQuery.finish();

            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::internalInsertTrack" << oneTrack << oneTrack.resourceURI();
            qDebug() << "DatabaseInterface::internalInsertTrack" << d->mInsertTrackQuery.lastQuery();
            qDebug() << "DatabaseInterface::internalInsertTrack" << d->mInsertTrackQuery.boundValues();
            qDebug() << "DatabaseInterface::internalInsertTrack" << d->mInsertTrackQuery.lastError();
        }
    }

    return resultId;
}

MusicAudioTrack DatabaseInterface::buildTrackFromDatabaseRecord(const QSqlRecord &trackRecord) const
{
    auto id = trackRecord.value(0).toULongLong();

    auto result = MusicAudioTrack{};

    if (result.isValid()) {
        return result;
    }

    result.setDatabaseId(id);
    result.setTitle(trackRecord.value(1).toString());
    result.setParentId(trackRecord.value(2).toString());
    result.setArtist(trackRecord.value(3).toString());

    if (trackRecord.value(4).isValid()) {
        result.setAlbumArtist(trackRecord.value(4).toString());
    }

    result.setResourceURI(trackRecord.value(5).toUrl());
    result.setFileModificationTime(trackRecord.value(6).toDateTime());
    result.setTrackNumber(trackRecord.value(7).toInt());
    result.setDiscNumber(trackRecord.value(8).toInt());
    result.setDuration(QTime::fromMSecsSinceStartOfDay(trackRecord.value(9).toInt()));
    result.setAlbumName(trackRecord.value(10).toString());
    result.setRating(trackRecord.value(11).toInt());
    result.setAlbumCover(trackRecord.value(12).toUrl());
    result.setIsSingleDiscAlbum(trackRecord.value(13).toBool());
    result.setGenre(trackRecord.value(14).toString());
    result.setComposer(trackRecord.value(15).toString());
    result.setLyricist(trackRecord.value(16).toString());
    result.setComment(trackRecord.value(17).toString());
    result.setYear(trackRecord.value(18).toInt());
    result.setChannels(trackRecord.value(19).toInt());
    result.setBitRate(trackRecord.value(20).toInt());
    result.setSampleRate(trackRecord.value(21).toInt());
    result.setAlbumId(trackRecord.value(2).toULongLong());
    result.setHasEmbeddedCover(trackRecord.value(22).toBool());

    result.setValid(true);

    return result;
}

DatabaseInterface::TrackDataType DatabaseInterface::buildTrackDataFromDatabaseRecord(const QSqlRecord &trackRecord) const
{
    TrackDataType result;

    result[TrackDataType::key_type::DatabaseIdRole] = trackRecord.value(0);
    result[TrackDataType::key_type::TitleRole] = trackRecord.value(1);
    result[TrackDataType::key_type::AlbumRole] = trackRecord.value(10);
    result[TrackDataType::key_type::AlbumIdRole] = trackRecord.value(2);
    result[TrackDataType::key_type::ArtistRole] = trackRecord.value(3);
    result[TrackDataType::key_type::AlbumArtistRole] = trackRecord.value(4);
    result[TrackDataType::key_type::ResourceRole] = trackRecord.value(5);
    result[TrackDataType::key_type::TrackNumberRole] = trackRecord.value(7);
    result[TrackDataType::key_type::DiscNumberRole] = trackRecord.value(8);
    result[TrackDataType::key_type::DurationRole] = QTime::fromMSecsSinceStartOfDay(trackRecord.value(9).toInt());
    result[TrackDataType::key_type::MilliSecondsDurationRole] = trackRecord.value(9).toInt();
    result[TrackDataType::key_type::RatingRole] = trackRecord.value(11);
    result[TrackDataType::key_type::ImageUrlRole] = QUrl(trackRecord.value(12).toString());
    result[TrackDataType::key_type::IsSingleDiscAlbumRole] = trackRecord.value(13);
    result[TrackDataType::key_type::GenreRole] = trackRecord.value(14);
    result[TrackDataType::key_type::ComposerRole] = trackRecord.value(15);
    result[TrackDataType::key_type::LyricistRole] = trackRecord.value(16);
    result[TrackDataType::key_type::HasEmbeddedCover] = trackRecord.value(22);
    result[TrackDataType::key_type::FileModificationTime] = trackRecord.value(6);
    result[TrackDataType::key_type::FirstPlayDate] = trackRecord.value(24);
    result[TrackDataType::key_type::LastPlayDate] = trackRecord.value(25);
    result[TrackDataType::key_type::PlayCounter] = trackRecord.value(26);
    result[TrackDataType::key_type::PlayFrequency] = trackRecord.value(27);
    result[DataType::key_type::ElementTypeRole] = ElisaUtils::Track;

    return result;
}

void DatabaseInterface::internalRemoveTracksList(const QList<QUrl> &removedTracks)
{
    for (const auto &removedTrackFileName : removedTracks) {
        d->mRemoveTracksMapping.bindValue(QStringLiteral(":fileName"), removedTrackFileName.toString());

        auto result = d->mRemoveTracksMapping.exec();

        if (!result || !d->mRemoveTracksMapping.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::internalRemoveTracksList" << d->mRemoveTracksMapping.lastQuery();
            qDebug() << "DatabaseInterface::internalRemoveTracksList" << d->mRemoveTracksMapping.boundValues();
            qDebug() << "DatabaseInterface::internalRemoveTracksList" << d->mRemoveTracksMapping.lastError();

            continue;
        }

        d->mRemoveTracksMapping.finish();
    }

    internalRemoveTracksWithoutMapping();
}

void DatabaseInterface::internalRemoveTracksList(const QHash<QUrl, QDateTime> &removedTracks, qulonglong sourceId)
{
    for (auto itRemovedTrack = removedTracks.begin(); itRemovedTrack != removedTracks.end(); ++itRemovedTrack) {
        d->mRemoveTracksMappingFromSource.bindValue(QStringLiteral(":fileName"), itRemovedTrack.key().toString());
        d->mRemoveTracksMappingFromSource.bindValue(QStringLiteral(":sourceId"), sourceId);

        auto result = d->mRemoveTracksMappingFromSource.exec();

        if (!result || !d->mRemoveTracksMappingFromSource.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::removeTracksList" << d->mRemoveTracksMappingFromSource.lastQuery();
            qDebug() << "DatabaseInterface::removeTracksList" << d->mRemoveTracksMappingFromSource.boundValues();
            qDebug() << "DatabaseInterface::removeTracksList" << d->mRemoveTracksMappingFromSource.lastError();

            continue;
        }

        d->mRemoveTracksMappingFromSource.finish();
    }

    internalRemoveTracksWithoutMapping();
}

void DatabaseInterface::internalRemoveTracksWithoutMapping()
{
    auto queryResult = d->mSelectTracksWithoutMappingQuery.exec();

    if (!queryResult || !d->mSelectTracksWithoutMappingQuery.isSelect() || !d->mSelectTracksWithoutMappingQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectTracksWithoutMappingQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectTracksWithoutMappingQuery.boundValues();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectTracksWithoutMappingQuery.lastError();

        d->mSelectTracksWithoutMappingQuery.finish();

        return;
    }

    QList<MusicAudioTrack> willRemoveTrack;

    while (d->mSelectTracksWithoutMappingQuery.next()) {
        const auto &currentRecord = d->mSelectTracksWithoutMappingQuery.record();

        willRemoveTrack.push_back(buildTrackFromDatabaseRecord(currentRecord));
    }

    d->mSelectTracksWithoutMappingQuery.finish();

    QSet<qulonglong> modifiedAlbums;

    QUrl::FormattingOptions currentOptions = QUrl::PreferLocalFile |
            QUrl::RemoveAuthority | QUrl::RemoveFilename | QUrl::RemoveFragment |
            QUrl::RemovePassword | QUrl::RemovePort | QUrl::RemoveQuery |
            QUrl::RemoveScheme | QUrl::RemoveUserInfo;

    for (const auto &oneRemovedTrack : willRemoveTrack) {
        removeTrackInDatabase(oneRemovedTrack.databaseId());

        Q_EMIT trackRemoved(oneRemovedTrack.databaseId());

        const auto &modifiedAlbumId = internalAlbumIdFromTitleAndArtist(oneRemovedTrack.albumName(), oneRemovedTrack.albumArtist());
        const auto &allTracksFromArtist = internalTracksFromAuthor(oneRemovedTrack.artist());
        const auto &allAlbumsFromArtist = internalAlbumIdsFromAuthor(oneRemovedTrack.artist());
        const auto &removedArtistId = internalArtistIdFromName(oneRemovedTrack.artist());
        const auto &trackPath = oneRemovedTrack.resourceURI().toString(currentOptions);

        recordModifiedAlbum(modifiedAlbumId);
        modifiedAlbums.insert(modifiedAlbumId);
        updateAlbumFromId(modifiedAlbumId, oneRemovedTrack.albumCover(), oneRemovedTrack, trackPath);

        if (removedArtistId != 0 && allTracksFromArtist.isEmpty() && allAlbumsFromArtist.isEmpty()) {
            removeArtistInDatabase(removedArtistId);
            Q_EMIT artistRemoved(removedArtistId);
        }
    }

    for (auto modifiedAlbumId : modifiedAlbums) {
        const auto &modifiedAlbumData = internalOneAlbumPartialData(modifiedAlbumId);

        auto tracksCount = fetchTrackIds(modifiedAlbumId).count();

        if (!modifiedAlbumData.isEmpty() && tracksCount) {
            Q_EMIT albumModified({{DatabaseIdRole, modifiedAlbumId}}, modifiedAlbumId);
        } else {
            removeAlbumInDatabase(modifiedAlbumId);
            Q_EMIT albumRemoved(modifiedAlbumId);

            const auto &allTracksFromArtist = internalTracksFromAuthor(modifiedAlbumData[AlbumDataType::key_type::ArtistRole].toString());
            const auto &allAlbumsFromArtist = internalAlbumIdsFromAuthor(modifiedAlbumData[AlbumDataType::key_type::ArtistRole].toString());
            const auto &removedArtistId = internalArtistIdFromName(modifiedAlbumData[AlbumDataType::key_type::ArtistRole].toString());

            if (removedArtistId != 0 && allTracksFromArtist.isEmpty() && allAlbumsFromArtist.isEmpty()) {
                removeArtistInDatabase(removedArtistId);
                Q_EMIT artistRemoved(removedArtistId);
            }
        }
    }
}

QUrl DatabaseInterface::internalAlbumArtUriFromAlbumId(qulonglong albumId)
{
    auto result = QUrl();

    d->mSelectAlbumArtUriFromAlbumIdQuery.bindValue(QStringLiteral(":albumId"), albumId);

    auto queryResult = d->mSelectAlbumArtUriFromAlbumIdQuery.exec();

    if (!queryResult || !d->mSelectAlbumArtUriFromAlbumIdQuery.isSelect() || !d->mSelectAlbumArtUriFromAlbumIdQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectAlbumArtUriFromAlbumIdQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectAlbumArtUriFromAlbumIdQuery.boundValues();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectAlbumArtUriFromAlbumIdQuery.lastError();

        d->mSelectAlbumArtUriFromAlbumIdQuery.finish();

        return result;
    }

    if (!d->mSelectAlbumArtUriFromAlbumIdQuery.next()) {
        d->mSelectAlbumArtUriFromAlbumIdQuery.finish();

        return result;
    }

    result = d->mSelectAlbumArtUriFromAlbumIdQuery.record().value(0).toUrl();

    d->mSelectAlbumArtUriFromAlbumIdQuery.finish();

    return result;
}

bool DatabaseInterface::isValidArtist(qulonglong albumId)
{
    auto result = false;

    d->mSelectAlbumQuery.bindValue(QStringLiteral(":albumId"), albumId);

    auto queryResult = d->mSelectAlbumQuery.exec();

    if (!queryResult || !d->mSelectAlbumQuery.isSelect() || !d->mSelectAlbumQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectAlbumQuery.lastQuery();
        qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectAlbumQuery.boundValues();
        qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectAlbumQuery.lastError();

        d->mSelectAlbumQuery.finish();

        return result;
    }

    if (!d->mSelectAlbumQuery.next()) {
        d->mSelectAlbumQuery.finish();

        return result;
    }

    const auto &currentRecord = d->mSelectAlbumQuery.record();

    result = !currentRecord.value(3).toString().isEmpty();

    return result;
}

qulonglong DatabaseInterface::internalSourceIdFromName(const QString &sourceName)
{
    qulonglong sourceId = 0;

    d->mSelectMusicSource.bindValue(QStringLiteral(":name"), sourceName);

    auto queryResult = d->mSelectMusicSource.exec();

    if (!queryResult || !d->mSelectMusicSource.isSelect() || !d->mSelectMusicSource.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectMusicSource.lastQuery();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectMusicSource.boundValues();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectMusicSource.lastError();

        d->mSelectMusicSource.finish();

        return sourceId;
    }

    if (!d->mSelectMusicSource.next()) {
        return sourceId;
    }

    sourceId = d->mSelectMusicSource.record().value(0).toULongLong();

    d->mSelectMusicSource.finish();

    return sourceId;
}

QHash<QUrl, QDateTime> DatabaseInterface::internalAllFileNameFromSource(qulonglong sourceId)
{
    QHash<QUrl, QDateTime> allFileNames;

    d->mSelectAllTrackFilesFromSourceQuery.bindValue(QStringLiteral(":discoverId"), sourceId);

    auto queryResult = d->mSelectAllTrackFilesFromSourceQuery.exec();

    if (!queryResult || !d->mSelectAllTrackFilesFromSourceQuery.isSelect() || !d->mSelectAllTrackFilesFromSourceQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectAllTrackFilesFromSourceQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectAllTrackFilesFromSourceQuery.boundValues();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectAllTrackFilesFromSourceQuery.lastError();

        d->mSelectAllTrackFilesFromSourceQuery.finish();

        return allFileNames;
    }

    while(d->mSelectAllTrackFilesFromSourceQuery.next()) {
        auto fileName = d->mSelectAllTrackFilesFromSourceQuery.record().value(0).toUrl();
        auto fileModificationTime = d->mSelectAllTrackFilesFromSourceQuery.record().value(1).toDateTime();

        allFileNames[fileName] = fileModificationTime;
    }

    d->mSelectAllTrackFilesFromSourceQuery.finish();

    return allFileNames;
}

bool DatabaseInterface::internalGenericPartialData(QSqlQuery &query)
{
    auto result = false;

    auto queryResult = query.exec();

    if (!queryResult || !query.isSelect() || !query.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalAllGenericPartialData" << query.lastQuery();
        qDebug() << "DatabaseInterface::internalAllGenericPartialData" << query.boundValues();
        qDebug() << "DatabaseInterface::internalAllGenericPartialData" << query.lastError();

        query.finish();

        auto transactionResult = finishTransaction();
        if (!transactionResult) {
            return result;
        }

        return result;
    }

    result = true;

    return result;
}

qulonglong DatabaseInterface::insertLyricist(const QString &name)
{
    auto result = qulonglong(0);

    if (name.isEmpty()) {
        return result;
    }

    d->mSelectLyricistByNameQuery.bindValue(QStringLiteral(":name"), name);

    auto queryResult = d->mSelectLyricistByNameQuery.exec();

    if (!queryResult || !d->mSelectLyricistByNameQuery.isSelect() || !d->mSelectLyricistByNameQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertLyricist" << d->mSelectLyricistByNameQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertLyricist" << d->mSelectLyricistByNameQuery.boundValues();
        qDebug() << "DatabaseInterface::insertLyricist" << d->mSelectLyricistByNameQuery.lastError();

        d->mSelectLyricistByNameQuery.finish();

        return result;
    }

    if (d->mSelectLyricistByNameQuery.next()) {
        result = d->mSelectLyricistByNameQuery.record().value(0).toULongLong();

        d->mSelectLyricistByNameQuery.finish();

        return result;
    }

    d->mSelectLyricistByNameQuery.finish();

    d->mInsertLyricistQuery.bindValue(QStringLiteral(":lyricistId"), d->mLyricistId);
    d->mInsertLyricistQuery.bindValue(QStringLiteral(":name"), name);

    queryResult = d->mInsertLyricistQuery.exec();

    if (!queryResult || !d->mInsertLyricistQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertLyricist" << d->mInsertLyricistQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertLyricist" << d->mInsertLyricistQuery.boundValues();
        qDebug() << "DatabaseInterface::insertLyricist" << d->mInsertLyricistQuery.lastError();

        d->mInsertLyricistQuery.finish();

        return result;
    }

    result = d->mLyricistId;

    ++d->mLyricistId;

    d->mInsertLyricistQuery.finish();

    Q_EMIT lyricistsAdded(internalAllLyricistsPartialData());

    return result;
}

qulonglong DatabaseInterface::internalArtistIdFromName(const QString &name)
{
    auto result = qulonglong(0);

    if (name.isEmpty()) {
        return result;
    }

    d->mSelectArtistByNameQuery.bindValue(QStringLiteral(":name"), name);

    auto queryResult = d->mSelectArtistByNameQuery.exec();

    if (!queryResult || !d->mSelectArtistByNameQuery.isSelect() || !d->mSelectArtistByNameQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectArtistByNameQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectArtistByNameQuery.boundValues();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectArtistByNameQuery.lastError();

        d->mSelectArtistByNameQuery.finish();

        return result;
    }

    if (!d->mSelectArtistByNameQuery.next()) {
        d->mSelectArtistByNameQuery.finish();

        return result;
    }

    result = d->mSelectArtistByNameQuery.record().value(0).toULongLong();

    d->mSelectArtistByNameQuery.finish();

    return result;
}

void DatabaseInterface::removeTrackInDatabase(qulonglong trackId)
{
    d->mRemoveTrackQuery.bindValue(QStringLiteral(":trackId"), trackId);

    auto result = d->mRemoveTrackQuery.exec();

    if (!result || !d->mRemoveTrackQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::removeTrackInDatabase" << d->mRemoveTrackQuery.lastQuery();
        qDebug() << "DatabaseInterface::removeTrackInDatabase" << d->mRemoveTrackQuery.boundValues();
        qDebug() << "DatabaseInterface::removeTrackInDatabase" << d->mRemoveTrackQuery.lastError();
    }

    d->mRemoveTrackQuery.finish();
}

void DatabaseInterface::updateTrackInDatabase(const MusicAudioTrack &oneTrack, const QString &albumPath)
{
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":trackId"), oneTrack.databaseId());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":title"), oneTrack.title());
    insertArtist(oneTrack.artist());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":artistName"), oneTrack.artist());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":albumTitle"), oneTrack.albumName());
    if (oneTrack.isValidAlbumArtist()) {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":albumArtistName"), oneTrack.albumArtist());
    } else {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":albumArtistName"), {});
    }
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":albumPath"), albumPath);
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":trackNumber"), oneTrack.trackNumber());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":discNumber"), oneTrack.discNumber());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":trackDuration"), QVariant::fromValue<qlonglong>(oneTrack.duration().msecsSinceStartOfDay()));
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":trackRating"), oneTrack.rating());
    if (insertGenre(oneTrack.genre()) != 0) {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":genre"), oneTrack.genre());
    } else {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":genre"), {});
    }
    if (insertComposer(oneTrack.composer()) != 0) {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":composer"), oneTrack.composer());
    } else {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":composer"), {});
    }
    if (insertLyricist(oneTrack.lyricist()) != 0) {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":lyricist"), oneTrack.lyricist());
    } else {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":lyricist"), {});
    }
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":comment"), oneTrack.comment());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":year"), oneTrack.year());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":channels"), oneTrack.channels());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":bitRate"), oneTrack.bitRate());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":sampleRate"), oneTrack.sampleRate());

    auto result = d->mUpdateTrackQuery.exec();

    if (!result || !d->mUpdateTrackQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::updateTrackInDatabase" << d->mUpdateTrackQuery.lastQuery();
        qDebug() << "DatabaseInterface::updateTrackInDatabase" << d->mUpdateTrackQuery.boundValues();
        qDebug() << "DatabaseInterface::updateTrackInDatabase" << d->mUpdateTrackQuery.lastError();
    }

    d->mUpdateTrackQuery.finish();
}

void DatabaseInterface::removeAlbumInDatabase(qulonglong albumId)
{
    d->mRemoveAlbumQuery.bindValue(QStringLiteral(":albumId"), albumId);

    auto result = d->mRemoveAlbumQuery.exec();

    if (!result || !d->mRemoveAlbumQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::removeAlbumInDatabase" << d->mRemoveAlbumQuery.lastQuery();
        qDebug() << "DatabaseInterface::removeAlbumInDatabase" << d->mRemoveAlbumQuery.boundValues();
        qDebug() << "DatabaseInterface::removeAlbumInDatabase" << d->mRemoveAlbumQuery.lastError();
    }

    d->mRemoveAlbumQuery.finish();
}

void DatabaseInterface::removeArtistInDatabase(qulonglong artistId)
{
    d->mRemoveArtistQuery.bindValue(QStringLiteral(":artistId"), artistId);

    auto result = d->mRemoveArtistQuery.exec();

    if (!result || !d->mRemoveArtistQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::removeArtistInDatabase" << d->mRemoveArtistQuery.lastQuery();
        qDebug() << "DatabaseInterface::removeArtistInDatabase" << d->mRemoveArtistQuery.boundValues();
        qDebug() << "DatabaseInterface::removeArtistInDatabase" << d->mRemoveArtistQuery.lastError();
    }

    d->mRemoveArtistQuery.finish();
}

void DatabaseInterface::reloadExistingDatabase()
{
    qDebug() << "DatabaseInterface::reloadExistingDatabase";

    d->mArtistId = initialId(DataUtils::DataType::AllArtists);
    d->mComposerId = initialId(DataUtils::DataType::AllComposers);
    d->mLyricistId = initialId(DataUtils::DataType::AllLyricists);
    d->mAlbumId = initialId(DataUtils::DataType::AllAlbums);
    d->mTrackId = initialId(DataUtils::DataType::AllTracks);
    d->mGenreId = initialId(DataUtils::DataType::AllGenres);;
}

qulonglong DatabaseInterface::initialId(DataUtils::DataType aType)
{
    switch (aType)
    {
    case DataUtils::DataType::AllAlbums:
        return genericInitialId(d->mQueryMaximumAlbumIdQuery);
    case DataUtils::DataType::AllArtists:
        return genericInitialId(d->mQueryMaximumArtistIdQuery);
    case DataUtils::DataType::AllComposers:
        return genericInitialId(d->mQueryMaximumComposerIdQuery);
    case DataUtils::DataType::AllGenres:
        return genericInitialId(d->mQueryMaximumGenreIdQuery);
    case DataUtils::DataType::AllLyricists:
        return genericInitialId(d->mQueryMaximumLyricistIdQuery);
    case DataUtils::DataType::AllTracks:
        return genericInitialId(d->mQueryMaximumTrackIdQuery);
    case DataUtils::DataType::UnknownType:
        break;
    }
    return 1;
}

qulonglong DatabaseInterface::genericInitialId(QSqlQuery &request)
{
    auto result = qulonglong(0);

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    auto queryResult = request.exec();

    if (!queryResult || !request.isSelect() || !request.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertMusicSource" << request.lastQuery();
        qDebug() << "DatabaseInterface::insertMusicSource" << request.boundValues();
        qDebug() << "DatabaseInterface::insertMusicSource" << request.lastError();

        request.finish();

        transactionResult = finishTransaction();
        if (!transactionResult) {
            return result;
        }

        return result;
    }

    if (request.next()) {
        result = request.record().value(0).toULongLong() + 1;

        request.finish();
    }

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

qulonglong DatabaseInterface::insertMusicSource(const QString &name)
{
    qulonglong result = 0;

    d->mSelectMusicSource.bindValue(QStringLiteral(":name"), name);

    auto queryResult = d->mSelectMusicSource.exec();

    if (!queryResult || !d->mSelectMusicSource.isSelect() || !d->mSelectMusicSource.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectMusicSource.lastQuery();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectMusicSource.boundValues();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectMusicSource.lastError();

        d->mSelectMusicSource.finish();

        return result;
    }

    if (d->mSelectMusicSource.next()) {
        result = d->mSelectMusicSource.record().value(0).toULongLong();

        d->mSelectMusicSource.finish();

        return result;
    }

    d->mSelectMusicSource.finish();

    d->mInsertMusicSource.bindValue(QStringLiteral(":discoverId"), d->mDiscoverId);
    d->mInsertMusicSource.bindValue(QStringLiteral(":name"), name);

    queryResult = d->mInsertMusicSource.exec();

    if (!queryResult || !d->mInsertMusicSource.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertMusicSource" << d->mInsertMusicSource.lastQuery();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mInsertMusicSource.boundValues();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mInsertMusicSource.lastError();

        d->mInsertMusicSource.finish();

        return d->mDiscoverId;
    }

    d->mInsertMusicSource.finish();

    ++d->mDiscoverId;

    return d->mDiscoverId - 1;
}

QList<qulonglong> DatabaseInterface::fetchTrackIds(qulonglong albumId)
{
    auto allTracks = QList<qulonglong>();

    d->mSelectTrackQuery.bindValue(QStringLiteral(":albumId"), albumId);

    auto result = d->mSelectTrackQuery.exec();

    if (!result || !d->mSelectTrackQuery.isSelect() || !d->mSelectTrackQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::fetchTrackIds" << d->mSelectTrackQuery.lastQuery();
        qDebug() << "DatabaseInterface::fetchTrackIds" << d->mSelectTrackQuery.boundValues();
        qDebug() << "DatabaseInterface::fetchTrackIds" << d->mSelectTrackQuery.lastError();
    }

    while (d->mSelectTrackQuery.next()) {
        const auto &currentRecord = d->mSelectTrackQuery.record();

        allTracks.push_back(currentRecord.value(0).toULongLong());
    }

    d->mSelectTrackQuery.finish();

    return allTracks;
}

qulonglong DatabaseInterface::internalAlbumIdFromTitleAndArtist(const QString &title, const QString &artist)
{
    auto result = qulonglong(0);

    d->mSelectAlbumIdFromTitleQuery.bindValue(QStringLiteral(":title"), title);
    d->mSelectAlbumIdFromTitleQuery.bindValue(QStringLiteral(":artistName"), artist);

    auto queryResult = d->mSelectAlbumIdFromTitleQuery.exec();

    if (!queryResult || !d->mSelectAlbumIdFromTitleQuery.isSelect() || !d->mSelectAlbumIdFromTitleQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalAlbumIdFromTitleAndArtist" << d->mSelectAlbumIdFromTitleQuery.lastQuery();
        qDebug() << "DatabaseInterface::internalAlbumIdFromTitleAndArtist" << d->mSelectAlbumIdFromTitleQuery.boundValues();
        qDebug() << "DatabaseInterface::internalAlbumIdFromTitleAndArtist" << d->mSelectAlbumIdFromTitleQuery.lastError();

        d->mSelectAlbumIdFromTitleQuery.finish();

        return result;
    }

    if (d->mSelectAlbumIdFromTitleQuery.next()) {
        result = d->mSelectAlbumIdFromTitleQuery.record().value(0).toULongLong();
    }

    d->mSelectAlbumIdFromTitleQuery.finish();

    if (result == 0) {
        d->mSelectAlbumIdFromTitleWithoutArtistQuery.bindValue(QStringLiteral(":title"), title);

        auto queryResult = d->mSelectAlbumIdFromTitleWithoutArtistQuery.exec();

        if (!queryResult || !d->mSelectAlbumIdFromTitleWithoutArtistQuery.isSelect() || !d->mSelectAlbumIdFromTitleWithoutArtistQuery.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::internalAlbumIdFromTitleAndArtist" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::internalAlbumIdFromTitleAndArtist" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.boundValues();
            qDebug() << "DatabaseInterface::internalAlbumIdFromTitleAndArtist" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.lastError();

            d->mSelectAlbumIdFromTitleWithoutArtistQuery.finish();

            return result;
        }

        if (d->mSelectAlbumIdFromTitleWithoutArtistQuery.next()) {
            result = d->mSelectAlbumIdFromTitleWithoutArtistQuery.record().value(0).toULongLong();
        }

        d->mSelectAlbumIdFromTitleWithoutArtistQuery.finish();
    }

    return result;
}

MusicAudioTrack DatabaseInterface::internalTrackFromDatabaseId(qulonglong id)
{
    auto result = MusicAudioTrack();

    if (result.isValid()) {
        return result;
    }

    if (!d || !d->mTracksDatabase.isValid() || !d->mInitFinished) {
        return result;
    }

    d->mSelectTrackFromIdQuery.bindValue(QStringLiteral(":trackId"), id);

    auto queryResult = d->mSelectTrackFromIdQuery.exec();

    if (!queryResult || !d->mSelectTrackFromIdQuery.isSelect() || !d->mSelectTrackFromIdQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalTrackFromDatabaseId" << d->mSelectAlbumQuery.lastQuery();
        qDebug() << "DatabaseInterface::internalTrackFromDatabaseId" << d->mSelectAlbumQuery.boundValues();
        qDebug() << "DatabaseInterface::internalTrackFromDatabaseId" << d->mSelectAlbumQuery.lastError();

        d->mSelectTrackFromIdQuery.finish();

        return result;
    }

    if (!d->mSelectTrackFromIdQuery.next()) {
        d->mSelectTrackFromIdQuery.finish();

        return result;
    }

    const auto &currentRecord = d->mSelectTrackFromIdQuery.record();

    result = buildTrackFromDatabaseRecord(currentRecord);

    d->mSelectTrackFromIdQuery.finish();

    return result;
}

qulonglong DatabaseInterface::internalTrackIdFromTitleAlbumTracDiscNumber(const QString &title, const QString &artist, const QString &album,
                                                                          int trackNumber, int discNumber)
{
    auto result = qulonglong(0);

    if (!d) {
        return result;
    }

    d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":title"), title);
    d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":artist"), artist);
    d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":album"), album);
    d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":trackNumber"), trackNumber);
    d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":discNumber"), discNumber);

    auto queryResult = d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.exec();

    if (!queryResult || !d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.isSelect() || !d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::trackIdFromTitleAlbumArtist" << d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.lastQuery();
        qDebug() << "DatabaseInterface::trackIdFromTitleAlbumArtist" << d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.boundValues();
        qDebug() << "DatabaseInterface::trackIdFromTitleAlbumArtist" << d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.lastError();

        d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.finish();

        return result;
    }

    if (d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.next()) {
        result = d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.record().value(0).toULongLong();
    }

    d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.finish();

    return result;
}

qulonglong DatabaseInterface::getDuplicateTrackIdFromTitleAlbumTrackDiscNumber(const QString &title, const QString &trackArtist,
                                                                               const QString &album, const QString &albumArtist,
                                                                               const QString &trackPath, int trackNumber, int discNumber)
{
    auto result = qulonglong(0);

    if (!d) {
        return result;
    }

    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":title"), title);
    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":trackArtist"), trackArtist);
    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":album"), album);
    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":albumPath"), trackPath);
    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":albumArtist"), albumArtist);
    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":trackNumber"), trackNumber);
    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":discNumber"), discNumber);

    auto queryResult = d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.exec();

    if (!queryResult || !d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.isSelect() || !d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::trackIdFromTitleAlbumArtist" << d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.lastQuery();
        qDebug() << "DatabaseInterface::trackIdFromTitleAlbumArtist" << d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.boundValues();
        qDebug() << "DatabaseInterface::trackIdFromTitleAlbumArtist" << d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.lastError();

        d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.finish();

        return result;
    }

    if (d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.next()) {
        result = d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.record().value(0).toULongLong();
    }

    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.finish();

    return result;
}

qulonglong DatabaseInterface::internalTrackIdFromFileName(const QUrl &fileName)
{
    auto result = qulonglong(0);

    if (!d) {
        return result;
    }

    d->mSelectTracksMapping.bindValue(QStringLiteral(":fileName"), fileName);

    auto queryResult = d->mSelectTracksMapping.exec();

    if (!queryResult || !d->mSelectTracksMapping.isSelect() || !d->mSelectTracksMapping.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMapping.lastQuery();
        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMapping.boundValues();
        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMapping.lastError();

        d->mSelectTracksMapping.finish();

        return result;
    }

    if (d->mSelectTracksMapping.next()) {
        const auto &currentRecordValue = d->mSelectTracksMapping.record().value(0);
        if (currentRecordValue.isValid()) {
            result = currentRecordValue.toULongLong();
        }
    }

    d->mSelectTracksMapping.finish();

    return result;
}

DatabaseInterface::ListTrackDataType DatabaseInterface::internalTracksFromAuthor(const QString &ArtistName)
{
    auto allTracks = ListTrackDataType{};

    d->mSelectTracksFromArtist.bindValue(QStringLiteral(":artistName"), ArtistName);

    auto result = d->mSelectTracksFromArtist.exec();

    if (!result || !d->mSelectTracksFromArtist.isSelect() || !d->mSelectTracksFromArtist.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::tracksFromAuthor" << d->mSelectTracksFromArtist.lastQuery();
        qDebug() << "DatabaseInterface::tracksFromAuthor" << d->mSelectTracksFromArtist.boundValues();
        qDebug() << "DatabaseInterface::tracksFromAuthor" << d->mSelectTracksFromArtist.lastError();

        return allTracks;
    }

    while (d->mSelectTracksFromArtist.next()) {
        const auto &currentRecord = d->mSelectTracksFromArtist.record();

        allTracks.push_back(buildTrackDataFromDatabaseRecord(currentRecord));
    }

    d->mSelectTracksFromArtist.finish();

    return allTracks;
}

QList<qulonglong> DatabaseInterface::internalAlbumIdsFromAuthor(const QString &ArtistName)
{
    auto allAlbumIds = QList<qulonglong>();

    d->mSelectAlbumIdsFromArtist.bindValue(QStringLiteral(":artistName"), ArtistName);

    auto result = d->mSelectAlbumIdsFromArtist.exec();

    if (!result || !d->mSelectAlbumIdsFromArtist.isSelect() || !d->mSelectAlbumIdsFromArtist.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::tracksFromAuthor" << d->mSelectAlbumIdsFromArtist.lastQuery();
        qDebug() << "DatabaseInterface::tracksFromAuthor" << d->mSelectAlbumIdsFromArtist.boundValues();
        qDebug() << "DatabaseInterface::tracksFromAuthor" << d->mSelectAlbumIdsFromArtist.lastError();

        return allAlbumIds;
    }

    while (d->mSelectAlbumIdsFromArtist.next()) {
        const auto &currentRecord = d->mSelectAlbumIdsFromArtist.record();

        allAlbumIds.push_back(currentRecord.value(0).toULongLong());
    }

    d->mSelectAlbumIdsFromArtist.finish();

    return allAlbumIds;
}

DatabaseInterface::ListArtistDataType DatabaseInterface::internalAllArtistsPartialData(QSqlQuery &artistsQuery)
{
    auto result = ListArtistDataType{};

    if (!internalGenericPartialData(artistsQuery)) {
        return result;
    }

    while(artistsQuery.next()) {
        auto newData = ArtistDataType{};

        const auto &currentRecord = artistsQuery.record();

        newData[DataType::key_type::DatabaseIdRole] = currentRecord.value(0);
        newData[DataType::key_type::TitleRole] = currentRecord.value(1);
        newData[DataType::key_type::GenreRole] = QVariant::fromValue(currentRecord.value(2).toString().split(QStringLiteral(", ")));
        newData[DataType::key_type::ElementTypeRole] = ElisaUtils::Artist;

        result.push_back(newData);
    }

    artistsQuery.finish();

    return result;
}

DatabaseInterface::ArtistDataType DatabaseInterface::internalOneArtistPartialData(qulonglong databaseId)
{
    auto result = ArtistDataType{};

    d->mSelectArtistQuery.bindValue(QStringLiteral(":artistId"), databaseId);

    if (!internalGenericPartialData(d->mSelectArtistQuery)) {
        return result;
    }

    if (d->mSelectArtistQuery.next()) {
        const auto &currentRecord = d->mSelectArtistQuery.record();

        result[DataType::key_type::DatabaseIdRole] = currentRecord.value(0);
        result[DataType::key_type::TitleRole] = currentRecord.value(1);
        result[DataType::key_type::GenreRole] = QVariant::fromValue(currentRecord.value(2).toString().split(QStringLiteral(", ")));
        result[DataType::key_type::ElementTypeRole] = ElisaUtils::Artist;
    }

    d->mSelectArtistQuery.finish();

    return result;
}

DatabaseInterface::ListAlbumDataType DatabaseInterface::internalAllAlbumsPartialData(QSqlQuery &query)
{
    auto result = ListAlbumDataType{};

    if (!internalGenericPartialData(query)) {
        return result;
    }

    while(query.next()) {
        auto newData = AlbumDataType{};

        const auto &currentRecord = query.record();

        newData[DataType::key_type::DatabaseIdRole] = currentRecord.value(0);
        newData[DataType::key_type::TitleRole] = currentRecord.value(1);
        newData[DataType::key_type::SecondaryTextRole] = currentRecord.value(2);
        newData[DataType::key_type::ImageUrlRole] = currentRecord.value(3);
        newData[DataType::key_type::ArtistRole] = currentRecord.value(4);
        newData[DataType::key_type::AllArtistsRole] = QVariant::fromValue(currentRecord.value(5).toString().split(QStringLiteral(", ")));
        newData[DataType::key_type::HighestTrackRating] = currentRecord.value(6);
        newData[DataType::key_type::IsSingleDiscAlbumRole] = currentRecord.value(8);
        newData[DataType::key_type::GenreRole] = QVariant::fromValue(currentRecord.value(7).toString().split(QStringLiteral(", ")));
        newData[DataType::key_type::ElementTypeRole] = ElisaUtils::Album;

        result.push_back(newData);
    }

    query.finish();

    return result;
}

DatabaseInterface::AlbumDataType DatabaseInterface::internalOneAlbumPartialData(qulonglong databaseId)
{
    auto result = AlbumDataType{};

    d->mSelectAlbumQuery.bindValue(QStringLiteral(":albumId"), databaseId);

    if (!internalGenericPartialData(d->mSelectAlbumQuery)) {
        return result;
    }

    if (d->mSelectAlbumQuery.next()) {
        const auto &currentRecord = d->mSelectAlbumQuery.record();

        result[DataType::key_type::DatabaseIdRole] = currentRecord.value(0);
        result[DataType::key_type::TitleRole] = currentRecord.value(1);
        result[DataType::key_type::SecondaryTextRole] = currentRecord.value(3);
        result[DataType::key_type::ImageUrlRole] = currentRecord.value(5);
        result[DataType::key_type::ArtistRole] = currentRecord.value(3);
        result[DataType::key_type::AllArtistsRole] = QVariant::fromValue(currentRecord.value(9).toString().split(QStringLiteral(", ")));
        result[DataType::key_type::HighestTrackRating] = currentRecord.value(10);
        result[DataType::key_type::IsSingleDiscAlbumRole] = currentRecord.value(8);
        result[DataType::key_type::GenreRole] = QVariant::fromValue(currentRecord.value(11).toString().split(QStringLiteral(", ")));
        result[DataType::key_type::ElementTypeRole] = ElisaUtils::Album;
    }

    d->mSelectAlbumQuery.finish();

    return result;
}

DatabaseInterface::ListTrackDataType DatabaseInterface::internalAllTracksPartialData()
{
    auto result = ListTrackDataType{};

    if (!internalGenericPartialData(d->mSelectAllTracksQuery)) {
        return result;
    }

    while(d->mSelectAllTracksQuery.next()) {
        const auto &currentRecord = d->mSelectAllTracksQuery.record();

        auto newData = buildTrackDataFromDatabaseRecord(currentRecord);

        result.push_back(newData);
    }

    d->mSelectAllTracksQuery.finish();

    return result;
}

DatabaseInterface::ListTrackDataType DatabaseInterface::internalRecentlyPlayedTracksData(int count)
{
    auto result = ListTrackDataType{};

    d->mSelectAllRecentlyPlayedTracksQuery.bindValue(QStringLiteral(":maximumResults"), count);

    if (!internalGenericPartialData(d->mSelectAllRecentlyPlayedTracksQuery)) {
        return result;
    }

    while(d->mSelectAllRecentlyPlayedTracksQuery.next()) {
        const auto &currentRecord = d->mSelectAllRecentlyPlayedTracksQuery.record();

        auto newData = buildTrackDataFromDatabaseRecord(currentRecord);

        result.push_back(newData);
    }

    d->mSelectAllRecentlyPlayedTracksQuery.finish();

    return result;
}

DatabaseInterface::ListTrackDataType DatabaseInterface::internalFrequentlyPlayedTracksData(int count)
{
    auto result = ListTrackDataType{};

    d->mSelectAllFrequentlyPlayedTracksQuery.bindValue(QStringLiteral(":maximumResults"), count);

    if (!internalGenericPartialData(d->mSelectAllFrequentlyPlayedTracksQuery)) {
        return result;
    }

    while(d->mSelectAllFrequentlyPlayedTracksQuery.next()) {
        const auto &currentRecord = d->mSelectAllFrequentlyPlayedTracksQuery.record();

        auto newData = buildTrackDataFromDatabaseRecord(currentRecord);

        result.push_back(newData);
    }

    d->mSelectAllFrequentlyPlayedTracksQuery.finish();

    return result;
}

DatabaseInterface::TrackDataType DatabaseInterface::internalOneTrackPartialData(qulonglong databaseId)
{
    auto result = TrackDataType{};

    d->mSelectTrackFromIdQuery.bindValue(QStringLiteral(":trackId"), databaseId);

    if (!internalGenericPartialData(d->mSelectTrackFromIdQuery)) {
        return result;
    }

    if (d->mSelectTrackFromIdQuery.next()) {
        const auto &currentRecord = d->mSelectTrackFromIdQuery.record();

        result = buildTrackDataFromDatabaseRecord(currentRecord);
    }

    d->mSelectTrackFromIdQuery.finish();

    return result;
}

DatabaseInterface::ListGenreDataType DatabaseInterface::internalAllGenresPartialData()
{
    ListGenreDataType result;

    if (!internalGenericPartialData(d->mSelectAllGenresQuery)) {
        return result;
    }

    while(d->mSelectAllGenresQuery.next()) {
        auto newData = GenreDataType{};

        const auto &currentRecord = d->mSelectAllGenresQuery.record();

        newData[DataType::key_type::DatabaseIdRole] = currentRecord.value(0);
        newData[DataType::key_type::TitleRole] = currentRecord.value(1);
        newData[DataType::key_type::ElementTypeRole] = ElisaUtils::Genre;

        result.push_back(newData);
    }

    d->mSelectAllGenresQuery.finish();

    return result;
}

DatabaseInterface::GenreDataType DatabaseInterface::internalOneGenrePartialData(qulonglong databaseId)
{
    auto result = GenreDataType{};

    d->mSelectGenreQuery.bindValue(QStringLiteral(":genreId"), databaseId);

    if (!internalGenericPartialData(d->mSelectGenreQuery)) {
        return result;
    }

    if (d->mSelectGenreQuery.next()) {
        const auto &currentRecord = d->mSelectGenreQuery.record();

        result[DataType::key_type::DatabaseIdRole] = currentRecord.value(0);
        result[DataType::key_type::TitleRole] = currentRecord.value(1);
        result[DataType::key_type::ElementTypeRole] = ElisaUtils::Genre;
    }

    d->mSelectGenreQuery.finish();

    return result;
}

DatabaseInterface::ListArtistDataType DatabaseInterface::internalAllComposersPartialData()
{
    ListArtistDataType result;

    if (!internalGenericPartialData(d->mSelectAllComposersQuery)) {
        return result;
    }

    while(d->mSelectAllComposersQuery.next()) {
        auto newData = ArtistDataType{};

        const auto &currentRecord = d->mSelectAllComposersQuery.record();

        newData[DataType::key_type::DatabaseIdRole] = currentRecord.value(0);
        newData[DataType::key_type::TitleRole] = currentRecord.value(1);
        newData[DataType::key_type::ElementTypeRole] = ElisaUtils::Composer;

        result.push_back(newData);
    }

    d->mSelectAllComposersQuery.finish();

    return result;
}

DatabaseInterface::ArtistDataType DatabaseInterface::internalOneComposerPartialData(qulonglong databaseId)
{
    auto result = ArtistDataType{};

    d->mSelectComposerQuery.bindValue(QStringLiteral(":composerId"), databaseId);

    if (!internalGenericPartialData(d->mSelectComposerQuery)) {
        return result;
    }

    if (d->mSelectComposerQuery.next()) {
        const auto &currentRecord = d->mSelectComposerQuery.record();

        result[DataType::key_type::DatabaseIdRole] = currentRecord.value(0);
        result[DataType::key_type::TitleRole] = currentRecord.value(1);
        result[DataType::key_type::ElementTypeRole] = ElisaUtils::Composer;
    }

    d->mSelectComposerQuery.finish();

    return result;
}

DatabaseInterface::ListArtistDataType DatabaseInterface::internalAllLyricistsPartialData()
{
    ListArtistDataType result;

    if (!internalGenericPartialData(d->mSelectAllLyricistsQuery)) {
        return result;
    }

    while(d->mSelectAllLyricistsQuery.next()) {
        auto newData = ArtistDataType{};

        const auto &currentRecord = d->mSelectAllLyricistsQuery.record();

        newData[DataType::key_type::DatabaseIdRole] = currentRecord.value(0);
        newData[DataType::key_type::TitleRole] = currentRecord.value(1);
        newData[DataType::key_type::ElementTypeRole] = ElisaUtils::Lyricist;

        result.push_back(newData);
    }

    d->mSelectAllLyricistsQuery.finish();

    return result;
}

DatabaseInterface::ArtistDataType DatabaseInterface::internalOneLyricistPartialData(qulonglong databaseId)
{
    auto result = ArtistDataType{};

    d->mSelectLyricistQuery.bindValue(QStringLiteral(":lyricistId"), databaseId);

    if (!internalGenericPartialData(d->mSelectLyricistQuery)) {
        return result;
    }

    if (d->mSelectLyricistQuery.next()) {
        const auto &currentRecord = d->mSelectLyricistQuery.record();

        result[DataType::key_type::DatabaseIdRole] = currentRecord.value(0);
        result[DataType::key_type::TitleRole] = currentRecord.value(1);
        result[DataType::key_type::ElementTypeRole] = ElisaUtils::Lyricist;
    }

    d->mSelectLyricistQuery.finish();

    return result;
}

bool DatabaseInterface::prepareQuery(QSqlQuery &query, const QString &queryText) const
{
    query.setForwardOnly(true);
    return query.prepare(queryText);
}

void DatabaseInterface::updateAlbumArtist(qulonglong albumId, const QString &title,
                                          const QString &albumPath,
                                          const QString &artistName)
{
    d->mUpdateAlbumArtistQuery.bindValue(QStringLiteral(":albumId"), albumId);
    insertArtist(artistName);
    d->mUpdateAlbumArtistQuery.bindValue(QStringLiteral(":artistName"), artistName);

    auto queryResult = d->mUpdateAlbumArtistQuery.exec();

    if (!queryResult || !d->mUpdateAlbumArtistQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::updateAlbumArtist" << d->mUpdateAlbumArtistQuery.lastQuery();
        qDebug() << "DatabaseInterface::updateAlbumArtist" << d->mUpdateAlbumArtistQuery.boundValues();
        qDebug() << "DatabaseInterface::updateAlbumArtist" << d->mUpdateAlbumArtistQuery.lastError();

        d->mUpdateAlbumArtistQuery.finish();

        return;
    }

    d->mUpdateAlbumArtistQuery.finish();

    d->mUpdateAlbumArtistInTracksQuery.bindValue(QStringLiteral(":albumTitle"), title);
    d->mUpdateAlbumArtistInTracksQuery.bindValue(QStringLiteral(":albumPath"), albumPath);
    d->mUpdateAlbumArtistInTracksQuery.bindValue(QStringLiteral(":artistName"), artistName);

    queryResult = d->mUpdateAlbumArtistInTracksQuery.exec();

    if (!queryResult || !d->mUpdateAlbumArtistInTracksQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::updateAlbumArtist" << d->mUpdateAlbumArtistInTracksQuery.lastQuery();
        qDebug() << "DatabaseInterface::updateAlbumArtist" << d->mUpdateAlbumArtistInTracksQuery.boundValues();
        qDebug() << "DatabaseInterface::updateAlbumArtist" << d->mUpdateAlbumArtistInTracksQuery.lastError();

        d->mUpdateAlbumArtistInTracksQuery.finish();

        return;
    }

    d->mUpdateAlbumArtistInTracksQuery.finish();
}

void DatabaseInterface::updateTrackStatistics(qulonglong databaseId, const QDateTime &time)
{
    d->mUpdateTrackStatistics.bindValue(QStringLiteral(":trackId"), databaseId);
    d->mUpdateTrackStatistics.bindValue(QStringLiteral(":playDate"), time.toMSecsSinceEpoch());

    auto queryResult = d->mUpdateTrackStatistics.exec();

    if (!queryResult || !d->mUpdateTrackStatistics.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::updateTrackStatistics" << d->mUpdateTrackStatistics.lastQuery();
        qDebug() << "DatabaseInterface::updateTrackStatistics" << d->mUpdateTrackStatistics.boundValues();
        qDebug() << "DatabaseInterface::updateTrackStatistics" << d->mUpdateTrackStatistics.lastError();

        d->mUpdateTrackStatistics.finish();

        return;
    }

    d->mUpdateTrackStatistics.finish();

    d->mUpdateTrackFirstPlayStatistics.bindValue(QStringLiteral(":trackId"), databaseId);
    d->mUpdateTrackFirstPlayStatistics.bindValue(QStringLiteral(":playDate"), time.toMSecsSinceEpoch());

    queryResult = d->mUpdateTrackFirstPlayStatistics.exec();

    if (!queryResult || !d->mUpdateTrackFirstPlayStatistics.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::updateTrackStatistics" << d->mUpdateTrackFirstPlayStatistics.lastQuery();
        qDebug() << "DatabaseInterface::updateTrackStatistics" << d->mUpdateTrackFirstPlayStatistics.boundValues();
        qDebug() << "DatabaseInterface::updateTrackStatistics" << d->mUpdateTrackFirstPlayStatistics.lastError();

        d->mUpdateTrackFirstPlayStatistics.finish();

        return;
    }

    d->mUpdateTrackFirstPlayStatistics.finish();
}


#include "moc_databaseinterface.cpp"
