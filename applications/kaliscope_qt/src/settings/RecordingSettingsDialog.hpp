#ifndef _RECORDINGSETTINGSDIALOG_HPP_
#define	_RECORDINGSETTINGSDIALOG_HPP_

#include "ui_RecordingSettingsDialog.hpp"

#include <kali-core/settingsTools.hpp>

#include <mvp-player-core/Settings.hpp>

#include "TablePluginItem.hpp"

#include <tuttle/common/utils/global.hpp>
#include <tuttle/host/Graph.hpp>

namespace kaliscope
{
namespace gui
{
namespace qt
{

static const std::string kKaliscopeDefaultPipelineSettingsFilename( ".kaliDefaultPipeline.json" );

/**
 * @brief recording dialog settings
 * allow the user to edit the processing pipeline
 */
class RecordingSettingsDialog : public QDialog
{
    Q_OBJECT
private:
    typedef RecordingSettingsDialog This;
    typedef QDialog Parent;
public:
    RecordingSettingsDialog( QWidget *parent = nullptr );
    virtual ~RecordingSettingsDialog();

    const mvpplayer::Settings & pipelineSettings() const
    { return _pipelineSettings; }

private Q_SLOTS:
    void removePluginSelection();
    void addPlugin();
    void editPluginParams( QListWidgetItem * item );

private:
    TablePluginItem * addPlugin( const tuttle::host::ofx::imageEffect::OfxhImageEffectPlugin & plugin );
    void buildPipelineFrom( const mvpplayer::Settings & pipelineSettings );
    void rebuildPipeline();
    QWidget* buildPluginWidgetFrom( TablePluginItem *plugItem );
    void accept();

private:
    Ui::RecordingSettingsDialog widget;
    mvpplayer::Settings _pipelineSettings;                          ///< The pipeline global settings
    std::map<PluginItem, mvpplayer::Settings> _pluginsSettings;    ///< Local parameters for plugins
};

}
}
}

#endif
