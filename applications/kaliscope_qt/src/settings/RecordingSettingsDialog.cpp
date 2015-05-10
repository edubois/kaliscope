#include "RecordingSettingsDialog.hpp"
#include "PluginListDialog.hpp"
#include "EditPluginParamsDialog.hpp"

#include <kali-core/settingsTools.hpp>

#include <QtCore/QSize>
#include <QtGui/QDropEvent>
#include <QtCore/QDir>

#include <boost/format.hpp>

namespace kaliscope
{
namespace gui
{
namespace qt
{

RecordingSettingsDialog::RecordingSettingsDialog( QWidget *parent )
: QDialog( parent )
{
    widget.setupUi( this );
    connect( widget.btnAddPlugin, SIGNAL( released() ), this, SLOT( addPlugin() ) );

    widget.listPipeline->setMovement( QListView::Static );
    widget.listPipeline->setDragDropMode( QAbstractItemView::InternalMove );
    widget.listPipeline->setDragDropMode( QAbstractItemView::InternalMove );
    connect( widget.listPipeline, SIGNAL( itemDoubleClicked(QListWidgetItem *) ), this, SLOT( editPluginParams( QListWidgetItem * ) ) );

    _pipelineSettings.read( QDir::homePath().toStdString() + "/" + kKaliscopeDefaultPipelineSettingsFilename );
    buildPipelineFrom( _pipelineSettings );
}

RecordingSettingsDialog::~RecordingSettingsDialog()
{
}

void RecordingSettingsDialog::buildPipelineFrom( const mvpplayer::Settings & pipelineSettings )
{
    _pluginsSettings = splitOfxNodesSettings( _pipelineSettings );
    for( const auto &p: _pluginsSettings )
    {
        using namespace tuttle::host;
        try
        {
            ofx::imageEffect::OfxhImageEffectPlugin* plugFx = core().getImageEffectPluginById( p.first.pluginIdentifier );
            if ( !plugFx )
            {
                continue;
            }
            plugFx->loadAndDescribeActions();

            addPlugin( *plugFx );
        }
        catch( ... )
        {
            std::cerr << "Unable to load plugin: " << p.first.pluginIdentifier << std::endl;
        }
    }
}

void RecordingSettingsDialog::addPlugin()
{
    PluginListDialog dlg( !widget.listPipeline->count(), this );
    if ( dlg.exec() && dlg.selectedPlugin() )
    {
        TablePluginItem *plugItem = addPlugin( *dlg.selectedPlugin() );
        
        // Edit plugin parameters
        editPluginParams( plugItem );
    }
}

TablePluginItem * RecordingSettingsDialog::addPlugin( const tuttle::host::ofx::imageEffect::OfxhImageEffectPlugin & plugin )
{
    TablePluginItem *plugItem = new TablePluginItem( plugin );
    widget.listPipeline->addItem( plugItem );
    QWidget *w = buildPluginWidgetFrom( plugItem );
    widget.listPipeline->setItemWidget( plugItem, w );
    plugItem->setSizeHint( w->sizeHint() );
    widget.listPipeline->setGridSize( QSize( w->sizeHint().width(), 0 ) );
    return plugItem;
}

QWidget* RecordingSettingsDialog::buildPluginWidgetFrom( TablePluginItem *plugItem )
{
    QWidget *w = new QWidget( widget.listPipeline );
    QVBoxLayout *layout = new QVBoxLayout();
    w->setLayout( layout );
    w->setContentsMargins( 0, 0, 0, 0 );

    const tuttle::host::ofx::imageEffect::OfxhImageEffectPlugin & plugin = plugItem->plugin();
    const std::string itemString = ( boost::format( "%1% - v%2%.%3%" ) % plugin.getIdentifier() % plugin.getVersionMajor() % plugin.getVersionMinor() ).str();

    QIcon iconStd;
    iconStd.addFile( QStringLiteral(":/kaliscope/icons/plugins/puzzle.png"), QSize( 100, 100 ) );
    QLabel *icon = new QLabel();
    icon->setContentsMargins( 0, 0, 0, 0 );
    icon->setPixmap( iconStd.pixmap( 64, 64 ) );
    layout->addWidget( icon );
    icon->setToolTip( QString::fromStdString( itemString ) );

    QLabel *caption = new QLabel( QString::fromStdString( itemString ) );
    caption->setContentsMargins( 0, 0, 0, 0 );
    caption->setFixedWidth( 128 );
    caption->setWordWrap( true );
    layout->addWidget( caption );

    return w;
}

void RecordingSettingsDialog::editPluginParams( QListWidgetItem * item )
{
    TablePluginItem * pluginItem = static_cast<TablePluginItem*>( item );
    const int itemIndex = widget.listPipeline->row( item );

    PluginItem key( itemIndex, pluginItem->plugin().getIdentifier() );
    mvpplayer::Settings *currentSettings = nullptr;
    const auto itPlugSettings = _pluginsSettings.find( key );
    if ( itPlugSettings != _pluginsSettings.end() )
    {
        currentSettings = &itPlugSettings->second;
    }

    std::unique_ptr<tuttle::host::INode> plugNode( tuttle::host::createNode( pluginItem->plugin().getIdentifier() ) );
    EditPluginParamsDialog dlg( pluginItem->plugin().getIdentifier(), *plugNode, currentSettings, this );
    if ( dlg.exec() )
    {
        _pluginsSettings[ key ] = dlg.nodeSettings();
    }
}

void RecordingSettingsDialog::accept()
{
    const int cnt = widget.listPipeline->count();
    _pipelineSettings.clear();
    for( int i = 0; i < cnt; ++i )
    {
        TablePluginItem * pluginItem = static_cast<TablePluginItem*>( widget.listPipeline->item( i ) );
        const auto itPlugSettings = _pluginsSettings.find( PluginItem( i, pluginItem->plugin().getIdentifier() ) );
        if ( itPlugSettings != _pluginsSettings.end() )
        {
            _pipelineSettings.set( std::string(), std::to_string( i ), itPlugSettings->second.tree() );
        }
    }
    
    Parent::accept();
}

}
}
}
