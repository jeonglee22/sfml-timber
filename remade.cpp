#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

enum Side { LEFT, RIGHT, NONE};

void sidePlayerandBranch(sf::Sprite* playerSprite, sf::Texture treeTexture, Side* branchSides, Side* playerside, int NUM_BRANCHES, float dir);
void branchMove(Side* branches, int size);
void setBranchDir(sf::Sprite* branchSprites, int size, Side* branchSide);
void setbackgroundObjectPositionScale(sf::Sprite* sprite, int num, float dir, int beeCount ,float random);
float setDir(float* dir);
void drawSprites(sf::Sprite* sprites, int size, sf::RenderWindow* window, int offset);

int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "log cutting game!");
    float halfWindowWidth = window.getSize().x / 2.f;
    
    sf::Texture backgroundTexture, treeTexture, cloudTexture, beeTexture, branchTexture, playerTexture;
    backgroundTexture.loadFromFile("graphics/background.png");
    treeTexture.loadFromFile("graphics/tree.png");
    cloudTexture.loadFromFile("graphics/cloud.png");
    beeTexture.loadFromFile("graphics/bee.png");
    branchTexture.loadFromFile("graphics/branch.png");
    playerTexture.loadFromFile("graphics/player.png");

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    sf::Sprite treeSprite;
    treeSprite.setTexture(treeTexture);
    treeSprite.setOrigin(treeTexture.getSize().x / 2.f, 0.f);
    treeSprite.setPosition(halfWindowWidth, 0);

    const int beeCount = 2;
    const int cloudCount = 3;

    sf::Sprite backgroundObjectSprites[beeCount + cloudCount];
    float backgroundObjectSpritesSpeed[beeCount + cloudCount];
    sf::Vector2f backgroundObjectSpritesDirection[beeCount + cloudCount];
    for (int i = 0; i < beeCount + cloudCount; i++)
    {
        float dir;
        float random = setDir(&dir);

        backgroundObjectSpritesSpeed[i] = 100.f + (rand() % 100) * 2.f;
        backgroundObjectSpritesDirection[i] = {1.f * dir,0.f};

        if (i < beeCount)
        {
            backgroundObjectSprites[i].setTexture(beeTexture);
            backgroundObjectSprites[i].setPosition(500.f + rand() % 1000, 700.f + 50.f * (i - beeCount));
            backgroundObjectSprites[i].setScale(-1.f * dir,1.f);
        }
        else
        {
            backgroundObjectSprites[i].setTexture(cloudTexture);
            backgroundObjectSprites[i].setPosition(500.f + rand() % 1000, 150.f *(i-beeCount));
            backgroundObjectSprites[i].setScale(-1.f * dir * (random + 0.5f), 1.f * (random + 0.5f));
        }
    }

    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setOrigin(playerTexture.getSize().x / 2.f, playerTexture.getSize().y);
    playerSprite.setPosition(halfWindowWidth + 300.f, treeTexture.getSize().y + 50.f);
    Side playerSide = Side::RIGHT;

    const int NUM_BRANCHES = 6;
    sf::Sprite branchSprites[NUM_BRANCHES];
    Side branchSides[NUM_BRANCHES];
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        int random = rand() % 3;
        switch (random)
        {
        case 0:
            branchSides[i] = Side::LEFT;
            branchSprites[i].setScale(-1.f,1.f);
            break;
        case 1:
            branchSides[i] = Side::RIGHT;
            branchSprites[i].setScale(1.f, 1.f);
            break;
        case 2:
            branchSides[i] = Side::NONE;
            break;
        default:
            break;
        }
        branchSprites[i].setTexture(branchTexture);
        branchSprites[i].setOrigin(-1.f * treeTexture.getSize().x / 2.f, 0.f);
        branchSprites[i].setPosition(halfWindowWidth, 150.f * i);

    }
    branchSides[5] = Side::NONE;

    sf::RectangleShape timeBar;
    timeBar.setFillColor(sf::Color::Red);
    float timeBarWidth = 500.f;
    timeBar.setSize({ timeBarWidth, 50.f });
    timeBar.setPosition(halfWindowWidth - timeBarWidth / 2.f, treeTexture.getSize().y + 100.f);

    sf::Clock clock;

    bool isLeft = false;
    bool isRight = false;
    bool leftDown = false;
    bool rightDown = false;
    bool gameStop = false;

    float remainTime = 6.f;
    float timeVelocity = timeBarWidth / remainTime;

    while (window.isOpen())
    {   
        sf::Time time = clock.restart();
        float deltatime = time.asSeconds();

        remainTime -= deltatime;
        if (remainTime <= 0.f)
        {
            gameStop = true;
        }

        leftDown = false;
        rightDown = false;

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Left:
                    if (!isLeft)
                    {
                        leftDown = true;
                    }
                    isLeft = true;
                    break;
                case sf::Keyboard::Right:
                    if (!isRight)
                    {
                        rightDown = true;
                    }
                    isRight = true;
                    break;
                case sf::Keyboard::Enter:
                    gameStop = !gameStop;
                    branchSides[NUM_BRANCHES - 1] = Side::NONE;
                    remainTime = 6.f;
                    break;
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                default:
                    break;
                }
                break;
            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                case sf::Keyboard::Left:
                    isLeft = false;
                    break;
                case sf::Keyboard::Right:
                    isRight = false;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        if(!gameStop)
        {
            timeBar.setSize({remainTime * timeVelocity, 50.f});

            for (int i = 0; i < cloudCount + beeCount; i++)
            {
                float dir;
                float random = setDir(&dir);

                sf::Vector2f position = backgroundObjectSprites[i].getPosition();
                position += backgroundObjectSpritesDirection[i] * backgroundObjectSpritesSpeed[i] * deltatime;
                backgroundObjectSprites[i].setPosition(position);

                if (position.x <= -300.f || position.x >= 1920.f + 300.f)
                {
                    setbackgroundObjectPositionScale(&(backgroundObjectSprites[i]), i, dir, beeCount, random);

                    backgroundObjectSpritesDirection[i] = { dir, 0.f };
                    backgroundObjectSpritesSpeed[i] = 100.f + (rand() % 100) * 2.f;
                }
            }

            if (leftDown || rightDown)
            {
                if (isLeft)
                {
                    sidePlayerandBranch(&playerSprite, treeTexture, branchSides, &playerSide, NUM_BRANCHES, -1.f);
                }
                else
                {
                    sidePlayerandBranch(&playerSprite, treeTexture, branchSides, &playerSide, NUM_BRANCHES, 1.f);
                }
                setBranchDir(branchSprites, NUM_BRANCHES, branchSides);

                if (playerSide == branchSides[NUM_BRANCHES - 1])
                {
                    gameStop = true;
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        drawSprites(backgroundObjectSprites, cloudCount, &window, beeCount);
        window.draw(treeSprite);
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            if(branchSides[i] != Side::NONE)
            {
                window.draw(branchSprites[i]);
            }
        }
        drawSprites(backgroundObjectSprites, beeCount, &window, 0);
        window.draw(playerSprite);
        window.draw(timeBar);
        window.display();
    }

    return 0;
}

void sidePlayerandBranch(sf::Sprite* playerSprite, sf::Texture treeTexture, Side* branchSides, Side* playerside, int NUM_BRANCHES, float dir)
{
    if (dir == -1.f)
    {
        *playerside = Side::LEFT;
    }
    else
    {
        *playerside = Side::RIGHT;
    }
    (*playerSprite).setScale(dir, 1.f);
    (*playerSprite).setPosition(1920.f / 2.f + 300.f * dir, treeTexture.getSize().y + 50.f);
    branchMove(branchSides, NUM_BRANCHES);
}

void branchMove(Side* branches, int size)
{
    for (int i = size - 1; i >= 1; i--)
    {
        branches[i] = branches[i - 1];
    }

    int random = rand() % 3;
    switch (random)
    {
    case 0:
        branches[0] = Side::LEFT;
        break;
    case 1:
        branches[0] = Side::RIGHT;
        break;
    case 2:
        branches[0] = Side::NONE;
        break;
    default:
        break;
    }
}

void setBranchDir(sf::Sprite* branchSprites, int size, Side* branchSide)
{
    for (int i = 0; i < size; i++)
    {
        switch (branchSide[i])
        {
        case Side::LEFT:
            branchSprites[i].setScale(-1.f, 1.f);
            break;
        case Side::RIGHT:
            branchSprites[i].setScale(1.f, 1.f);
            break;
        default:
            break;
        }
    }
}

void setbackgroundObjectPositionScale(sf::Sprite* sprite, int num, float dir, int beeCount ,float random)
{
    if (num < beeCount)
    {
        (*sprite).setPosition(1920.f / 2.f - dir * (1920.f / 2.f) - dir * 200.f, 600.f + 100.f * (num) + rand() % 50);
        (*sprite).setScale(-1 * dir, 1.f);
    }
    else
    {
        (*sprite).setPosition(1920.f / 2.f - dir * (1920.f / 2.f) - dir * 200.f, 150.f * (num - beeCount) + rand() % 50);
        (*sprite).setScale(-1 * dir * (random + 0.5f), 1.f * (random + 0.5f));
    }
}

float setDir(float* dir)
{
    float Random = (float)rand() / RAND_MAX;
    *dir = 1.f;
    if (Random > 0.5f)
    {
        *dir = -1.f;
    }
    return Random;
}

void drawSprites(sf::Sprite* sprites , int size, sf::RenderWindow* window, int offset)
{
    for (int i = 0; i < size; i++)
    {
        (*window).draw(sprites[i + offset]);
    }
}