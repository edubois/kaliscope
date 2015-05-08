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
    TablePluginItem( tuttle::host::ofx::OfxhPlugin & plugin, const QIcon & icon, const QString & text, QListWidget * parent = 0, int type = QListWidgetItem::Type )
    : QListWidgetItem( icon, text, parent, type )
    , _plugin( plugin )
    {
    }
    
    tuttle::host::ofx::OfxhPlugin & plugin()
    { return _plugin; }

protected:
    tuttle::host::ofx::OfxhPlugin & _plugin;
};

}
}
}

#endif
