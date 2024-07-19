legalizer: Legalizer_One.o ; g++ -o legalizer Legalizer_One.o -lstdc++ 
Legalizer_One.o: Legalizer_One.cpp ; gcc -c Legalizer_One.cpp
clean: ; rm *.o legalizer