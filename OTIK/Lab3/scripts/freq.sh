IFS=$'\n'
FILENAME=$1

hexdump -ve '16/1 "%02x " " "' $FILENAME | sed -r 's/ /&\n/g' | sed -r '/^\s*$/d' | sort -f | uniq -c | sort -n

wc -m $FILENAME | cut -d' ' -f1
