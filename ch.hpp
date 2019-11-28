#pragma once

#include <vector>

// input: a cloud of points
// output: the convex hull polygon (as a sequence of indices of the points)
std::vector<int> convexHull(const std::vector<P>& p);

struct P { int x, y; };

P operator+(P a, P b) {
	return {a.x + b.x, a.y + b.y}; 
}

P operator-(P a, P b) {
	return {a.x - b.x, a.y - b.y};
}

bool operator==(P a, P b) {
	return a.x == b.x && a.y == b.y;
}

int dot(P v1, P v2) {
	return v1.x*v2.x + v1.y*v2.y;
}

int len2(P p) {
	return p.x*p.x + p.y*p.y;
}

int leftOrRight(P v1, P v2) // right: -, left: +
{
	P v90 = {-v1.y, v1.x};
	return dot(v90, v2);
}

#ifdef SIMPLE_CONVEX_HULL_2D_IMPL
std::vector<int> convexHull(const std::vector<P>& p)
{
	const int n = p.size();
	std::vector<int> poly;
	{ // find the point with lowest x
		int a = 0;
		int minX = p[0].x;
		for(int i = 1; i < n; i++) {
			if(p[i].x < minX) {
				a = i;
				minX = p[i].x;
			}
		}
		poly.push_back(a);
	}
	
    // keep finding the next point b such that, the segment ab doesn't have any points to the right
	do {
		int a = poly.back();
		for(int b = 0; b < n; b++)
		if(a != b) {
			const P ab = p[b] - p[a];
			int c;
			for(c = 0; c < n; c++) {
				const P ac = p[c] - p[a];
				int lr = leftOrRight(ab, ac);
				if(lr < 0)
                    break;
                if(lr == 0 && len2(ab) < len2(ac)) //< if several points are on the same line we take the furthest
                    break;
			}
			if(c == n) {
				poly.push_back(b);
				break;
			}
            else {
                b = c-1;
            }
		}
	} while(poly[0] != poly.back()); // until we close the polygon
    poly.pop_back();

	return poly;
}

#endif //SIMPLE_CONVEX_HULL_2D_IMPL