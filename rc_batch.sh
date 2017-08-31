#! /usr/local/bin/bash -l

function usage
{
    echo "Usage:"
    echo "$1 RC/Born = 1/2"
    echo "$2 xbin [0,19]"
    echo "$3 output path"
    exit
}

if [[ $# != 3 ]]; then
 usage
fi

cd $3
echo "XBIN = " $2
COUNTER=$2
COUNTERB=$(($COUNTER+1))
if [[ $1 != 2 ]]; then
  echo "RC GENERATION"
  mkdir $2\_RC
  cd $2\_RC
	/afs/in2p3.fr/home/n/npierre/npierre/TDJANGOH/bin/test /afs/in2p3.fr/home/n/npierre/npierre/TDJANGOH/utils/djangohSettings_RC_xbin_$2\_$COUNTERB\.xml 10000000 -verbose 0
	mv TDjangoh_out.dat Out_RC_xbin_$2\_$COUNTERB\.dat
  mv TDjangoh_evt.dat RC_10M_evt_xbin_$2\_$COUNTERB\.dat
  mv *_$2\_$COUNTERB\.dat ..
	cd ..
	rm -rf $2\_RC
else
  echo "BORN GENERATION"
  mkdir $2\_BORN
  cd $2\_BORN
  /afs/in2p3.fr/home/n/npierre/npierre/TDJANGOH/bin/test /afs/in2p3.fr/home/n/npierre/npierre/TDJANGOH/utils/djangohSettings_Born_xbin_$2\_$COUNTERB\.xml 10000000 -verbose 0
  mv TDjangoh_out.dat Out_Born_xbin_$COUNTER\_$COUNTERB\.dat
  mv TDjangoh_evt.dat Born_10M_evt_xbin_$COUNTER\_$COUNTERB\.dat
  mv *_$2\_$COUNTERB\.dat ..
	cd ..
	rm -rf $2\_BORN
fi
echo "JOB DONE"
