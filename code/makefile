C := g++
STND := -std=c++03
OBJ_FL := -o
BLD_FL := -c
SRC := handmade.cpp
OBJ := handmade.o
LIBS := -lgdi32 -luser32
DEPS :=  framework.c

OUT := handmade.exe

.PHONY: all clean

object: $(SRC)
	$(C) $(SRC) $(STND) $(DEPS) $(LIBS) $(OBJ_FL) $(OUT)

executable: $(OBJ)
	$(C) $(OBJ) $(BLD_FL) $(OUT)

clean:
	rm $(OBJ)