#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!");

    sf::Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");
    sf::Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    sf::Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    sf::Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");

    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);

    sf::Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setOrigin(textureTree.getSize().x / 2.0f, 0);
    spriteTree.setPosition(window.getSize().x / 2.0f, 0);
    /*spriteTree.setPosition(window.getSize().x / 2.0f - textureTree.getSize().x / 2.0f, 0);*/

    /*sf::Sprite spriteCloud1;
    spriteCloud1.setTexture(textureCloud);
    sf::Sprite spriteCloud2;
    spriteCloud2.setTexture(textureCloud);
    spriteCloud2.setPosition(0, textureCloud.getSize().y);
    sf::Sprite spriteCloud3;
    spriteCloud3.setTexture(textureCloud);
    spriteCloud3.setPosition(0, textureCloud.getSize().y * 2.0f);*/

    sf::Sprite spriteCloud[3];
    for (int i = 0; i < sizeof(spriteCloud) / sizeof(sf::Sprite); i++)
    {
        spriteCloud[i].setTexture(textureCloud);
        spriteCloud[i].setPosition(0, textureCloud.getSize().y * i);
    }

    sf::Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, window.getSize().y * 3.0f / 4.0f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear();

        window.draw(spriteBackground);
        for (int i = 0; i < sizeof(spriteCloud) / sizeof(sf::Sprite); i++)
        {
            window.draw(spriteCloud[i]);
        }
        window.draw(spriteTree);
        window.draw(spriteBee);

        window.display();
    }

    return 0;
}