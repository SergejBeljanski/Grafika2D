# Akvarijum

Sergej Beljanski, RA 202/2021.

Otvori Kostur.sln, izaberi Debug | x64 ili Release | x64 i pokreni sa Ctrl+F5.
Gotova aplikacija je x64/Release/Kostur.exe. Pri pokretanju iz terminala postavi radni direktorijum na folder projekta ili folder u kome je exe.
Uz exe treba da budu glfw3.dll, glew32.dll, Resources i Shaders. Slike i sejderi se automatski kopiraju pri build-u.

Sav kod scene je u Source/Main.cpp, kao u primeru sa vezbi: globalne promenljive, callback, preprocessTexture, formVAO, setUniforms, processInput i pojedinacne draw funkcije. Svaki element u main() ima svoj vertices niz i svoj VAO/VBO, na primer verticesGoldfish, verticesSand i verticesDiamond. Svi glDrawArrays pozivi pocinju od temena 0. Source/Util.cpp sadrzi pomocne funkcije za ucitavanje sejdera, slika i kursora. Nema klase Aquarium ni njenih fajlova.

OpenGL 3.3 Core, GLSL 330, fullscreen, 75 FPS limiter i izlaz na Escape. Ribe se pomeraju pomocu uPos uniforme i ogledaju preko flipped uniforme. Skok krabe koristi vertikalni hitac. Providnost koristi alfa blendovanje. Akvarijum zauzima 79% visine, a njegove tri crne ivice imaju debljinu 10 piksela.

Kontrole (ne ispisuju se u aplikaciji):
- W A S D: zlatna ribica.
- Strelice: riba-klovn.
- Z / K: tri mehurica odgovarajuce ribe.
- 1 / 2: kraba ulevo / udesno.
- Space: skok krabe.
- C: otvaranje i zatvaranje kovcega.
- Escape: izlaz.

U kovcegu su dva novcica i jedan dijamant. Hrana, jedenje i rast nisu implementirani prema zahtevu; Enter nema funkciju.

Sve slike su lokalno u Resources i pojedinacno su ukljucene u Visual Studio projekat, pod Resource Files. Nista se ne preuzima pri pokretanju. Potpis je PNG tekstura sa imenom i indeksom, sa smanjenom neprovidnoscu. Poreklo ilustracija je opisano u TEXTURES.md.

Dimenzije i horizontalne brzine sada su fiksne, prilagodjene ekranu 16:9. Nema aspect promenljive ni mnozenja sa a; na drugom odnosu stranica slike mogu biti razvucene.
