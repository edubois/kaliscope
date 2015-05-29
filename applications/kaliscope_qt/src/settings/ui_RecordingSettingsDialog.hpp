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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RecordingSettingsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *comboInput;
    QToolButton *btnBrowseInput;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *editOutput;
    QToolButton *btnBrowseOutput;
    QSpacerItem *verticalSpacer;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QToolButton *btnAddPlugin;
    QToolButton *btnRemovePlugin;
    QToolButton *btnLoadConfig;
    QToolButton *btnSaveConfig;
    QComboBox *comboPresets;
    QListWidget *listPipeline;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *RecordingSettingsDialog)
    {
        if (RecordingSettingsDialog->objectName().isEmpty())
            RecordingSettingsDialog->setObjectName(QStringLiteral("RecordingSettingsDialog"));
        RecordingSettingsDialog->resize(593, 375);
        QIcon icon;
        icon.addFile(QStringLiteral(":/kaliscope/icons/app/kaliscope.png"), QSize(), QIcon::Normal, QIcon::Off);
        RecordingSettingsDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(RecordingSettingsDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label = new QLabel(RecordingSettingsDialog);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetMaximumSize);
        comboInput = new QComboBox(RecordingSettingsDialog);
        comboInput->setObjectName(QStringLiteral("comboInput"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboInput->sizePolicy().hasHeightForWidth());
        comboInput->setSizePolicy(sizePolicy);
        comboInput->setEditable(true);

        horizontalLayout_2->addWidget(comboInput);

        btnBrowseInput = new QToolButton(RecordingSettingsDialog);
        btnBrowseInput->setObjectName(QStringLiteral("btnBrowseInput"));

        horizontalLayout_2->addWidget(btnBrowseInput);


        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout_2);

        label_2 = new QLabel(RecordingSettingsDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetMaximumSize);
        editOutput = new QLineEdit(RecordingSettingsDialog);
        editOutput->setObjectName(QStringLiteral("editOutput"));
        sizePolicy.setHeightForWidth(editOutput->sizePolicy().hasHeightForWidth());
        editOutput->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(editOutput);

        btnBrowseOutput = new QToolButton(RecordingSettingsDialog);
        btnBrowseOutput->setObjectName(QStringLiteral("btnBrowseOutput"));

        horizontalLayout_3->addWidget(btnBrowseOutput);


        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout_3);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

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

        comboPresets = new QComboBox(RecordingSettingsDialog);
        comboPresets->setObjectName(QStringLiteral("comboPresets"));
        comboPresets->setEditable(true);

        horizontalLayout->addWidget(comboPresets);


        verticalLayout->addLayout(horizontalLayout);

        listPipeline = new QListWidget(RecordingSettingsDialog);
        listPipeline->setObjectName(QStringLiteral("listPipeline"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(listPipeline->sizePolicy().hasHeightForWidth());
        listPipeline->setSizePolicy(sizePolicy1);
        listPipeline->setMaximumSize(QSize(16777215, 128));
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
        QWidget::setTabOrder(btnBrowseOutput, btnAddPlugin);
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
        label->setText(QApplication::translate("RecordingSettingsDialog", "Input source:", 0));
        btnBrowseInput->setText(QApplication::translate("RecordingSettingsDialog", "...", 0));
        label_2->setText(QApplication::translate("RecordingSettingsDialog", "Output directory:", 0));
        btnBrowseOutput->setText(QApplication::translate("RecordingSettingsDialog", "...", 0));
        label_3->setText(QApplication::translate("RecordingSettingsDialog", "Pipeline (ofx plugins linear chaining):", 0));
        btnAddPlugin->setText(QApplication::translate("RecordingSettingsDialog", "+", 0));
        btnRemovePlugin->setText(QApplication::translate("RecordingSettingsDialog", "-", 0));
        btnLoadConfig->setText(QApplication::translate("RecordingSettingsDialog", "Load", 0));
        btnSaveConfig->setText(QApplication::translate("RecordingSettingsDialog", "Save", 0));
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
