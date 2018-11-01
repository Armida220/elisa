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

#include <QFileInfo>
#include <QDir>

class CoverManagerPrivate
{
};

CoverManager::CoverManager() : d(std::make_unique<CoverManagerPrivate>())
{
}

CoverManager::~CoverManager() = default;

QUrl CoverManager::findCover(const MusicAudioTrack &newTrack)
{
    QFileInfo trackFilePath(newTrack.resourceURI().toLocalFile());
    QDir trackFileDir = trackFilePath.absoluteDir();
    QString dirNamePattern = QStringLiteral("*") + trackFileDir.dirName() + QStringLiteral("*");
    QStringList filters;
    filters << QStringLiteral("*[Cc]over*.jpg") << QStringLiteral("*[Cc]over*.png")
            << QStringLiteral("*[Ff]older*.jpg") << QStringLiteral("*[Ff]older*.png")
            << QStringLiteral("*[Ff]ront*.jpg") << QStringLiteral("*[Ff]ront*.png")
            << dirNamePattern + QStringLiteral(".jpg") << dirNamePattern + QStringLiteral(".png")
            << dirNamePattern.toLower() + QStringLiteral(".jpg") << dirNamePattern.toLower() + QStringLiteral(".png");
    dirNamePattern.remove(QLatin1Char(' '));
    filters << dirNamePattern + QStringLiteral(".jpg") << dirNamePattern + QStringLiteral(".png")
            << dirNamePattern.toLower() + QStringLiteral(".jpg") << dirNamePattern.toLower() + QStringLiteral(".png");
    trackFileDir.setNameFilters(filters);
    QFileInfoList coverFiles = trackFileDir.entryInfoList();
    if (coverFiles.isEmpty()) {
        return QUrl();
    } else {
        return QUrl::fromLocalFile(coverFiles.at(0).absoluteFilePath());
    }
}

