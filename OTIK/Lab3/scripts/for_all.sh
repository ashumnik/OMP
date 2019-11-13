IFS=$' '
DIRNAME=$1

for FILENAME in $DIRNAME/*; do

    echo "## $FILENAME"
    OUT=$(./relative_freq.sh $FILENAME)
    echo $OUT | tail -n4

done
