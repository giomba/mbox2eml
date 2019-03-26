compile:
	g++ -o mbox2eml -std=c++11 -Wall mbox2eml.cpp

clean:
	rm mbox2eml
	rm *.eml
