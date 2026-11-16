#include "../Header/Util.h"

#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "../Header/stb_image.h"

using namespace std;

// Autor: Nedeljko Tesanovic
// Opis: pomocne funkcije za ucitavanje sejdera i tekstura

unsigned int compileShader(GLenum type, const char* source)
{
    ifstream file(source, ios::binary);
    if (!file.is_open())
    {
        cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << endl;
        return 0;
    }

    stringstream ss;
    ss << file.rdbuf();
    string content = ss.str();
    cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << endl;

    // UTF-8 BOM nije deo GLSL koda i mora se ukloniti pre kompajliranja.
    if (content.compare(0, 3, "\xEF\xBB\xBF") == 0)
        content.erase(0, 3);
    const char* sourceCode = content.c_str();

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    int success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        char infoLog[512] = {};
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        const char* shaderType = type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";
        cout << shaderType << " sejder ima gresku (" << source << ")! Greska:\n"
                  << infoLog << endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

unsigned int createShader(const char* vsSource, const char* fsSource)
{
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    if (vertexShader == 0)
        return 0;

    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);
    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return 0;
    }

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Uspeh povezivanja programa proverava se preko GL_LINK_STATUS.
    int success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        char infoLog[512] = {};
        glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
        cout << "Objedinjeni sejder ima gresku! Greska:\n"
                  << infoLog << endl;
    }

    // Pojedinacni sejderi vise nisu potrebni nakon povezivanja.
    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    if (success == GL_FALSE)
    {
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

unsigned loadImageToTexture(const char* filePath) {
    int width, height, channels;
    // Učitavamo i alfa kanal, da se oko ribica i ostalih slika ne vidi pravougaona pozadina.
    unsigned char* pixels = stbi_load(filePath, &width, &height, &channels, STBI_rgb_alpha);
    if (!pixels) { cerr << "Tekstura nije ucitana: " << filePath << '\n'; return 0; }
    const int row = width * 4;
    for (int y = 0; y < height / 2; ++y)
        for (int x = 0; x < row; ++x)
            swap(pixels[y * row + x], pixels[(height - 1 - y) * row + x]);
    unsigned texture;
    glGenTextures(1, &texture); glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0); stbi_image_free(pixels);
    return texture;
}
GLFWcursor* loadImageToCursor(const char* filePath) {
    int width, height, channels;
    unsigned char* pixels = stbi_load(filePath, &width, &height, &channels, STBI_rgb_alpha);
    if (!pixels) { cerr << "Kursor nije ucitan: " << filePath << '\n'; return nullptr; }
    // Za kursor nam treba mala slika od 48x48 piksela. Ovde sliku ne obrćemo kao OpenGL teksture.
    constexpr int size = 48;
    unsigned char scaled[size * size * 4]{};
    for (int y = 0; y < size; ++y) for (int x = 0; x < size; ++x) {
        // Više piksela originalne slike svedemo na jedan. Alfa se uzima u obzir da rub ne potamni.
        int x0 = x * width / size, x1 = (x + 1) * width / size;
        int y0 = y * height / size, y1 = (y + 1) * height / size;
        unsigned sumA = 0, sumR = 0, sumG = 0, sumB = 0, count = 0;
        for (int sy = y0; sy < y1; ++sy) for (int sx = x0; sx < x1; ++sx) {
            const unsigned char* p = pixels + (sy * width + sx) * 4;
            sumA += p[3]; sumR += p[0] * p[3]; sumG += p[1] * p[3]; sumB += p[2] * p[3]; ++count;
        }
        unsigned char* out = scaled + (y * size + x) * 4;
        if (sumA) { out[0] = sumR / sumA; out[1] = sumG / sumA; out[2] = sumB / sumA; }
        out[3] = count ? sumA / count : 0;
    }
    GLFWimage image{size, size, scaled};
    // Vrh sidra i aktivna tacka su u gornjem levom uglu.
    GLFWcursor* cursor = glfwCreateCursor(&image, 1, 1);
    stbi_image_free(pixels);
    return cursor;
}


