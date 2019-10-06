IFS=$'\n'
DIRNAME=$1

for FILENAME in $DIRNAME/*; do

    echo "## $FILENAME"
    ./relative_freq_non_printable.sh $FILENAME | head -n2
    ./relative_freq_non_printable.sh $FILENAME | tail -n4

done
