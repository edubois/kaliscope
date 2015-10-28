/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

/********************************************************************************
** Form generated from reading UI file 'RecordingSettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef RECORDINGSETTINGSDIALOG_H
#define RECORDINGSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RecordingSettingsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *comboInput;
    QToolButton *btnBrowseInput;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *editOutput;
    QToolButton *btnBrowseOutput;
    QLabel *label_4;
    QLineEdit *editPrefix;
    QLabel *label_8;
    QLineEdit *editExtension;
    QCheckBox *cbInputIsSequence;
    QCheckBox *cbOutputIsSequence;
    QHBoxLayout *horizontalLayout_5;
    QLabel *labelEstimatedInputMins;
    QSpinBox *spinInputMinutes;
    QLabel *label_5;
    QDoubleSpinBox *spinFps;
    QLabel *label_6;
    QSpinBox *spinNbImg;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer;
    QFrame *line;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QToolButton *btnAddPlugin;
    QToolButton *btnRemovePlugin;
    QToolButton *btnLoadConfig;
    QToolButton *btnSaveConfig;
    QCheckBox *cbDefault;
    QComboBox *comboPresets;
    QListWidget *listPipeline;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *RecordingSettingsDialog)
    {
        if (RecordingSettingsDialog->objectName().isEmpty())
            RecordingSettingsDialog->setObjectName(QStringLiteral("RecordingSettingsDialog"));
        RecordingSettingsDialog->resize(722, 481);
        QIcon icon;
        icon.addFile(QStringLiteral(":/kaliscope/icons/app/kaliscope.png"), QSize(), QIcon::Normal, QIcon::Off);
        RecordingSettingsDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(RecordingSettingsDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(RecordingSettingsDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetMaximumSize);
        comboInput = new QComboBox(groupBox);
        comboInput->setObjectName(QStringLiteral("comboInput"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboInput->sizePolicy().hasHeightForWidth());
        comboInput->setSizePolicy(sizePolicy);
        comboInput->setEditable(true);

        horizontalLayout_2->addWidget(comboInput);

        btnBrowseInput = new QToolButton(groupBox);
        btnBrowseInput->setObjectName(QStringLiteral("btnBrowseInput"));

        horizontalLayout_2->addWidget(btnBrowseInput);


        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout_2);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetMinAndMaxSize);
        editOutput = new QLineEdit(groupBox);
        editOutput->setObjectName(QStringLiteral("editOutput"));
        sizePolicy.setHeightForWidth(editOutput->sizePolicy().hasHeightForWidth());
        editOutput->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(editOutput);

        btnBrowseOutput = new QToolButton(groupBox);
        btnBrowseOutput->setObjectName(QStringLiteral("btnBrowseOutput"));

        horizontalLayout_3->addWidget(btnBrowseOutput);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(label_4);

        editPrefix = new QLineEdit(groupBox);
        editPrefix->setObjectName(QStringLiteral("editPrefix"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(editPrefix->sizePolicy().hasHeightForWidth());
        editPrefix->setSizePolicy(sizePolicy2);
        editPrefix->setMaximumSize(QSize(96, 16777215));

        horizontalLayout_3->addWidget(editPrefix);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QStringLiteral("label_8"));
        sizePolicy1.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(label_8);

        editExtension = new QLineEdit(groupBox);
        editExtension->setObjectName(QStringLiteral("editExtension"));
        sizePolicy2.setHeightForWidth(editExtension->sizePolicy().hasHeightForWidth());
        editExtension->setSizePolicy(sizePolicy2);
        editExtension->setMaximumSize(QSize(64, 16777215));

        horizontalLayout_3->addWidget(editExtension);


        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout_3);


        verticalLayout_2->addLayout(formLayout);

        cbInputIsSequence = new QCheckBox(groupBox);
        cbInputIsSequence->setObjectName(QStringLiteral("cbInputIsSequence"));

        verticalLayout_2->addWidget(cbInputIsSequence);

        cbOutputIsSequence = new QCheckBox(groupBox);
        cbOutputIsSequence->setObjectName(QStringLiteral("cbOutputIsSequence"));

        verticalLayout_2->addWidget(cbOutputIsSequence);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(-1, 0, -1, -1);
        labelEstimatedInputMins = new QLabel(groupBox);
        labelEstimatedInputMins->setObjectName(QStringLiteral("labelEstimatedInputMins"));

        horizontalLayout_5->addWidget(labelEstimatedInputMins);

        spinInputMinutes = new QSpinBox(groupBox);
        spinInputMinutes->setObjectName(QStringLiteral("spinInputMinutes"));
        spinInputMinutes->setMinimum(1);
        spinInputMinutes->setMaximum(999999999);

        horizontalLayout_5->addWidget(spinInputMinutes);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        sizePolicy1.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy1);
        label_5->setMinimumSize(QSize(0, 0));
        label_5->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_5->addWidget(label_5);

        spinFps = new QDoubleSpinBox(groupBox);
        spinFps->setObjectName(QStringLiteral("spinFps"));
        spinFps->setDecimals(3);
        spinFps->setMaximum(999);
        spinFps->setValue(24);

        horizontalLayout_5->addWidget(spinFps);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_5->addWidget(label_6);

        spinNbImg = new QSpinBox(groupBox);
        spinNbImg->setObjectName(QStringLiteral("spinNbImg"));
        spinNbImg->setReadOnly(false);
        spinNbImg->setMaximum(999999999);
        spinNbImg->setValue(1440);

        horizontalLayout_5->addWidget(spinNbImg);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_5->addWidget(label_7);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_5);


        verticalLayout->addWidget(groupBox);

        line = new QFrame(RecordingSettingsDialog);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        label_3 = new QLabel(RecordingSettingsDialog);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnAddPlugin = new QToolButton(RecordingSettingsDialog);
        btnAddPlugin->setObjectName(QStringLiteral("btnAddPlugin"));

        horizontalLayout->addWidget(btnAddPlugin);

        btnRemovePlugin = new QToolButton(RecordingSettingsDialog);
        btnRemovePlugin->setObjectName(QStringLiteral("btnRemovePlugin"));

        horizontalLayout->addWidget(btnRemovePlugin);

        btnLoadConfig = new QToolButton(RecordingSettingsDialog);
        btnLoadConfig->setObjectName(QStringLiteral("btnLoadConfig"));

        horizontalLayout->addWidget(btnLoadConfig);

        btnSaveConfig = new QToolButton(RecordingSettingsDialog);
        btnSaveConfig->setObjectName(QStringLiteral("btnSaveConfig"));

        horizontalLayout->addWidget(btnSaveConfig);

        cbDefault = new QCheckBox(RecordingSettingsDialog);
        cbDefault->setObjectName(QStringLiteral("cbDefault"));
        sizePolicy2.setHeightForWidth(cbDefault->sizePolicy().hasHeightForWidth());
        cbDefault->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(cbDefault);

        comboPresets = new QComboBox(RecordingSettingsDialog);
        comboPresets->setObjectName(QStringLiteral("comboPresets"));
        comboPresets->setEditable(true);

        horizontalLayout->addWidget(comboPresets);


        verticalLayout->addLayout(horizontalLayout);

        listPipeline = new QListWidget(RecordingSettingsDialog);
        listPipeline->setObjectName(QStringLiteral("listPipeline"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(listPipeline->sizePolicy().hasHeightForWidth());
        listPipeline->setSizePolicy(sizePolicy3);
        listPipeline->setMaximumSize(QSize(16777215, 192));
        listPipeline->setAutoScroll(true);
        listPipeline->setTabKeyNavigation(true);
        listPipeline->setIconSize(QSize(0, 0));
        listPipeline->setFlow(QListView::LeftToRight);
        listPipeline->setResizeMode(QListView::Fixed);
        listPipeline->setGridSize(QSize(0, 0));
        listPipeline->setViewMode(QListView::ListMode);
        listPipeline->setUniformItemSizes(true);
        listPipeline->setWordWrap(true);
        listPipeline->setSelectionRectVisible(true);

        verticalLayout->addWidget(listPipeline);

        buttonBox = new QDialogButtonBox(RecordingSettingsDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        QWidget::setTabOrder(comboInput, btnBrowseInput);
        QWidget::setTabOrder(btnBrowseInput, editOutput);
        QWidget::setTabOrder(editOutput, btnBrowseOutput);
        QWidget::setTabOrder(btnBrowseOutput, editPrefix);
        QWidget::setTabOrder(editPrefix, editExtension);
        QWidget::setTabOrder(editExtension, cbInputIsSequence);
        QWidget::setTabOrder(cbInputIsSequence, cbOutputIsSequence);
        QWidget::setTabOrder(cbOutputIsSequence, spinInputMinutes);
        QWidget::setTabOrder(spinInputMinutes, spinFps);
        QWidget::setTabOrder(spinFps, spinNbImg);
        QWidget::setTabOrder(spinNbImg, btnAddPlugin);
        QWidget::setTabOrder(btnAddPlugin, btnRemovePlugin);
        QWidget::setTabOrder(btnRemovePlugin, btnLoadConfig);
        QWidget::setTabOrder(btnLoadConfig, btnSaveConfig);
        QWidget::setTabOrder(btnSaveConfig, comboPresets);
        QWidget::setTabOrder(comboPresets, listPipeline);

        retranslateUi(RecordingSettingsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), RecordingSettingsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), RecordingSettingsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(RecordingSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *RecordingSettingsDialog)
    {
        RecordingSettingsDialog->setWindowTitle(QApplication::translate("RecordingSettingsDialog", "Recording settings", 0));
        groupBox->setTitle(QApplication::translate("RecordingSettingsDialog", "Paths configurations", 0));
        label->setText(QApplication::translate("RecordingSettingsDialog", "Input source:", 0));
        btnBrowseInput->setText(QApplication::translate("RecordingSettingsDialog", "...", 0));
        label_2->setText(QApplication::translate("RecordingSettingsDialog", "Output directory:", 0));
        btnBrowseOutput->setText(QApplication::translate("RecordingSettingsDialog", "...", 0));
        label_4->setText(QApplication::translate("RecordingSettingsDialog", "Prefix:", 0));
        editPrefix->setText(QApplication::translate("RecordingSettingsDialog", "output_", 0));
        label_8->setText(QApplication::translate("RecordingSettingsDialog", "Extension:", 0));
        editExtension->setInputMask(QString());
        cbInputIsSequence->setText(QApplication::translate("RecordingSettingsDialog", "Input is a sequence", 0));
        cbOutputIsSequence->setText(QApplication::translate("RecordingSettingsDialog", "Output is a sequence", 0));
#ifndef QT_NO_TOOLTIP
        labelEstimatedInputMins->setToolTip(QApplication::translate("RecordingSettingsDialog", "Used to set the right filename's number of padding '0'", 0));
#endif // QT_NO_TOOLTIP
        labelEstimatedInputMins->setText(QApplication::translate("RecordingSettingsDialog", "Estimated max input minutes:", 0));
        label_5->setText(QApplication::translate("RecordingSettingsDialog", "at fps:", 0));
        label_6->setText(QApplication::translate("RecordingSettingsDialog", "Total:", 0));
        label_7->setText(QApplication::translate("RecordingSettingsDialog", "images.", 0));
        label_3->setText(QApplication::translate("RecordingSettingsDialog", "Pipeline (ofx plugins linear chaining):", 0));
        btnAddPlugin->setText(QApplication::translate("RecordingSettingsDialog", "+", 0));
        btnRemovePlugin->setText(QApplication::translate("RecordingSettingsDialog", "-", 0));
        btnLoadConfig->setText(QApplication::translate("RecordingSettingsDialog", "Load", 0));
        btnSaveConfig->setText(QApplication::translate("RecordingSettingsDialog", "Save", 0));
        cbDefault->setText(QApplication::translate("RecordingSettingsDialog", "Default", 0));
#ifndef QT_NO_TOOLTIP
        comboPresets->setToolTip(QApplication::translate("RecordingSettingsDialog", "preset name", 0));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class RecordingSettingsDialog: public Ui_RecordingSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // RECORDINGSETTINGSDIALOG_H
