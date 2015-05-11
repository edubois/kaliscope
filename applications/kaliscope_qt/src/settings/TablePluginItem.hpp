#ifndef _GUI_TABLEPLUGINITEM_HPP_
#define	_GUI_TABLEPLUGINITEM_HPP_

#include <tuttle/common/utils/global.hpp>
#include <tuttle/host/Graph.hpp>
#include <mvp-player-core/Settings.hpp>

namespace kaliscope
{
namespace gui
{
namespace qt
{


class TablePluginItem : public QListWidgetItem
{
public:
    TablePluginItem( const tuttle::host::ofx::imageEffect::OfxhImageEffectPlugin & plugin, const mvpplayer::Settings & settings, const QIcon & icon, const QString & text, QListWidget * parent = 0, int type = QListWidgetItem::Type )
    : QListWidgetItem( icon, text, parent, type )
    , _plugin( plugin )
    , _settings( settings )
    {
        setInternalFlags();
    }

    TablePluginItem( const tuttle::host::ofx::imageEffect::OfxhImageEffectPlugin & plugin, const mvpplayer::Settings & settings, const QString & text = QString(), QListWidget * parent = 0, int type = QListWidgetItem::Type )
    : QListWidgetItem( text, parent, type )
    , _plugin( plugin )
    , _settings( settings )
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

    /**
     * @brief node settings getter
     * @return settings tree
     */    
    inline mvpplayer::Settings & settings()
    { return _settings; }

    /**
     * @brief node settings getter
     * @return settings tree
     */    
    inline const mvpplayer::Settings & settings() const
    { return _settings; }

protected:
    const tuttle::host::ofx::imageEffect::OfxhImageEffectPlugin & _plugin;
    mvpplayer::Settings _settings;
};

}
}
}

#endif
