IFS=$'\n'
FILENAME=$1

sed -r 's/./&\n/g' $FILENAME | sort -f | uniq -ic | sort -n

wc -m $FILENAME | cut -d' ' -f1
