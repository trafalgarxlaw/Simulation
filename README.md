# Travail pratique 2: Automates cellulaires

## Description

Ce programme, écrit en C, permet de simuler différents phénomènes à l'aide
d'[automates cellulaires](https://fr.wikipedia.org/wiki/Automate_cellulaire),
qui sont des matrices de cellules évoluant dans le temps selon des règles qui
dépendent du voisinage de chaque cellule.

Il y a actuellement 3 types de simulation qui sont supportées:

- Le [jeu de la vie](https://fr.wikipedia.org/wiki/Jeu_de_la_vie), popularisé
  par Conway, qui est sans doute l'exemple le plus connu sur les automates
  cellulaires.
- La [propagation d'un virus](https://gitlab.com/ablondin/inf3135-aut2017-tp1-enonce),
  basé sur un travail pratique donné dans le cadre du cours INF3135 à la
  session d'automne 2017.
- Un modèle de [ propagation de
  feux](https://bib.irb.hr/datoteka/278897.Ljiljana_Bodrozic_ceepus2006_2.pdf).

## Auteurs

- [Alexandre Blondin Massé](https://gitlab.com/ablondin) (implémentation
  principale);
- [Alexandre Terrasa](https://gitlab.com/Morriar) (propagation d'un virus).

## Compilation

Après avoir cloné le dépôt, il suffit de se placer à la racine du projet et
d'entrer la commande

```sh
$ make
```

qui produit un exécutable nommé `automaton` dans le répertoire `bin`.

En tout temps, il est possible de nettoyer le répertoire (c'est-à-dire
supprimer les fichiers auxiliaires) en entrant

```sh
$ make clean
```

## Fonctionnement

Pour avoir plus d'informations sur le fonctionnement général du programme, il
suffit d'entrer la commande

```sh
$ bin/automaton --help
```

qui devrait produire

```
Usage: bin/automaton [-h|--help] [-r|--num-rows VALUE] [-c|--num-cols VALUE]
    [-n|--num_steps VALUE] [-t|--type STRING] [-a|--allowed-cells STRING]
    [-d|--distribution VALUES] [-i|--interactive]

Simulates a cellular automaton.

Optional arguments:
  -h, --help                  Shows this help message and exit
  -r, --num-rows VALUE        The number of rows.
                              The default value is 5.
  -c, --num-cols VALUE        The number of columns.
                              The default value is 5.
  -n, --num-steps VALUE       The number of steps in the simulation.
                              The default value is 5.
  -t, --type STRING           The type of simulation.
                              Currently, there are 3 supported types:
                              "game-of-life", "pandemy" and "fire".
                              The default type is "game-of-life".
  -b, --boundary STRING       The boundary type.
                              Currently, there are 2 supported types:
                              "truncate" and "periodic".
                              The default boundary is "truncate".
  -a, --allowed-cells STRING  The allowed cells, as characters.
                              The number of cells must be consistent
                              with the type of simulation.
  -d, --distribution VALUES   The initial probability distribution,
                              which are comma-separated positive values.
                              E.g: if the allowed cells are "abc"
                              and the distribution is "1,1,2", then, 'c'
                              will appear twice as more as 'a' and 'b'.
  -i, --interactive           Enables interactive simulation.
```

Par exemple, supposons que nous souhaitions produire une simulation avec les
paramètres suivants:

- **Type**: propagation d'un virus (`pandemy`);
- **Frontière** (*boundary*): tronquée (`truncate`);
- **Dimensions**: `10` lignes et `15` colonnes;
- **Cellules permises**: `.XH` (`.` indique une cellule non habitée, `X`
  indique une cellule infectée et `H` indique une cellule saine);
- **Nombre d'étapes** (*steps*): `5`;
- **Distribution**: `10,1,3`, qui indique que les cellules `.`, `X` et `H`
  apparaissent avec un ratio `10:1:3` (l'ordre des valeurs pour la distribution
  est le même que l'ordre d'énumération des cellules permises). Autrement dit,
  la cellule `.` apparaît `10` fois plus souvent que la cellule `X` et la
  cellule `H` apparaît `3` fois plus souvent que la cellule `X`.

Alors on peut entrer la commande

```sh
$ bin/automaton -t pandemy -b truncate -r 10 -c 15 -a .XH -n 5 -d 10,1,3
```

et on obtient (le résultat peut varier, puisque l'état initial est généré
aléatoirement):

```
Step 0
.....X......H..
X....X......H.H
.......X.....H.
......X...H..H.
...H......X..X.
...............
......X........
...H......H....
...X..........H
.HH.HH.HH....X.
Step 1
.............H.
......X.....H..
......X.....HHH
............HXH
...............
...............
...............
...............
...X...........
..XHX..........
Step 2
...............
............H.H
...........H..H
............H.H
.............H.
...............
...............
...............
..XXX..........
..XXX..........
Step 3
...............
.............H.
...........HH.H
............H.H
.............H.
...............
...............
...X...........
..X.X..........
..X.X..........
Step 4
...............
............HH.
...........HH.H
...........HH.H
.............H.
...............
...............
...X...........
..X.X..........
...............
```

## Mode interactif

Par défaut, la simulation de l'automate est affichée sur la sortie standard
comme une suite de cellules, étape par étape. Il est possible d'activer le mode
interactif à l'aide de l'option courte `-i` ou l'option longue
``--interactive``. Par exemple, on peut entrer

```sh
$ bin/automaton -i -r 30 -c 50 -n 100 -t fire -a ,.Bb -d 1,10,1,0
```

Cela devrait afficher quelque qui ressemble à ceci:

```
┌──────────────────────────────────────────────────┐
│.,....B................BB..................,,.B.,.│
│,,.......,,.B....,........B.,....,........,,,...,.│
│....,......B.B.................BB........BB....,..│
│...B,........,.B.......B.B.....B..B......,.....,..│
│..........,.B.........,..,...............,....B...│
│B...BB,...,..B..........,....,.....B..............│
│................,..,.......,....,...............B.│
│...............B....B..B.....,....,B.....B...B....│
│.BB..B.......,.....B......B............,B,B,B.....│
│,..............B...B.......,B....B.,...,.,........│
│......,.B..........B...,....B............,..,.....│
│...,B...B...........,................,.......B....│
│.....B....,.B....B.....,..,..,...,...,,...,.,....B│
│.........,.,......B...,..B...B.....,...B.....,....│
│........B.......B.....BBB,........B..,........,...│
│......,,,....B,..........B.......,.,...B........B.│
│..................,..B.B........B...........B..B..│
│,..B..,.....B..................,,B..B...B,........│
│.....BB........B........,..B.....,....B...........│
│.....,........B.,B.........B.B.....,..,...B.......│
│....,...,..B.B.......,.,.B....,,B..,......,.....,.│
│.........B,..B.......,..B..,...........,..B....B..│
│B....B...,...B......BB,.........,.B.B.B...B.,.,...│
│.B.B......,...,.....BB.B.......B...B..........BB..│
│B..........,.............B.,..........,..,......,.│
│.....B..........................................,.│
│B.B..............,..........,B...............B....│
│..,..................B...............,......B.,...│
│.....,,.,,...............,.....,..............,B..│
│.......B.........B,....B.........BB..B............│
└──────────────────────────────────────────────────┘
┌─ Keys ────────────────────── Step: 1 ──────┐
│*F*: Play forward     *B*: Play backward    │
│*s*: Go to start      *e*: Go to end        │
│*f*: One step forward *b*: One step backward│
│*p*: Pause            *q*: Quit             │
└────────────────────────────────────────────┘
```
Il suffit ensuite d'utiliser les touches suivantes pour contrôler l'animation:

- `q` pour *quitter*;
- `f` pour avancer d'une étape;
- `b` pour reculer d'une étape;
- `F` pour faire jouer l'animation vers l'avant;
- `B` pour faire jouer l'animation vers l'arrière;
- `p` pour arrêter l'animation;
- `s` pour se rendre au début de l'animation;
- `e` pour se rendre à la fin de l'animation;

## Documentation

Pour générer la version HTML de ce fichier, il suffit d'entrer la commande

```sh
$ make html
```

et ensuite d'ouvrir le fichier `README.html`.

## Tests

En tout temps, il est possible de vérifier si le programme compile et si son
comportement est cohérent en lançant une suite de tests unitaires. Pour cela,
il suffit d'entrer la commande

```sh
$ make test
```

S'assurer d'avoir installé toutes les [dépendances](#dépendances) avant de
lancer les tests automatiques.

## Plateformes supportées

Le programme a été testé sur MacOS X (Yosemite) et Linux (Ubuntu)

## Dépendances

- [GCC](https://gcc.gnu.org/), pour compiler le projet. C'est le compilateur
  GNU. La version C11 doit être supportée.
- [Ncurses](https://www.gnu.org/software/ncurses/), une bibliothèque
  C permettant un affichage console interactif.
- [PKG-config](https://www.freedesktop.org/wiki/Software/pkg-config/), pour
  simplifier la compilation et l'édition des liens avec d'autres bibliothèques.
- [CUnit](http://cunit.sourceforge.net/doc/index.html) (optionnel), pour
  définir un cadre de tests unitaires en C.
- [Pandoc](https://pandoc.org/) (optionnel), pour générer la documentation.

## Références

- [Jeu de la vie](https://fr.wikipedia.org/wiki/Jeu_de_la_vie), un des premiers
  automates cellulaires rendus populaires par Conway.
- [Pandemi.c](https://gitlab.com/ablondin/inf3135-aut2017-tp1-enonce), un
  travail pratique donné dans le cadre du cours INF3135 à la session d'automne
  2017, qui simule la propagation d'un virus.
- [Modèle de propagation de
  feux](https://bib.irb.hr/datoteka/278897.Ljiljana_Bodrozic_ceepus2006_2.pdf),
  qui propose un modèle simulant la propagation de feux de forêt.
- [Style CSS](https://gist.github.com/dashed/6714393), pour produire le fichier
  `README.html`.

## Licence

Le contenu de ce projet est sous [licence GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html).
