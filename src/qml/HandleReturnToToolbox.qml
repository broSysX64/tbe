/* The Butterfly Effect
 * This file copyright (C) 2016 Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

import QtQuick 2.5

Image {
    id: returnToToolboxHandle

    anchors.right: parent.right
    anchors.verticalCenter: parent.verticalCenter

    rotation : - theDecorated.rotation;
    source: img("ActionReturnToolbox")

    visible: theDecorator.isBackInToolbox === true;

    width: ResizeInfo.handleWidth * 1.5
    height: ResizeInfo.handleHeight * 1.5
    sourceSize.width: width
    sourceSize.height: height

    // TODO: on destruction?
}
