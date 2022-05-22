#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>
#include "../inc/game.h"
#include <vector>
#include <string>
#include "../inc/utils.h"
#include "../inc/pod.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 32;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "Le meilleur jeu!", sf::Style::Default, settings);
    window.setView(sf::View(sf::Rect(0.f,0.f,16000.f,9000.f)));
    window.setVerticalSyncEnabled(false);

    //frame per second
    float fps = 60;
    //temps minimal entre deux frame
    sf::Time refreshTime = sf::microseconds(1e6/fps);
    window.setFramerateLimit(120);

    //horloge depuis le début du jeu
    sf::Clock globalClock;
    //horloge réinitialisée à chaque frame
    sf::Clock refreshClock;

    //temps de la physique du jeu
    sf::Time physicsTime = sf::Time::Zero;

    //Checkpoints
    unsigned int nbCP = 5;
    std::vector<sf::Vector2f> checkpointsPositions = randomCP(nbCP);
    //Vous pouvez aussi initialiser myGame avec une liste de checkpoints prédéfinie
    Game myGame(checkpointsPositions);

    //pods 
    unsigned int nbPods = 4;
    std::vector<sf::Vector2f> positionPods = randomCP(nbPods);
    myGame.addPod(nbPods,positionPods);
    myGame.pods_[0].IA_=false;


    //affichage texte
    sf::Font font;
    font.loadFromFile("../repository/Fredoka-Bold.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(400);
    text.setFillColor(sf::Color::Black);


    //affichage bonus
    sf::Sprite sp_champi;
    sf::Texture tex_champi;
    tex_champi.loadFromFile("../repository/Images/champignon.png");
    sp_champi.setTexture(tex_champi);
    setOriginToCenter(sp_champi);
    sp_champi.setPosition(sf::Vector2f(15500.f,500.f));
    scaleToMinSize(sp_champi,800,800);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        //si on a dépassé le refreshTime depuis la dernière frame
        if (refreshClock.getElapsedTime() >= refreshTime)
        {
            //réinitialise l'horloge qui mesure le temps entre les frames
            refreshClock.restart();

            //récupère le temps écoulé depuis le début du jeu
            sf::Time frameTime = globalClock.getElapsedTime();

            //si on a dépassé le temps de la physique
            if (frameTime > physicsTime)
            {
                //met à jour les sprite au temps de la physique
                myGame.updateGraphics(physicsTime);
                //fait avancer la physique d'un pas de temps
                myGame.updatePhysics();
                physicsTime += PHYSICS_TIME_STEP;

                //reprend le temps écoulé depuis le début pour rester précis
                frameTime = globalClock.getElapsedTime();

                //on met a jour les tours
                std::string score="";
                for (unsigned int i=0; i<nbPods ;++i) {
                    if (i==0) {
                        score += "JOUEUR : " +(std::to_string(myGame.pods_[i].lapCount_))+"\n";
                    } else {
                        score += "IA " + (std::to_string(i)) + " : " + (std::to_string(myGame.pods_[i].lapCount_))+"\n";
                    }
                }
                text.setString(score);

                

            }
            
            //met à jour les sprites au temps actuel
            myGame.updateGraphics(frameTime);
            window.draw(myGame);
            window.draw(text);

            //bonus actif
            if (myGame.pods_[0].champignon_>=0 && myGame.pods_[0].champignon_<=100 ) {
                window.draw(sp_champi);
            }

            window.display();
        }
    }

    return 0;
}