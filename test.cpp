// TEST WITH SFML
// ----------------------------------------------------------------------
#include <SFML/Graphics.hpp>
#include "ch.hpp"

int main()
{
    using namespace std;
	sf::RenderWindow window(sf::VideoMode(800, 800), "point cloud to convex poly");

	vector<P> p;
	vector<int> poly;
    const int N = 23;
	const int RADIUS = 6;
	const int SEPAR = 35;
	const sf::Color WHITE(255, 255, 255, 255);
	const sf::Color GRAY(50, 50, 50, 255);
	const sf::Color RED(255, 0, 0, 255);
	vector<vector<bool>> select(N, vector<bool>(N, false));
	auto refreshPoly = [&] {
		if(p.size() >= 3)
			poly = convexHull(p);
		else 
			poly.clear();
	};

	bool prevMousePressed = false;

	while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        const bool cPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
        if(cPressed) {
            for(int y = 0; y < N; y++)
            for(int x = 0; x < N; x++)
                select[y][x] = false;
            poly.clear();
        }

        const bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        if(!prevMousePressed && mousePressed) {
            auto mp = sf::Mouse::getPosition(window);
            int x = mp.x / SEPAR;
            int y = mp.y / SEPAR;
            if( mp.x - SEPAR * x < 2*RADIUS &&
                mp.y - SEPAR * y < 2*RADIUS &&
                x >= 0 && x < N && y >= 0 && y < N)
            {
                select[y][x] = !select[y][x];
                p.clear();
                for(int y = 0; y < N; y++)
                for(int x = 0; x < N; x++) {
                    if(select[y][x])
                        p.push_back({x, y});
                }
                refreshPoly();
            }
        }
        prevMousePressed = mousePressed;

        window.clear();
        
		// DRAW
		if(poly.size()) {
			sf::VertexArray verts(sf::LineStrip, poly.size()+1);
			for(int i = 0; i < poly.size(); i++) {
				const auto pp = p[poly[i]];
				verts[i].position = {(float)pp.x*SEPAR + RADIUS, (float)pp.y*SEPAR + RADIUS};
				verts[i].color = WHITE;
			} {
				const auto pp = p[poly[0]];
				verts[poly.size()].position = {(float)pp.x*SEPAR + RADIUS, (float)pp.y*SEPAR + RADIUS};
				verts[poly.size()].color = WHITE;
			}
			window.draw(verts);
		}
		for(int y = 0; y < N; y++)
		for(int x = 0; x < N; x++)
		{
			sf::CircleShape circ;
			const bool isInPoly = [&]() {
				for(int i : poly)
				if(p[i] == P{x, y})
					return true;
				return false;
			}();
			circ.setFillColor(isInPoly ? RED : select[y][x] ? WHITE : GRAY);
			circ.setRadius(RADIUS);
			circ.setPosition({(float)x*SEPAR, (float)y*SEPAR});
			window.draw(circ);
		}

        window.display();
        sf::sleep(sf::seconds(0.02));
    }
}