gcc -shared -fPIC softbound.c -o libsoftbound.so
clang output.ll -o output_binary -L. -lsoftbound -lm -Wl,-rpath, .
# link 할 때 라이브러리를 못찾아서 추가해줌
