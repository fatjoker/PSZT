#include <limits>
#include "PathFinder.h"
#include <iomanip>
#include <ctime>


void main(int argc, char** argv)
{
	Graph g("in0.txt");
	cout << "NODES: \n";
	for (auto i = g.nodes.begin(); i != g.nodes.end(); ++i)
	{
		Node& n = i->second;
		printf("Node: %s (%lf, %lf)\n", n.name.c_str(), n.point.x, n.point.y);
		cout << "connected with: \n";
		for (auto e = n.edges.begin(); e != n.edges.end(); e++)
		{
			printf("\t%s cost: %lf\n", e->otherNode(&n)->name.c_str(), e->cost());
		}
	}
	PathFinder pf(&g);
	Node& src = g.nodes["A"];
	Node& trgt = g.nodes["C"];
	Path& sp = pf.A_star(src, trgt);
	for (auto i = sp.begin(); i != sp.end(); i++)
	{
		Node* n = *i;
		cout << n->name << " - ";
	}
	cout << "\Total cost: " << sp.total_cost;
	getchar(); getchar();
	return;
}
