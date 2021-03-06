/* The Butterfly Effect
 * This file copyright (C) 2015  Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#ifndef HINT_H
#define HINT_H

#include "Property.h"
#include "HintSerializer.h"

// forward declarations
class World;
class AbstractObject;


/** Hints provide an object name and its location.
 *  If you apply a hint, andobject is removed from the toolbox
 *  and inserted into the world
 */
class Hint
{
public:
    Hint();
    virtual ~Hint();

    const QString &getHintInternalName() const
    {
        return theObjectName;
    }

    static const char *WIDTH_STRING;
    static const char *HEIGHT_STRING;
    static const char *ANGLE_STRING;
    static const char *XPOS_STRING;
    static const char *YPOS_STRING;

    /// only updates aFloat if the property aString exists
    /// @returns - true if property does not exist or if it was correctly read
    ///          - false if property parsing went wrong (e.g. X="lala" cannot be a float)
    bool updateFromHint(qreal &aFloat, const char *aString);

protected:
    int     theHintIndex;
    QString theObjectName;
    PropertyList theParams;
    friend class HintSerializer;
};


#endif // HINT_H
