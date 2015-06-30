#!/bin/bash
for filename in *.dylib; do
        ./package.sh $(basename "$filename") @loader_path/
done
for filename in *.ofx; do
        ./package.sh $(basename "$filename") @loader_path/
done
