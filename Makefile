OBJ=snake
LIBS=-lsfml-window -lsfml-graphics -lsfml-system -lpthread

all:
	g++ *.cpp -o $(OBJ) -O3 $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ)
