# Horloge_Analogique-Dinamique
Réaliser une horloge offrant à  la fois des services d'horloge analogique et digital; donnant la possibilité à l'utilisateur de faire des navettes entre différents thèmes . Cette horloge offre aussi un service d'alarme!

## En quoi ça consiste réellement?

Ce projet ( Horloge) a pour but de permettre aux utulisateurs de pour se familliariser avec une app horlographique , comme tout autre app particulière. 

En effet, elle permettra de :
* Naviguer entre diffenrents thèmes (personnaliser son fond d'app à sa préférence) et rendre l'explérience plus alaichante...
* D'éviter de faire un choix entre analogique et digital ( en donnant la possibilité d'utiliser les deux) plus besoin de faire un choix...
* D'utiliser une alarme pour des rappels d'évennements...

Vous vous direz que *C'est pas très innovateur* et *très courant* mais *pour nous l'est!*

# ♦Structuration ou Arborescence

```
Horloge_SDL3/
├── main.cpp              
├── Game.h
├── Game.cpp              
├── Renderer.h
├── Renderer.cpp          
├── UI.h
├── UI.cpp                
├── TimeManager.h
├── TimeManager.cpp       
├── Alarm.h
├── Alarm.cpp            
├── assets/
│   ├── fonts/
│   └── sounds/
|
│── imgui/
│       ├── imgui.cpp
│       ├── imgui_draw.cpp
│       ├── imgui_tables.cpp
│       ├── imgui_widgets.cpp
│       ├── imgui.h
│       ├── imconfig.h
│       └── backends/
│           ├── imgui_impl_sdl3.cpp
│           ├── imgui_impl_sdl3.h
│           ├── imgui_impl_sdlrenderer3.cpp
│           └── imgui_impl_sdlrenderer3.h
           
├── CMakeLists.txt
└── README.md


```

# Instalations demandé : 
* Vérification ou instalation de  SDL3
* Utilisation de ImGui (Selection des fichiers nécéssaires dans l'élaboration du projet!)
* Utililisation et collaboration avec Cheat Sheet c++( Pour un rappels de notions, syntaxes et commandes sur le langage C++).
