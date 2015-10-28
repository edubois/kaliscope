/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

/********************************************************************************
** Form generated from reading UI file 'KaliscopeWin.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef KALISCOPEWIN_H
#define KALISCOPEWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KaliscopeWin
{
public:
    QAction *action_About;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *layoutVideo;
    QVBoxLayout *layoutViewer;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *cbInvertColors;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lblCurrentTrack;
    QHBoxLayout *horizontalLayout_3;
    QSlider *sliderPosition;
    QLabel *lblTrackLength;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QToolButton *btnSettings;
    QToolButton *btnConnect;
    QHBoxLayout *layoutButtonsBar;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menubar;
    QMenu *menu_About;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *KaliscopeWin)
    {
        if (KaliscopeWin->objectName().isEmpty())
            KaliscopeWin->setObjectName(QStringLiteral("KaliscopeWin"));
        KaliscopeWin->resize(718, 514);
        QIcon icon;
        icon.addFile(QStringLiteral(":/kaliscope/icons/app/kaliscope.png"), QSize(), QIcon::Normal, QIcon::Off);
        KaliscopeWin->setWindowIcon(icon);
        action_About = new QAction(KaliscopeWin);
        action_About->setObjectName(QStringLiteral("action_About"));
        centralwidget = new QWidget(KaliscopeWin);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        layoutVideo = new QVBoxLayout();
        layoutVideo->setObjectName(QStringLiteral("layoutVideo"));
        layoutViewer = new QVBoxLayout();
        layoutViewer->setObjectName(QStringLiteral("layoutViewer"));
        layoutViewer->setSizeConstraint(QLayout::SetMaximumSize);

        layoutVideo->addLayout(layoutViewer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        cbInvertColors = new QCheckBox(centralwidget);
        cbInvertColors->setObjectName(QStringLiteral("cbInvertColors"));

        horizontalLayout_4->addWidget(cbInvertColors);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        layoutVideo->addLayout(horizontalLayout_4);

        lblCurrentTrack = new QLabel(centralwidget);
        lblCurrentTrack->setObjectName(QStringLiteral("lblCurrentTrack"));

        layoutVideo->addWidget(lblCurrentTrack);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        sliderPosition = new QSlider(centralwidget);
        sliderPosition->setObjectName(QStringLiteral("sliderPosition"));
        sliderPosition->setEnabled(false);
        sliderPosition->setMaximum(100);
        sliderPosition->setPageStep(1);
        sliderPosition->setOrientation(Qt::Horizontal);
        sliderPosition->setTickPosition(QSlider::TicksBelow);

        horizontalLayout_3->addWidget(sliderPosition);

        lblTrackLength = new QLabel(centralwidget);
        lblTrackLength->setObjectName(QStringLiteral("lblTrackLength"));

        horizontalLayout_3->addWidget(lblTrackLength);


        layoutVideo->addLayout(horizontalLayout_3);


        verticalLayout->addLayout(layoutVideo);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        btnSettings = new QToolButton(centralwidget);
        btnSettings->setObjectName(QStringLiteral("btnSettings"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/mvpplayer/toolbar/applications-settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSettings->setIcon(icon1);
        btnSettings->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnSettings);

        btnConnect = new QToolButton(centralwidget);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/mvpplayer/toolbar/network-settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnConnect->setIcon(icon2);
        btnConnect->setIconSize(QSize(32, 32));
        btnConnect->setCheckable(true);

        horizontalLayout->addWidget(btnConnect);


        horizontalLayout_2->addLayout(horizontalLayout);

        layoutButtonsBar = new QHBoxLayout();
        layoutButtonsBar->setObjectName(QStringLiteral("layoutButtonsBar"));
        layoutButtonsBar->setSizeConstraint(QLayout::SetMaximumSize);

        horizontalLayout_2->addLayout(layoutButtonsBar);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        KaliscopeWin->setCentralWidget(centralwidget);
        menubar = new QMenuBar(KaliscopeWin);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 718, 22));
        menu_About = new QMenu(menubar);
        menu_About->setObjectName(QStringLiteral("menu_About"));
        KaliscopeWin->setMenuBar(menubar);
        statusbar = new QStatusBar(KaliscopeWin);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        KaliscopeWin->setStatusBar(statusbar);
        QWidget::setTabOrder(cbInvertColors, sliderPosition);
        QWidget::setTabOrder(sliderPosition, btnSettings);

        menubar->addAction(menu_About->menuAction());
        menu_About->addAction(action_About);

        retranslateUi(KaliscopeWin);

        QMetaObject::connectSlotsByName(KaliscopeWin);
    } // setupUi

    void retranslateUi(QMainWindow *KaliscopeWin)
    {
        KaliscopeWin->setWindowTitle(QApplication::translate("KaliscopeWin", "Kaliscope film scanner", 0));
        action_About->setText(QApplication::translate("KaliscopeWin", "&About", 0));
        cbInvertColors->setText(QApplication::translate("KaliscopeWin", "Invert colors", 0));
        lblCurrentTrack->setText(QString());
        lblTrackLength->setText(QString());
        btnSettings->setText(QApplication::translate("KaliscopeWin", "S", 0));
        btnConnect->setText(QApplication::translate("KaliscopeWin", "...", 0));
        menu_About->setTitle(QApplication::translate("KaliscopeWin", "&About", 0));
    } // retranslateUi

};

namespace Ui {
    class KaliscopeWin: public Ui_KaliscopeWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // KALISCOPEWIN_H
