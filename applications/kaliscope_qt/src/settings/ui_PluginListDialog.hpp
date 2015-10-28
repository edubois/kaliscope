/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

/********************************************************************************
** Form generated from reading UI file 'PluginListDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PLUGINLISTDIALOG_H
#define PLUGINLISTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PluginListDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *layoutPlugins;
    QLabel *labelPlugins;
    QListWidget *listPlugins;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PluginListDialog)
    {
        if (PluginListDialog->objectName().isEmpty())
            PluginListDialog->setObjectName(QStringLiteral("PluginListDialog"));
        PluginListDialog->resize(323, 352);
        QIcon icon;
        icon.addFile(QStringLiteral(":/kaliscope/icons/plugins/puzzle.png"), QSize(), QIcon::Normal, QIcon::Off);
        PluginListDialog->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(PluginListDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        layoutPlugins = new QVBoxLayout();
        layoutPlugins->setObjectName(QStringLiteral("layoutPlugins"));
        labelPlugins = new QLabel(PluginListDialog);
        labelPlugins->setObjectName(QStringLiteral("labelPlugins"));

        layoutPlugins->addWidget(labelPlugins);

        listPlugins = new QListWidget(PluginListDialog);
        listPlugins->setObjectName(QStringLiteral("listPlugins"));
        listPlugins->setIconSize(QSize(32, 32));
        listPlugins->setViewMode(QListView::ListMode);
        listPlugins->setUniformItemSizes(true);
        listPlugins->setSortingEnabled(true);

        layoutPlugins->addWidget(listPlugins);


        verticalLayout_2->addLayout(layoutPlugins);

        buttonBox = new QDialogButtonBox(PluginListDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(PluginListDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), PluginListDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PluginListDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(PluginListDialog);
    } // setupUi

    void retranslateUi(QDialog *PluginListDialog)
    {
        PluginListDialog->setWindowTitle(QApplication::translate("PluginListDialog", "Select a plugin", 0));
        labelPlugins->setText(QApplication::translate("PluginListDialog", "Available plugins:", 0));
    } // retranslateUi

};

namespace Ui {
    class PluginListDialog: public Ui_PluginListDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PLUGINLISTDIALOG_H
