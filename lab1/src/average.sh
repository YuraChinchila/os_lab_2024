for var in "$@"
do
    S=$(expr $S + $var)
    N=$(expr $N + 1)
done
A=$(expr $S / $N)
echo "$S"
echo "$A"
