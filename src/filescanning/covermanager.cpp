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
#include <QImage>

class CoverManagerPrivate
{
public:
    static const QStringList constSearchStrings;
    const KFileMetaData::EmbeddedImageData imageReader;
};

const QStringList CoverManagerPrivate::constSearchStrings = {
    QStringLiteral("*[Cc]over*.jpg"),
    QStringLiteral("*[Cc]over*.png"),
    QStringLiteral("*[Ff]older*.jpg"),
    QStringLiteral("*[Ff]older*.png"),
    QStringLiteral("*[Ff]ront*.jpg"),
    QStringLiteral("*[Ff]ront*.png")
};

CoverManager::CoverManager() : d(std::make_unique<CoverManagerPrivate>())
{
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
    if (coverFiles.isEmpty()) {
        return QUrl();
    } else {
        return QUrl::fromLocalFile(coverFiles.at(0).absoluteFilePath());
    }
}

QUrl CoverManager::loadAlbumCoverFromMetaData(const MusicAudioTrack &newTrack) const
{
    if (newTrack.resourceURI().isLocalFile()) {
        auto imageMap = d->imageReader.imageData(newTrack.resourceURI().toLocalFile());
        auto frontCoverEntry = imageMap.find(KFileMetaData::EmbeddedImageData::FrontCover);
        if (frontCoverEntry != imageMap.end()) {
            QImage frontCover;
            if (frontCover.loadFromData(frontCoverEntry.value())) {
                frontCover.save(QStringLiteral("test.png"));
            }
        }
    }
    return QUrl();
}

