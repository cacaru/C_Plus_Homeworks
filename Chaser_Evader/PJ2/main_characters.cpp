#include "Graph.h"
#include "Simple_window.h"
#include "Character.h"

using namespace Graph_lib;

int main()
{
	srand(time(nullptr));

	Point tl(100, 100);
	Simple_window win(tl, 800, 800, "Chasing");

	int x_size = win.x_max();
	int y_size = win.y_max();
	int x_grid = 100;
	int y_grid = 100;
	//set grid for easy to  see
	Lines grid;
	for (int x = x_grid; x<x_size; x += x_grid)	// veritcal lines
		grid.add(Point(x, 0), Point(x, y_size));
	for (int y = y_grid; y<y_size; y += y_grid)	// horizontal lines
		grid.add(Point(0, y), Point(x_size, y));
	grid.set_color(Color::black);
	win.attach(grid);

	vector<Character*> characters;

	Pursuer* eater = new Pursuer(&win, 3, 0);
	Evader* ghost = new Evader(&win, 4, 7);

	eater->setEnemy(ghost);
	ghost->setEnemy(eater);

	characters.push_back(eater);
	characters.push_back(ghost);

	Graph_lib::Text endsign(Point(200, 300), "THE END");
	endsign.set_font_size(100);
	endsign.set_style(Line_style::solid);
	endsign.set_color(Color::black);

	int end = 0;

	do
	{
		win.wait_for_button();
		for (Character* ch : characters)
		{
			ch->move();
			if (eater->col() == ghost->col() && eater->row() == ghost->row()) {
				win.attach(endsign);
				end = 1;
			}
		}
		if (end == 1)
			break;
	} while (true);

	win.wait_for_button();
}
