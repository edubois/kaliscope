#include "RecordingSettingsDialog.hpp"
#include "PluginListDialog.hpp"

#include <QtCore/QSize>
#include <QtGui/QDropEvent>

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
}

RecordingSettingsDialog::~RecordingSettingsDialog()
{
}

void RecordingSettingsDialog::addPlugin()
{
    PluginListDialog dlg( !widget.listPipeline->count(), this );
    if ( dlg.exec() && dlg.selectedPlugin() )
    {
        const tuttle::host::ofx::imageEffect::OfxhImageEffectPlugin & plugin = *dlg.selectedPlugin();
        TablePluginItem *plugItem = new TablePluginItem( plugin );
        widget.listPipeline->addItem( plugItem );
        QWidget *w = buildPluginWidgetFrom( plugItem );
        widget.listPipeline->setItemWidget( plugItem, w );
        plugItem->setSizeHint( w->sizeHint() );
        widget.listPipeline->setGridSize( QSize( w->sizeHint().width(), 0 ) );
    }
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

}
}
}
