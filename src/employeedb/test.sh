#!/data/data/com.termux/files/usr/bin/bash
(
	printf "8\n"
	for i in `seq 0 1 4096`;do
		printf "1\nname$i\nsurname$i\n"
	done
	for i in `seq 0 1 4096`;do
		printf "3\n$i\n$RANDOM\n"
	done
	for i in `seq 0 1 4096`;do
		printf "4\n$i\n$RANDOM\n"
	done
	printf "5\n0\n"
)|./bin/a.out
