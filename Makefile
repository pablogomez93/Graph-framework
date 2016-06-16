CFLAGS = -Wall -Wfatal-errors -O3 -std=c++11
RM = rm
CC = g++
COMPILE = ${CC} ${CFLAGS}

.PHONY: cleantest compiletest runtest run_stress_test test stress_test

all: clean
	@${COMPILE} example.cpp graph.cpp -o main

run:
	@./main

clean:
	@${RM} -f main

clean_test:
	@${RM} -f test stress_test

compile_test:
	@${COMPILE} test.cpp graph.cpp -o test
	@${COMPILE} stress_test.cpp graph.cpp -o stress_test	

run_test:
	@./test
  
run_stress_test:
	@./stress_test

test: clean_test compile_test run_test

stress_test: clean_test compile_test run_stress_test