Import( 'project' )
Import( 'libs' )


kaliscopeFlags = { 'LIBPATH': [project.inOutputLib()],
                'CCFLAGS': [project.CC['warning3'],project.CC['sharedobject'] ],
                'CXXFLAGS':[],
                'CPPDEFINES':
                    [
                    ('TUTTLE_PLUGIN_PATH','"'+project.inOutputPlugin()+'"'),
                    ],
             }

if project.env['mode'] == 'production' :
	kaliscopeFlags['CPPDEFINES'].append( 'KALISCOPE_PRODUCTION' )
	if 'visibilityhidden' in project.CC:
		kaliscopeFlags['SHCCFLAGS'] = [project.CC['visibilityhidden']]

# If your compiler as a flag to mark undefined flags as error in shared libraries
if 'sharedNoUndefined' in project.CC:
	kaliscopeFlags['SHLINKFLAGS'] = [project.CC['sharedNoUndefined']]

SConscript( [
              'libraries/terry/SConscript',
              'libraries/SConscript.sequenceparser',
              'libraries/openfxHack/SConscript',
              'libraries/tuttlePlugin/SConscript',
              'libraries/mvp-player/SConscript',
              'libraries/mvp-player/mvp-player-core/SConscript',
              'libraries/mvp-player/mvp-player-gui/SConscript',
              'libraries/mvp-player/mvp-player-net/SConscript',
              'libraries/mvp-player/mvp-player-qtgui/SConscript',
              'libraries/mvp-player/mvp-player-ncursesgui/SConscript',
              'libraries/kali-core/SConscript',
            ]
            +
            project.scanFiles( [ 'ofxPlugins', 'applications', 'tests'], accept=['SConscript'] )
          )

