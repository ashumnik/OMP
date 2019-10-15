IFS=$'\n'
FILENAME=$1

RAW_FREQUENCY=$(./freq.sh $FILENAME)
IFS=$' '
FREQUENCY=$( echo $RAW_FREQUENCY | sed -r '$d' )


SYM_COUNT=$( echo $RAW_FREQUENCY | tail -n1 )

echo '|Относительная частота|Байт|'
echo '|---------------------|----|'

echo $FREQUENCY | awk --non-decimal-data -v sc=$SYM_COUNT '
{
    if(int("0x"$2) <= 31){
        print "|", $1/sc,"\t|",$2 , "|"
    }
}'
