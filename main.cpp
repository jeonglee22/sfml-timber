#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <SFML/Audio.hpp>

enum class Side { LEFT, RIGHT, NONE};

void spawnPos(sf::Sprite*, int, sf::Vector2f*, float*, int);
void resetPos(sf::Sprite*, sf::Vector2f*, float*, int*, int, int, int);
void updateBranch(Side*, int);

int main()
{
    srand((int)time(0));

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!");

    char files[12][100] = { "graphics/background.png", "graphics/tree.png", "graphics/bee.png",
        "graphics/cloud.png" , "graphics/player.png", "graphics/branch.png", "graphics/axe.png",
        "fonts/KOMIKAP_.ttf", "sound/chop.wav", "sound/death.wav","sound/out_of_time.wav", "graphics/log.png"
    };

    sf::Texture backgroudTexture, treeTexture, beeTexture, cloudTexture, playerTexture, branchTexture, axeTexture, logTexture;
    backgroudTexture.loadFromFile(files[0]);
    treeTexture.loadFromFile(files[1]);
    beeTexture.loadFromFile(files[2]);
    cloudTexture.loadFromFile(files[3]);
    playerTexture.loadFromFile(files[4]);
    branchTexture.loadFromFile(files[5]);
    axeTexture.loadFromFile(files[6]);
    logTexture.loadFromFile(files[11]);

    sf::Font gameFont;
    gameFont.loadFromFile(files[7]);

    /*sf::SoundBuffer chopSoundBuffer, deathSoundBuffer, outoftimeSoundBuffer;
    chopSoundBuffer.loadFromFile(files[8]);
    deathSoundBuffer.loadFromFile(files[9]);
    outoftimeSoundBuffer.loadFromFile(files[10]);*/

    sf::Sprite spriteBackground;
    spriteBackground.setTexture(backgroudTexture);

    sf::Sprite spriteAxe;
    spriteAxe.setTexture(axeTexture);
    spriteAxe.setOrigin(axeTexture.getSize().x, axeTexture.getSize().y);
    spriteAxe.setPosition(500.f, 500.f);

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
    Side sideBranch[NUM_BRANCHES] = {};

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        spriteBranch[i].setTexture(branchTexture);
        spriteBranch[i].setOrigin(-1.f * treeTexture.getSize().x / 2.f, 0.f);
        spriteBranch[i].setPosition(window.getSize().x / 2.f, 150.f * i);

        int newbranch = rand() % 3;
        switch (newbranch)
        {
        case 0:
            sideBranch[i] = Side::LEFT;
            break;
        case 1:
            sideBranch[i] = Side::RIGHT;
            break;
        default:
            sideBranch[i] = Side::NONE;
            break;
        }
    }
    sideBranch[5] = Side::NONE;

    const int beeCount = 1;
    const int cloudCount = 3;

    sf::Sprite spriteBackgroundObjects[beeCount + cloudCount];
    for (int i = 0; i < beeCount + cloudCount; i++)
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

    sf::Vector2f direction[beeCount + cloudCount] = {  };
    float speed[beeCount + cloudCount] = {  };

    spawnPos(spriteBackgroundObjects, beeCount + cloudCount, direction, speed, beeCount);

    sf::Text scoreText;
    scoreText.setFont(gameFont);
    scoreText.setString("SCORE : 0");
    scoreText.setCharacterSize(100);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20,20);

    char texts[3][100] = { "Press Enter to start!", "Press Enter to resume!", "Press Enter to restart!" };

    sf::Text gameText;
    gameText.setFont(gameFont);
    gameText.setString(texts[0]);
    gameText.setCharacterSize(100);
    gameText.setFillColor(sf::Color::White);
    gameText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
    sf::Vector2f messageOrigin;
    messageOrigin.x = gameText.getLocalBounds().width * 0.5f;
    messageOrigin.y = gameText.getLocalBounds().height * 0.5f;
    gameText.setOrigin(messageOrigin);

    bool beginGame = true;

    sf::RectangleShape timeBar;
    float timeBarWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize({ timeBarWidth ,timeBarHeight });
    timeBar.setFillColor(sf::Color::Red);
    timeBar.setPosition(window.getSize().x / 2.f - timeBarWidth / 2.f, window.getSize().y - 100.f);

    /*sf::Sound chopSound, deathSound, outoftimeSound;
    chopSound.setBuffer(chopSoundBuffer);
    deathSound.setBuffer(deathSoundBuffer);
    outoftimeSound.setBuffer(outoftimeSoundBuffer);*/

    sf::Sprite spriteLogs[40];
    sf::Vector2f logInitPosition = spriteTree.getPosition();
    logInitPosition.y = treeTexture.getSize().y;
    bool logMoving[40];
    sf::Vector2f logDirection[40];
    sf::Vector2f logVelocity[40];
    float logRotation[40];
    float logRotationDir[40];
    float logSpeed = 1000.f;
    for (int i =0; i<40; i++)
    {
        spriteLogs[i].setTexture(logTexture);
        spriteLogs[i].setOrigin(logTexture.getSize().x / 2.f, logTexture.getSize().y);
        spriteLogs[i].setPosition(logInitPosition);

        logMoving[i] = false;
        logDirection[i] = { 1.f, -1.f };
        logVelocity[i] = logDirection[i] * logSpeed;
        logRotation[i] = 0.f;
        logRotationDir[i] = 1.f;
    }
    sf::Vector2f gravity = {0.f, 1000.f};

    sf::Clock clock;
    int angle = 0;
    float spawnPadding = 100.f;
    float deletePadding = 350.f;
    bool isLeft = false;
    bool isRight = false;
    bool axeAppear = false;
    bool stopGame = true;
    int score = 0;
    float remainingTime = 5.f;
    float timeBarWidthPerSecond = timeBarWidth / remainingTime;
    int logCount = 0;

    while (window.isOpen())
    {   
        sf::Time time = clock.restart();
        float deltaTime = time.asSeconds();
        
        bool leftDown = false;
        bool leftUp = false;
        bool rightDown = false;
        bool rightUp = false;

        // event loop
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
                case sf::Keyboard::Enter:
                    stopGame = !stopGame;
                    if (beginGame)
                    {
                        beginGame = !beginGame;
                    }
                    else
                    {
                        if (remainingTime <= 0.f || sideBranch[NUM_BRANCHES - 1] == sidePlayer)
                        {
                            remainingTime = 5.f;
                            score = 0;
                            scoreText.setString("SCORE : " + std::to_string(score));
                            sideBranch[NUM_BRANCHES - 1] = Side::NONE;
                            logCount = 0;
                        }
                        else
                        {
                            gameText.setString(texts[1]);
                            sf::Vector2f messageOrigin;
                            messageOrigin.x = gameText.getLocalBounds().width * 0.5f;
                            messageOrigin.y = gameText.getLocalBounds().height * 0.5f;
                            gameText.setOrigin(messageOrigin);
                        }
                    }
                    break;
                case sf::Keyboard::Left:
                    isLeft = false;
                    leftUp = true;
                    break;
                case sf::Keyboard::Right:
                    isRight = false;
                    rightUp = true;
                    break;
                default:
                    break;
                }
                
                break;
            default:
                break;
            }
        }

        if(!stopGame)
        {
            remainingTime -= deltaTime;
            if (remainingTime <= 0.f)
            { 
                //outoftimeSound.play();
                remainingTime = 0;
                stopGame = true;
                gameText.setString(texts[2]);
                sf::Vector2f messageOrigin;
                messageOrigin.x = gameText.getLocalBounds().width * 0.5f;
                messageOrigin.y = gameText.getLocalBounds().height * 0.5f;
                gameText.setOrigin(messageOrigin);
            }
            timeBar.setSize({ timeBarWidthPerSecond * remainingTime , timeBarHeight });

            // update area
            if (leftDown || rightDown)
            {
                if (leftDown)
                {
                    sidePlayer = Side::LEFT;
                    logDirection[logCount] = {1.f,-1.f};
                    logRotationDir[logCount] = 1.f;
                }
                else
                {
                    sidePlayer = Side::RIGHT;
                    logDirection[logCount] = {-1.f,-1.f};
                    logRotationDir[logCount] = -1.f;
                }

                logMoving[logCount] = true;
                spriteLogs[logCount].setPosition(logInitPosition);
                logVelocity[logCount] = logDirection[logCount] * logSpeed;
                logCount++;

                updateBranch(sideBranch, NUM_BRANCHES);
                if (sideBranch[NUM_BRANCHES - 1] == sidePlayer)
                {   
                    //deathSound.play();
                    stopGame = true;
                    gameText.setString(texts[2]);
                    sf::Vector2f messageOrigin;
                    messageOrigin.x = gameText.getLocalBounds().width * 0.5f;
                    messageOrigin.y = gameText.getLocalBounds().height * 0.5f;
                    gameText.setOrigin(messageOrigin);
                }
                else
                {
                    score += 10;
                    scoreText.setString("SCORE : " + std::to_string(score));
                }
            }

            direction[0].y = ((90 - angle % 180) / 90.f) * 3.f;
            if (angle % 360 > 180)
            {
                direction[0].y *= -1.f;
            }
            angle++;

            for (int i = 0; i < beeCount + cloudCount; i++)
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
                spritePlayer.setPosition(spriteTree.getPosition().x - 300.f, playerYPos);
                spritePlayer.setScale(-1.f, 1.f);
                spriteAxe.setPosition(spritePlayer.getPosition().x + playerTexture.getSize().x / 2.f - 20.f, playerYPos - 60.f);
                spriteAxe.setScale(-1.f, 1.f);
                break;
            case Side::RIGHT:
                spritePlayer.setPosition(spriteTree.getPosition().x + 300.f, playerYPos);
                spritePlayer.setScale(1.f, 1.f);
                spriteAxe.setPosition(spritePlayer.getPosition().x - playerTexture.getSize().x / 2.f + 20.f, playerYPos - 60.f);
                spriteAxe.setScale(1.f, 1.f);
                break;
            default:
                break;
            }
            for(int i = 0 ; i<logCount; i++)
            {
                logVelocity[i] += gravity * deltaTime;
                logRotation[i] += 1.f * logRotationDir[i];

                sf::Vector2f position = spriteLogs[i].getPosition();
                position += logVelocity[i] * deltaTime;
                spriteLogs[i].setPosition(position);
                spriteLogs[i].setRotation(logRotation[i]);

                if (spriteLogs[i].getPosition().y >= window.getSize().y + logTexture.getSize().y)
                {
                    logCount--;
                    for (int j = 0; j < logCount; j++)
                    {
                        spriteLogs[j] = spriteLogs[j+1];
                        logMoving[j] = logMoving[j+1];
                        logDirection[j] = logDirection[j+1];
                        logVelocity[j] = logVelocity[j+1];
                        logRotation[j] = logRotation[j + 1];
                    }
                    i--;
                }
            }
        }

        // draw area
        window.clear();

        window.draw(spriteBackground);
        for (int i = beeCount; i < beeCount + cloudCount; i++)
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
        for (int i = 0; i < logCount; i++)
        {
            window.draw(spriteLogs[i]);
        }
        window.draw(spriteBackgroundObjects[0]);
        window.draw(spritePlayer);
        if ((isLeft || isRight) && !stopGame)
        {
            window.draw(spriteAxe);
            //chopSound.play();
        }
        if (stopGame)
        {
            window.draw(gameText);
            if (!beginGame)
            {
                window.draw(scoreText);
            }
        }
        else
        {
            window.draw(scoreText);
        }
        window.draw(timeBar);
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
            sprites[i].setPosition(rand() % (1920 / 2) + (1920 / 4.f), (rand() % 100) + 1080 * 2.0f / 4.0f);
            sprites[i].setScale(-1.f * dir, 1.f);
        }
        else
        {
            sprites[i].setPosition(rand() % (1920 / 2) + (1920 / 4.f), (rand() % 100) + 1080 * ((i-beeCount) / 8.0f));
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

void updateBranch(Side* Branches, int size)
{
    for (int i = size - 1; i > 0 ; i--)
    {
        Branches[i] = Branches[i - 1];
    }

    int newbranch = rand() % 3;
    switch (newbranch)
    {
    case 0:
        Branches[0] = Side::LEFT;
        break;
    case 1:
        Branches[0] = Side::RIGHT;
        break;
    default:
        Branches[0] = Side::NONE;
        break;
    }
}