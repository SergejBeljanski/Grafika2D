#version 330 core

in vec2 chTex;
in vec4 chCol;
out vec4 outCol;

uniform sampler2D uTex0;
uniform bool useTexture;
uniform float uB;
uniform float uA;

void main()
{
    if (!useTexture) {
        outCol = vec4(chCol.rg, chCol.b + uB, chCol.a + uA);
    } else {
        outCol = texture(uTex0, chTex);
        // Potpis je poluprovidan. Providna pozadina slike i dalje ostaje providna.
        outCol.a = outCol.a * (1.0 + uA);
    }
}
