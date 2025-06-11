#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

int main()
{
    srand((int)time(0));

    //printf("%d\n\n", RAND_MAX);

    /*for (int i = 0; i < 10; i++)
    {
        printf("%d\n", rand());
    }*/

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

    // sf::Vector2f velo = { 200.0f, 0.f };
    // 
    // sf::Vector2f direction = {1.f,0.f};
    // float speed = 200.f;
    // sf::Vector2f velo = direction * speed;
    //
    // 두 가지 표현은 같은 표시 백터 : 좌표 <-> 방향과 크기

    sf::Vector2f velo[sizeof(spriteCloud) / sizeof(sf::Sprite) + 1] = {  };
    for (int i = 0; i < sizeof(spriteCloud) / sizeof(sf::Sprite) + 1; i++)
    {
        velo[i] = { ((rand() % 20) / 2.0f + 1) * 150.0f, 0.f };
    }

    sf::Clock clock;

    while (window.isOpen())
    {   
        sf::Time time = clock.restart();
        float deltaTime = time.asSeconds();

        // event loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // update area
        sf::Vector2f posBee = spriteBee.getPosition();
        if (posBee.x > window.getSize().x)
        {
            posBee.x = -1.0f * textureBee.getSize().x;
        }
        posBee += velo[0] * deltaTime;
        spriteBee.setPosition(posBee);

        for (int i = 0; i < sizeof(spriteCloud) / sizeof(sf::Sprite); i++)
        {
            sf::Vector2f posCloud = spriteCloud[i].getPosition();
            if (posCloud.x > window.getSize().x)
            {
                posCloud.x = -1.0f *textureCloud.getSize().x;
            }
            posCloud += velo[i+1] * deltaTime;
            spriteCloud[i].setPosition(posCloud);
        }

        // draw area
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