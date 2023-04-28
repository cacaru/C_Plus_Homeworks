#include "Graph.h"
#include "Simple_window.h"

using namespace Graph_lib;

int main()
{
	Simple_window win(Point(100,100), 800, 800, "Shapes"); 
	Point ab(400, 400);

	Vector_ref<Graph_lib::Box> vb;
	Vector_ref<Graph_lib::Regular_ploygon> vr;
	Vector_ref<Graph_lib::Star> vs;
	int h = 100;;
	int w, r, n;
	int a = 10;
	int b = 10;
	// drawing Boxes
	for (int i = 0; i < 3; i++) {
		r = 5;
		a = 10;
		w = 50;
		for (int j = 0; j < 3; j++) {
			vb.push_back(new Graph_lib::Box(Point(a, b), w, h, r));
			vb[vb.size() - 1].set_fill_color(i + j * 16);
			vb[vb.size() - 1].set_color(i + j);
			win.attach(vb[vb.size() - 1]);
			w += 100;
			r += 20;
			a += 200;
		}
		h += 100;
		b += 220;
	}
	win.wait_for_button();
	for (int i = 0; i < vb.size(); i++)
		win.detach(vb[i]);

	//Drawing Regular_polygons
	a = 70; b = 70; r = 50; n = 3;
	for (int i = 0; i < 3; i++)
	{
		a = 70;
		for (int j = 0; j < 3; j++)
		{
			vr.push_back(new Graph_lib::Regular_ploygon(Point(a, b), r, n));
			vr[vr.size() - 1].set_color(i + j);
			vr[vr.size() - 1].set_fill_color(i + j);
			win.attach(vr[vr.size() - 1]);
			n++;
			a += 250;
		}
		b += 250;
	}
	win.wait_for_button();
	for (int i = 0; i < vr.size(); i++)
		win.detach(vr[i]);

	//Drawing Stars
	a = 70; b = 70; r = 20; n = 3;
	int r2 = 50;
	for (int i = 0; i < 3; i++)
	{
		a = 70;
		for (int j = 0; j < 3; j++)
		{
			vs.push_back(new Graph_lib::Star(Point(a, b), r, r2, n));
			vs[vs.size() - 1].set_color(i + j);
			vs[vs.size() - 1].set_fill_color(i + j);
			win.attach(vs[vs.size() - 1]);
			n++;
			a += 250;
		}
		b += 250;
	}

	win.wait_for_button();
}
