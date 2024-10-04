cd ~/Documents/os_lab_2024/lab3/src
echo "==========1=========="
gcc find_min_max.c test1.c
./a.out

echo "==========2/3=========="
gcc -o parallel_min_max utils.c find_min_max.c parallel_min_max.c
./parallel_min_max --seed 42 --array_size 100 --pnum 3
./parallel_min_max --seed 42 --array_size 100 --pnum 3 --by_files
# time ./sequential_min_max 42 300000000
# time ./parallel_min_max --seed 42 --array_size 300000000 --pnum 2

echo "==========4=========="
make clean
make
./sequential_min_max 42 100
./parallel_min_max --seed 42 --array_size 100 --pnum 3

echo "==========5=========="
make exec
./exec

cd ../..
git add .
git commit -m "3"
git push
