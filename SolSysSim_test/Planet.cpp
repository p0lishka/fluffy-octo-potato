#include "Planet.h"
#include "Constants.h"
#include <cmath>

using namespace sf;

Planet::Planet(float semiMajorAxisAU, float eccentricity, float sizeKm,
    float orbitPeriodDays, float rotationPeriodDays, sf::Color color)
    : semiMajorAxis(semiMajorAxisAU), eccentricity(eccentricity) {

  
    float earthSize = 12742.0f;
    planetRadius = (sizeKm / earthSize) * 20.0f * Constants::BASE_PLANET_SIZE_SCALE;

     
    orbitSpeed = 360.0f / orbitPeriodDays * 0.01f;

    
    rotationSpeed = 360.0f / rotationPeriodDays * 0.1f;

    angle = 0.0f;
    rotationAngle = 0.0f;

     
    shape.setRadius(planetRadius);
    shape.setFillColor(color);
    shape.setOrigin(planetRadius, planetRadius);
}

void Planet::update() {
    
    angle += orbitSpeed;
    if (angle >= 360.0f) angle -= 360.0f;

    
    rotationAngle += rotationSpeed;
    if (rotationAngle >= 360.0f) rotationAngle -= 360.0f;
    shape.setRotation(rotationAngle);
}

Vector2f Planet::getPosition(sf::Vector2f sunPosition) {
    float trueAnomaly = angle * 3.14159f / 180.0f;
     
    extern float currentAUToPixels;  
    float a = semiMajorAxis * currentAUToPixels;
    float b = a * sqrt(1 - eccentricity * eccentricity);
    float c = eccentricity * a;

    float x = a * cos(trueAnomaly);
    float y = b * sin(trueAnomaly);

    return sf::Vector2f(sunPosition.x + x - c, sunPosition.y + y);
}