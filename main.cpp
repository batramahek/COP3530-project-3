#include <SFML/Graphics.hpp>

int main (){
    sf::RenderWindow window(sf::VideoMode(800, 600), "Project 3");
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEven(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        /* This will show the actual graph in the window, set up later
        window.draw(example);
        */
        window.dislplay();
    }
    return 0;
}