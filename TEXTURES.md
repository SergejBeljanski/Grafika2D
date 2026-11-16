# Generisane teksture

Ilustracije su generisane u ovom zadatku pomocu ugradjenog alata **image_gen**, pojedinacno za svaku teksturu. Nijedna ilustracija nije preuzeta sa interneta. Transparentni PNG fajlovi cuvaju originalni alfa kanal; pesak je neprovidan. Generisani originali su kopirani u `Resources`. Velicina OS kursora prilagodjava se pri ucitavanju u C++ kodu.

Zajednicka specifikacija promptova: "Use case: stylized-concept. Asset type: single isolated 2D aquarium game sprite PNG. Hand-painted storybook game illustration, clean readable contours, polished charming art, soft highlights. Transparent background with 4 percent empty margin. No water backdrop, no ground, no text, no frame, no additional objects."

Pojedinacni motivi promptova:

- `goldfish.png`: "One beautiful friendly GOLD FISH, full body strict side view, swimming RIGHT, tail left, face right. Bright golden orange scales, delicate translucent flowing fins, dark expressive eye, crisp readable silhouette. Landscape 3:2 composition."
- `clownfish.png`: "One friendly clownfish, full body strict side view swimming RIGHT, tail left, face right. Orange body with three clean white stripes bordered black, expressive dark eye, small delicate fins. Landscape 3:2 composition."
- `crab.png`: "One friendly red-orange crab, front view, symmetrical two raised claws and eight splayed legs, two eyes on stalks, full body centered, wide silhouette. Landscape 3:2 composition."
- `seaweed.png`: "One rooted cluster of exactly three long gently curved seaweed blades, rich emerald green, underwater kelp with subtle golden light on edges. Upright tall narrow silhouette, fully visible from rooted base to tips. Portrait 2:3 composition."
- `bubble.png`: "One single round soap-like underwater air bubble, hollow clear transparent interior, fine pale cyan circular rim with tiny white highlights upper left, subtle pearlescence. Circle fills square composition, no drop shadow."
- `sand.png`: "A seamless edge-to-edge tile of fine warm golden aquarium sand, top view with delicate wavy ripples and small speckles, soft illustrated texture. Opaque full square, no horizon, no objects, no text."
- `chest_closed.png`: "One small CLOSED pirate treasure chest, dark walnut planks, warm antique gold metal bands and central latch, near front view with only a slight top perspective, broad horizontal rectangular base. Full object, isolated transparent background, 5 percent margin. No coins outside, no ground, no water."
- `chest_open.png`: "One small OPEN EMPTY pirate treasure chest, dark walnut planks and warm antique gold metal bands, lid raised behind the base, near front view with slight top perspective. Interior is dark and EMPTY so separate coin and gemstone textures can be drawn over it. Full object isolated transparent background, 5 percent margin. No treasure, no ground, no water."
- `coin.png`: "One single shiny round golden pirate coin, face-on slight tilt, embossed simple star, thick bevel edge, warm golden highlights. Isolated transparent background, square, no ground, no text."
- `gem.png`: "One single bright turquoise cut diamond gemstone, large flat crown and pointed base, clean geometric facets with cyan white reflections, vivid blue-green. Isolated transparent background, square, no ground, no text."
- `anchor.png`: "One nautical anchor CURSOR icon diagonally oriented from TOP LEFT to BOTTOM RIGHT. Small eyelet and tip at the TOP LEFT CORNER, straight diagonal shaft down and right toward bottom right, curved flukes extending to BOTTOM LEFT and TOP RIGHT. Brushed pale gold metal, strong dark navy outline, highly readable at 48 pixels. Fully isolated on transparent background, full square, no rope, no ground, no text."

`student.png` je generisan deterministicki skriptom `tools/Generate-Labels.ps1` pomocu System.Drawing i fonta Segoe UI, radi tacnog ispisa teksta i indeksa. U programu se prikazuju kao obicne OpenGL teksture.

