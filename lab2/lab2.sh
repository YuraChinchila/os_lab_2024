cd ~/Documents/os_lab_2024/lab2/src
echo "==========1=========="
gcc swap/main.c swap/swap.c
./a.out

echo "==========2/3=========="
cd revert_string
gcc -c revert_string.c -o revert_string.o
ar cr librevert_string.a revert_string.o
gcc -static main.c -L. -lrevert_string
./a.out "abcd"

gcc -fPIC -c revert_string.c -o revert_string.o
gcc -shared revert_string.o -o librevert_string.so
gcc main.c -L. -lrevert_string -Wl,-rpath,.
ldd a.out
./a.out "dcba"

cp ../tests/tests.c .
export LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu/:$(pwd)"
echo $LD_LIBRARY_PATH
gcc tests.c -lcunit -L. -lrevert_string
./a.out

ls /bin | grep libsimple

cd ~/Documents/os_lab_2024
git add .
git commit -m "2"
git push
