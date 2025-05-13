#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
    // 1) Création de la fenêtre
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u{800, 600}),
        "Flappy Bird",
        sf::Style::Default
    );

    // 2) Fond unique
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("./assets/ground.jpg"))
        return 1;
    sf::Sprite bgSprite(bgTexture);
    {
        auto ts = static_cast<sf::Vector2f>(bgTexture.getSize());
        bgSprite.setScale({800.f/ts.x, 600.f/ts.y});
    }

    // 3) Oiseau
    sf::Texture birdTexture;
    if (!birdTexture.loadFromFile("./assets/bird.png"))
        return 1;
    sf::Sprite bird(birdTexture);
    {
        auto szF = static_cast<sf::Vector2f>(birdTexture.getSize());
        bird.setOrigin(szF/2.f);
        const float D = 40.f;
        auto szU = birdTexture.getSize();
        bird.setScale({D/szU.x, D/szU.y});
    }
    bird.setPosition({100.f,300.f});

    // 4) “Game Over”
    sf::Font font;
    if (!font.openFromFile("./fonts/arial.ttf"))
        return 1;
    sf::Text gameOver(font, "Game Over!", 48u);
    gameOver.setFillColor(sf::Color::Red);
    {
        auto b = gameOver.getLocalBounds();
        gameOver.setOrigin({b.size.x/2.f, b.size.y/2.f});
        gameOver.setPosition({400.f,300.f});
    }

    // 5) Physique & tuyaux
    float velocity = 0.f;
    const float gravity = 800.f;
    const float jumpStrength = -350.f;
    sf::Clock clock;

    struct Pipe { sf::RectangleShape top, bottom; };
    std::vector<Pipe> pipes;
    float spawnTimer = 0.f;
    const float spawnInterval = 1.5f;
    const float pipeSpeed = 200.f;
    const float gapSize = 150.f;

    std::srand((unsigned)std::time(nullptr));

    // 6) Boucle principale
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // a) Spawn tuyaux
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval) {
            spawnTimer = 0.f;
            float gapY = 100.f + std::rand()%301;

            // ─── Tuyau du haut ───────────────────────────
            sf::RectangleShape top({60.f, gapY});
            top.setFillColor({0,200,0});             // vert clair
            top.setOutlineColor({0,150,0});          // bordure vert foncé
            top.setOutlineThickness(5.f);
            top.setPosition({800.f, 0.f});

            // ─── Tuyau du bas ────────────────────────────
            float botH = 600.f - (gapY + gapSize);
            sf::RectangleShape bottom({60.f, botH});
            bottom.setFillColor({0,200,0});
            bottom.setOutlineColor({0,150,0});
            bottom.setOutlineThickness(5.f);
            bottom.setPosition({800.f, gapY + gapSize});

            pipes.push_back({top, bottom});
        }

        // b) Game Over si hors-écran
        {
            auto pos = bird.getPosition();
            if (pos.y < 0.f || pos.y > 600.f) {
                window.clear(sf::Color::Black);
                window.draw(gameOver);
                window.display();
                sf::sleep(sf::seconds(2.f));
                bird.setPosition({100.f,300.f});
                velocity = 0.f;
                pipes.clear();
                spawnTimer = 0.f;
                clock.restart();
                continue;
            }
        }

        // c) Événements
        while (auto ev = window.pollEvent()) {
            auto& e = *ev;
            if (e.is<sf::Event::Closed>()) window.close();
            else if (auto kp = e.getIf<sf::Event::KeyPressed>())
                if (kp->code == sf::Keyboard::Key::Space)
                    velocity = jumpStrength;
        }

        // d) Physique
        velocity += gravity * dt;
        bird.move({0.f, velocity * dt});

        // e) Défilement + nettoyage des tuyaux
        for (auto& p : pipes) {
            p.top.move({-pipeSpeed*dt,0.f});
            p.bottom.move({-pipeSpeed*dt,0.f});
        }
        if (!pipes.empty() && pipes.front().top.getPosition().x + 60.f < 0.f)
            pipes.erase(pipes.begin());

        // f) Collision
        for (auto& p : pipes) {
            auto bb = bird.getGlobalBounds();
            if (bb.findIntersection(p.top.getGlobalBounds()).has_value() ||
                bb.findIntersection(p.bottom.getGlobalBounds()).has_value()) {
                window.clear(sf::Color::Black);
                window.draw(gameOver);
                window.display();
                sf::sleep(sf::seconds(2.f));
                bird.setPosition({100.f,300.f});
                velocity = 0.f;
                pipes.clear();
                spawnTimer = 0.f;
                clock.restart();
                break;
            }
        }

        // g) Rendu
        window.clear({135,206,235});     // bleu ciel
        window.draw(bgSprite);          // sol de fond
        for (auto& p : pipes) {         // tuyaux stylés
            window.draw(p.top);
            window.draw(p.bottom);
        }
        window.draw(bird);              // oiseau
        window.display();
    }

    return 0;
}
