# Horloge_Analogique-Dinamique
Réaliser une horloge offrant à  la fois des services d'horloge analogique et digital; donnant la possibilité à l'utilisateur de faire des navettes entre différents thèmes . Cette horloge offre aussi un service d'alarme!

## En quoi ça consiste réellement?

Ce projet ( Horloge) a pour but de permettre aux utulisateurs de pour se familliariser avec une app horlographique , comme tout autre app particulière. 

En effet, elle permettra de :
* Navigeur entre diffenrents thèmes (personnaliser son fond d'app à sa préférence) et rendre l'explérience plus alaichante...
* D'éviter de faire un choix entre analogique et digital ( en donnant la possibilité d'utiliser les deux) plus besoin de faire un choix...
* D'utiliser une alarme pour des rappels d'évennements...

Vous vous direz que *C'est pas très innovateur* et *très courant* mais *pour nous l'est!*

# ♦Structuration ou Arborescence

```
Horloge_Analogique_Digital\
|
|___include\
|   |___Audio\
|   |     |---AudioManager.h
|   |___Core\
|   |     |___App.h
|   |     |___ClockApp.h
|   |___Graphics\
|   |     |___AnalogClock.h
|   |     |___DigitalClock.h
|   |     |___Renderer.h
|   |     |___Theme.h
|   |
|   |___Time\
|         |___Alarm.h
|         |___TimeManager.h
|
|___src\     
|   |___Audio\
|   |     |___AudioManager.cpp
|   |
|   |___Core\ 
|   |     |___App.cpp
|   |     |___ClockApp.cpp
|   |     |___main.cpp
|   |      
|   |___Graphics\
|   |     |___AnalogClock.cpp
|   |     |___DigitalClock.cpp
|   |     |___Renderer.cpp
|   |     |___Theme.cpp
|   |
|   |
|   |___Time\
|         |___Alarm.cpp
|         |___TimeManager.cpp
|
|
|
|___.gitignore
|___CMakeList.txt
|___LICENSE
|___README

```

# Instalations demandé : 
* Vérification ou instalation de  SDL3
* Utilisation de ImGui (Selection des fichiers nécéssaires dans l'élaboration du projet!)
* Utililisation et collaboration avec Cheat Sheet c++( Pour un rappels de notions, syntaxes et commandes sur le langage C++).