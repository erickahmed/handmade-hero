C := g++
STND := -std=c++03
OBJ_FL := -o
BLD_FL := -c
SRC32 := handmade32.cpp
SRC64 := handmade64.cpp
OBJ32 := handmade32.o
OBJ64 := handmade64.o
LIBS := -lgdi32 -luser32
#DEPS := .h .c

OUT32 := handmade32.exe
OUT64 := handmade64.exe

.PHONY: all clean

handmade32.o: handmade32.cpp
	$(C) $(SRC32) $(STND) $(LIBS) $(OBJ_FL) $(OUT32)

handmade32.exe: handmade.o
	$(C) $(OBJ32) $(BLD_FL) $(OUT32)

clean:
	rm handmade32.o