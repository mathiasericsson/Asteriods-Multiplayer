#define _USE_MATH_DEFINES
#include <math.h>

#include <SFML/Graphics.hpp>
#include "common.cpp"


class clientObject {

    public: 
    sf::CircleShape mesh;
    sf::Vector2f velocity;
    bool connected = false;

    clientObject() {

        mesh.setRadius(10.f);
        mesh.setPointCount(3);
        mesh.setFillColor(sf::Color::Yellow);
        mesh.setScale(1.f, 1.5f);
        mesh.setOrigin(10.f, 10.f);
        mesh.setPosition(500,500);

    }
    void setVelocity(float32 x, float32 y)
    {
        velocity.x = x;
        velocity.y = y;
    }
    void setPosition()
    {
        mesh.setPosition(velocity);
    }
    void setPosition(sf::Vector2f position)
    {
        mesh.setPosition(position);
    }

    void setRotation(uint8 rotation)
    {   
        mesh.rotate(rotation);
    }
    void increaseVelocity()
    {
        float32 test = mesh.getRotation();

        //Increase velocity towards current rotation
        velocity.x += (sin((mesh.getRotation() / 180 * M_PI))) / 10;
        velocity.y += (-cos((mesh.getRotation() / 180 * M_PI))) / 10;
 
    }
    void rotateLeft()
    {
        mesh.setRotation(mesh.getRotation() - c_turn_speed);
    }
    void rotateRight()
    {
        mesh.setRotation(mesh.getRotation() + c_turn_speed);
    }

    void updatePosition()
    {
        sf::Vector2f new_position = mesh.getPosition();

        new_position.x += velocity.x;
        new_position.y += velocity.y;

        if (new_position.x < 0.0f) new_position.x = new_position.x + 1000;
        if (new_position.x >= 1000) new_position.x = new_position.x - 1000;

        if (new_position.y < 1000) new_position.y = new_position.y + 1000;
        if (new_position.y >= 1000) new_position.y = new_position.y - 1000;

        setPosition(new_position);

    }
 
};