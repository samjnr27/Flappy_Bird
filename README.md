# FlappyBird

Un mini-jeu inspiré du célèbre Flappy Bird, codé en C++ et utilisant la bibliothèque SFML 3.0.0.

## Fonctionnalités

* Contrôles simples : appuyez sur la barre espace pour battre des ailes.
* Tuyaux verts à éviter pour survivre le plus longtemps possible.
* Affichage du score.

## Prérequis

* Un compilateur C++ compatible C++17 (ou supérieur).
* [Make](https://www.gnu.org/software/make/) pour exécuter la compilation.
* [SFML 3.0.0](https://www.sfml-dev.org/) installé sur votre système.

## Installation et lancement

1. Ouvrez un terminal à la racine du projet.

2. Lancez la commande suivante pour compiler le jeu :

   ```bash
   make
   ```

3. Une fois la compilation terminée, exécutez :

   ```bash
   ./FlappyBird
   ```

## Comment jouer

* **Objectif** : Faire passer l'oiseau entre les tuyaux verts pour survivre et établir le meilleur record.
* **Contrôle** :

  * Appuyez sur la **barre espace** pour faire monter l'oiseau.
  * Relâchez la barre espace (ou ne faites rien) pour que l'oiseau redescende.

