PRG=gnu.exe
GCC=clang
GCCFLAGS=-Werror -Wall -Wextra -std=c11 -pedantic -g -O2 -pthread

OBJECTS0=cachelist.c PRNG.c
DRIVER0=driver.c

VALGRIND_OPTIONS=-q --leak-check=full
DIFF_OPTIONS=-y -W200 --strip-trailing-cr --suppress-common-lines -b

OSTYPE := $(shell uname)
ifeq ($(OSTYPE),Linux)
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

clang:
	clang -o $(PRG) $(CYGWIN) $(DRIVER0) $(OBJECTS0) $(GCCFLAGS)
gcc:
	gcc -o $(PRG) $(CYGWIN) $(DRIVER0) $(OBJECTS0) $(GCCFLAGS)
pdf-tablen:
	/home/mmead/bin/tablen -v cachelist.c >>studentout$@
	/home/mmead/bin/checkpdf.sh refman.pdf >>studentout$@
	diff output.txt studentout$@ $(DIFF_OPTIONS) >> difference$@

0:
	#echo "running test$@"
	#@echo "should run in less than 200 ms"
	@echo "should run in less than 200 ms"
	./$(PRG) input.txt >studentout$@
	#diff output$@.txt studentout$@ $(DIFF_OPTIONS) > difference$@
clean : 
	rm *.exe student* difference*
