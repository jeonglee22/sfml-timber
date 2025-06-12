#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

enum class Side { LEFT, RIGHT, NONE};

void spawnPos(sf::Sprite*, int, sf::Vector2f*, float*, int);
void resetPos(sf::Sprite*, sf::Vector2f*, float*, int*, int, int, int);

int main()
{
    srand((int)time(0));

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!");

    char files[6][100] = { "graphics/background.png", "graphics/tree.png", "graphics/bee.png", "graphics/cloud.png" , "graphics/player.png", "graphics/branch.png"};

    sf::Texture backgroudTexture, treeTexture, beeTexture, cloudTexture, playerTexture, branchTexture;
    backgroudTexture.loadFromFile(files[0]);
    treeTexture.loadFromFile(files[1]);
    beeTexture.loadFromFile(files[2]);
    cloudTexture.loadFromFile(files[3]);
    playerTexture.loadFromFile(files[4]);
    branchTexture.loadFromFile(files[5]);

    sf::Sprite spriteBackground;
    spriteBackground.setTexture(backgroudTexture);

    sf::Sprite spriteTree;
    spriteTree.setTexture(treeTexture);
    spriteTree.setOrigin(treeTexture.getSize().x / 2.0f, 0);
    spriteTree.setPosition(window.getSize().x / 2.0f, 0);

    sf::Sprite spritePlayer;
    Side sidePlayer = Side::RIGHT;
    float playerYPos = treeTexture.getSize().y + 50.f;

    spritePlayer.setTexture(playerTexture);
    spritePlayer.setOrigin(playerTexture.getSize().x / 2.f, playerTexture.getSize().y);
    spritePlayer.setPosition(window.getSize().x / 2.f, playerYPos);

    const int NUM_BRANCHES = 6;
    sf::Sprite spriteBranch[NUM_BRANCHES];
    Side sideBranch[NUM_BRANCHES] = { Side::LEFT , Side::RIGHT, Side::NONE, Side::LEFT , Side::RIGHT, Side::NONE };

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        spriteBranch[i].setTexture(branchTexture);
        spriteBranch[i].setOrigin(-1.f * treeTexture.getSize().x / 2.f, 0.f);
        spriteBranch[i].setPosition(window.getSize().x / 2.f, 150.f * i);
    }

    int beeCount = 1;

    sf::Sprite spriteBackgroundObjects[4];
    for (int i = 0; i < 4; i++)
    {
        if (i < beeCount)
        {
            spriteBackgroundObjects[i].setTexture(beeTexture);
        }
        else
        {
            spriteBackgroundObjects[i].setTexture(cloudTexture);
        }
    }

    sf::Vector2f direction[sizeof(spriteBackgroundObjects) / sizeof(sf::Sprite)] = {  };
    float speed[sizeof(spriteBackgroundObjects) / sizeof(sf::Sprite)] = {  };

    spawnPos(spriteBackgroundObjects, sizeof(spriteBackgroundObjects) / sizeof(sf::Sprite), direction, speed, beeCount);

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

        for (int i = 0; i < sizeof(spriteBackgroundObjects) / sizeof(sf::Sprite); i++)
        {
            sf::Vector2f pos = spriteBackgroundObjects[i].getPosition();
            pos += direction[i] * speed[i] * deltaTime;
            spriteBackgroundObjects[i].setPosition(pos);

            if (pos.x < 0 - deletePadding || pos.x > 1920 + deletePadding)
            {
                resetPos(spriteBackgroundObjects, direction, speed, &angle, i, spawnPadding, beeCount);
            }
        }

        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            switch (sideBranch[i])
            {
            case Side::LEFT:
                spriteBranch[i].setScale(-1.f, 1.f);
                break;
            case Side::RIGHT:
                spriteBranch[i].setScale(1.f, 1.f);
                break;
            default:
                break;
            }
        }

        switch (sidePlayer)
        {
        case Side::LEFT:
            spritePlayer.setPosition(window.getSize().x / 2.f - 300.f, playerYPos);
            spritePlayer.setScale(-1.f, 1.f);
            break;
        case Side::RIGHT:
            spritePlayer.setPosition(window.getSize().x / 2.f + 300.f, playerYPos);
            spritePlayer.setScale(1.f, 1.f);
            break;
        default:
            break;
        }

        // draw area
        window.clear();

        window.draw(spriteBackground);
        for (int i = beeCount; i < sizeof(spriteBackgroundObjects) / sizeof(sf::Sprite); i++)
        {
            window.draw(spriteBackgroundObjects[i]);
        }
        window.draw(spriteTree);
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            if (sideBranch[i] != Side::NONE)
            {
                window.draw(spriteBranch[i]);
            }
        }
        window.draw(spriteBackgroundObjects[0]);
        window.draw(spritePlayer);
        window.display();
    }

    return 0;
}

void spawnPos(sf::Sprite* sprites, int size, sf::Vector2f* direction, float* speed, int beeCount)
{
    for (int i = 0; i < size; i++)
    {
        float random = (float)rand() / RAND_MAX;
        int dir = -1;
        if (random < 0.5f)
        {
            dir = 1;
        }
        
        if (i < beeCount)
        {
            sprites[i].setPosition(rand() % (1920 / 2) + (1920 / 4), (rand() % 100) + 1080 * 2.0f / 4.0f);
            sprites[i].setScale(-1.f * dir, 1.f);
        }
        else
        {
            sprites[i].setPosition(rand() % (1920 / 2) + (1920 / 4), (rand() % 100) + 1080 * ((i-beeCount) / 8.0f));
            sprites[i].setScale(-1.f * dir * (random + 0.7f), 1.f * (random + 0.7f));
        }
        speed[i] = rand() % 300 + 150.f;
        direction[i] = { 1.0f * dir ,0.0f };
    }
}

void resetPos(sf::Sprite* sprites, sf::Vector2f* direction, float* speed, int* angle, int num, int spawnPadding, int beeCount)
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

    if (num < beeCount)
    {
        sprites[num].setPosition((1.f - dir) * (1920 / 2.f) + (dir * -1 * (spawnPadding)), (rand() % 100) + 1080 * 2.0f / 4.0f);
        sprites[num].setScale(-1.f * dir, 1.f);
    }
    else
    {
        sprites[num].setPosition((1.f - dir) * (1920 / 2.f) + (dir * -1 * (spawnPadding)), (rand() % 100) + 1080 * ((num - beeCount) / 8.0f));
        sprites[num].setScale(-1.f * dir * (random + 0.7f), 1.f * (random + 0.7f));
    }
            
    direction[num] = { 1.0f * dir ,0.0f };
    speed[num] = rand() % 300 + 150.f;
}