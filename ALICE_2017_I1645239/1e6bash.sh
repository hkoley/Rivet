#!/bin/bash
for i in {1..5}
do
	run-pythia -n $1 -o PythiaHeavy."$i".fifo -b "LEAD" -B 1575 -f "PROTON" -F 4000 -c "HeavyIon:mode 1" -r "$i"0"$i" >PythiaHeavypPb1e6new."$i".log &
	rivet --ignore-beams --pwd -a ALICE_2017_I1645239 PythiaHeavy."$i".fifo -o pythia8pPb5TeV1e6new."$i".yoda -n $1 >RivetPythia8pPb1e6new."$i".log &
done	
