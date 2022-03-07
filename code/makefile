GCC := g++
STND := -std=c++20
OBJ_FL := -o
BLD_FL := -c
SRC64 := handmade32.cpp
SRC64 := handmade64.cpp
OBJ32 := handmade32.o
OBJ64 := handmade64.o
LIBS := -lgdi32 -luser32
#DEPS := .h .c

OUT32 := handmade32
OUT64 := handmade64

# Uncomment only the destination needed
# If both are uncommented, default destination is 32-bit
# If both are commented out, default destination in 64-bit
DEST := 32
#DEST := 64

.PHONY: all clean
all: handmade

if [$(DEST) == 32]; then \
    handmade32: handmade32.o
	$(GCC) $(STND) $(OBJ32) $(LIBS) $(OBJ_FL) $(OUT32)

	handmade32.o: handmande32.cpp
		$(GCC) $(BLD_FL) $(SRC32)
else \
    handmade64: handmade64.o
	$(GCC) $(STND) $(OBJ64) $(LIBS) $(OBJ_FL) $(OUT64)

	handmade64.o: handmande64.cpp
		$(GCC) $(BLD_FL) $(SRC64)
fi

clean:
	rm *.o output