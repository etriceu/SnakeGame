OBJ=snake
LIBS=-lsfml-window -lsfml-graphics -lsfml-system

all:
	g++ -O3 $(LIBS) *.cpp -o $(OBJ)

.PHONY: clean

clean:
	rm -f $(OBJ)
