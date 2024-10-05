cd ~/Documents/os_lab_2024/lab4/src

echo "==========1=========="
make clean
make
./parallel_min_max --seed 42 --array_size 100 --pnum 2 --timeout 1
./parallel_min_max --seed 42 --array_size 100000000000 --pnum 2 --timeout 2

# cd ../..
# git add .
# git commit -m "4"
# git push
