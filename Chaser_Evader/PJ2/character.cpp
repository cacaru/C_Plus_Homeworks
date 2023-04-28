#include "Character.h"

using namespace Graph_lib;

const int grid_width = 8;
const int grid_height = 8;

Character::Character(Graph_lib::Window* win, int r, int c)
	: my_window{ win }, pos_row{ r }, pos_col{ c }, my_enemy{ nullptr }
{
}

void Character::move()
{
	pair<int, int> dir = nextDirection();

	int dr = dir.first;
	int dc = dir.second;
	pos_row += dr;
	pos_col += dc;

	int w = my_window->x_max() / grid_width;
	int h = my_window->y_max() / grid_height;
	for (Shape* s : body_shapes)
	{
		s->move(w*dc, h*dr);
	}
}

pair<int, int> Character::nextDirection()
{
	return make_pair(0, 0);
}

Dummy::Dummy(Graph_lib::Window * win, int r, int c)
	: Character(win, r, c)
{
	int w = win->x_max() / grid_width;
	int h = win->y_max() / grid_height;

	Graph_lib::Ellipse* body = new Graph_lib::Ellipse(Point(c*w + w / 2, r*h + h / 2), w / 2, h / 2);
	body_shapes.push_back(body);

	win->attach(*body);
}

pair<int, int> Dummy::nextDirection()
{
	vector<pair<int, int>> directions = { { -1,0 },{ +1,0 },{ 0,-1 },{ 0,+1 } };
	random_shuffle(directions.begin(), directions.end());

	int dr = 0, dc = 0, new_r = 0, new_c = 0;
	for (int i = 0; i < 4; i++)
	{
		dr = directions[i].first;
		dc = directions[i].second;
		new_r = pos_row + dr;
		new_c = pos_col + dc;

		if (new_r >= 0 && new_r < grid_height &&
			new_c >= 0 && new_c < grid_width)
			return directions[i];
	}
}

//----------------------------------------new code------------------------------------------


Pursuer::Pursuer(Graph_lib::Window* win, int r, int c) : Character(win, r, c)
{
	int w = win->x_max() / grid_width;
	int h = win->y_max() / grid_height;
	 // Create Pursuer's shapes , 5 pieces of 2 series
	Graph_lib::Circle* body0 = new Graph_lib::Circle(Point(c*w + w / 2, r*h + h / 2), w / 2);
	Graph_lib::Regular_ploygon* body1 = new Graph_lib::Regular_ploygon(Point(c*w + 3 * w / 4, r*h + h / 2), w / 4, 4);
	Graph_lib::Regular_ploygon* body2 = new Graph_lib::Regular_ploygon(Point(c*w + w / 2, r*h + h / 2), w / 4, 4);
	Graph_lib::Regular_ploygon* body3 = new Graph_lib::Regular_ploygon(Point(c*w + w / 4, r*h + h / 2), w / 4, 4);
	Graph_lib::Circle* body4 = new Graph_lib::Circle(Point(c*w + w / 2, r*h + h / 2), w / 8);

	// set color area
	body0->set_color(Color::black);
	body0->set_fill_color(Color::dark_blue);
	body1->set_color(Color::red);
	body1->set_fill_color(Color::red);
	body2->set_color(Color::red);
	body2->set_fill_color(Color::red);
	body3->set_color(Color::red);
	body3->set_fill_color(Color::red);
	body4->set_color(Color::black);
	body4->set_fill_color(Color::black);
	// save body shape
	body_shapes.push_back(body0);
	body_shapes.push_back(body1);
	body_shapes.push_back(body2);
	body_shapes.push_back(body3);
	body_shapes.push_back(body4);

	// attach for see
	win->attach(*body0);
	win->attach(*body1);
	win->attach(*body2);
	win->attach(*body3);
	win->attach(*body4);
	
}

pair<int, int> Pursuer::nextDirection()
{   // direction's type   
	//{-1,0} = up, {1,0} = down, {0,-1} = left, {0,1} = right 
	//create for move , declare pair type vector
	vector<pair<int, int>> directions ;
	// get my locate and enemy's locate (each value) >  use more comfortable, substitute function for value
	int Y = row();
	int X = col();
	int Y1 = my_enemy->row();
	int X1 = my_enemy->col();
	// how Pursuer move to the Evader for catch, set directions for each case
	if (X < X1) {
		if (Y < Y1) directions = { { 1,0 },{ 0,1 } };
		else if (Y > Y1) directions = { { -1,0 },{ 0,1 } };
		else directions = { { 0,1 } ,{0,1} };
	}
	else if (X > X1) {
		if (Y < Y1) directions = { { 1,0 },{ 0,-1 } };
		else if (Y > Y1) directions = { { -1,0 },{ 0,-1 } };
		else directions = { { 0,-1 },{0,-1} };
	}
	else {
		if (Y < Y1) directions = { { 1,0 }, {1,0} };
		else if (Y > Y1)directions = { { -1,0 },{-1,0} };
		else directions = { { 0,0 },{0,0} };
	}
	// set over , so shuffle value!
	random_shuffle(directions.begin(), directions.end());
	// move directions part
	int dr = 0, dc = 0, new_r = 0, new_c = 0;
	for (int i = 0; i < 2; i++)
	{
		dr = directions[i].first;
		dc = directions[i].second;
		new_r = pos_row + dr;
		new_c = pos_col + dc;

		if (new_r >= 0 && new_r < grid_height && new_c >= 0 && new_c < grid_width)
			return directions[i];
	}
}

Evader::Evader(Graph_lib::Window* win, int r, int c) : Character(win, r, c)
{
	int w = win->x_max() / grid_width;
	int h = win->y_max() / grid_height;
	// Create Evader's shape  6 pieces of 3 series
	Graph_lib::Circle* body0 = new Graph_lib::Circle(Point(c*w + w / 2, r*h + h / 4), w / 4);
	Graph_lib::Rectangle* body1 = new Graph_lib::Rectangle(Point(c*w + w/4, r*h + h / 4), w/2, 5*h/8 );
	Graph_lib::Regular_ploygon* body2 = new Graph_lib::Regular_ploygon(Point(c*w + w / 4, r*h + 5*h / 8), w/16 ,4);
	Graph_lib::Regular_ploygon* body3 = new Graph_lib::Regular_ploygon(Point(c*w + 3*w / 4, r*h + 5 * h / 8), w / 16, 4);
	Graph_lib::Circle* body4 = new Graph_lib::Circle(Point(c*w + 6 * w / 16, r*h + h / 4), h / 16);
	Graph_lib::Circle* body5 = new Graph_lib::Circle(Point(c*w + 10 * w / 16, r*h + h / 4), h / 16);

	//set color Evader like cuty
	body0->set_color(Color::white);
	body0->set_fill_color(Color::white);
	body1->set_color(Color::white);
	body1->set_fill_color(Color::white);
	body2->set_color(Color::white);
	body2->set_fill_color(Color::white);
	body3->set_color(Color::white);
	body3->set_fill_color(Color::white);
	body4->set_color(Color::cyan);
	body4->set_fill_color(Color::cyan);
	body5->set_color(Color::cyan);
	body5->set_fill_color(Color::cyan);

	//save Evader's shpae
	body_shapes.push_back(body0);
	body_shapes.push_back(body1);
	body_shapes.push_back(body2);
	body_shapes.push_back(body3);
	body_shapes.push_back(body4);
	body_shapes.push_back(body5);
	//attach for see
	win->attach(*body0);
	win->attach(*body1);
	win->attach(*body2);
	win->attach(*body3);
	win->attach(*body4);
	win->attach(*body5);

}

pair<int, int> Evader::nextDirection()
{  // direction's type   :
   //{-1,0} = up, {1,0} = down, {0,-1} = left, {0,1} = right 
   //create for move , declare pair type vector
	vector<pair<int, int>> directions;
	// get my locate and enemy's locate (each value) >  use more comfortable, substitute function for value
	int Y1 = my_enemy->row();
	int X1 = my_enemy->col();
	int Y = row();
	int X = col();
	// how Pursuer move to the Evader for catch, set directions for each case
	if (X < X1) {
		if (Y < Y1) directions = { {-1,0}, {0,-1},{1,0} };
		else if (Y > Y1) directions = { {1,0}, {0,-1}, {-1,0} };
		else directions = { {1,0},{-1,0},{0,-1} };
	}
	else if (X > X1) {
		if (Y < Y1) directions = { { -1,0 },{ 0,1 },{ 0,-1 } };
		else if (Y > Y1) directions = { { 1,0 },{ 0,1 },{ -1,0 } };
		else directions = { { 1,0 },{ -1,0 },{ 0,1 } };
	}
	else {
		if (Y < Y1) directions = { { -1,0 },{ 0,-1 },{ 0,1 } };
		else if (Y > Y1) directions = { { 1,0 },{ 0,-1 },{ 0,1 } };
		else directions = { { 0,0 },{ 0,0 },{ 0,0 } };
	}
	// set over , so shuffle value!
	random_shuffle(directions.begin(), directions.end());
	// move directions part
	int dr = 0, dc = 0, new_r = 0, new_c = 0;
	for (int j = 0; j < 3; j++)
	{
		dr = directions[j].first;
		dc = directions[j].second;
		new_r = pos_row + dr;
		new_c = pos_col + dc;

		if (new_r >= 0 && new_r < grid_height && new_c >= 0 && new_c < grid_width)
		{
			return directions[j];
		}
	}
}
