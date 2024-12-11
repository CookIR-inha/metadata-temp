gcc -shared -fPIC shadow_memory.c -o libshadow_memory.so
gcc -shared -fPIC softbound.c -o libsoftbound.so -L. -lshadow_memory -Wl,-rpath,.
gcc -shared -fPIC softbound-wrapper.c -o libsoftbound-wrapper.so -L. -lsoftbound -lshadow_memory -Wl,-rpath,.
clang output.ll -o output_binary -L. -lsoftbound-wrapper -lsoftbound -lshadow_memory -lm -Wl,-rpath,.

# link 할 때 라이브러리를 못찾아서 추가해줌
