# Roadmap : les niveaux

Objectif : finir le jeu le plus vite possible, sur 20 niveaux enchaînés dans
le même ordre (pas d'aléatoire pour l'instant), avec une difficulté
croissante, de plus en plus de boss, des monstres différents, des obstacles
et un niveau « death run ».

## Décision sur le gain de temps par kill

Choix retenu : **le saut de niveau** (option 2), pas la réduction du chrono.

- **Option 1, retirer du temps au chrono** : simple, mais le chrono n'est plus
  le temps écoulé, et le jeu ne change pas. À garder pour plus tard comme
  bonus de classement (temps final = temps réel − bonus).
- **Option 2, faire avancer le niveau** : chaque monstre tué fait avancer le
  défilement de X secondes, donc les vagues et le boss arrivent plus tôt. Le
  chrono reste honnête. Point de départ : 0,5 s par monstre, avec un plafond
  par niveau (environ 40 % de sa durée).
- Ne pas combiner les deux : le joueur serait récompensé deux fois.

## Les étapes

1. **Structure de niveau** (le plus gros chantier, tout en dépend)
   - Aujourd'hui `Waves.hpp` est un tableau fixe de 4 vagues et le boss
     arrive à une distance fixe.
   - Créer une définition de niveau : vagues, boss ou non, durée,
     multiplicateurs de difficulté. `Game` garde un numéro de niveau.
   - Extraire cette logique dans une classe à part (par exemple
     `LevelRunner`) : `Game.hpp` est déjà gros.
2. **Enchaînement des 20 niveaux**
   - Fin de niveau proposée : toutes les vagues déclenchées et éliminées, plus
     le boss s'il y en a un.
   - Entre deux niveaux : on garde les joueurs, la bourse et le chrono, on
     détruit le reste.
   - Envoyer le numéro de niveau dans `STATUS` (protocole en version 7) et
     l'afficher dans le HUD. Victoire seulement après le niveau 20.
   - La pause entre niveaux peut accueillir l'achat d'augments avec la bourse.
3. **Difficulté croissante**
   - Générer les niveaux à partir de quelques gabarits de vagues et d'une
     formule : PV, dégâts, cadence de tir et nombre de monstres augmentent avec
     le numéro du niveau.
   - Les stats sont appliquées à la création du monstre.
4. **Monstres différents**
   - Un type de monstre (`MonsterKind`) ou un composant de comportement :
     zigzag, tireur, tank, kamikaze.
   - Chaque type : un sprite (nouvelle valeur de `SpriteId`, donc côté
     protocole) et un système de mouvement.
   - Commencer par deux types pour valider l'approche.
5. **Boss**
   - Généraliser `BossTag` et `bossSystem` pour avoir plusieurs patterns.
   - La règle « de plus en plus de boss » se met dans la définition de niveau :
     mini-boss tous les N niveaux, plusieurs boss dans les derniers niveaux,
     boss final au niveau 20.
6. **Saut de niveau par kill**
   - Une fois les niveaux définis, régler X avec leur durée réelle.
7. **Obstacles**
   - Composant `Obstacle` : une hitbox qui défile avec la carte à
     `SCROLL_SPEED`.
   - Règle de collision à décider : bloquant ou mortel au contact.
   - Niveau « death run » : obstacles mortels formant un couloir étroit, à
     détailler plus tard. La définition de niveau doit pouvoir décrire une
     disposition d'obstacles.
8. **Équilibrage, tests, documentation**
   - Un test par étape, la doc de protocole mise à jour à chaque changement de
     format, des parties complètes pour régler la difficulté.

## Ordre

La structure de niveau d'abord : monstres, boss, obstacles et saut de niveau
s'y accrochent. Les types de monstres et les obstacles peuvent ensuite être
codés en parallèle par des personnes différentes.

## À trancher avant de démarrer

1. La règle de fin de niveau : vagues éliminées plus boss, ou distance
   atteinte ?
2. Le contenu entre les niveaux : boutique d'augments ?
3. Qui fait quoi : une partie du travail touche `Game.hpp`, qui contient déjà
   les augments de l'équipe. S'accorder avant d'extraire `LevelRunner`, pour
   éviter les conflits de fusion.

## Niveaux, types de monstres et de boss (design retenu)

### Les niveaux

- Les niveaux sont écrits à la main dans un tableau, en C++ (pas de JSON pour
  l'instant). La structure `Segments` (`Levels.hpp`) est déjà prête à être
  remplie depuis un fichier plus tard.
- Une ligne de niveau contient, pour chaque vague : le type de monstre
  (`kind`), leur nombre, la distance où la vague se déclenche. Elle contient
  aussi le boss et son type, et la longueur du niveau.
- Les distances sont **relatives au début du niveau**. Une fonction
  `levelBuilder` met les niveaux bout à bout en ajoutant le décalage, et
  remplit les trois listes de `Segments` (vagues, boss, numéros de niveau).
- Le boss bloque la distance tant qu'il est vivant.

### Les types de monstres (`kind`)

- Un enum `MonsterKind`, envoyé à la fabrique au moment de la création.
- Chaque monstre reçoit un composant qui contient la valeur de l'enum, y
  compris le monstre de base (`Basic`), pour que le système de comportement
  les voie tous.
- Une **table de définitions indexée par l'enum** donne, pour chaque type : PV,
  vitesse, dégâts, sprite et la fonction de comportement. La fabrique lit la
  définition, le système de comportement appelle la fonction de la table.
- Correction par rapport à l'idée de départ : on ne peut pas stocker un
  *type* de composant dans un `vector` de paires (les types n'existent qu'à la
  compilation). On associe l'enum à une fonction, dans un tableau indexé par
  l'enum, avec un `Count` et un `static_assert` pour vérifier qu'il y a une
  entrée par valeur.
- Le dispatch du comportement se fait avec un tableau de fonctions, pas un
  gros `switch` (fonctions courtes, pas de ternaire).
- L'état propre à un comportement (un minuteur de zigzag, par exemple) va dans
  un petit composant commun, par exemple un âge depuis l'apparition.
- Cette table est ce qu'on chargerait depuis un JSON plus tard.

Types prévus (comportements à décider) : `Basic` (ligne droite), `Tank` (lent,
beaucoup de PV), `Kamikaze` (fonce vers un joueur), `Elite` (plus fort, tire
plus), `Rocket`, `Bomber`. On verra plus tard lesquels on fait vraiment.

### Les types de boss (`BossKind`)

Même schéma que les monstres, avec leur propre enum et leur propre table.

- `Standard` : le boss actuel (grand, entre, s'arrête, balance, éventail).
- `Dasher` : petit et rapide, se déplace partout sur l'écran.
- `Summoner` : reste à droite et fait apparaître des monstres.
- `Fortress` : énorme, lent, beaucoup de PV, tirs denses.

L'identifiant de boss de `bossSpawn` (le `size_t`) deviendra un `BossKind`.
