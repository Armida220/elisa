/*
 * Copyright 2016-2018 Matthieu Gallien <matthieu_gallien@yahoo.fr>
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

import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2
import org.kde.elisa 1.0

RowLayout {
    id: contentViewContainer
    spacing: 0

    property bool showPlaylist
    property alias currentViewIndex: listViews.currentIndex

    signal toggleSearch()

    function goBack() {
        viewManager.goBack()
    }

    ViewManager {
        id: viewManager

        onSwitchOffAllViews: {
            listViews.setCurrentIndex(pageModel.indexFromViewType(viewType))

            while(browseStackView.depth > 1) {
                browseStackView.pop()
            }
        }

        onSwitchRecentlyPlayedTracksView: {
            listViews.setCurrentIndex(pageModel.indexFromViewType(viewType))

            while(browseStackView.depth > expectedDepth) {
                browseStackView.pop()
            }

            browseStackView.push(allRecentlyPlayedTracksView, {
                                     viewType: viewType,
                                     mainTitle: mainTitle,
                                     image: imageUrl,
                                     modelType: dataType,
                                     stackView: browseStackView,
                                     opacity: 0,
                                 })
        }

        onSwitchFrequentlyPlayedTracksView: {
            listViews.setCurrentIndex(pageModel.indexFromViewType(viewType))

            while(browseStackView.depth > expectedDepth) {
                browseStackView.pop()
            }

            browseStackView.push(allFrequentlyPlayedTracksView, {
                                     viewType: viewType,
                                     mainTitle: mainTitle,
                                     image: imageUrl,
                                     modelType: dataType,
                                     stackView: browseStackView,
                                     opacity: 0,
                                 })
        }

        onOpenGridView: {
            if (expectedDepth === 1) {
                listViews.setCurrentIndex(pageModel.indexFromViewType(viewType))
            }

            while(browseStackView.depth > expectedDepth) {
                browseStackView.pop()
            }

            browseStackView.push(dataGridView, {
                                     viewType: viewType,
                                     mainTitle: pageModel.viewMainTitle(viewType, mainTitle),
                                     secondaryTitle: secondaryTitle,
                                     image: pageModel.viewImageUrl(viewType, imageUrl),
                                     modelType: dataType,
                                     defaultIcon: viewDefaultIcon,
                                     showRating: viewShowRating,
                                     delegateDisplaySecondaryText: viewDelegateDisplaySecondaryText,
                                     genreFilterText: genreNameFilter,
                                     artistFilter: artistNameFilter,
                                     isSubPage: (browseStackView.depth >= 2),
                                     stackView: browseStackView,
                                     opacity: 0,
                                 })
        }

        onSwitchOneAlbumView: {
            while(browseStackView.depth > expectedDepth) {
                browseStackView.pop()
            }

            browseStackView.push(albumView, {
                                     viewType: viewType,
                                     mainTitle: mainTitle,
                                     secondaryTitle: secondaryTitle,
                                     image: imageUrl,
                                     stackView: browseStackView,
                                     opacity: 0,
                                 })
        }

        onSwitchAllTracksView: {
            listViews.setCurrentIndex(pageModel.indexFromViewType(viewType))

            while(browseStackView.depth > expectedDepth) {
                browseStackView.pop()
            }

            browseStackView.push(allTracksView, {
                                     viewType: viewType,
                                     mainTitle: mainTitle,
                                     image: imageUrl,
                                     modelType: dataType,
                                     stackView: browseStackView,
                                     opacity: 0,
                                 })
        }

        onSwitchFilesBrowserView: {
            listViews.setCurrentIndex(pageModel.indexFromViewType(viewType))

            while(browseStackView.depth > expectedDepth) {
                browseStackView.pop()
            }

            browseStackView.push(filesBrowserView, {
                                     viewType: viewType,
                                     mainTitle: mainTitle,
                                     image: imageUrl,
                                     opacity: 0,
                                 })
        }

        onPopOneView: {
            browseStackView.pop()
        }
    }

    ViewsModel {
        id: pageModel
    }

    ViewSelector {
        id: listViews

        model: pageModel

        Layout.fillHeight: true
        Layout.maximumWidth: mainWindow.width * 0.14

        maximumSize: mainWindow.width * 0.14

        Behavior on Layout.maximumWidth {
            NumberAnimation {
                duration: 150
            }
        }

        onSwitchView: viewManager.openParentView(viewType, pageModel.viewMainTitle(viewType, ""), pageModel.viewImageUrl(viewType, ""))
    }

    Rectangle {
        id: viewSelectorSeparatorItem

        border.width: 1
        border.color: myPalette.mid
        color: myPalette.mid
        visible: true

        Layout.bottomMargin: elisaTheme.layoutVerticalMargin
        Layout.topMargin: elisaTheme.layoutVerticalMargin

        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

        Layout.fillHeight: true

        Layout.preferredWidth: 1
        Layout.minimumWidth: 1
        Layout.maximumWidth: 1
    }

    ColumnLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true

        spacing: 0

        TopNotification {
            id: invalidBalooConfiguration

            Layout.fillWidth: true

            musicManager: elisa.musicManager

            focus: true
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent

                spacing: 0

                id: contentZone

                FocusScope {
                    id: mainContentView

                    focus: true

                    Layout.fillHeight: true

                    Layout.minimumWidth: 0
                    Layout.maximumWidth: 0
                    Layout.preferredWidth: 0


                    visible: Layout.minimumWidth != 0

                    Rectangle {
                        border {
                            color: (mainContentView.activeFocus ? myPalette.highlight : myPalette.base)
                            width: 1
                        }

                        radius: 3
                        color: myPalette.base

                        anchors.fill: parent

                        StackView {
                            id: browseStackView

                            anchors.fill: parent

                            clip: true

                            initialItem: Item {
                            }

                            popEnter: Transition {
                                OpacityAnimator {
                                    from: 0.0
                                    to: 1.0
                                    duration: 300
                                }
                            }

                            popExit: Transition {
                                OpacityAnimator {
                                    from: 1.0
                                    to: 0.0
                                    duration: 300
                                }
                            }

                            pushEnter: Transition {
                                OpacityAnimator {
                                    from: 0.0
                                    to: 1.0
                                    duration: 300
                                }
                            }

                            pushExit: Transition {
                                OpacityAnimator {
                                    from: 1.0
                                    to: 0.0
                                    duration: 300
                                }
                            }

                            replaceEnter: Transition {
                                OpacityAnimator {
                                    from: 0.0
                                    to: 1.0
                                    duration: 300
                                }
                            }

                            replaceExit: Transition {
                                OpacityAnimator {
                                    from: 1.0
                                    to: 0.0
                                    duration: 300
                                }
                            }
                        }

                        Behavior on border.color {
                            ColorAnimation {
                                duration: 300
                            }
                        }
                    }
                }

                Rectangle {
                    id: firstViewSeparatorItem

                    border.width: 1
                    border.color: myPalette.mid
                    color: myPalette.mid
                    visible: true

                    Layout.bottomMargin: elisaTheme.layoutVerticalMargin
                    Layout.topMargin: elisaTheme.layoutVerticalMargin

                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

                    Layout.fillHeight: true

                    Layout.preferredWidth: 1
                    Layout.minimumWidth: 1
                    Layout.maximumWidth: 1
                }

                MediaPlayListView {
                    id: playList

                    Layout.fillHeight: true
                    Layout.leftMargin: elisaTheme.layoutHorizontalMargin
                    Layout.rightMargin: elisaTheme.layoutHorizontalMargin

                    Layout.minimumWidth: contentZone.width
                    Layout.maximumWidth: contentZone.width
                    Layout.preferredWidth: contentZone.width

                    onStartPlayback: elisa.audioControl.ensurePlay()

                    onPausePlayback: elisa.audioControl.playPause()

                    onDisplayError: messageNotification.showNotification(errorText)
                }

                Rectangle {
                    id: viewSeparatorItem

                    border.width: 1
                    border.color: myPalette.mid
                    color: myPalette.mid
                    visible: Layout.minimumWidth != 0

                    Layout.bottomMargin: elisaTheme.layoutVerticalMargin
                    Layout.topMargin: elisaTheme.layoutVerticalMargin

                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

                    Layout.fillHeight: true

                    Layout.preferredWidth: 1
                    Layout.minimumWidth: 1
                    Layout.maximumWidth: 1
                }

                ContextView {
                    id: albumContext

                    Layout.fillHeight: true

                    Layout.minimumWidth: contentZone.width
                    Layout.maximumWidth: contentZone.width
                    Layout.preferredWidth: contentZone.width

                    visible: Layout.minimumWidth != 0

                    artistName: elisa.manageHeaderBar.artist
                    albumName: elisa.manageHeaderBar.album
                    albumArtUrl: elisa.manageHeaderBar.image
                }
            }
        }

        states: [
            State {
                name: 'playList'
                when: listViews.currentIndex === 0
                PropertyChanges {
                    target: mainContentView
                    Layout.minimumWidth: 0
                    Layout.maximumWidth: 0
                    Layout.preferredWidth: 0
                }
                PropertyChanges {
                    target: firstViewSeparatorItem
                    Layout.minimumWidth: 0
                    Layout.maximumWidth: 0
                    Layout.preferredWidth: 0
                }
                PropertyChanges {
                    target: playList
                    Layout.minimumWidth: contentZone.width / 2
                    Layout.maximumWidth: contentZone.width / 2
                    Layout.preferredWidth: contentZone.width / 2
                }
                PropertyChanges {
                    target: viewSeparatorItem
                    Layout.minimumWidth: 1
                    Layout.maximumWidth: 1
                    Layout.preferredWidth: 1
                }
                PropertyChanges {
                    target: albumContext
                    Layout.minimumWidth: contentZone.width / 2
                    Layout.maximumWidth: contentZone.width / 2
                    Layout.preferredWidth: contentZone.width / 2
                }
            },
            State {
                name: "browsingViewsNoPlaylist"
                when: listViews.currentIndex !== 0 && contentViewContainer.showPlaylist !== true
                extend: "browsingViews"
                PropertyChanges {
                    target: mainContentView
                    Layout.minimumWidth: contentZone.width
                    Layout.maximumWidth: contentZone.width
                    Layout.preferredWidth: contentZone.width
                }
                PropertyChanges {
                    target: playList
                    Layout.minimumWidth: 0
                    Layout.maximumWidth: 0
                    Layout.preferredWidth: 0
                }
            },
            State {
                name: 'browsingViews'
                when: listViews.currentIndex !== 0
                PropertyChanges {
                    target: mainContentView
                    Layout.minimumWidth: contentZone.width * 0.66
                    Layout.maximumWidth: contentZone.width * 0.68
                    Layout.preferredWidth: contentZone.width * 0.68
                }
                PropertyChanges {
                    target: firstViewSeparatorItem
                    Layout.minimumWidth: 1
                    Layout.maximumWidth: 1
                    Layout.preferredWidth: 1
                }
                PropertyChanges {
                    target: playList
                    Layout.minimumWidth: contentZone.width * 0.33
                    Layout.maximumWidth: contentZone.width * 0.33
                    Layout.preferredWidth: contentZone.width * 0.33
                }
                PropertyChanges {
                    target: viewSeparatorItem
                    Layout.minimumWidth: 0
                    Layout.maximumWidth: 0
                    Layout.preferredWidth: 0
                }
                PropertyChanges {
                    target: albumContext
                    Layout.minimumWidth: 0
                    Layout.maximumWidth: 0
                    Layout.preferredWidth: 0
                }
            }
        ]
        transitions: Transition {
            NumberAnimation {
                properties: "Layout.minimumWidth, Layout.maximumWidth, Layout.preferredWidth, opacity"
                easing.type: Easing.InOutQuad
                duration: 300
            }
        }
    }

    Component {
        id: allFrequentlyPlayedTracksView

        FrequentlyPlayedTracks {
            StackView.onActivated: viewManager.viewIsLoaded(viewType)
        }
    }

    Component {
        id: allRecentlyPlayedTracksView

        RecentlyPlayedTracks {
            StackView.onActivated: viewManager.viewIsLoaded(viewType)
        }
    }

    Component {
        id: dataGridView

        DataGridView {
            StackView.onActivated: viewManager.viewIsLoaded(viewType)
        }
    }

    Component {
        id: allTracksView

        TracksView {
            StackView.onActivated: viewManager.viewIsLoaded(viewType)
        }
    }

    Component {
        id: albumView

        AlbumView {
            StackView.onActivated: viewManager.viewIsLoaded(viewType)
        }
    }

    Component {
        id: filesBrowserView

        FileBrowserView {
            StackView.onActivated: viewManager.viewIsLoaded(viewType)
        }
    }
}
