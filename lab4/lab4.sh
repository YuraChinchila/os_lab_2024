cd ~/Documents/os_lab_2024/lab4/src

echo "==========1=========="
make clean
make
./parallel_min_max --seed 42 --array_size 100 --pnum 2 --timeout 1
./parallel_min_max --seed 42 --array_size 100000000000 --pnum 2 --timeout 1

echo "==========2=========="
gcc zombie.c
./a.out &
sleep 3
ps aux | grep a.out
wait

echo "==========3/4=========="
./parallel_sum --threads_num 2 --seed 42 --array_size 10000000
./parallel_sum --threads_num 3 --seed 42 --array_size 10000000
./parallel_sum --threads_num 4 --seed 42 --array_size 10000000

cd ../..
git add .
git commit -m "4"
git push
