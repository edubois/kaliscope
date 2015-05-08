#include "PluginListDialog.hpp"

#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <unordered_set>

namespace kaliscope
{
namespace gui
{
namespace qt
{

PluginListDialog::PluginListDialog( QWidget *parent )
: QDialog( parent )
{
    widget.setupUi( this );
    connect( widget.listPlugins, SIGNAL( itemDoubleClicked( QListWidgetItem * ) ), this, SLOT( accept() ) );
    
    buildListOfPlugins();
}

PluginListDialog::~PluginListDialog()
{
}

void PluginListDialog::accept()
{
    if ( widget.listPlugins->currentItem() )
    {
        _selectedPlugin = &( ( static_cast<TablePluginItem*>( widget.listPlugins->currentItem() ) )->plugin() );
        Parent::accept();
    }
}

void PluginListDialog::buildListOfPlugins()
{
    using namespace tuttle::host;
    widget.listPlugins->clear();
    std::unordered_set<std::string> items;
    QIcon iconStd;
    iconStd.addFile(QStringLiteral(":/kaliscope/icons/plugins/puzzle.png"), QSize( 32, 32 ), QIcon::Normal, QIcon::Off);
    QIcon iconTuttle;
    iconTuttle.addFile(QStringLiteral(":/kaliscope/icons/app/tuttle.png"), QSize( 32, 32 ), QIcon::Normal, QIcon::Off);
    BOOST_FOREACH( ofx::OfxhPlugin* plugin, core().getPluginCache().getPlugins() )
    {
        // Avoid duplicates
        const std::string itemString = ( boost::format( "%1% : %2%.%3%" ) % plugin->getIdentifier() % plugin->getVersionMajor() % plugin->getVersionMinor() ).str();
        if ( plugin && items.insert( itemString ).second )
        {
            QIcon *icon = &iconStd;
            if ( boost::algorithm::istarts_with( plugin->getIdentifier(), "tuttle" ) )
            {
                icon = &iconTuttle;
            }
            TablePluginItem * item = new TablePluginItem( *plugin, *icon, QString::fromStdString( itemString ) );
            widget.listPlugins->addItem( item );
        }
    }
}

}
}
}
