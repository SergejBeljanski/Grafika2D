#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec4 inCol;
layout(location = 2) in vec2 inTex;
out vec2 chTex;
out vec4 chCol;

uniform vec2 uPos;
uniform float uS;
uniform bool flipped;

void main()
{
    gl_Position = vec4(inPos.x + uPos.x, inPos.y * uS + uPos.y, 0.0, 1.0);
    chCol = inCol;
    if (flipped) {
        // Menjanjem s koordinate invertujemo slike.
        chTex = vec2(1.0 - inTex.s, inTex.t);
    } else {
        chTex = inTex;
    }
}
