SHELL=/bin/bash
# compil
CC = gcc
CFLAGS = -O2 -Wall -g

# dirs
#TESTDIR = $(dir $(lastword $(MAKEFILE_LIST)))
#CURDIR = $(shell pwd)

# targets
EXEC = $(patsubst %.c,%,$(wildcard *.c))
TESTS = test_args test_ls test_wc test_script test_paral
.PHONY: all $(TESTS)

# rules
all: $(TESTS)

$(TESTS): $(EXEC) testdir

# crée le répertoire testdir et quelques fichiers à l'intérieur
testdir:
	mkdir -p testdir/cde
	for f in a b c d e f; do mkdir testdir/$$f; touch testdir/$$f/vide$$f; done
	seq 100 >testdir/a/100
	seq 1000 >testdir/b/1000
	echo "bonjour" >testdir/b/bonjour
	for (( i=0 ; i<200 ; i++ )) ; do echo "hello" >>testdir/b/bonjour200 ; done
	for (( i=0 ; i<1000 ; i++ )) ; do echo $$i >testdir/c/i$$i ; done
	mkdir -p  testdir/d/e/f/g/h/i/j/k/l/m/n
	echo abc >testdir/d/e/f/g/h/i/j/k/l/m/n/abc

distclean: clean
	/bin/rm -rf testdir
clean:
	/bin/rm -f $(EXEC) *.o

test_args:
	@echo "################### TESTING: $@"
	./$(EXEC) -h >/dev/null 2>&1
	./$(EXEC) -unknown_option >/dev/null 2>&1 || exit 0 && exit 1
	./$(EXEC) -n 10 -e wc testdir/a >/dev/null 2>&1
	@echo "TESTING: $@ passed"

test_ls:
	@echo "################### TESTING: $@"
	./$(EXEC) -e ls testdir >test_sortie_ls
	./$(EXEC)       testdir >test_sortie_sans_arg
	diff test_sortie_ls test_sortie_sans_arg

	./$(EXEC) -e ls testdir/a testdir/b testdir/d >test_sortie_ls3
	./$(EXEC)       testdir/a testdir/b testdir/d >test_sortie_sans_arg3
	diff test_sortie_ls3 test_sortie_sans_arg3

	@echo "TESTING: $@ passed"

test_wc:
	@echo "################### TESTING: $@"
	./$(EXEC) -e wc testdir >test_sortie_wc
	test `egrep "^[[:space:]]*1[[:space:]]" test_sortie_wc | wc -l` -eq 1002
	test `egrep "^[[:space:]]*0[[:space:]]" test_sortie_wc | wc -l` -eq 6
	test `egrep "^[[:space:]]*1000[[:space:]]" test_sortie_wc | wc -l` -eq 1
	@/bin/rm -r test_sortie_wc
	@echo "TESTING: $@ passed"

test_script:
	@echo "################### TESTING: $@"
	echo "#!/bin/bash" >test_script
	echo "grep 10 \$$1 && echo ^\$$1" >>test_script
	echo "exit 0" >>test_script
	chmod +x test_script
	./$(EXEC) -e ./test_script testdir >test_sortie_script
	test `cat test_sortie_script | wc -l` -eq 65
	./$(EXEC) -n 10 -e ./test_script testdir >test_sortie_script
	test `cat test_sortie_script | wc -l` -eq 65
	@/bin/rm -f test_script test_sortie_script
	@echo "TESTING: $@ passed"

test_paral:
		@echo "################### TESTING: $@"
		# avec -n 1 la sortie devrait être identique
		./$(EXEC) -e ls testdir >test_sortie_ls
		./$(EXEC) -n 1  testdir >test_sortie_sans_arg
		diff test_sortie_ls test_sortie_sans_arg
		@/bin/rm -f test_sortie_ls test_sortie_sans_arg
		# avec -n 20 la sortie devrait être différente
		./$(EXEC) -e ls testdir >test_sortie_ls
		./$(EXEC) -n 20  testdir >test_sortie_sans_arg
		diff test_sortie_ls test_sortie_sans_arg >/dev/null || exit 0 && exit 1
		# mais triées elles doivent être identiques
		sort test_sortie_ls >test_sortie_ls2
		sort test_sortie_sans_arg >test_sortie_sans_arg2
		diff test_sortie_ls2 test_sortie_sans_arg2
		@/bin/rm -f test_sortie_ls test_sortie_sans_arg test_sortie_ls2 test_sortie_sans_arg2
		@echo "TESTING: $@ passed"
