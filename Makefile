FILE= main.cpp

RUNFILE= projekt

INCLUDE= C:\Development\SDL-mingw32\include
STATLIB= C:\Development\SDL-mingw32\lib



all: $(FILE)
	g++ $(FILE) -I$(INCLUDE) -I$(STATLIB) -w -Wl, -lmingw32 -lSDL2main -lSDL2 -o $(RUNFILE)
