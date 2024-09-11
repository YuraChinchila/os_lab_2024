cd /home/chinchilla/Documents/os_lab_2024/lab0/src
mkdir hello
touch hello/empty
cp hello.c hello/
mv hello/hello.c hello/newhello.c
# sh ../../update.sh
cd hello
gcc newhello.c -o hello_world
./hello_world
git add .
git commit -m "добавлен hello.c и empty"
git push
