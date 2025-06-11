#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

void spawnPos(sf::Sprite*, int, sf::Vector2f*, float*);
void spriteTextureSetting(sf::Sprite*, int, sf::Texture*);
void resetPos(sf::Sprite*, sf::Vector2f*, float*, int*, int, int);

int main()
{
    srand((int)time(0));

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!");

    char files[4][100] = { "graphics/background.png", "graphics/tree.png", "graphics/bee.png", "graphics/cloud.png"};

    sf::Texture textures[4];
    for (int i = 0; i < 4; i++)
    {
        textures[i].loadFromFile(files[i]);
    }

    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textures[0]);

    sf::Sprite spriteTree;
    spriteTree.setTexture(textures[1]);
    spriteTree.setOrigin(textures[1].getSize().x / 2.0f, 0);
    spriteTree.setPosition(window.getSize().x / 2.0f, 0);

    sf::Sprite sprites[4];
    spriteTextureSetting(sprites, sizeof(sprites) / sizeof(sf::Sprite), textures);

    sf::Vector2f direction[sizeof(sprites) / sizeof(sf::Sprite)] = {  };
    float speed[sizeof(sprites) / sizeof(sf::Sprite)] = {  };

    spawnPos(sprites, sizeof(sprites) / sizeof(sf::Sprite), direction, speed);

    sf::Clock clock;
    int angle = 0;
    int spawnPadding = 100.f;
    int deletePadding = 350.f;

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
        direction[0].y = ((90 - angle % 180) / 90.f) * 3.f;
        if (angle % 360 > 180)
        {
            direction[0].y *= -1.f;
        }
        angle++;

        for (int i = 0; i < sizeof(sprites) / sizeof(sf::Sprite); i++)
        {
            sf::Vector2f pos = sprites[i].getPosition();
            pos += direction[i] * speed[i] * deltaTime;
            sprites[i].setPosition(pos);

            if (pos.x < 0 - deletePadding || pos.x > 1920 + deletePadding)
            {
                resetPos(sprites, direction, speed, &angle, i, spawnPadding);
            }
        }

        // draw area
        window.clear();

        window.draw(spriteBackground);
        for (int i = 1; i < sizeof(sprites) / sizeof(sf::Sprite); i++)
        {
            window.draw(sprites[i]);
        }
        window.draw(spriteTree);
        window.draw(sprites[0]);

        window.display();
    }

    return 0;
}

void spawnPos(sf::Sprite* sprites, int size, sf::Vector2f* direction, float* speed)
{
    for (int i = 0; i < size; i++)
    {
        float random = (float)rand() / RAND_MAX;
        int dir = -1;
        if (random < 0.5f)
        {
            dir = 1;
        }
        
        if (!i)
        {
            sprites[i].setPosition(rand() % (1920 / 2) + (1920 / 4), 1080 * 2.0f / 4.0f);
        }
        else
        {
            sprites[i].setPosition(rand() % (1920 / 2) + (1920 / 4), 1080 * ((i-1) / 8.0f));
        }
        sprites[i].setScale(-1.f * dir, 1.f);
        speed[i] = rand() % 300 + 150.f;
        direction[i] = { 1.0f * dir ,0.0f };
    }
}

void spriteTextureSetting(sf::Sprite* sprites, int size, sf::Texture* textures)
{
    for (int i = 0; i < size; i++)
    {
        if (!i)
        {
            sprites[i].setTexture(textures[2]);
        }
        else
        {
            sprites[i].setTexture(textures[3]);
        }
    }
}

void resetPos(sf::Sprite* sprites, sf::Vector2f* direction, float* speed, int* angle, int num, int spawnPadding)
{
    if (num == 0)
    {
        *angle = 0;
    }

    float random = (float)rand() / RAND_MAX;

    int dir = -1;
    if (random < 0.5f)
    {
        dir = 1;
    }

    sprites[num].setScale(-1.f * dir, 1.f);

    if (!num)
    {
        sprites[num].setPosition((1.f - dir) * (1920 / 2.f) + (dir * -1 * (spawnPadding)), 1080 * 2.0f / 4.0f);
    }
    else
    {
        sprites[num].setPosition((1.f - dir) * (1920 / 2.f) + (dir * -1 * (spawnPadding)), 1080 * ((num - 1) / 8.0f));
    }
            
    direction[num] = { 1.0f * dir ,0.0f };
    speed[num] = rand() % 300 + 150.f;
}