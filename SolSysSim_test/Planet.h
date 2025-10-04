            
#ifndef PLANET_H
#define PLANET_H

#include <SFML/Graphics.hpp>

    class Planet {
    public:
        sf::CircleShape shape;
        float semiMajorAxis;     
        float eccentricity;      
        float planetRadius;      
        float orbitSpeed;
        float angle;
        float rotationSpeed;
        float rotationAngle;

        
        Planet(float semiMajorAxisAU, float eccentricity, float sizeKm,
            float orbitPeriodDays, float rotationPeriodDays, sf::Color color);

        void update();
        sf::Vector2f getPosition(sf::Vector2f sunPosition);

       
        float getSemiMajorAxis() const { return semiMajorAxis; }
        float getSemiMinorAxis() const { return semiMajorAxis * sqrt(1 - eccentricity * eccentricity); }
        float getEccentricity() const { return eccentricity; }

       
        float getBaseRadius() const { return planetRadius; }
    };

#endif