/*
 * Copyright 2018 Matthieu Gallien <matthieu_gallien@yahoo.fr>
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

import QtQuick 2.10
import QtQuick.Controls 2.3
import org.kde.elisa 1.0

FocusScope {
    id: allArtists

    property alias mainTitle: gridView.mainTitle
    property alias image: gridView.image

    focus: true

    DataModel {
        id: realModel
    }

    AllArtistsProxyModel {
        id: proxyModel

        sourceModel: realModel

        onArtistToEnqueue: elisa.mediaPlayList.enqueue(newEntries, databaseIdType, enqueueMode, triggerPlay)
    }

    GridBrowserView {
        id: gridView

        focus: true

        anchors.fill: parent

        showRating: false
        delegateDisplaySecondaryText: false
        defaultIcon: elisaTheme.artistIcon

        contentModel: proxyModel

        image: elisaTheme.artistIcon
        mainTitle: i18nc("Title of the view of all artists", "Artists")

        onEnqueue: elisa.mediaPlayList.enqueue(databaseId, name, ElisaUtils.Artist,
                                               ElisaUtils.AppendPlayList,
                                               ElisaUtils.DoNotTriggerPlay)

        onReplaceAndPlay: elisa.mediaPlayList.enqueue(databaseId, name, ElisaUtils.Artist,
                                                      ElisaUtils.ReplacePlayList,
                                                      ElisaUtils.TriggerPlay)

        onOpen: viewManager.openOneArtist(innerMainTitle, innerImage, 0)


        onGoBack: viewManager.goBack()
    }

    Connections {
        target: elisa

        onMusicManagerChanged: realModel.initialize(elisa.musicManager, ElisaUtils.Artist)
    }

    Component.onCompleted: {
        if (elisa.musicManager) {
            realModel.initialize(elisa.musicManager, ElisaUtils.Artist)
        }
    }
}
