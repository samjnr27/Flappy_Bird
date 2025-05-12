#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
    // ─── 1) Création de la fenêtre (800×600, titre, style par défaut) ───────
    sf::RenderWindow window(
        sf::VideoMode({800u, 600u}), 
        "Flappy Bird", 
        sf::Style::Default
    );

    // ─── 2) Chargement de la texture et création du sprite de l’oiseau ─────
    sf::Texture birdTexture;
    if (!birdTexture.loadFromFile("./assets/bird.png")) {
        // Si l'image n'est pas trouvée, on quitte
        return 1;
    }
    // Création du sprite à partir de la texture
    sf::Sprite birdSprite(birdTexture);

    // ─── 2a) Centre l’origine du sprite pour un déplacement/rotation symétrique
    {
        auto texSizeF = static_cast<sf::Vector2f>(birdTexture.getSize());
        birdSprite.setOrigin(texSizeF / 2.f);
    }

    // ─── 2b) Redimensionne le sprite pour qu’il fasse toujours 40×40 pixels
    {
        auto texSize = birdTexture.getSize();  // sf::Vector2u { width, height }
        const float desiredW = 40.f;
        const float desiredH = 40.f;
        float scaleX = desiredW / static_cast<float>(texSize.x);
        float scaleY = desiredH / static_cast<float>(texSize.y);
        birdSprite.setScale({ scaleX, scaleY });
    }

    birdSprite.setPosition({100.f, 300.f});  // position initiale

    // ─── 3) Préparation du texte “Game Over” ────────────────────────────────
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        return 1;  // quitte si la police ne se charge pas
    }
    sf::Text gameOverText(font, "Game Over!", 48u);
    gameOverText.setFillColor(sf::Color::Red);
    {
        auto bounds = gameOverText.getLocalBounds();
        gameOverText.setOrigin({bounds.size.x / 2, bounds.size.y / 2});
        gameOverText.setPosition({400.f, 300.f});
    }

    // ─── 4) Variables de physique ────────────────────────────────────────────
    float velocity         = 0.f;     // vitesse verticale (px/s)
    const float gravity      = 800.f; // gravité (px/s²)
    const float jumpStrength = -350.f;// impulsion de saut (px/s)
    sf::Clock clock;                 // mesure du delta time

    // ─── 5) Configuration des tuyaux ────────────────────────────────────────
    struct Pipe {
        sf::RectangleShape top, bottom;
    };
    std::vector<Pipe> pipes;         // stocke les tuyaux à l’écran
    float spawnTimer        = 0.f;   // accumulateur pour spawn
    const float spawnInterval = 1.5f;// secondes entre chaque spawn
    const float pipeSpeed     = 200.f;// vitesse de défilement (px/s)
    const float gapSize       = 150.f;// ouverture (px)

    std::srand(static_cast<unsigned>(std::time(nullptr))); // seed aléatoire

    // ─── 6) Boucle principale ────────────────────────────────────────────────
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds(); // Δt depuis la dernière frame

        // a) Génération automatique des tuyaux
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval) {
            spawnTimer = 0.f;
            float gapY = 100.f + std::rand() % 301; // Y du haut du gap (100–400)

            // tuyau supérieur
            sf::RectangleShape top({60.f, gapY});
            top.setFillColor(sf::Color(0,200,0));
            top.setPosition({800.f, 0.f});

            // tuyau inférieur
            sf::RectangleShape bottom({60.f, 600.f - (gapY + gapSize)});
            bottom.setFillColor(sf::Color(0,200,0));
            bottom.setPosition({800.f, gapY + gapSize});

            pipes.push_back({top, bottom});
        }

        // b) Détection : l’oiseau sort-il de la fenêtre ?
        {
            auto pos = birdSprite.getPosition();
            if (pos.y < 0.f || pos.y > 600.f) {
                // “Game Over”, pause puis reset complet
                window.clear(sf::Color::Black);
                window.draw(gameOverText);
                window.display();
                sf::sleep(sf::seconds(2.f));

                birdSprite.setPosition({100.f, 300.f});
                velocity = 0.f;
                pipes.clear();
                spawnTimer = 0.f;
                clock.restart();
                continue; // passe à l’itération suivante
            }
        }

        // c) Gestion des événements (fermeture + saut)
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (event->is<sf::Event::KeyPressed>()) {
                auto key = event->getIf<sf::Event::KeyPressed>();
                if (key && key->code == sf::Keyboard::Key::Space) {
                    velocity = jumpStrength;
                }
            }
        }

        // d) Physique de l’oiseau (gravité + déplacement)
        velocity += gravity * dt;
        birdSprite.move({0.f, velocity * dt});

        // e) Défilement et nettoyage des tuyaux
        for (auto& p : pipes) {
            p.top.move({-pipeSpeed * dt, 0.f});
            p.bottom.move({-pipeSpeed * dt, 0.f});
        }
        if (!pipes.empty() && pipes.front().top.getPosition().x + 60.f < 0.f) {
            pipes.erase(pipes.begin());
        }

        // f) Détection de collision oiseau ↔ tuyaux
        for (auto& p : pipes) {
            bool hitTop = birdSprite.getGlobalBounds()
                              .findIntersection(p.top.getGlobalBounds()).has_value();
            bool hitBot = birdSprite.getGlobalBounds()
                              .findIntersection(p.bottom.getGlobalBounds()).has_value();
            if (hitTop || hitBot) {
                // “Game Over” + reset
                window.clear(sf::Color::Black);
                window.draw(gameOverText);
                window.display();
                sf::sleep(sf::seconds(2.f));

                birdSprite.setPosition({100.f, 300.f});
                velocity = 0.f;
                pipes.clear();
                spawnTimer = 0.f;
                clock.restart();
                break;
            }
        }

        // g) Rendu final (fond, tuyaux, oiseau texturé)
        window.clear(sf::Color(135,206,235)); // bleu ciel
        for (auto& p : pipes) {
            window.draw(p.top);
            window.draw(p.bottom);
        }
        window.draw(birdSprite);
        window.display();
    }

    return 0;
}
