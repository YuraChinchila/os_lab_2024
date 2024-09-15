cd ~/Documents/os_lab_2024/lab1/src
sh background.sh > out.txt &
cat out.txt
wc out.txt
cat cake_rhymes.txt | grep cake > with_cake.txt
cat out.txt > /dev/null
chmod +x hello.sh
pwd
date
echo $PATH
od -N150 -t d4 < /dev/random > numbers.txt
cat numbers.txt | ./average.sh `xargs`

git add .
git commit -m "1"
git push
