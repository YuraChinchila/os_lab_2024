cd ~/Documents/os_lab_2024/lab5/src

cd ../..
git add .
git commit -m "5"
git push

echo "==========1=========="
gcc mutex.c
./a.out

echo "==========2=========="
gcc factorial.c -pthread
./a.out --k 10000000 --pnum 2 --mod 1000000007

echo "==========3==========" 
gcc deadlock.c -pthread
./a.out
