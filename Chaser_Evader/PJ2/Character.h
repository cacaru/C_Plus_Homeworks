#pragma once

#include "Window.h"
#include "Graph.h"

class Character
{
public:
	Character(Graph_lib::Window* win, int r, int c);
	void move();

	void setEnemy(Character* e) { my_enemy = e; }
	Character* enemy() { return my_enemy; }

	int row() { return pos_row; }
	int col() { return pos_col; }

protected:
	virtual pair<int, int> nextDirection();

	int pos_row;
	int pos_col;

	Graph_lib::Window* my_window;
	Character* my_enemy;

	vector<Graph_lib::Shape*> body_shapes;
};

class Dummy : public Character   
{
public:
	Dummy(Graph_lib::Window* win, int r, int c);

protected:
	pair<int, int> nextDirection() override;
};

//--------------------------------- real character -------------------------------------
class Pursuer : public Character
{// create Pursuer class  like Dummy
public:
	Pursuer(Graph_lib::Window* win, int r, int c); // constructor for Pursuer
protected:
	pair<int, int> nextDirection() override;
};

class Evader : public Character
{ // create Evader class like Dummy 
public:
	Evader(Graph_lib::Window* win, int r, int c); // constructor for Evader
protected:
	pair<int, int> nextDirection() override;
};