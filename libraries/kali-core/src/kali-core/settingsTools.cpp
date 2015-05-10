#include "settingsTools.hpp"

#include <queue>
#include <stack>
#include <deque>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

namespace kaliscope
{

/**
 * @brief setup a node parameters according to settings
 * @param plugIdentifier target plugin identifier
 * @param fxNode ofx node
 * @param settings input settings
 */
void setNodeSettings( const std::string & plugIdentifier, tuttle::host::INode & fxNode, const mvpplayer::Settings & settings )
{
    using namespace std;

    // Keep track of keys and values (subtrees) in second property tree
    queue<string> qKeys;
    queue<boost::property_tree::ptree> qValues;
    qValues.push( settings.tree() );
    const char separator = '%';

    // Iterate over second property tree
    while( !qValues.empty() )
    {
        typedef boost::property_tree::ptree::path_type PathT;
        // Setup keys and corresponding values
        boost::property_tree::ptree ptree = qValues.front();
        qValues.pop();
        string keychain = "";
        if( !qKeys.empty() )
        {
            keychain = qKeys.front();
            qKeys.pop();
        }

        // Iterate over keys level-wise
        for( const auto & child: ptree )
        {
            // Leaf
            if( child.second.size() == 0 )
            {
                vector<string> path;
                boost::split( path, keychain, boost::is_from_range( separator, separator ) );

                if( path.size() == 1 )
                {
                    const string settingPlugIdentifier = path[0];
                    if ( plugIdentifier != settingPlugIdentifier )
                    {
                        continue;
                    }

                    const string paramName = child.first;
                    const string value = ptree.get<std::string>( paramName );

                    try
                    {
                        using namespace tuttle::host::ofx::attribute;
                        OfxhParam & param = fxNode.getParam( paramName );
                        if ( dynamic_cast<OfxhParamString*>( &param ) )
                        {
                            OfxhParamString& paramString = dynamic_cast<OfxhParamString&>( param );
                            paramString.setValue( value, eChangeUserEdited );
                        }
                        else if ( dynamic_cast<OfxhParamChoice*>( &param ) )
                        {
                            OfxhParamChoice& paramChoice = dynamic_cast<OfxhParamChoice&>( param );
                            paramChoice.setValue( value, eChangeUserEdited );
                        }
                        else if ( dynamic_cast<OfxhParamDouble*>( &param ) )
                        {
                            OfxhParamDouble& paramDouble = dynamic_cast<OfxhParamDouble&>( param );
                            
                            paramDouble.setValue( boost::lexical_cast<double>( boost::replace_all_copy(value, ",", ".") ), eChangeUserEdited );
                        }
                        else if ( dynamic_cast<OfxhParamInteger*>( &param ) )
                        {
                            OfxhParamInteger& paramInt = dynamic_cast<OfxhParamInteger&>( param );
                            paramInt.setValue( boost::lexical_cast<int>( value ), eChangeUserEdited );
                        }
                        else if ( dynamic_cast<OfxhParamBoolean*>( &param ) )
                        {
                            OfxhParamBoolean& paramBool = dynamic_cast<OfxhParamBoolean&>( param );
                            paramBool.setValue( boost::lexical_cast<bool>( value ), eChangeUserEdited );
                        }
                    }
                    catch( ... )
                    {}
                }
            }
            // Subtree
            else
            {
                if( keychain != "" )
                    qKeys.push( keychain + separator + child.first.data() );
                else
                    qKeys.push( child.first.data() );

                qValues.push( child.second );
            }
        }
    }
}

/**
 * @brief split nodes settings
 * @param settings the input settings
 * @return map of <plugin-identifier, settings>
 */
std::map<PluginItem, mvpplayer::Settings> splitOfxNodesSettings( mvpplayer::Settings & settings )
{
    using namespace std;

    std::map<PluginItem, mvpplayer::Settings> splittedSettings;
    // Keep track of keys and values (subtrees) in second property tree
    queue<string> qKeys;
    queue<boost::property_tree::ptree> qValues;
    qValues.push( settings.tree() );

    const char separator = '%';

    // Iterate over second property tree
    while( !qValues.empty() )
    {
        typedef boost::property_tree::ptree::path_type PathT;
        typedef boost::property_tree::ptree::value_type ValueT;
        // Setup keys and corresponding values
        boost::property_tree::ptree ptree = qValues.front();
        qValues.pop();
        string keychain = "";
        if( !qKeys.empty() )
        {
            keychain = qKeys.front();
            qKeys.pop();
        }

        // Iterate over keys level-wise
        for( const auto & child: ptree )
        {
            if( child.second.size() != 0 )
            {
                vector<string> path;
                boost::split( path, keychain, boost::is_from_range( separator, separator ) );
                if( path.size() == 2 )
                {
                    const int pluginIndex = boost::lexical_cast<int>( path[0] );
                    const string pluginIdentification = child.first;
                    splittedSettings[ PluginItem( pluginIndex, pluginIdentification ) ] = mvpplayer::Settings( ptree );
                }

                if( keychain != "" )
                    qKeys.push( keychain + separator + child.first.data() );
                else
                    qKeys.push( child.first.data() );

                qValues.push( child.second );
            }
        }
    }
    return splittedSettings;
}


}
