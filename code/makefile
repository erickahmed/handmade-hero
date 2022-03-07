handmade: win32_handmade.o
	g++ win32_handmade.o -lgdi32 -luser32 -o handmade

win32_handmade.o: win32_handmade.cpp
	g++ -c win32_handmade.cpp

clean:
	rm *.o output