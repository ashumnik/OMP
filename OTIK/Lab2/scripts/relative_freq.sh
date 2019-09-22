IFS=$'\n'
FILENAME=$1

RAW_FREQUENCY=$(./freq.sh $FILENAME)
IFS=$' '
FREQUENCY=$(echo $RAW_FREQUENCY | sed '$d')

SYM_COUNT=$( echo $RAW_FREQUENCY | tail -n1)

echo $FREQUENCY | awk -v sc=$SYM_COUNT '{print $1/sc, $2}'
