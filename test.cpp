#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

void blockDestroyChecking(bool [][7], sf::Vector2f, sf::RectangleShape [][7], sf::Vector2f*);

int main()
{

    sf::RenderWindow window(sf::VideoMode(1366, 768), "block destroy game!");

    sf::RectangleShape background;
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(3.f);
    background.setFillColor(sf::Color::Black);
    background.setOrigin( 400.f, 0);
    background.setPosition(window.getSize().x / 2.f, 0);
    background.setSize({ 800.f,768.f });

    sf::RectangleShape player;
    player.setOutlineColor(sf::Color::Black);
    player.setOutlineThickness(3.f);
    player.setFillColor(sf::Color::Blue);
    player.setOrigin(50.f, 0);
    player.setPosition(window.getSize().x / 2.f, window.getSize().y - 100.f);
    player.setSize({100.f,10.f });

    sf::CircleShape ball;
    ball.setRadius(5.f);
    ball.setOrigin(5.f, 5.f);
    ball.setFillColor(sf::Color::Red);
    ball.setOutlineColor(sf::Color::Black);
    ball.setOutlineThickness(1.f);
    ball.setPosition(window.getSize().x / 2.f, 500.f);

    sf::RectangleShape blocks[7][7];
    bool blockDestroy[7][7];
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            blocks[i][j].setOutlineColor(sf::Color::Black);
            blocks[i][j].setOutlineThickness(1.f);
            blocks[i][j].setFillColor(sf::Color::Green);
            blocks[i][j].setOrigin(40.f, 15.f);
            blocks[i][j].setPosition(window.getSize().x / 2.f + (j - 3.f) * 100.f, 200.f + (i - 3.f) * 40.f);
            blocks[i][j].setSize({ 80.f,30.f });

            blockDestroy[i][j] = false;
        }
    }

    float ballSpeed = 200.f;
    sf::Vector2f ballDirection = {1.f, 1.f};

    sf::Clock clock;

    float playerSpeed = 300.f;
    sf::Vector2f playerDir = { 0.f,0.f };

    while (window.isOpen())
    {
        sf::Time time = clock.restart();
        float deltaTime = time.asSeconds();
        

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
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                case sf::Keyboard::Left:
                    playerDir = { -1.f , 0.f };
                    break;
                case sf::Keyboard::Right:
                    playerDir = {1.f, 0.f };
                    break;
                default:
                    break;
                }
                break;
            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                case sf::Keyboard::Left:
                case sf::Keyboard::Right:
                    playerDir = { 0.f , 0.f };
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        // update area
        sf::Vector2f ballPos = ball.getPosition();
        ballPos += ballDirection * ballSpeed * deltaTime;
        ball.setPosition(ballPos);

        if (ballPos.x <= player.getPosition().x + player.getSize().x / 2.f &&
            ballPos.x > player.getPosition().x - player.getSize().x / 2.f &&
            ballPos.y > player.getPosition().y - player.getSize().y &&
            ballPos.y <= player.getPosition().y)
        {
            ballDirection = { ballDirection.x ,-ballDirection.y };
        }
        if (ballPos.y <= ball.getRadius())
        {
            ballDirection = { ballDirection.x,-ballDirection.y };
        }
        if (ballPos.x <= window.getSize().x / 2.f - background.getSize().x / 2.f)
        {
            ballDirection = { -ballDirection.x,ballDirection.y };
        }
        if (ballPos.x >= window.getSize().x / 2.f + background.getSize().x / 2.f - 10.f)
        {
            ballDirection = { -ballDirection.x,ballDirection.y };
        }

        blockDestroyChecking(blockDestroy, ballPos, blocks, &ballDirection);
            
        sf::Vector2f playerPos = player.getPosition();
        playerPos += playerDir * deltaTime * playerSpeed;
        if (playerPos.x > window.getSize().x / 2.f - background.getSize().x / 2.f + player.getSize().x / 2.0f &&
            playerPos.x < window.getSize().x / 2.f + background.getSize().x / 2.f - 10.f - player.getSize().x / 2.0f)
        {
            player.setPosition(playerPos);
        }

        // draw area
        window.clear();
        window.draw(background);
        window.draw(player);
        window.draw(ball);
        //window.draw(ball2);
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                if (!blockDestroy[i][j])
                    window.draw(blocks[i][j]);
            }
        }
        window.display();
    }

    return 0;
}

void blockDestroyChecking(bool blockDestroy[][7], sf::Vector2f ballPos, sf::RectangleShape blocks[][7], sf::Vector2f* ballDirection)
{
    for (int i = 0; i < 7; i++)
    {
        for (int j =0 ;j <7; j++)
        {
            if (!blockDestroy[i][j] && ballPos.x >= blocks[i][j].getPosition().x - blocks[i][j].getSize().x / 2.f &&
                ballPos.x <= blocks[i][j].getPosition().x + blocks[i][j].getSize().x / 2.f &&
                ballPos.y >= blocks[i][j].getPosition().y + blocks[i][j].getSize().y / 2.f &&
                ballPos.y <= blocks[i][j].getPosition().y + blocks[i][j].getSize().y / 2.f + 5.f)
            {
                *ballDirection = { ballDirection->x,-1 * ballDirection->y };
                blockDestroy[i][j] = true;
            }
            else if (!blockDestroy[i][j] && ballPos.x >= blocks[i][j].getPosition().x - blocks[i][j].getSize().x / 2.f &&
                ballPos.x <= blocks[i][j].getPosition().x + blocks[i][j].getSize().x / 2.f &&
                ballPos.y <= blocks[i][j].getPosition().y - blocks[i][j].getSize().y / 2.f &&
                ballPos.y >= blocks[i][j].getPosition().y - blocks[i][j].getSize().y / 2.f - 5.f)
            {
                *ballDirection = { ballDirection->x, -1 * ballDirection->y };
                blockDestroy[i][j] = true;
            }
            else if (!blockDestroy[i][j] && ballPos.x <= blocks[i][j].getPosition().x - blocks[i][j].getSize().x / 2.f &&
                ballPos.x >= blocks[i][j].getPosition().x - blocks[i][j].getSize().x / 2.f - 5.f &&
                ballPos.y >= blocks[i][j].getPosition().y - blocks[i][j].getSize().y / 2.f &&
                ballPos.y <= blocks[i][j].getPosition().y + blocks[i][j].getSize().y / 2.f)
            {
                *ballDirection = { -1 *ballDirection->x, ballDirection->y };
                blockDestroy[i][j] = true;
            }
            else if (!blockDestroy[i][j] && ballPos.x >= blocks[i][j].getPosition().x + blocks[i][j].getSize().x / 2.f &&
                ballPos.x <= blocks[i][j].getPosition().x + blocks[i][j].getSize().x / 2.f + 5.f &&
                ballPos.y >= blocks[i][j].getPosition().y - blocks[i][j].getSize().y / 2.f &&
                ballPos.y <= blocks[i][j].getPosition().y + blocks[i][j].getSize().y / 2.f)
            {
                *ballDirection = { -1 *ballDirection->x,ballDirection->y };
                blockDestroy[i][j] = true;
            }
        }
    }
}