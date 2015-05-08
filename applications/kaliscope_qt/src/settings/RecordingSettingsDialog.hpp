#ifndef _RECORDINGSETTINGSDIALOG_HPP_
#define	_RECORDINGSETTINGSDIALOG_HPP_

#include "ui_RecordingSettingsDialog.hpp"

namespace kaliscope
{
namespace gui
{
namespace qt
{

/**
 * @brief recording dialog settings
 * allow the user to edit the processing pipeline
 */
class RecordingSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    RecordingSettingsDialog( QWidget *parent = nullptr );
    virtual ~RecordingSettingsDialog();
    
private Q_SLOTS:
    void addPlugin();

private:
    Ui::RecordingSettingsDialog widget;
};

}
}
}

#endif
