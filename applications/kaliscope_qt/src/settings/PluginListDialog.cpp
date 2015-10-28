/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

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

PluginListDialog::PluginListDialog( const bool readerOnly, QWidget *parent )
: QDialog( parent )
, _showReaderOnly( readerOnly )
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

    try
    {
        core().preload();
        for( const ofx::OfxhPlugin* plugin: core().getImageEffectPluginCache().getPlugins() )
        {
            const std::string identifier = plugin->getRawIdentifier();
            ofx::imageEffect::OfxhImageEffectPlugin* plugFx = core().getImageEffectPluginById( identifier );
            if ( !plugFx )
            {
                continue;
            }
            try { plugFx->loadAndDescribeActions(); } catch( ... ) { continue; }

            if ( _showReaderOnly )
            {
                if ( !plugFx->supportsContext( kOfxImageEffectContextReader ) )
                {
                    continue;
                }
            }
            // Avoid duplicates
            const std::string itemString = ( boost::format( "%1% : %2%.%3%" ) % identifier % plugin->getVersionMajor() % plugin->getVersionMinor() ).str();
            if ( plugin && items.insert( itemString ).second )
            {
                QIcon *icon = &iconStd;
                if ( boost::algorithm::istarts_with( identifier, "tuttle" ) )
                {
                    icon = &iconTuttle;
                }
                TablePluginItem * item = new TablePluginItem( *plugFx, mvpplayer::Settings(), *icon, QString::fromStdString( itemString ) );
                widget.listPlugins->addItem( item );
            }
        }
    }
    catch( ... )
    {
        TUTTLE_LOG_CURRENT_EXCEPTION;
    }
}

}
}
}
