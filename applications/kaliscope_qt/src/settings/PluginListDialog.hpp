#ifndef _PLUGINLISTDIALOG_HPP_
#define	_PLUGINLISTDIALOG_HPP_

#include "ui_PluginListDialog.hpp"
#include "TablePluginItem.hpp"

namespace kaliscope
{
namespace gui
{
namespace qt
{

/**
 * @brief dialog with a list of plugins (plugin choser)
 */
class PluginListDialog : public QDialog
{
    Q_OBJECT
private:
    typedef PluginListDialog This;
    typedef QDialog Parent;
public:
    PluginListDialog( QWidget *parent = nullptr );
    virtual ~PluginListDialog();

    inline tuttle::host::ofx::OfxhPlugin *selectedPlugin()
    { return _selectedPlugin; }
    
private Q_SLOTS:
    void accept();
    void buildListOfPlugins();

private:
    Ui::PluginListDialog widget;
    tuttle::host::ofx::OfxhPlugin *_selectedPlugin = nullptr;        ///< The current selected plugin
};

}
}
}

#endif
