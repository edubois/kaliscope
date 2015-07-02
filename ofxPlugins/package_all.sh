#!/bin/bash

function abspath() { pushd . > /dev/null; if [ -d "$1" ]; then cd "$1"; dirs -l +0; else cd "`dirname \"$1\"`"; cur_dir=`dirs -l +0`; if [ "$cur_dir" == "/" ]; then echo "$cur_dir`basename \"$1\"`"; else echo "$cur_dir/`basename \"$1\"`"; fi; fi; popd > /dev/null; }

if [ -z "$BOOST_ROOT" ]; then
	echo "\$BOOST_ROOT is empty!"
	exit 1
fi

if [ -z "$1" ]; then
	echo "Usage: package_all.sh <path of the plugin with tailing .../xxx.ofx/Contents/MacOS>"
	exit 2
fi

export basepath=`abspath $1`

echo "Copying boost dependencies"
pushd . > /dev/null
cd $BOOST_ROOT/stage/lib
cp libboost_log.dylib $basepath/
cp libboost_date_time.dylib $basepath/
cp libboost_locale.dylib $basepath/
cp libboost_regex.dylib $basepath/
cp libboost_thread.dylib $basepath/
cp libboost_chrono.dylib $basepath/
cp libboost_filesystem.dylib $basepath/
cp libboost_system.dylib $basepath/
popd > /dev/null

for filename in $basepath/*.dylib; do
   export f=`abspath $filename`
   echo "Packaging $f"
   ./package.sh "$f" @loader_path/
done
for filename in $basepath/*.ofx; do
   export f=`abspath $filename`
   echo "Packaging $f"
   ./package.sh "$f" @loader_path/
done

echo "You can now distribute your plugin!"

