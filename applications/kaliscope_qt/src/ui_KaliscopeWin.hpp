/********************************************************************************
** Form generated from reading UI file 'KaliscopeWin.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef KALISCOPEWIN_H
#define KALISCOPEWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
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
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *layoutVideo;
    QOpenGLWidget *openglView;
    QLabel *lblCurrentTrack;
    QHBoxLayout *horizontalLayout_3;
    QSlider *sliderPosition;
    QLabel *lblTrackLength;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QToolButton *btnSettings;
    QHBoxLayout *layoutButtonsBar;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *KaliscopeWin)
    {
        if (KaliscopeWin->objectName().isEmpty())
            KaliscopeWin->setObjectName(QStringLiteral("KaliscopeWin"));
        KaliscopeWin->resize(718, 510);
        QIcon icon;
        icon.addFile(QStringLiteral(":/kaliscope/icons/app/kaliscope.png"), QSize(), QIcon::Normal, QIcon::Off);
        KaliscopeWin->setWindowIcon(icon);
        centralwidget = new QWidget(KaliscopeWin);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        layoutVideo = new QVBoxLayout();
        layoutVideo->setObjectName(QStringLiteral("layoutVideo"));
        openglView = new QOpenGLWidget(centralwidget);
        openglView->setObjectName(QStringLiteral("openglView"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(openglView->sizePolicy().hasHeightForWidth());
        openglView->setSizePolicy(sizePolicy);

        layoutVideo->addWidget(openglView);

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
        KaliscopeWin->setMenuBar(menubar);
        statusbar = new QStatusBar(KaliscopeWin);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        KaliscopeWin->setStatusBar(statusbar);

        retranslateUi(KaliscopeWin);

        QMetaObject::connectSlotsByName(KaliscopeWin);
    } // setupUi

    void retranslateUi(QMainWindow *KaliscopeWin)
    {
        KaliscopeWin->setWindowTitle(QApplication::translate("KaliscopeWin", "Kaliscope film scanner", 0));
        lblCurrentTrack->setText(QString());
        lblTrackLength->setText(QString());
        btnSettings->setText(QApplication::translate("KaliscopeWin", "S", 0));
    } // retranslateUi

};

namespace Ui {
    class KaliscopeWin: public Ui_KaliscopeWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // KALISCOPEWIN_H
