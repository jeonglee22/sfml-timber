#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

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

    sf::CircleShape ball1;
    ball1.setRadius(5.f);
    ball1.setFillColor(sf::Color::Red);
    ball1.setOutlineColor(sf::Color::Black);
    ball1.setOutlineThickness(1.f);
    ball1.setPosition(window.getSize().x / 2.f, 200.f);

    sf::CircleShape ball2;
    ball2.setRadius(5.f);
    ball2.setFillColor(sf::Color::Red);
    ball2.setOutlineColor(sf::Color::Black);
    ball2.setOutlineThickness(1.f);
    ball2.setPosition(window.getSize().x / 2.f, 400.f);

    float ball1Speed = 200.f;
    sf::Vector2f ball1Direction = {1.f, 1.f};

    float ball2Speed = 200.f;
    sf::Vector2f ball2Direction = { 1.f, 0.f };

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
        sf::Vector2f ball1Pos = ball1.getPosition();
        ball1Pos += ball1Direction * ball1Speed * deltaTime;
        ball1.setPosition(ball1Pos);

        if (ball1Pos.x <= player.getPosition().x + player.getSize().x / 2.f &&
            ball1Pos.x > player.getPosition().x - player.getSize().x / 2.f &&
            ball1Pos.y > player.getPosition().y - player.getSize().y &&
            ball1Pos.y <= player.getPosition().y)
        {
            ball1Direction = { ball1Direction.x ,-ball1Direction.y };
        }
        if (ball1Pos.y <= 0)
        {
            ball1Direction = { ball1Direction.x,-ball1Direction.y };
        }
        if (ball1Pos.x <= window.getSize().x / 2.f - background.getSize().x / 2.f)
        {
            ball1Direction = { -ball1Direction.x,ball1Direction.y };
        }
        if (ball1Pos.x >= window.getSize().x / 2.f + background.getSize().x / 2.f - 10.f)
        {
            ball1Direction = { -ball1Direction.x,ball1Direction.y };
        }

        sf::Vector2f ball2Pos = ball2.getPosition();
        ball2Pos += ball2Direction * ball2Speed * deltaTime;
        ball2.setPosition(ball2Pos);

        if (ball2Pos.x <= player.getPosition().x + player.getSize().x / 2.f &&
            ball2Pos.x > player.getPosition().x - player.getSize().x / 2.f &&
            ball2Pos.y > player.getPosition().y - player.getSize().y &&
            ball2Pos.y <= player.getPosition().y)
        {
            ball2Direction = { ball2Direction.x,-ball2Direction.y };
        }
        if (ball2Pos.y <= 0)
        {
            ball2Direction = { ball2Direction.x,-ball2Direction.y };
        }
        if (ball2Pos.x <= window.getSize().x / 2.f - background.getSize().x / 2.f)
        {
            ball2Direction = { -ball2Direction.x,ball2Direction.y };
        }
        if (ball2Pos.x >= window.getSize().x / 2.f + background.getSize().x / 2.f - 10.f)
        {
            ball2Direction = { -ball2Direction.x,ball2Direction.y };
        }
            
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
        window.draw(ball1);
        window.draw(ball2);
        window.display();
    }

    return 0;
}