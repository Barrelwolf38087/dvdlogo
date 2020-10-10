#include <iostream>

#include <SFML/Graphics.hpp>

#include "logo.hpp"

// Config values
constexpr int winWidth = 1280;
constexpr int winHeight = 720;
constexpr float logoScale = 0.25;

class RigidObject {
    sf::Sprite *_sprite;

    // Used for collision detection. It should never change, so we just grab it once
    // at construction and re-use it.
    sf::Rect<float> _spriteBounds;

    // Window reference (needed for bounds check)
    sf::Window *_win;

    // The "idle" velocity that the logo moves at with no user interaction
    static constexpr float _minVelocity = 25; // Placeholder, adjust to something more reasonable
    // Constant deceleration (pixels/second/second)
    static constexpr float _decel = -2;

    // Pixels/second
    float _xVel = _minVelocity, _yVel = _minVelocity;

public:

    RigidObject(sf::Window *win, sf::Sprite *sprite) :
            _win(win),
            _sprite(sprite),
            _spriteBounds(_sprite->getGlobalBounds()) {}

    void setPosition(float x, float y) {
        _sprite->setPosition(x, y);
    }

    void setVelocity(float x, float y) {
        _xVel = x;
        _yVel = y;
    }

    // Do not call when dragging object
    void update(sf::Time elapsed) {
        bool xBounce = false, yBounce = false;
        float seconds = elapsed.asSeconds();
        sf::Vector2u winSize = _win->getSize();

        // Position update
        float xPot = static_cast<float>(_sprite->getGlobalBounds().left + (_xVel * seconds));
        float yPot = static_cast<float>(_sprite->getGlobalBounds().top + (_yVel * seconds));

        // Bounds checking
        if (xPot + _spriteBounds.width > winSize.x) {
            xPot = winSize.x - _spriteBounds.width;
            xBounce = true;
        } else if (xPot < 0) {
            xPot = 0;
            xBounce = true;
        }

        // Not part of the above chain in case we actually hit the corner perfectly
        if (yPot + _spriteBounds.height > winSize.y) {
            yPot = winSize.y - _spriteBounds.height;
            yBounce = true;
        } else if (yPot < 0) {
            yPot = 0;
            yBounce = true;
        }

        _sprite->setPosition(xPot, yPot);

        if (xBounce && yBounce) {
            std::cout << "Perfect corner hit!" << std::endl;
        }

        // Direction update
        if (xBounce)
            _xVel = -_xVel;

        if (yBounce)
            _yVel = -_yVel;

        // TODO: Velocity update


    }
};

class Application {

    // Graphics
    sf::RenderWindow _win;
    sf::Texture _logoTexture;
    sf::Sprite _logoSprite;

    // Physics
    RigidObject _logoObject;

    // Timing
    sf::Clock _clock;

    // Misc. State
    bool _quit = true;

    void update() {
        sf::Event e{};
        while (_win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                _quit = true;
            }
        }


    }

    void render() {
        _win.clear();
        _win.draw(_logoSprite);
        _win.display();
    }

public:
    Application() :
            _win(sf::VideoMode(winWidth, winHeight), "Hello SFML!"),
            _logoObject(&_win, &_logoSprite) {
        if (!_logoTexture.loadFromMemory(&dvd_png, dvd_png_len)) {
            _quit = true;
        }
        _logoTexture.setSmooth(true);

        _logoSprite.setTexture(_logoTexture);
        _logoSprite.setScale(logoScale, logoScale);
    }

    int run() {
        _clock.restart();

        while (_win.isOpen()) {
            update();
            render();

            if (_quit) {
                _win.close();
            }
        }

        return 0;
    }
};

int main() {
    Application dvdlogo{};

    return dvdlogo.run();
}
