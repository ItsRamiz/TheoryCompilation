:reset
@echo on
Created By Ramiz Srour
cd C:\Users\El_Ra\Desktop\path_to_mcomp


gcc -g -c lex.yy.c

g++ -g -c miny.tab.cpp

g++ -g -c main.cpp

g++ -o pcodeGen main.o miny.tab.o lex.yy.o

pcodeGen ./input3.in

START /MAX C:\Users\El_Ra\Desktop\path_to_mcomp\outputFile.txt
PAUSE
cls
goto reset

