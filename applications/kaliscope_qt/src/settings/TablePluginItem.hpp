#ifndef _GUI_TABLEPLUGINITEM_HPP_
#define	_GUI_TABLEPLUGINITEM_HPP_

#include <tuttle/common/utils/global.hpp>
#include <tuttle/host/Graph.hpp>

namespace kaliscope
{
namespace gui
{
namespace qt
{


class TablePluginItem : public QListWidgetItem
{
public:
    TablePluginItem( const tuttle::host::ofx::imageEffect::OfxhImageEffectPlugin & plugin, const QIcon & icon, const QString & text, QListWidget * parent = 0, int type = QListWidgetItem::Type )
    : QListWidgetItem( icon, text, parent, type )
    , _plugin( plugin )
    {
        setInternalFlags();
    }

    TablePluginItem( const tuttle::host::ofx::imageEffect::OfxhImageEffectPlugin & plugin, const QString & text = QString(), QListWidget * parent = 0, int type = QListWidgetItem::Type )
    : QListWidgetItem( text, parent, type )
    , _plugin( plugin )
    {
        setInternalFlags();
    }

    const tuttle::host::ofx::imageEffect::OfxhImageEffectPlugin & plugin() const
    { return _plugin; }

    void setInternalFlags()
    {
        // Reader can't change their position
        if ( _plugin.supportsContext( kOfxImageEffectContextReader ) )
        {
            setFlags( flags() ^ Qt::ItemIsDropEnabled );
        }
    }
    
protected:
    const tuttle::host::ofx::imageEffect::OfxhImageEffectPlugin & _plugin;
};

}
}
}

#endif
