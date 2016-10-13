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

#ifndef GAMEFLOW_H
#define GAMEFLOW_H

#include "GameStateMachine.h"
#include <QObject>

class MainWindow;
class QQuickItem;
class RequestDialog;


class GameFlow : public QObject
{
    Q_OBJECT

public:
    explicit GameFlow(MainWindow *parent, RequestDialog *anRDPtr);

    GameStateMachine* theGameStateMachinePtr;

signals:

private slots:
    void slot_levelDeath(void);
    void slot_levelWon(void);

public slots:
    void slot_clearWinFailDialogPtr();
    void slot_showGameResourcesDialog();


private:
    void setupWinFail(bool isAWin);

    MainWindow* theMainWindowPtr;
    QQuickItem* theNewWinFailDialogPtr;
    RequestDialog* theRequestDialogItfPtr;
};

#endif // GAMEFLOW_H