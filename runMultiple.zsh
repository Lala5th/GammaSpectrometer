#!/usr/bin/zsh

mkdir data 2> /dev/null
mkdir ECrits 2> /dev/null
for ((i = 0; i < 10; i++))
do  echo "/run/initialize" > r.mac
	echo "/gun/energy $(($RANDOM*1.5/100)) MeV" >> r.mac
	echo "/control/execute autogen.mac" >> r.mac
	./gamma_spectrometer r.mac | grep -i 'run' #2> /dev/null
	echo "-----------------------------------------"
	echo "Completed Run $i"
	echo "-----------------------------------------"
	mkdir "data/$i" >/dev/null 2>/dev/null
	mkdir "ECrits/$i" >/dev/null 2>/dev/null
	mv y.npy "data/$i"
	mv z.npy "data/$i"
	mv DepositedEnergy.npy "data/$i/DepositedEnergy.npy"
	mv Mangling.npy "ECrits/$i/Mangling.npy"
	mv Energy.npy "ECrits/$i/Energy.npy"
	mv ECrit.npy "ECrits/$i/ECrit.npy"
	#mv GeneratedParticles.npy "ECrits/$i/GeneratedParticles.npy"
done
