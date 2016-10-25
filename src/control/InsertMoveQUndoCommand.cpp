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

#include "InsertMoveQUndoCommand.h"
#include "ToolboxItemGroup.h"
#include "ViewItem.h"
#include "UndoSingleton.h"

#include <QQmlProperty>
#include <QQuickItem>

InsertMoveQUndoCommand::InsertMoveQUndoCommand(ViewItem* anViewItemPtr,
                                       QQuickItem* aHandlePtr,
                                       QUndoCommand *parent)
    : AbstractQUndoCommand(anViewItemPtr, aHandlePtr,
                           QObject::tr("Insert %1"), parent)
{
    theVIConnection = connect(anViewItemPtr, SIGNAL(signalUpdateVars(qreal,qreal,qreal,qreal,qreal)),
                              this, SLOT(slot_updateVars(qreal,qreal,qreal,qreal,qreal)));

}

void InsertMoveQUndoCommand::commit()
{
    disconnect(theVIConnection);
    AbstractQUndoCommand::commit();
}

QQuickItem *InsertMoveQUndoCommand::getTheDecorated()
{
    return getVIPtr();
}

void InsertMoveQUndoCommand::doneMoving()
{
    theOrigPos = theNewPos;
    commit();
}

bool InsertMoveQUndoCommand::isChanged()
{
    if (areQRealsTheSame(theNewPos.x,theOrigPos.x)
            && areQRealsTheSame(theNewPos.y, theOrigPos.y)) {
        return false;
    }
    return true;
}

void InsertMoveQUndoCommand::redo()
{
    // REDO:
    // 1) (if VI gone): take AO out of the TIG and create a new VI
    // 2) (if VI exists): do little updating only
    assert(nullptr != theTIGPtr);
    if (nullptr == theAOPtr)
    {
        theAOPtr = theTIGPtr->getAOfromToolbox();
        updateAO(theNewPos);
        theAOPtr->createViewItem();
    }
    updateAO(theNewPos);
    updateVI();
    AbstractQUndoCommand::redo();
}

void InsertMoveQUndoCommand::setToolboxItemGroupPtr(ToolboxItemGroup *aTIGPtr)
{
    assert(nullptr != aTIGPtr);
    theTIGPtr = aTIGPtr;
}

void InsertMoveQUndoCommand::slot_updateVars(qreal anXM, qreal aYM, qreal /*aRotDegrees*/, qreal /*aWidthM*/, qreal /*aHeightM*/)
{
    theNewPos = Position(anXM, aYM, theOrigPos.angle);
    if (!isObjectColliding())
        theLastKnownGood = theNewPos;
    updateAO(theNewPos);
    updateVI();
}

void InsertMoveQUndoCommand::undo()
{
    // UNDO: Take AO from world, return AO to the ToolboxItemGroup, delete VI.
    assert(nullptr != theTIGPtr);
    theAOPtr->deleteViewItem();
    assert(nullptr==getVIPtr());

    theTIGPtr->returnAO2Toolbox(theAOPtr);
    theAOPtr = nullptr;

    AbstractQUndoCommand::undo();
}
