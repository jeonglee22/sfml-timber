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

    sf::Sprite spriteCloud[3];
    for (int i = 0; i < sizeof(spriteCloud) / sizeof(sf::Sprite); i++)
    {
        spriteCloud[i].setTexture(textureCloud);
        spriteCloud[i].setPosition(0, textureCloud.getSize().y * (i/2.0f));
    }

    sf::Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, window.getSize().y * 2.0f / 4.0f);

    // sf::Vector2f velo = { 200.0f, 0.f };
    // 
    // sf::Vector2f direction = {1.f,0.f};
    // float speed = 200.f;
    // sf::Vector2f velo = direction * speed;
    //
    // 두 가지 표현은 같은 표시 백터 : 좌표 <-> 방향과 크기

    int spawnPadding = 200.f;
    int deletePadding = 300.f;

    sf::Vector2f direction[sizeof(spriteCloud) / sizeof(sf::Sprite) + 1] = {  };
    float speed[sizeof(spriteCloud) / sizeof(sf::Sprite) + 1] = {  };
    for (int i = 0; i < sizeof(spriteCloud) / sizeof(sf::Sprite) + 1; i++)
    {
        float random = (float) rand() / RAND_MAX;
        int dir;
        if (random < 0.5f)
        {
            dir = 1;
        }
        else
        {
            dir = -1;
        }
        direction[i] = { 1.0f * dir ,0.0f };
        if (i == 0)
        {
            spriteBee.setScale(-1.f * dir, 1.f);
            spriteBee.setPosition(rand() % (window.getSize().x / 2) + (window.getSize().x / 4), spriteBee.getPosition().y);
        }
        else
        {
            spriteCloud[i - 1].setScale(-1.f * dir * (random + 0.7), 1.f * (random + 0.7));
            spriteCloud[i - 1].setPosition(rand() % (window.getSize().x / 2) + (window.getSize().x / 4), spriteCloud[i - 1].getPosition().y);
        }

        direction[i] = { dir * 1.f, 0.f };
        speed[i] = rand() % 300 + 150.f;
    }

    sf::Clock clock;
    int rightTime = 0;
    int angle = 0;

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

        direction[0].y = ((90 - angle % 180) / 90.f) * 3.f;
        if (angle % 360 > 180)
        {
            direction[0].y *= -1.f;
        }
        angle++;
        
        posBee += direction[0] * speed[0] * deltaTime;
        spriteBee.setPosition(posBee);

        if (posBee.x < 0 - deletePadding || posBee.x > window.getSize().x + deletePadding)
        {
            float random = (float)rand() / RAND_MAX;
            int dir;
            if (random < 0.5f)
            {
                dir = 1;
            }
            else
            {
                dir = -1;
            }
            spriteBee.setScale(-1.f * dir, 1.f);
            spriteBee.setPosition((1.f - dir) * (window.getSize().x / 2.f) + (dir * -1 * spawnPadding), window.getSize().y * 2.0f / 4.0f);

            direction[0] = { 1.0f * dir ,0.0f };
            speed[0] = rand() % 300 + 150.f;
        }

        for (int i = 0; i < sizeof(spriteCloud) / sizeof(sf::Sprite); i++)
        {
            sf::Vector2f posCloud = spriteCloud[i].getPosition();
            posCloud += direction[i+1] * speed[i+1] * deltaTime;
            spriteCloud[i].setPosition(posCloud);

            if (posCloud.x < 0 - deletePadding - 150 || posCloud.x > window.getSize().x + deletePadding + 150)
            {
                float random = (float)rand() / RAND_MAX;
                int dir;
                if (random < 0.5f)
                {
                    dir = 1;
                }
                else
                {
                    dir = -1;
                }
                spriteCloud[i].setScale(-1.f * dir * (random+0.7), 1.f * (random + 0.7));
                spriteCloud[i].setPosition((1.f - dir) * (window.getSize().x / 2.f) + (dir * -1 * (spawnPadding + 150)), spriteCloud[i].getPosition().y);

                direction[i+1] = { 1.0f * dir ,0.0f };
                speed[i+1] = rand() % 300 + 150.f;
            }
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