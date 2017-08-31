#! /usr/local/bin/bash -l

function usage
{
    echo "Usage:"
    echo "$0 RC/Born = 1/2"
    exit
}

if [[ $# != 1 ]]; then
 usage
fi

if [[ $1 != 0 ]]; then
	echo "RC GENERATION"
	mkdir tempRC
	cd tempRC
	COUNTER=0
	COUNTERB=1
	while (( `echo "$COUNTER < 20" | bc` )); do
		echo "STEP" $COUNTER "OVER 20"
		/afs/in2p3.fr/home/n/npierre/npierre/TDJANGOH/bin/test /afs/in2p3.fr/home/n/npierre/npierre/TDJANGOH/utils/djangohSettings_RC_xbin_$COUNTER\_$COUNTERB\.xml 10000000
	  mv TDjangoh_out.dat Out_RC_xbin_$COUNTER\_$COUNTERB\.dat
    mv TDjangoh_evt.dat RC_10M_evt_xbin_$COUNTER\_$COUNTERB\.dat
		mv *.dat ..
		COUNTER=`echo "$COUNTER + 1" | bc`
		COUNTERB=`echo "$COUNTERB + 1" | bc`
	done
	cd ..
	rm -rf tempRC
else
	echo "BORN GENERATION"
	mkdir tempBorn
  cd tempBorn
	COUNTER=0
	COUNTERB=1
	while (( `echo "$COUNTER < 20" | bc` )); do
		echo "STEP" $COUNTER "OVER 20"
		/afs/in2p3.fr/home/n/npierre/npierre/TDJANGOH/bin/test /afs/in2p3.fr/home/n/npierre/npierre/TDJANGOH/utils/djangohSettings_Born_xbin_$COUNTER\_$COUNTERB\.xml 10000000
		mv TDjangoh_out.dat Out_Born_xbin_$COUNTER\_$COUNTERB\.dat
		mv TDjangoh_evt.dat Born_10M_evt_xbin_$COUNTER\_$COUNTERB\.dat
		mv *.dat ..
		COUNTER=`echo "$COUNTER + 1" | bc`
		COUNTERB=`echo "$COUNTERB + 1" | bc`
	done
	cd ..
	rm -rf tempBorn
fi
