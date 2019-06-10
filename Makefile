LIBS=-lm -lrt


exam: main.cpp platform.h ANS.h
	g++ -o $@ $< -O3 $(LIBS)
