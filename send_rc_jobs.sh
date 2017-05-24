#! /usr/local/bin/bash

function usage
{
    echo "Usage:"
    echo "$1 output path"
    exit
}

if [[ $# != 1 ]]; then
 usage
fi

COUNTER=0
while (( `echo "$COUNTER < 20" | bc` )); do
#        qsub -P P_compass -N RC_TDj -q long -l s_rss=4G -l sps=1  ./rc_batch.sh 1 $COUNTER $1
        qsub -P P_compass -N Born_TDj -q long -l s_rss=4G -l sps=1  ./rc_batch.sh 2 $COUNTER $1
        COUNTER=`echo "$COUNTER + 1" | bc`
done
