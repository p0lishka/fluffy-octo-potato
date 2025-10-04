#include <SFML/Graphics.hpp>
#include <cmath>
#include "Planet.h"
#include "Constants.h"

using namespace std;
using namespace sf;

float currentZoom = 1.0f;
float currentAUToPixels = Constants::BASE_AU_TO_PIXELS;
float currentPlanetSizeScale = Constants::BASE_PLANET_SIZE_SCALE;

Vector2f cameraCenter(Constants::CENTER_X, Constants::CENTER_Y);
bool isDragging = false;
Vector2f lastMousePos;
float cameraMoveSpeed = 5.0f;

 void updateZoom(float zoomLevel) {
	currentZoom = zoomLevel;
	currentAUToPixels = Constants::BASE_AU_TO_PIXELS * currentZoom;
	currentPlanetSizeScale = Constants::BASE_PLANET_SIZE_SCALE * currentZoom;
}

void drawEllipticalOrbit(RenderWindow & window, const Planet & planet, Vector2f center) {
	float a = planet.getSemiMajorAxis() * Constants::BASE_AU_TO_PIXELS * currentZoom;
	float b = planet.getSemiMinorAxis() * Constants::BASE_AU_TO_PIXELS * currentZoom;
	float c = planet.getEccentricity() * a;


	 
	VertexArray orbit(LineStrip, 361);

	for (int i = 0; i <= 360; ++i) {
		float angle = i * 3.14159f / 180.0f;
		float x = center.x + a * cos(angle) - c;   
		float y = center.y + b * sin(angle);
		orbit[i].position = Vector2f(x, y);
		orbit[i].color = Color(100, 100, 100, 100);
	}

	window.draw(orbit);
}

int main() {

	RenderWindow window(
		VideoMode(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT),
		"Solar System with Elliptical Orbits"
	);
	window.setFramerateLimit(60);

	CircleShape sun(20.f * currentZoom);
	sun.setFillColor(Color::Yellow);
	sun.setPosition(Constants::CENTER_X, Constants::CENTER_Y);
	sun.setOrigin(20 * currentZoom, 20 * currentZoom);

	CircleShape spot(1.0f);
	spot.setFillColor(Color::Red);
	spot.setOrigin(1, 1);
	float sunRotation = 0.0f;
	float sunRotationSpeed = 0.5f;
	float spotDistance = 15.0f;

	Planet mercury(0.387f, 0.2056f, 4879.0f, 88.0f, 58.6f, Color(169, 169, 169));
	Planet venus(0.723f, 0.0068f, 12104.0f, 225.0f, 243.0f, Color(245, 75, 66));
	Planet earth(1.0f, 0.0167f, 12742.0f, 365.5f, 24.0f, Color(0, 71, 171));
	Planet mars(1.524f, 0.0934f, 6792.0f, 686.98f, 24.6f, Color(201, 58, 32));
	Planet jupiter(5.204f, 0.048775f, 133708.0f, 4332.589f, 9.9f, Color(212, 148, 75));
	Planet saturn(9.54f, 0.0565f, 120000.0f, 10756.0f, 10.7f , Color(201, 179, 117));
	Planet uranus(19.191f, 0.04717f, 50724.0f, 30687.0f, 0.72f, Color(150, 230, 230));
	Planet neptune(30.069f, 0.00859f, 49244.0f, 60190.0f, 0.67f, Color(80, 80, 255));

	vector<Planet*> planets = { &mercury, &venus, &earth, &mars, &jupiter, &saturn, &uranus, &neptune};

	while (window.isOpen())
	{

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Middle) {  // Колесико мыши
					isDragging = true;
					lastMousePos = Vector2f(event.mouseButton.x, event.mouseButton.y);
				}
			}

			if (event.type == Event::MouseButtonReleased) {
				if (event.mouseButton.button == Mouse::Middle) {
					isDragging = false;
				}
			}

			if (event.type == Event::MouseMoved) {
				if (isDragging) {
					Vector2f currentMousePos(event.mouseMove.x, event.mouseMove.y);
					Vector2f delta = lastMousePos - currentMousePos;
					cameraCenter += delta * (1.0f / currentZoom); // Масштабируем скорость перемещения
					lastMousePos = currentMousePos;
				}
			}

			if (event.type == Event::MouseWheelScrolled) {
				if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
					float zoomChange = event.mouseWheelScroll.delta * 0.1f;
					float newZoom = currentZoom + zoomChange;


					if (newZoom < Constants::MIN_ZOOM) newZoom = Constants::MIN_ZOOM;
					if (newZoom > Constants::MAX_ZOOM) newZoom = Constants::MAX_ZOOM;

					updateZoom(newZoom);


					sun.setRadius(20.f * currentZoom);
					sun.setOrigin(20 * currentZoom, 20 * currentZoom);

					spot.setRadius(1.f * currentZoom);
					spot.setOrigin(1 * currentZoom, 1 * currentZoom);
					spotDistance = 15.0f * currentZoom;
				}
			}

		}

		sunRotation += sunRotationSpeed;
		sun.setRotation(sunRotation);

		float radianAngle = sunRotation * 3.1415f / 180.0f;
		float spotX = Constants::CENTER_X + spotDistance * cos(radianAngle);
		float spotY = Constants::CENTER_Y + spotDistance * sin(radianAngle);

		spot.setPosition(spotX, spotY);
		spot.setRotation(sunRotation);

		for (auto& planet : planets) {
			planet->update();
			planet->shape.setPosition(planet->getPosition(
				Vector2f(Constants::CENTER_X, Constants::CENTER_Y)
			));

			float baseRadius = planet->planetRadius / Constants::BASE_PLANET_SIZE_SCALE;
			planet->shape.setRadius(baseRadius * currentPlanetSizeScale);
			planet->shape.setOrigin(baseRadius * currentPlanetSizeScale,
				baseRadius * currentPlanetSizeScale);
		};

		View view;
		view.setCenter(cameraCenter);
		view.setSize(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
		window.setView(view);

		window.clear(Color::Black);


		for (auto& planet : planets) {
			drawEllipticalOrbit(window, *planet, Vector2f(Constants::CENTER_X, Constants::CENTER_Y));
		}


		for (auto& planet : planets) {
			window.draw(planet->shape);
		}


		window.draw(sun);
		window.draw(spot);

		window.display();
	}

	return 0;

}
