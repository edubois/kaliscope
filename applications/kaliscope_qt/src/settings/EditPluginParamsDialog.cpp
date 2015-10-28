/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

#include "EditPluginParamsDialog.hpp"

#include <kali-core/settingsTools.hpp>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>

namespace kaliscope
{
namespace gui
{
namespace qt
{

EditPluginParamsDialog::EditPluginParamsDialog( const std::string & pluginIdentifier, tuttle::host::INode & fxNode, mvpplayer::Settings *currentSettings, QWidget * parent )
: Parent( parent )
, _pluginIdentifier( pluginIdentifier )
, _fxNode( fxNode )
{
    widget.setupUi(this);
    widget.labelPluginName->setText( QString( "Plugin : " ) + QString::fromStdString( fxNode.getLabel() ) );
    
    widget.treeParameters->setEditTriggers( QAbstractItemView::NoEditTriggers );
    widget.treeParameters->setColumnCount( 2 );
    connect( widget.treeParameters, SIGNAL( itemDoubleClicked( QTreeWidgetItem *, int ) ), this, SLOT( onTreeWidgetItemDoubleClicked( QTreeWidgetItem *, int ) ) );

    QTreeWidgetItem *root = new QTreeWidgetItem( QStringList() << QString::fromStdString( fxNode.getLabel() ) );
    widget.treeParameters->addTopLevelItem( root );

    // Add parameters to the tree
    if ( currentSettings )
    {
        // set the initial settings to the node
        _nodeSettings = *currentSettings;
        setNodeSettings( _pluginIdentifier, _fxNode, _nodeSettings );
    }

    for( const auto & p: _fxNode.getParamSet().getParamsByName() )
    {
        // Don't display secret attributes.
        if ( p.second->getSecret() )
        { continue; }

        using namespace tuttle::host::ofx::attribute;
        details::PluginParamTreeItem *item = nullptr;
        if ( dynamic_cast<OfxhParamString*>( p.second ) )
        {
            OfxhParamString& paramString = dynamic_cast<OfxhParamString&>( *p.second );
            item = new details::PluginParamTreeItem( root, paramString );
            QLineEdit *lineedit = new QLineEdit();
            lineedit->setText( QString::fromStdString( paramString.getStringValue() ) );
            widget.treeParameters->setItemWidget( item, 1, lineedit );
        }
        else if ( dynamic_cast<OfxhParamChoice*>( p.second ) )
        {
            OfxhParamChoice& paramChoice = dynamic_cast<OfxhParamChoice&>( *p.second );
            if ( paramChoice.getChoiceKeys().size() == 0 )
            { continue; }
            item = new details::PluginParamTreeItem( root, paramChoice );
            QComboBox *comboChoice = new QComboBox();
            int choiceIndex = paramChoice.getIntValue();
            for( const std::string & c: paramChoice.getChoiceKeys() )
            {
                comboChoice->addItem( QString::fromStdString( c ) );
            }
            if ( choiceIndex >= 0 && choiceIndex < paramChoice.getChoiceKeys().size() )
            {
                comboChoice->setCurrentIndex( choiceIndex );
            }
            widget.treeParameters->setItemWidget( item, 1, comboChoice );
        }
        else if ( dynamic_cast<OfxhParamDouble*>( p.second ) )
        {
            OfxhParamDouble& paramDouble = dynamic_cast<OfxhParamDouble&>( *p.second );
            item = new details::PluginParamTreeItem( root, paramDouble );
            QDoubleSpinBox *spinDouble = new QDoubleSpinBox();
            spinDouble->setMinimum( std::numeric_limits<double>::lowest() );
            spinDouble->setMaximum( std::numeric_limits<double>::max() );
            spinDouble->setValue( paramDouble.getDoubleValue() );
            widget.treeParameters->setItemWidget( item, 1, spinDouble );
        }
        else if ( dynamic_cast<OfxhParamInteger*>( p.second ) )
        {
            OfxhParamInteger& paramInt = dynamic_cast<OfxhParamInteger&>( *p.second );
            item = new details::PluginParamTreeItem( root, paramInt );
            QSpinBox *spinInt = new QSpinBox();
            spinInt->setMinimum( std::numeric_limits<int>::lowest() );
            spinInt->setMaximum( std::numeric_limits<int>::max() );
            spinInt->setValue( paramInt.getIntValue() );
            widget.treeParameters->setItemWidget( item, 1, spinInt );
        }
        else if ( dynamic_cast<OfxhParamBoolean*>( p.second ) )
        {
            OfxhParamBoolean& paramBool = dynamic_cast<OfxhParamBoolean&>( *p.second );
            item = new details::PluginParamTreeItem( root, paramBool );
            QCheckBox *checkbox = new QCheckBox();
            checkbox->setChecked( paramBool.getBoolValue() );
            widget.treeParameters->setItemWidget( item, 1, checkbox );
        }
    }
    root->setExpanded( true );
    widget.treeParameters->setSortingEnabled( true );
}

EditPluginParamsDialog::~EditPluginParamsDialog()
{
}

void EditPluginParamsDialog::onTreeWidgetItemDoubleClicked( QTreeWidgetItem * item, int column )
{
    if ( column > 0 && dynamic_cast<details::PluginParamTreeItem*>( item ) )
    {
        widget.treeParameters->editItem( item, column );
    }
}

void EditPluginParamsDialog::accept()
{
    QTreeWidgetItemIterator it( widget.treeParameters );
    while( *it )
    {
        details::PluginParamTreeItem * paramItem = dynamic_cast<details::PluginParamTreeItem *>( *it );
        if ( paramItem )
        {
            QWidget *w = widget.treeParameters->itemWidget( paramItem, 1 );
            if ( w )
            {
                const char separator = '%';
                const std::string key = paramItem->paramName();
                if ( dynamic_cast<QCheckBox*>( w ) )
                {
                    QCheckBox *checkbox = dynamic_cast<QCheckBox*>( w );
                    _nodeSettings.set( _pluginIdentifier, key, checkbox->isChecked(), separator );
                }
                else if ( dynamic_cast<QComboBox*>( w ) )
                {
                    QComboBox *combobox = dynamic_cast<QComboBox*>( w );
                    if ( combobox->currentIndex() >= 0 )
                    {
                        _nodeSettings.set( _pluginIdentifier, key, combobox->currentText().toStdString(), separator );
                    }
                }
                else if ( dynamic_cast<QDoubleSpinBox*>( w ) )
                {
                    QDoubleSpinBox *valueBox = dynamic_cast<QDoubleSpinBox*>( w );
                    _nodeSettings.set( _pluginIdentifier, key, valueBox->value(), separator );
                }
                else if ( dynamic_cast<QSpinBox*>( w ) )
                {
                    QSpinBox *valueBox = dynamic_cast<QSpinBox*>( w );
                    _nodeSettings.set( _pluginIdentifier, key, valueBox->value(), separator );
                }
                else if ( dynamic_cast<QLineEdit*>( w ) )
                {
                    QLineEdit *lineedit = dynamic_cast<QLineEdit*>( w );
                    _nodeSettings.set( _pluginIdentifier, key, lineedit->text().toStdString(), separator );
                }
            }
        }
        ++it;
    }
    Parent::accept();
}

}
}
}
