#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
int main()
{
    sf::Texture texture;
    if (!texture.loadFromFile("../graphics/connect-4.png"))
    {
        // Error
        cout << "Error Loading Graphics.." << endl;
    }
    sf::Sprite background(texture);

    sf::RenderWindow window(sf::VideoMode(800, 800), "Connect Four");
    while (window.isOpen())
    {
        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            if (evnt.type == sf::Event::Closed)
            {
                window.close();
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                cout << "Mouse: " << mousePosition.x << " " << mousePosition.y << endl;

            }
        }
            // if (sf::Mouse::isButtonReleased(sf::Mouse::Left))
            // {
            //     sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            //     cout << "Mouse: " <<mousePosition.x << mousePosition.y << endl;
            // }
        // window.clear(sf::Color::Black);

        window.draw(background);
        window.display();
    }

    return 0;
}