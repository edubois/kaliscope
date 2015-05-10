#ifndef _EDITPLUGINPARAMSDIALOG_HPP
#define	_EDITPLUGINPARAMSDIALOG_HPP

#include "ui_EditPluginParamsDialog.hpp"

#include <kali-core/settingsTools.hpp>
#include <mvp-player-core/Settings.hpp>

#include <tuttle/common/utils/global.hpp>
#include <tuttle/host/ofx/attribute/OfxhParamString.hpp>
#include <tuttle/host/ofx/attribute/OfxhParamChoice.hpp>
#include <tuttle/host/ofx/attribute/OfxhParamDouble.hpp>
#include <tuttle/host/ofx/attribute/OfxhParamInteger.hpp>
#include <tuttle/host/ofx/attribute/OfxhParamBoolean.hpp>

namespace kaliscope
{
namespace gui
{
namespace qt
{

namespace details
{
using namespace tuttle::host::ofx::attribute;

class PluginParamTreeItem : public QTreeWidgetItem
{
public:
    PluginParamTreeItem( QTreeWidgetItem *parent, const std::string & paramName, const std::string & value )
    : QTreeWidgetItem( parent )
    , _paramName( paramName )
    {
        setText( 0, QString::fromStdString( paramName ) );
        setText( 1, QString::fromStdString( value ) );
        
        setFlags( flags() | Qt::ItemIsEditable );
    }

    PluginParamTreeItem( QTreeWidgetItem *parent, OfxhParamString& param )
    : QTreeWidgetItem( parent )
    , _paramName( param.getName() )
    {
        setText( 0, QString::fromStdString( param.getName() ) );
        setText( 1, QString::fromStdString( param.getStringValue() ) );
        
        setFlags( flags() | Qt::ItemIsEditable );
    }

    PluginParamTreeItem( QTreeWidgetItem *parent, OfxhParamChoice& param )
    : QTreeWidgetItem( parent )
    , _paramName( param.getName() )
    {
        setText( 0, QString::fromStdString( param.getName() ) );
        setText( 1, QString::fromStdString( param.getStringValue() ) );
        
        setFlags( flags() | Qt::ItemIsEditable );
    }

    PluginParamTreeItem( QTreeWidgetItem *parent, OfxhParamDouble& param )
    : QTreeWidgetItem( parent )
    , _paramName( param.getName() )
    {
        setText( 0, QString::fromStdString( param.getName() ) );
        setText( 1, QString::number( param.getDoubleValue() ) );
        
        setFlags( flags() | Qt::ItemIsEditable );
    }

    PluginParamTreeItem( QTreeWidgetItem *parent, OfxhParamInteger& param )
    : QTreeWidgetItem( parent )
    , _paramName( param.getName() )
    {
        setText( 0, QString::fromStdString( param.getName() ) );
        setText( 1, QString::number( param.getIntValue() ) );

        setFlags( flags() | Qt::ItemIsEditable );
    }

    PluginParamTreeItem( QTreeWidgetItem *parent, OfxhParamBoolean& param )
    : QTreeWidgetItem( parent )
    , _paramName( param.getName() )
    {
        setText( 0, QString::fromStdString( param.getName() ) );
        setText( 1, QString::number( param.getBoolValue() ) );
        
        setFlags( flags() | Qt::ItemIsEditable );
    }
    
    inline const std::string & paramName() const
    { return _paramName; }

private:
    std::string _paramName;     ///< Parameter name
};

}

class EditPluginParamsDialog : public QDialog
{
    Q_OBJECT
private:
    typedef QDialog Parent;
    typedef EditPluginParamsDialog This;

public:
    EditPluginParamsDialog( const std::string & pluginIdentifier, tuttle::host::INode & fxNode, mvpplayer::Settings *currentSettings = nullptr, QWidget * parent = NULL );
    virtual ~EditPluginParamsDialog();

    void accept();

    inline const mvpplayer::Settings & nodeSettings() const
    { return _nodeSettings; }


private Q_SLOTS:
    void onTreeWidgetItemDoubleClicked( QTreeWidgetItem * item, int column );

private:
    Ui::EditPluginParamsDialog widget;
    const std::string & _pluginIdentifier;          ///< Plugin identifier
    tuttle::host::INode & _fxNode;                  ///< Plugin node
    mvpplayer::Settings _nodeSettings;              ///< Node settings
};

}
}
}

#endif
