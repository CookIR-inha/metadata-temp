cd build
cmake ..
make
cd ..
opt -load-pass-plugin ./build/libSoftBoundPass.so --passes=softbound -o output.ll test.ll 
