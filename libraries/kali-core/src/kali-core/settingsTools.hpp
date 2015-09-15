#ifndef _KALICORE_SETTINGSTOOLS_HPP_
#define	_KALICORE_SETTINGSTOOLS_HPP_

#include <mvp-player-core/Settings.hpp>

#include <tuttle/host/Node.hpp>
#include <tuttle/common/utils/global.hpp>
#include <tuttle/host/Graph.hpp>
#include <tuttle/host/ofx/attribute/OfxhParamString.hpp>
#include <tuttle/host/ofx/attribute/OfxhParamChoice.hpp>
#include <tuttle/host/ofx/attribute/OfxhParamDouble.hpp>
#include <tuttle/host/ofx/attribute/OfxhParamInteger.hpp>
#include <tuttle/host/ofx/attribute/OfxhParamBoolean.hpp>

#include <string>

namespace kaliscope
{
    static const std::string kDefaultSettingsFilename( ".kaliscopeSettings.json" );
    static const std::string kKaliscopePluginEnvKey( "MVPPLAYER_PLUGIN_PATH" );

    struct PluginItem
    {
        PluginItem()
        {}

        PluginItem( const std::size_t idx, const std::string & identifier )
        : index( idx )
        , pluginIdentifier( identifier )
        {}

        inline bool operator<( const PluginItem & other ) const
        {
            if ( index != other.index )
            {
                return index < other.index;
            }
            else
            {
                return pluginIdentifier < other.pluginIdentifier;
            }
        }

        std::size_t index = 0;
        std::string pluginIdentifier;
    };

/**
 * @brief get settings from a node
 * @param plugIdentifier target plugin identifier
 * @param settings output settings
 * @param fxNode input ofx node
 */
void getSettingsFromNode( std::string & plugIdentifier, mvpplayer::Settings & settings, const tuttle::host::INode & fxNode );

/**
 * @brief setup a node parameters according to settings
 * @param plugIdentifier target plugin identifier
 * @param fxNode ofx node
 * @param settings input settings
 */
void setNodeSettings( const std::string & plugIdentifier, tuttle::host::INode & fxNode, const mvpplayer::Settings & settings );

/**
 * @brief split nodes settings
 * @param settings the input settings
 * @return map of <plugin-identifier, settings>
 */
std::map<PluginItem, mvpplayer::Settings> splitOfxNodesSettings( const mvpplayer::Settings & settings );

/**
 * @brief setup graph using given settings
 * @param graph the processing graph
 * @param settings the input settings
 */
void setupGraphWithSettings( tuttle::host::Graph & graph, const mvpplayer::Settings & settings );

}

#endif
