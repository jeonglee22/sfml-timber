#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>

enum Side { LEFT, RIGHT, NONE};

void sidePlayerandBranch(sf::Sprite* playerSprite, sf::Texture treeTexture, Side* branchSides, Side* playerside, int NUM_BRANCHES, float dir);
void branchMove(Side* branches, int size);
void setBranchDir(sf::Sprite* branchSprites, int size, Side* branchSide);
void setbackgroundObjectPositionScale(sf::Sprite* sprite, int num, float dir, int beeCount ,float random);
float setDir(float* dir);
void drawSprites(sf::Sprite* sprites, int size, sf::RenderWindow* window, int offset);
void setSpriteOPS(sf::Sprite* sprite, sf::Vector2f origin, sf::Vector2f position, sf::Vector2f scale);
void setTextSetting(sf::Text* text, sf::String string, sf::Color color, sf::Vector2f position, float fontSize);
void resetTextOrigin(sf::Text* text);

int main()
{
    srand((int)time(0));

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "log cutting game!");
    float halfWindowWidth = window.getSize().x / 2.f;
    float halfWindowHeight = window.getSize().y / 2.f;
    
    sf::Texture backgroundTexture, treeTexture, cloudTexture, beeTexture, branchTexture, playerTexture, logTexture, axeTexture;
    backgroundTexture.loadFromFile("graphics/background.png");
    treeTexture.loadFromFile("graphics/tree.png");
    cloudTexture.loadFromFile("graphics/cloud.png");
    beeTexture.loadFromFile("graphics/bee.png");
    branchTexture.loadFromFile("graphics/branch.png");
    playerTexture.loadFromFile("graphics/player.png");
    logTexture.loadFromFile("graphics/log.png");
    axeTexture.loadFromFile("graphics/axe.png");
    
    sf::Font gameFont;
    gameFont.loadFromFile("fonts/KOMIKAP_.ttf");

    sf::SoundBuffer chopSoundBuffer, deathSoundBuffer, outoftimeSoundBuffer;
    chopSoundBuffer.loadFromFile("sound/chop.wav");
    deathSoundBuffer.loadFromFile("sound/death.wav");
    outoftimeSoundBuffer.loadFromFile("sound/out_of_time.wav");

    sf::Vector2f baseScale = { 1.f,1.f };

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    sf::Sprite treeSprite;
    treeSprite.setTexture(treeTexture);
    setSpriteOPS(&treeSprite, { treeTexture.getSize().x / 2.f, 0.f }, { halfWindowWidth, 0 }, baseScale);

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
            setSpriteOPS(&backgroundObjectSprites[i], { 0.f,0.f }, { 500.f + rand() % 1000, 700.f + 50.f * (i - beeCount) }, {- 1.f * dir, 1.f});
        }
        else
        {
            backgroundObjectSprites[i].setTexture(cloudTexture);
            setSpriteOPS(&backgroundObjectSprites[i], { 0.f,0.f }, { 500.f + rand() % 1000, 150.f * (i - beeCount) }, { -1.f * dir * (random + 0.5f), 1.f * (random + 0.5f) });
        }
    }

    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    setSpriteOPS(&playerSprite, { playerTexture.getSize().x / 2.f, playerTexture.getSize().y * 1.f}, { halfWindowWidth + 300.f, treeTexture.getSize().y + 50.f }, baseScale);
    Side playerSide = Side::RIGHT;

    const int NUM_BRANCHES = 6;
    sf::Sprite branchSprites[NUM_BRANCHES];
    Side branchSides[NUM_BRANCHES];
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        int random = rand() % 3;
        sf::Vector2f scale = { 1.f, 1.f };
        switch (random)
        {
        case 0:
            branchSides[i] = Side::LEFT;
            scale = { -1.f,1.f };
            break;
        case 1:
            branchSides[i] = Side::RIGHT;
            break;
        case 2:
            branchSides[i] = Side::NONE;
            break;
        default:
            break;
        }
        setSpriteOPS(&branchSprites[i], { -1.f * treeTexture.getSize().x / 2.f, 0.f }, { halfWindowWidth, 150.f * i }, scale);
        branchSprites[i].setTexture(branchTexture);
    }
    branchSides[5] = Side::NONE;

    sf::RectangleShape timeBar;
    timeBar.setFillColor(sf::Color::Red);
    float timeBarWidth = 500.f;
    timeBar.setSize({ timeBarWidth, 50.f });
    timeBar.setPosition(halfWindowWidth - timeBarWidth / 2.f, treeTexture.getSize().y + 100.f);

    sf::Text scoreText, menuText;
    scoreText.setFont(gameFont);
    setTextSetting(&scoreText, "Score : 0", sf::Color::White, { 300.f, 50.f }, 100.f);
    menuText.setFont(gameFont);
    setTextSetting(&menuText, "Press Enter to Start!" , sf::Color::White, { halfWindowWidth, halfWindowHeight}, 100.f);

    sf::Sound chopSound, deathSound, outoftimeSound;
    chopSound.setBuffer(chopSoundBuffer);
    chopSound.setPitch(0.5f);
    chopSound.setVolume(20.f);
    deathSound.setBuffer(deathSoundBuffer);
    deathSound.setPitch(0.5f);
    deathSound.setVolume(20.f);
    outoftimeSound.setBuffer(outoftimeSoundBuffer);
    outoftimeSound.setPitch(0.5f);
    outoftimeSound.setVolume(20.f);

    const int NUM_LOGS = 40;
    sf::Sprite logSprites[NUM_LOGS];
    sf::Vector2f logDirections[NUM_LOGS];
    sf::Vector2f logVelocities[NUM_LOGS];
    float logSpeeds = 1000.f;
    sf::Vector2f logInitPosition = { halfWindowWidth, treeTexture.getSize().y * 1.f };
    for (int i = 0; i < NUM_LOGS; i++)
    {
        logSprites[i].setTexture(logTexture);
        setSpriteOPS(&logSprites[i], { logTexture.getSize().x / 2.f, logTexture.getSize().y * 1.f }, logInitPosition, baseScale);
    }

    sf::Sprite axeSprite;
    axeSprite.setTexture(axeTexture);
    setSpriteOPS(&axeSprite, { -90.f, 35.f }, { halfWindowWidth, treeTexture.getSize().y * 1.f }, baseScale);

    sf::Clock clock;

    sf::Vector2f gravity = { 0.f,1000.f };

    bool isLeft = false;
    bool isRight = false;
    bool leftDown = false;
    bool rightDown = false;
    bool gameStop = true;

    float remainTime = 6.f;
    float timeVelocity = timeBarWidth / remainTime;

    int score = 0;
    int chopLogCount = 0;

    while (window.isOpen())
    {   
        sf::Time time = clock.restart();
        float deltatime = time.asSeconds();

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
                    if (branchSides[NUM_BRANCHES - 1] == playerSide || remainTime <= 0.f)
                    {
                        branchSides[NUM_BRANCHES - 1] = Side::NONE;
                        remainTime = 6.f;
                        score = 0;
                        scoreText.setString("Score : " + std::to_string(score));
                        chopLogCount = 0;
                    }
                    else
                    {
                        menuText.setString("Press Enter to Resume!");
                        resetTextOrigin(&menuText);
                    }
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
            if (remainTime <= 0.f)
            {
                gameStop = true;
                outoftimeSound.play();
                menuText.setString("Press Enter to Restart!");
                resetTextOrigin(&menuText);
            }

            remainTime -= deltatime;

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
                    axeSprite.setScale({-1.f,1.f});
                    logDirections[chopLogCount] = {1.f,-1.f};
                }
                else
                {
                    sidePlayerandBranch(&playerSprite, treeTexture, branchSides, &playerSide, NUM_BRANCHES, 1.f);
                    axeSprite.setScale({ 1.f,1.f });
                    logDirections[chopLogCount] = { -1.f,-1.f };
                }
                
                logSprites[chopLogCount].setPosition(logInitPosition);
                logVelocities[chopLogCount] = logDirections[chopLogCount] * logSpeeds;
                chopLogCount++;
                setBranchDir(branchSprites, NUM_BRANCHES, branchSides);

                if (playerSide == branchSides[NUM_BRANCHES - 1])
                {
                    gameStop = true;
                    deathSound.play();
                    menuText.setString("Press Enter to Restart!");
                    resetTextOrigin(&menuText);
                }
                else
                {
                    chopSound.play();
                    score += 10;
                    scoreText.setString("Score : " + std::to_string(score));
                }
            }

            for (int i = 0; i < chopLogCount; i++)
            {
                sf::Vector2f chopPosition = logSprites[i].getPosition();
                logVelocities[i] += gravity * deltatime;
                chopPosition += logVelocities[i] * deltatime;
                logSprites[i].setPosition(chopPosition);
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
        for (int i = 0; i < chopLogCount; i++)
        {
            window.draw(logSprites[i]);
        }
        drawSprites(backgroundObjectSprites, beeCount, &window, 0);
        window.draw(playerSprite);
        if ((isLeft || isRight) && !gameStop)
        {
            window.draw(axeSprite);
        }
        window.draw(timeBar);
        if (gameStop)
        {
            window.draw(menuText);
        }
        else
        {
            window.draw(scoreText);
        }
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

void setSpriteOPS(sf::Sprite* sprite, sf::Vector2f origin, sf::Vector2f position, sf::Vector2f scale) 
{
    (*sprite).setOrigin(origin);
    (*sprite).setPosition(position);
    (*sprite).setScale(scale);
}

void setTextSetting(sf::Text* text, sf::String string, sf::Color color, sf::Vector2f position, float fontSize)
{
    (*text).setString(string);
    (*text).setFillColor(color);
    (*text).setPosition(position);
    (*text).setCharacterSize(fontSize);

    resetTextOrigin(text);
}

void resetTextOrigin(sf::Text* text)
{
    float textWidth = (*text).getGlobalBounds().width;
    float textHeight = (*text).getGlobalBounds().height;
    (*text).setOrigin({ textWidth / 2.f, textHeight / 2.f });
}