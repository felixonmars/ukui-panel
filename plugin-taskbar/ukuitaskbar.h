/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Copyright: 2011 Razor team
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *   Maciej Płaza <plaza.maciej@gmail.com>
 *   Kuzma Shapran <kuzma.shapran@gmail.com>
 *
 * Copyright: 2019 Tianjin KYLIN Information Technology Co., Ltd. *
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */


#ifndef UKUITASKBAR_H
#define UKUITASKBAR_H

#include "../panel/iukuipanel.h"
#include "../panel/iukuipanelplugin.h"
#include "ukuitaskbarconfiguration.h"
#include "ukuitaskgroup.h"
#include "ukuitaskbutton.h"

#include <QFrame>
#include <QBoxLayout>
#include <QMap>
//#include <lxqt-globalkeys.h>
#include "../panel/iukuipanel.h"
#include <KWindowSystem/KWindowSystem>
#include <KWindowSystem/KWindowInfo>
#include <KWindowSystem/NETWM>

class QSignalMapper;
class UKUITaskButton;
class ElidedButtonStyle;

namespace UKUi {
class GridLayout;
}

class UKUITaskBar : public QFrame
{
    Q_OBJECT

public:
    explicit UKUITaskBar(IUKUIPanelPlugin *plugin, QWidget* parent = 0);
    virtual ~UKUITaskBar();

    void realign();

    Qt::ToolButtonStyle buttonStyle() const { return mButtonStyle; }
    int buttonWidth() const { return mButtonWidth; }
    bool closeOnMiddleClick() const { return mCloseOnMiddleClick; }
    bool raiseOnCurrentDesktop() const { return mRaiseOnCurrentDesktop; }
    bool isShowOnlyOneDesktopTasks() const { return mShowOnlyOneDesktopTasks; }
    int showDesktopNum() const { return mShowDesktopNum; }
    bool isShowOnlyCurrentScreenTasks() const { return mShowOnlyCurrentScreenTasks; }
    bool isShowOnlyMinimizedTasks() const { return mShowOnlyMinimizedTasks; }
    bool isAutoRotate() const { return mAutoRotate; }
    bool isGroupingEnabled() const { return mGroupingEnabled; }
    bool isShowGroupOnHover() const { return mShowGroupOnHover; }
    bool isIconByClass() const { return mIconByClass; }
    void setShowGroupOnHover(bool bFlag);
    inline IUKUIPanel * panel() const { return mPlugin->panel(); }
    inline IUKUIPanelPlugin * plugin() const { return mPlugin; }

public slots:
    void settingsChanged();

signals:
    void buttonRotationRefreshed(bool autoRotate, IUKUIPanel::Position position);
    void buttonStyleRefreshed(Qt::ToolButtonStyle buttonStyle);
    void refreshIconGeometry();
    void showOnlySettingChanged();
    void iconByClassChanged();
    void popupShown(UKUITaskGroup* sender);

protected:
    virtual void dragEnterEvent(QDragEnterEvent * event);
    virtual void dragMoveEvent(QDragMoveEvent * event);

private slots:
    void refreshTaskList();
    void refreshButtonRotation();
    void refreshPlaceholderVisibility();
    void groupBecomeEmptySlot();
    void onWindowChanged(WId window, NET::Properties prop, NET::Properties2 prop2);
    void onWindowAdded(WId window);
    void onWindowRemoved(WId window);
    void registerShortcuts();
    void shortcutRegistered();
    void activateTask(int pos);

private:
    typedef QMap<WId, UKUITaskGroup*> windowMap_t;

private:
    void addWindow(WId window);
    windowMap_t::iterator removeWindow(windowMap_t::iterator pos);
    void buttonMove(UKUITaskGroup * dst, UKUITaskGroup * src, QPoint const & pos);

private:
    QMap<WId, UKUITaskGroup*> mKnownWindows; //!< Ids of known windows (mapping to buttons/groups)
    UKUi::GridLayout *mLayout;
//    QList<GlobalKeyShortcut::Action*> mKeys;
    QSignalMapper *mSignalMapper;

    // Settings
    Qt::ToolButtonStyle mButtonStyle;
    int mButtonWidth;
    int mButtonHeight;
    bool mCloseOnMiddleClick;
    bool mRaiseOnCurrentDesktop;
    bool mShowOnlyOneDesktopTasks;
    int mShowDesktopNum;
    bool mShowOnlyCurrentScreenTasks;
    bool mShowOnlyMinimizedTasks;
    bool mAutoRotate;
    bool mGroupingEnabled;
    bool mShowGroupOnHover;
    bool mIconByClass;
    bool mCycleOnWheelScroll; //!< flag for processing the wheelEvent

    bool acceptWindow(WId window) const;
    void setButtonStyle(Qt::ToolButtonStyle buttonStyle);

    void wheelEvent(QWheelEvent* event);
    void changeEvent(QEvent* event);
    void resizeEvent(QResizeEvent *event);

    IUKUIPanelPlugin *mPlugin;
    QWidget *mPlaceHolder;
    LeftAlignedTextStyle *mStyle;
};

#endif // UKUITASKBAR_H
