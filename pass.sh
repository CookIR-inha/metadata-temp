cd build
cmake ..
make
cd ..
opt -load-pass-plugin ./build/libSoftBoundPass.so --passes=softbound -S -o output.ll test.ll 
