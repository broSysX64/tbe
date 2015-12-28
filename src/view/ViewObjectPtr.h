/* The Butterfly Effect
 * This file copyright (C) 2015 Klaas van Gend
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


#ifndef VIEWOBJECTPTR_H
#define VIEWOBJECTPTR_H

/// For making object lifetime code simpler, for ViewObject
/// we're going to use the QPointer type of Qt.
/// it will result NULL if the ViewObject we're pointing to, has been removed
#include <QtCore/QPointer>
class ViewObject;
typedef QPointer<ViewObject> ViewObjectPtr;

#endif // VIEWOBJECTPTR_H
