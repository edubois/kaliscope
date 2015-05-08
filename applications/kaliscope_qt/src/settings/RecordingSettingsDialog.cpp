#include "RecordingSettingsDialog.hpp"
#include "PluginListDialog.hpp"

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
}

RecordingSettingsDialog::~RecordingSettingsDialog()
{
}

void RecordingSettingsDialog::addPlugin()
{
    PluginListDialog dlg( this );
    QIcon iconStd;
    iconStd.addFile(QStringLiteral(":/kaliscope/icons/plugins/puzzle.png"), QSize( 100, 100 ), QIcon::Normal, QIcon::Off);
    if ( dlg.exec() && dlg.selectedPlugin() )
    {
        tuttle::host::ofx::OfxhPlugin & plugin = *dlg.selectedPlugin();
        const std::string itemString = ( boost::format( "%1% : %2%.%3%" ) % plugin.getIdentifier() % plugin.getVersionMajor() % plugin.getVersionMinor() ).str();
        TablePluginItem *plugItem = new TablePluginItem( plugin, iconStd, QString::fromStdString( itemString ) );
        widget.listPipeline->addItem( plugItem );
    }
}

}
}
}
