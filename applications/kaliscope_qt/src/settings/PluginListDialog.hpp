/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

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
    PluginListDialog( const bool readerOnly, QWidget *parent = nullptr );
    virtual ~PluginListDialog();

    inline const tuttle::host::ofx::imageEffect::OfxhImageEffectPlugin *selectedPlugin() const
    { return _selectedPlugin; }

private Q_SLOTS:
    void accept();
    void buildListOfPlugins();

private:
    Ui::PluginListDialog widget;
    bool _showReaderOnly;
    const tuttle::host::ofx::imageEffect::OfxhImageEffectPlugin* _selectedPlugin = nullptr;        ///< The current selected plugin
};

}
}
}

#endif
