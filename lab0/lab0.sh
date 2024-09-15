cd ~/Documents/os_lab_2024/lab0/src
mkdir hello
touch hello/empty
cp hello.c hello/
mv hello/hello.c hello/newhello.c
# sh ../../update.sh
cd hello
gcc newhello.c -o hello_world.o
./hello_world.o

touch ../../../.gitignore
echo "*.o\n*.so\n*.out\n*.a" > ../../../.gitignore
git add ../../../.gitignore
git commit -m "update gitignore"

cd ~/Documents/os_lab_2024
git add .
git commit -m "добавлен hello.c и empty"
git push
