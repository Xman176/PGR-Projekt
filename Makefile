FILE= main.cpp

RUNFILE= projekt


all: $(FILE)
	g++ $(FILE) -w -Wl, -lmingw32 -lSDL2main -lSDL2 -o $(RUNFILE)
