#!/bin/sh

#export CC=/usr/bin/clang;
#export CXX=/usr/bin/clang++;
./init-linux.sh
cd build;
cmake --build . --config release --target RavEngine_Sample_AirHockey RavEngine_Sample_GUIKitchenSink RavEngine_Sample_Playground RavEngine_Sample_Performance_C RavEngine_Samples_TestBasics
