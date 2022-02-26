#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

#define CELL_SIZE 16
#define SPEED 4

int width = 20, height = 20;
class Snake : public sf::Drawable
{
public:
	enum {UP, DOWN, LEFT, RIGHT};
	
	Snake()
	{
		head.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
		body = head;
		food = head;
		head.setFillColor(sf::Color(0, 255, 0));
		body.setFillColor(sf::Color(0, 128, 0));
		food.setFillColor(sf::Color(255, 0, 0));
		
		int x = width/2, y = height/2;
		bodyParts.clear();
		bodyParts.push_back({x, y});
		bodyParts.push_back({x, y+1});
		bodyParts.push_back({x, y+2});
		
		spawnFood();
		dir = UP;
		grow = false;
		gameover = false;
	}
	
	void setDir(char d)
	{
		if(d == UP && dirOld == DOWN || d == DOWN && dirOld == UP ||
			d == LEFT && dirOld == RIGHT || d == RIGHT && dirOld == LEFT)
			return;
		
		dir = d;
	}
	
	void update()
	{
		if(gameover)
			return;
		
		sf::Vector2i last = bodyParts.back();
		for(int n = bodyParts.size()-1; n > 0; n--)
			bodyParts[n] = bodyParts[n-1];

		if(grow)
		{
			bodyParts.push_back(last);
			grow = false;
		}
		
		if(dir == UP) bodyParts[0].y--;
		else if(dir == DOWN) bodyParts[0].y++;
		else if(dir == LEFT) bodyParts[0].x--;
		else if(dir == RIGHT) bodyParts[0].x++;
		
		if(!sf::Rect(0, 0, width, height).contains(bodyParts[0]))
		{
			gameover = true;
			body.setFillColor(sf::Color(100, 100, 100));
			return;
		}
		
		for(int n = 1; n < bodyParts.size(); n++)
			if(bodyParts[n] == bodyParts[0])
			{
				gameover = true;
				body.setFillColor(sf::Color(100, 100, 100));
				return;
			}
			
		if(bodyParts[0] == foodPos && !gameover)
		{
			spawnFood();
			grow = true;
		}
		
		dirOld = dir;
	}
	
// 	bool isGameover()
// 	{
// 		return gameover;
// 	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(food);
		sf::RectangleShape shape = head;
		shape.setPosition((sf::Vector2f)(bodyParts[0]*CELL_SIZE));
		target.draw(shape);
		shape = body;
		
		for(auto it = bodyParts.begin()+1; it != bodyParts.end(); it++)
		{
			shape.setPosition((sf::Vector2f)((*it)*CELL_SIZE));
			target.draw(shape);
		}
	}
	
	void spawnFood()
	{
		bool invalid;
		do
		{
			invalid = false;
			foodPos = {rand()%width, rand()%width};
			for(auto pos : bodyParts)
				if(pos == foodPos)
				{
					invalid = true;
					break;
				}
		}
		while(invalid);
		food.setPosition((sf::Vector2f)(foodPos*CELL_SIZE));
	}
	
	sf::RectangleShape head, body, food;
	sf::Vector2i foodPos;
	vector <sf::Vector2i> bodyParts;
	char dir, dirOld;
	bool grow, gameover;
};

int main(int argc, char **argv)
{
	srand(time(NULL));
	if(argc == 3)
	{
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}
	else if(argc > 1)
		printf("%s [width] [height]\n", argv[0]);
	
	sf::RenderWindow window(sf::VideoMode(width*CELL_SIZE, height*CELL_SIZE), "Snake");
	window.setFramerateLimit(60);
	Snake snake;
	sf::Clock timer;
	
	while(window.isOpen())
	{
		sf::Event e;
		while(window.pollEvent(e))
			if(e.type == sf::Event::KeyPressed)
			{
				if(e.key.code == sf::Keyboard::W) snake.setDir(Snake::UP);
				else if(e.key.code == sf::Keyboard::S) snake.setDir(Snake::DOWN);
				else if(e.key.code == sf::Keyboard::A) snake.setDir(Snake::LEFT);
				else if(e.key.code == sf::Keyboard::D) snake.setDir(Snake::RIGHT);
			}
			else if(e.type == sf::Event::Closed)
				window.close();
		
		if(timer.getElapsedTime() >= sf::seconds(1.0/SPEED))
		{
			snake.update();
			timer.restart();
// 			if(snake.isGameover())
// 				snake = Snake();
		}
		
		window.clear();
		window.draw(snake);
		window.display();
	}
	return 0;
}
