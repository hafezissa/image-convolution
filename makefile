CFLAG=-Wall -ansi

filter: main_conv.o conv.o
	gcc -o filter $?
runall:
	./filter bill.ppm edge billME.ppm
	./filter chambers.ppm edge chambersME.ppm
	./filter flowers.ppm edge flowersME.ppm
clean:
	@rm -rf filter *.o 