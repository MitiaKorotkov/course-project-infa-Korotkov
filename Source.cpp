//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <math.h>
//#include <time.h>

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct Point;
class Placement;

bool ivent_with_prob(double prob);
double dist(Point x, Point y);
double min_dist_sum(vector<Point> points, Point A, Point B, Point C);
Placement crossing(vector<Point> points, Placement mom, Placement dad);
Placement mutation(vector<Point> points, Placement& individual);
bool comp(Placement left, Placement right);
void selection(vector<Point> points, vector<Placement>& population);

struct Point
{
	double x;
	double y;

	Point() : x(0), y(0) {}

	Point(double _x, double _y) {
		x = _x;
		y = _y;
	}

	friend bool operator==(const Point& left, const Point& right) {
		return (left.x == right.x && left.y == right.y);
	}
};

class Placement
{
public:
	Point A;
	Point B;
	Point C;

public:
	double k;
	double gen[6];

public:
	Placement() {
		gen[0] = A.x;
		gen[1] = A.y;
		gen[2] = B.x;
		gen[3] = B.y;
		gen[4] = C.x;
		gen[5] = C.y;
		k = 0;
	}
	Placement(vector<Point> points, Point _A, Point _B, Point _C) {
		gen[0] = A.x = _A.x;
		gen[1] = A.y = _A.y;
		gen[2] = B.x = _B.x;
		gen[3] = B.y = _B.y;
		gen[4] = C.x = _C.x;
		gen[5] = C.y = _C.y;
		k = min_dist_sum(points, A, B, C);
	}

	friend bool operator==(const Placement& left, const Placement& right) {
		return ((left.A == right.A) && (left.B == right.B) && (left.C == right.C));
	}

	void recalculate_k(vector<Point> points) {
		k = min_dist_sum(points, A, B, C);
	}
};

bool ivent_with_prob(double prob) {
	return rand() % 100 < (prob * 100);
}

double dist(Point x, Point y) {
	return sqrt((x.x - y.x) * (x.x - y.x) + (x.y - y.y) * (x.y - y.y));
}

double min_dist_sum(vector<Point> points, Point A, Point B, Point C) {
	double sum = 0;
	for (auto i : points) {
		sum += min(min(dist(A, i), dist(B, i)), dist(C, i));
	}
	return sum;
}

Placement crossing(vector<Point> points, Placement mom, Placement dad) {
	int a = rand() % 12, b = rand() % 12, c = rand() % 12,
		d = rand() % 12, e = rand() % 12, f = rand() % 12;
	double tmp_arr[12];
	for (int i = 0; i < 6; ++i)
		tmp_arr[i] = mom.gen[i];
	for (int i = 6; i < 12; ++i)
		tmp_arr[i] = dad.gen[i / 2];
	Point A(tmp_arr[a], tmp_arr[d]);
	Point B(tmp_arr[b], tmp_arr[e]);
	Point C(tmp_arr[c], tmp_arr[f]);
	Placement tmp(points, A, B, C);
	return tmp;
}

Placement mutation(vector<Point> points, Placement& individual) {
	int n = rand() % 6;
	for (int i = 0; i < n; ++i) {
		int j = rand() % 6;
		individual.gen[j] += rand() % 1000 - 500;
	}
	return individual;
}

bool comp(Placement left, Placement right) {
	return left.k < right.k;
}

void selection(vector<Point> points, vector<Placement>& population) {
	for (auto& i : population)
		i.recalculate_k(points);
	sort(population.begin(), population.end(), comp);
	while (population.size() > 100)
		population.pop_back();
}

int main() {
	srand(time(NULL));
	
	vector<Point> M;
	int N = 100;

	for (int i = 0; i < 100; ++i) {
		double x = -12000 + rand() % 10000 - 5000, 
			y = -12000 + rand() % 10000 - 5000;
		Point tmp(x, y);
		M.push_back(tmp);
	}
	for (int i = 0; i < 100; ++i) {
		double x = -12000 + rand() % 10000 - 5000,
			y = -24000 + rand() % 10000 - 5000;
		Point tmp(x, y);
		M.push_back(tmp);
	}
	for (int i = 0; i < 100; ++i) {
		double x = -24000 + rand() % 10000 - 5000,
			y = -12000 + rand() % 10000 - 5000;
		Point tmp(x, y);
		M.push_back(tmp);
	}
	/*for (int i = 0; i < N; ++i) {
		double x = rand() % 130000 - 65000, y = rand() % 130000 - 65000;
		Point tmp(x, y);
		M.push_back(tmp);
	}*/
	
	vector<Placement> population;
	for (int i = 0; i < 100; ++i) {
		double x1 = rand() % 130000 - 65000, x2 = rand() % 130000 - 65000, x3 = rand() % 130000 - 65000,
			y1 = rand() % 130000 - 65000, y2 = rand() % 130000 - 65000, y3 = rand() % 130000 - 65000;
		Point a(x1, y1);
		Point b(x2, y2);
		Point c(x3, y3);
		Placement tmp(M, a, b, c);
		population.push_back(tmp);
	}


	RenderWindow window(sf::VideoMode(2000, 1000), L"Govnocode");
	window.setPosition(Vector2i(0, 0));
	window.setFramerateLimit(50);

	vector<CircleShape> dots;
	for (auto i : M) {
		CircleShape tmp(2);
		tmp.setFillColor(Color::Green);
		tmp.setPosition((i.x + 65000) / 65, (i.y + 65000) / 65);
		dots.push_back(tmp);
		window.draw(tmp);
	}
	CircleShape dot_a(3), dot_b(3), dot_c(3);
	dot_a.setFillColor(Color::Blue);
	dot_b.setFillColor(Color::Blue);
	dot_c.setFillColor(Color::Blue);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		bool flag = true;
		if (flag)
			for (int generation = 0; generation < 100; ++generation) {
				//скрещиванипе
				for (int i = 0; i < 80; ++i)
					population.pop_back();
				for (int i = 0; i < 19; ++i) {
					for (int k = i + 1; k < 20; ++k)
						population.push_back(crossing(M, population[i], population[k]));
				}

				//мутации
				double prob_of_mutation = 0.14;
				for (auto& i : population)
					if (ivent_with_prob(prob_of_mutation))
						i = mutation(M, i);

				//отбор
				selection(M, population);

				window.clear();
				for (auto i : dots)
					window.draw(i);
				double mini = 0;
				for (int i = 0; i < population.size(); ++i)
					if (population[mini].k > population[i].k)
						mini = i;

				dot_a.setPosition((population[mini].A.x + 65000) / 65, (population[mini].A.y + 65000) / 65);
				dot_b.setPosition((population[mini].B.x + 65000) / 65, (population[mini].B.y + 65000) / 65);
				dot_c.setPosition((population[mini].C.x + 65000) / 65, (population[mini].C.y + 65000) / 65);
				window.draw(dot_a);
				window.draw(dot_b);
				window.draw(dot_c);
				window.display();
				if (generation == 99)
					flag = false;
			}
	}
	
	return 0;
}