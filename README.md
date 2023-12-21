# Implementation de l'algorithme de hachage Keccak


## Membres de l'équipe

    Sabir Mohamed Ben Ali
    Agustin Bouchon
    Anthony Becuwe

Ce projet implémente l'algorithme de hachage Keccak, une famille de fonctions de hachage cryptographiques basée sur la structure de permutation SPONGE. L'algorithme Keccak a été sélectionné comme le standard NIST pour SHA-3 (Secure Hash Algorithm 3) en 2015.

## Algorithme Keccak

### Concept de base

L'algorithme Keccak suit le modèle SPONGE, qui absorbe les données en blocs et produit un hachage de taille fixe en sortie. Il comprend plusieurs étapes clés, notamment θ (theta), ρ (rho), π (pi), χ (chi) et ι (iota), qui contribuent à sa résistance cryptographique.

### Paramètres de la fonction `keccak_f`

La fonction principale `keccak_f` réalise une ronde de l'algorithme Keccak. Elle utilise des constantes spécifiques, des décalages, des permutations et des rotations pour transformer l'état interne.

## Implémentation

### Structures de données

L'implémentation utilise une structure `keccak_t` pour stocker l'état interne de l'algorithme. Cette structure est mise à jour au fur et à mesure de l'absorption des données et de l'exécution des rondes.

### Fonctions principales

    keccak_init : Initialise l'état interne d'un objet Keccak (sha3).
    keccak_absorb : Absorbe les données d'entrée.
    keccak_squeeze : Squeeze les données de sortie.
    keccak : Fonction générique pour calculer le hachage SHA-3.

### Fonctions de hachage SHA-3

Plusieurs fonctions de hachage SHA-3, telles que `sha3_224`, `sha3_256`, `sha3_384` et `sha3_512`, sont fournies pour simplifier l'utilisation de l'algorithme Keccak avec différentes longueurs de sortie.

## Comment utiliser

    ./SHA3-256 -m 256 <fichier>
    ./SHA3-256 -m 254 <fichier>
    ./SHA3-256 -m 384 <fichier>
    ./SHA3-256 -m 512 <fichier>

    ./SHA3-256 <fichier1> <fichier2>

## Nos difficultés lors du projet : 
Nous avons eu un peu de mal à comprendre le bon fonctionnement de la fonction de sponge/squeeze au départ.

L'algorithme proposé par [Keccak_team](https://keccak.team/keccak_specs_summary.html) ainsi que leur explication nous on bien aidé à comprendre la technique derriere keccak.

Afin d'avoir une bonne implémentation, en plus de suivre le guide de la keccak_team nous avons regardé diffrentes implementations de la fonction Keccak : [tiny_sha3](https://github.com/mjosaarinen/tiny_sha3/blob/master/sha3.c) , [keccak_tiny](https://github.com/coruus/keccak-tiny/blob/singlefile/keccak-tiny.c). 
Tous validé par la [Keccak_team](https://keccak.team/software.html)
