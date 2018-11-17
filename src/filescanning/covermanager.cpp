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

#include "covermanager.h"

#include "musicaudiotrack.h"

#include <KFileMetaData/EmbeddedImageData>

#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QImage>
#include <QDebug>

class CoverManagerPrivate
{
public:
    static const QStringList constSearchStrings;
    const KFileMetaData::EmbeddedImageData imageReader;
    QImage frontCover;
    QDir directoryUrl;
    QSize maximumImageSize;

    QUrl saveToFile(const MusicAudioTrack &track);
};

const QStringList CoverManagerPrivate::constSearchStrings = {
    QStringLiteral("*[Cc]over*.jpg"),
    QStringLiteral("*[Cc]over*.png"),
    QStringLiteral("*[Ff]older*.jpg"),
    QStringLiteral("*[Ff]older*.png"),
    QStringLiteral("*[Ff]ront*.jpg"),
    QStringLiteral("*[Ff]ront*.png")
};

QUrl CoverManagerPrivate::saveToFile(const MusicAudioTrack &track)
{
    QString fileUrl = directoryUrl.absolutePath() + QStringLiteral("/");
    if (track.albumArtist().isEmpty() || track.albumName().isEmpty()) {
        fileUrl += track.resourceURI().fileName() + QStringLiteral(".png");
    } else {
        fileUrl += track.albumArtist() + track.albumName() + QStringLiteral(".png");
    }
    if (frontCover.size().width() > maximumImageSize.width()
            || frontCover.size().height() > maximumImageSize.height()) {
        frontCover = frontCover.scaled(maximumImageSize, Qt::KeepAspectRatio);
    }
    if (frontCover.save(fileUrl)) {
        qDebug() << QStringLiteral("Saved cover art to file") << fileUrl;
        return QUrl::fromLocalFile(fileUrl);
    }
    return QUrl();
}

CoverManager::CoverManager() : d(std::make_unique<CoverManagerPrivate>())
{
    d->directoryUrl = QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QStringLiteral("/covers/"));
    if (!d->directoryUrl.exists()) {
        d->directoryUrl.mkpath(d->directoryUrl.absolutePath());
    }
    d->maximumImageSize.setHeight(512);
    d->maximumImageSize.setWidth(512);
}

CoverManager::~CoverManager() = default;

QUrl CoverManager::findAlbumCoverInDirectory(const MusicAudioTrack &newTrack) const
{
    QFileInfo trackFilePath(newTrack.resourceURI().toLocalFile());
    QDir trackFileDir = trackFilePath.absoluteDir();
    trackFileDir.setFilter(QDir::Files);
    trackFileDir.setNameFilters(d->constSearchStrings);
    QFileInfoList coverFiles = trackFileDir.entryInfoList();
    if (coverFiles.isEmpty()) {
        QString dirNamePattern = QStringLiteral("*") + trackFileDir.dirName() + QStringLiteral("*");
        QString dirNameNoSpaces = dirNamePattern.remove(QLatin1Char(' '));
        QStringList filters = {
            dirNamePattern + QStringLiteral(".jpg"),
            dirNamePattern + QStringLiteral(".png"),
            newTrack.albumName() + QStringLiteral(".jpg"),
            newTrack.albumName() + QStringLiteral(".png"),
            dirNameNoSpaces + QStringLiteral(".jpg"),
            dirNameNoSpaces + QStringLiteral(".png")
        };
        trackFileDir.setNameFilters(filters);
        coverFiles = trackFileDir.entryInfoList();
    }
    if (!coverFiles.isEmpty()) {
        if (d->frontCover.load(coverFiles.at(0).absoluteFilePath())) {
            return d->saveToFile(newTrack);
        }
    }
    return QUrl();
}

QUrl CoverManager::loadAlbumCoverFromMetaData(const MusicAudioTrack &newTrack) const
{
    if (newTrack.resourceURI().isLocalFile()) {
        auto imageMap = d->imageReader.imageData(newTrack.resourceURI().toLocalFile());
        auto frontCoverEntry = imageMap.find(KFileMetaData::EmbeddedImageData::FrontCover);
        if (frontCoverEntry != imageMap.end()) {
            if (d->frontCover.loadFromData(frontCoverEntry.value())) {
                return d->saveToFile(newTrack);
            }
        }
    }
    return QUrl();
}



