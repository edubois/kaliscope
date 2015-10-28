/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

/********************************************************************************
** Form generated from reading UI file 'EditPluginParamsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef EDITPLUGINPARAMSDIALOG_H
#define EDITPLUGINPARAMSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EditPluginParamsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *labelPluginName;
    QTreeWidget *treeParameters;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *EditPluginParamsDialog)
    {
        if (EditPluginParamsDialog->objectName().isEmpty())
            EditPluginParamsDialog->setObjectName(QStringLiteral("EditPluginParamsDialog"));
        EditPluginParamsDialog->resize(512, 393);
        QIcon icon;
        icon.addFile(QStringLiteral(":/kaliscope/icons/plugins/puzzle.png"), QSize(), QIcon::Normal, QIcon::Off);
        EditPluginParamsDialog->setWindowIcon(icon);
        EditPluginParamsDialog->setModal(true);
        verticalLayout = new QVBoxLayout(EditPluginParamsDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        labelPluginName = new QLabel(EditPluginParamsDialog);
        labelPluginName->setObjectName(QStringLiteral("labelPluginName"));

        verticalLayout->addWidget(labelPluginName);

        treeParameters = new QTreeWidget(EditPluginParamsDialog);
        treeParameters->setObjectName(QStringLiteral("treeParameters"));
        treeParameters->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        treeParameters->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeParameters->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeParameters->setSortingEnabled(true);
        treeParameters->header()->setCascadingSectionResizes(false);
        treeParameters->header()->setDefaultSectionSize(200);

        verticalLayout->addWidget(treeParameters);

        buttonBox = new QDialogButtonBox(EditPluginParamsDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(EditPluginParamsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), EditPluginParamsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), EditPluginParamsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(EditPluginParamsDialog);
    } // setupUi

    void retranslateUi(QDialog *EditPluginParamsDialog)
    {
        EditPluginParamsDialog->setWindowTitle(QApplication::translate("EditPluginParamsDialog", "Plugin parameters", 0));
        labelPluginName->setText(QString());
        QTreeWidgetItem *___qtreewidgetitem = treeParameters->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("EditPluginParamsDialog", "parameter value", 0));
        ___qtreewidgetitem->setText(0, QApplication::translate("EditPluginParamsDialog", "parameter name", 0));
    } // retranslateUi

};

namespace Ui {
    class EditPluginParamsDialog: public Ui_EditPluginParamsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // EDITPLUGINPARAMSDIALOG_H
