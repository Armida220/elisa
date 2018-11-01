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

#ifndef COVERMANAGER_H
#define COVERMANAGER_H

#include <QUrl>
#include <memory>


class CoverManagerPrivate;
class MusicAudioTrack;

class CoverManager
{
public:

    CoverManager();

    virtual ~CoverManager();

    QUrl findCover(const MusicAudioTrack &track);

private:

    std::unique_ptr<CoverManagerPrivate> d;

};

#endif // COVERMANAGER_H
