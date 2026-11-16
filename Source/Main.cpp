#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <iostream>
#include <vector>
#include "../Header/Util.h"

using namespace std;

float goldX = -0.42f, goldY = 0.09f;
float clownX = 0.35f, clownY = -0.20f;
float crabX = -0.22f, crabY = -0.71f;
bool goldFlipped = false;
bool clownFlipped = false;
bool chestOpen = false;

int screenWidth = 800;
int screenHeight = 800;
float waterTop = 0.58f;
float sandTop = -0.76f;
double initialJumpTime = -1.0;

unsigned goldTexture, clownTexture, crabTexture, bubbleTexture;
unsigned sandTexture, grassTexture, closedChestTexture, openChestTexture;
unsigned coinTexture, diamondTexture, studentTexture;
GLFWcursor* cursor;

struct Bubble {
    float x;
    float y;
};
vector<Bubble> bubbles;

int endProgram(string message) {
    cout << message << endl;
    glfwTerminate();
    return -1;
}

void addBubbles(float x, float y, bool flipped, float fishHalfWidth) {
    // Mehuri izlaze sa strane na koju ribica gleda
    float direction = flipped ? -1.0f : 1.0f;
    for (int i = 0; i < 3; ++i) {
        Bubble bubble;
        bubble.x = x + direction * (fishHalfWidth * 0.8f + i * 0.0140625f);
        bubble.y = y + i * 0.045f;
        bubble.x = max(-1.0f + 20.0f / screenWidth + 0.01125f, bubble.x);
        bubble.x = min(1.0f - 20.0f / screenWidth - 0.01125f, bubble.x);
        bubbles.push_back(bubble);
    }
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_C) chestOpen = !chestOpen;
        if (key == GLFW_KEY_Z) addBubbles(goldX, goldY, goldFlipped, 0.135f);
        if (key == GLFW_KEY_K) addBubbles(clownX, clownY, clownFlipped, 0.1153125f);

        // Negativno vreme znači da kraba nije u skoku
        if (key == GLFW_KEY_SPACE && initialJumpTime < 0)
            initialJumpTime = glfwGetTime();

        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void preprocessTexture(unsigned& texture, const char* filepath) {
    texture = loadImageToTexture(filepath);
    glBindTexture(GL_TEXTURE_2D, texture);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void formVAO(float* vertices, size_t size, unsigned int& VAO, unsigned int& VBO) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void setUniforms(unsigned int shader, float uX, float uY, float uS,
                 bool flipped, bool useTexture, float uB, float uA) {
    glUniform2f(glGetUniformLocation(shader, "uPos"), uX, uY);
    glUniform1f(glGetUniformLocation(shader, "uS"), uS);
    glUniform1i(glGetUniformLocation(shader, "flipped"), flipped);
    glUniform1i(glGetUniformLocation(shader, "useTexture"), useTexture);
    glUniform1f(glGetUniformLocation(shader, "uB"), uB);
    glUniform1f(glGetUniformLocation(shader, "uA"), uA);
}

void processInput(GLFWwindow* window, float dt) {
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        goldX -= 0.2925f * dt;
        goldFlipped = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        goldX += 0.2925f * dt;
        goldFlipped = false;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) goldY += 0.52f * dt;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) goldY -= 0.52f * dt;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        clownX -= 0.2925f * dt;
        clownFlipped = true;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        clownX += 0.2925f * dt;
        clownFlipped = false;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) clownY += 0.52f * dt;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) clownY -= 0.52f * dt;

    // Granice računamo sa polovinom širine i visine
    float border = 20.0f / screenWidth;
    goldX = max(-1 + border + 0.135f, min(goldX, 1 - border - 0.135f));
    goldY = max(sandTop + 0.16f, min(goldY, waterTop - 0.16f));
    clownX = max(-1 + border + 0.1153125f, min(clownX, 1 - border - 0.1153125f));
    clownY = max(sandTop + 0.137f, min(clownY, waterTop - 0.137f));

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) crabX -= 0.264375f * dt;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) crabX += 0.264375f * dt;
    crabX = max(-1 + border + 0.1125f, min(crabX, 1 - border - 0.1125f));

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void drawWater(unsigned int shader, unsigned int VAOWater) {
    glUseProgram(shader);
    setUniforms(shader, 0, 0, 1, false, false, 0, 0);
    glBindVertexArray(VAOWater);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawSand(unsigned int shader, unsigned int VAOSand) {
    glUseProgram(shader);
    setUniforms(shader, 0, 0, 1, false, true, 0, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sandTexture);
    glBindVertexArray(VAOSand);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawGrass(unsigned int shader, unsigned int VAOGrassLeft, unsigned int VAOGrassRight) {
    glUseProgram(shader);
    setUniforms(shader, 0, 0, 1, false, true, 0, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    glBindVertexArray(VAOGrassLeft);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    setUniforms(shader, 0, 0, 1, true, true, 0, 0);
    glBindVertexArray(VAOGrassRight);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawChest(unsigned int shader, unsigned int VAOClosedChest, unsigned int VAOOpenChest, unsigned int VAOCoinLeft, unsigned int VAOCoinRight, unsigned int VAODiamond) {
    glUseProgram(shader);
    setUniforms(shader, 0, 0, 1, false, true, 0, 0);
    glActiveTexture(GL_TEXTURE0);
    if (chestOpen) {
        glBindTexture(GL_TEXTURE_2D, openChestTexture);
        glBindVertexArray(VAOOpenChest);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindTexture(GL_TEXTURE_2D, coinTexture);
        glBindVertexArray(VAOCoinLeft);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(VAOCoinRight);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindTexture(GL_TEXTURE_2D, diamondTexture);
        glBindVertexArray(VAODiamond);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    } else {
        glBindTexture(GL_TEXTURE_2D, closedChestTexture);
        glBindVertexArray(VAOClosedChest);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}

void drawGoldfish(unsigned int shader, unsigned int VAOGoldfish) {
    glUseProgram(shader);
    setUniforms(shader, goldX, goldY, 1, goldFlipped, true, 0, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, goldTexture);
    glBindVertexArray(VAOGoldfish);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawClownfish(unsigned int shader, unsigned int VAOClownfish) {
    glUseProgram(shader);
    setUniforms(shader, clownX, clownY, 1, clownFlipped, true, 0, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, clownTexture);
    glBindVertexArray(VAOClownfish);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawCrab(unsigned int shader, unsigned int VAOCrab) {
    glUseProgram(shader);
    if (initialJumpTime >= 0) {
        float t = (float)(glfwGetTime() - initialJumpTime);
        float v0 = 1.2f;
        float g = 2.0f;
        float s = v0 * t - g * t * t / 2;
        crabY = -0.71f + max(s, 0.0f);
        if (s <= 0 && t > 0) initialJumpTime = -1;
    }
    // Ovde je flipped uvek false, jer kraba ne treba da se okrece pri promeni smera.
    setUniforms(shader, crabX, crabY, 1, false, true, 0, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, crabTexture);
    glBindVertexArray(VAOCrab);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawBubbles(unsigned int shader, unsigned int VAOBubble, float dt) {
    glUseProgram(shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bubbleTexture);

    // Odsecamo deo slike koji je prešao povrsinu, da se mehuri ne vide iznad vode.
    glEnable(GL_SCISSOR_TEST);
    glScissor(0, 0, screenWidth, (int)((waterTop + 1) * screenHeight / 2));
    for (int i = 0; i < (int)bubbles.size(); ++i) {
        bubbles[i].y += 0.30f * dt;
        if (bubbles[i].y - 0.02f >= waterTop) {
            bubbles.erase(bubbles.begin() + i);
            --i; // Posle brisanja sledeci mehur dolazi na isto mesto u vektoru.
        } else {
            setUniforms(shader, bubbles[i].x, bubbles[i].y, 1, false, true, 0, 0);
            glBindVertexArray(VAOBubble);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }
    }
    glDisable(GL_SCISSOR_TEST);
}

void drawGlass(unsigned int shader, unsigned int VAOGlass) {
    glUseProgram(shader);
    setUniforms(shader, 0, 0, 1, false, false, 0, 0);
    glBindVertexArray(VAOGlass);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawLeftBorder(unsigned int shader, unsigned int VAOLeftBorder) {
    glUseProgram(shader);
    setUniforms(shader, 0, 0, 1, false, false, 0, 0);
    glBindVertexArray(VAOLeftBorder);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawRightBorder(unsigned int shader, unsigned int VAORightBorder) {
    glUseProgram(shader);
    setUniforms(shader, 0, 0, 1, false, false, 0, 0);
    glBindVertexArray(VAORightBorder);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawBottomBorder(unsigned int shader, unsigned int VAOBottomBorder) {
    glUseProgram(shader);
    setUniforms(shader, 0, 0, 1, false, false, 0, 0);
    glBindVertexArray(VAOBottomBorder);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawStudent(unsigned int shader, unsigned int VAOStudent) {
    glUseProgram(shader);
    setUniforms(shader, 0, 0, 1, false, true, 0, -0.22f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, studentTexture);
    glBindVertexArray(VAOStudent);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

int main() {
    if (!glfwInit()) return endProgram("GLFW nije uspeo da se inicijalizuje.");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = monitor ? glfwGetVideoMode(monitor) : NULL;
    if (mode == NULL) return endProgram("Monitor nije dostupan.");
    screenWidth = mode->width;
    screenHeight = mode->height;
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Akvarijum", monitor, NULL);
    if (window == NULL) return endProgram("Prozor nije uspeo da se kreira.");
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetKeyCallback(window, keyboard_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwDestroyWindow(window);
        return endProgram("GLEW nije uspeo da se inicijalizuje.");
    }
    while (glGetError() != GL_NO_ERROR) {}

    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cursor = loadImageToCursor("Resources/anchor.png");
    if (cursor == NULL) {
        glfwDestroyWindow(window);
        return endProgram("Kursor nije ucitan.");
    }
    glfwSetCursor(window, cursor);

    preprocessTexture(goldTexture, "Resources/goldfish.png");
    preprocessTexture(clownTexture, "Resources/clownfish.png");
    preprocessTexture(crabTexture, "Resources/crab.png");
    preprocessTexture(bubbleTexture, "Resources/bubble.png");
    preprocessTexture(sandTexture, "Resources/sand.png");
    preprocessTexture(grassTexture, "Resources/seaweed.png");
    preprocessTexture(closedChestTexture, "Resources/chest_closed.png");
    preprocessTexture(openChestTexture, "Resources/chest_open.png");
    preprocessTexture(coinTexture, "Resources/coin.png");
    preprocessTexture(diamondTexture, "Resources/gem.png");
    preprocessTexture(studentTexture, "Resources/student.png");
    unsigned textures[] = {goldTexture, clownTexture, crabTexture, bubbleTexture, sandTexture,
        grassTexture, closedChestTexture, openChestTexture, coinTexture, diamondTexture, studentTexture};
    for (int i = 0; i < 11; ++i) {
        if (textures[i] == 0) {
            glDeleteTextures(11, textures);
            glfwDestroyCursor(cursor);
            glfwDestroyWindow(window);
            return endProgram("Neka tekstura nije ucitana. Proverite Resources folder.");
        }
    }

    unsigned int basicShader = createShader("Shaders/basic.vert", "Shaders/basic.frag");
    if (basicShader == 0) {
        glDeleteTextures(11, textures);
        glfwDestroyCursor(cursor);
        glfwDestroyWindow(window);
        return endProgram("Sejder program nije uspesno kreiran.");
    }
    glUseProgram(basicShader);
    glUniform1i(glGetUniformLocation(basicShader, "uTex0"), 0);

    float borderX = 20.0f / screenWidth;
    float borderY = 20.0f / screenHeight;
    float verticesWater[] = {
        -1.0f, 0.58f,    0.04f, 0.41f, 0.51f, 1,    0, 1,
        -1.0f, -1.0f,    0.015f, 0.16f, 0.23f, 1,    0, 0,
        1.0f, -1.0f,    0.015f, 0.16f, 0.23f, 1,    1, 0,
        1.0f, 0.58f,    0.04f, 0.41f, 0.51f, 1,    1, 1,
    };

    float verticesGlass[] = {
        -1.0f, 0.58f,    1, 1, 1, 0.2f,    0, 1,
        -1.0f, -1.0f,    1, 1, 1, 0.2f,    0, 0,
        1.0f, -1.0f,    1, 1, 1, 0.2f,    1, 0,
        1.0f, 0.58f,    1, 1, 1, 0.2f,    1, 1,
    };

    float verticesLeftBorder[] = {
        -1.0f, 0.58f,    0, 0, 0, 1,    0, 1,
        -1.0f, -1.0f,    0, 0, 0, 1,    0, 0,
        -1.0f+borderX, -1.0f,    0, 0, 0, 1,    1, 0,
        -1.0f+borderX, 0.58f,    0, 0, 0, 1,    1, 1,
    };

    float verticesRightBorder[] = {
        1.0f-borderX, 0.58f,    0, 0, 0, 1,    0, 1,
        1.0f-borderX, -1.0f,    0, 0, 0, 1,    0, 0,
        1.0f, -1.0f,    0, 0, 0, 1,    1, 0,
        1.0f, 0.58f,    0, 0, 0, 1,    1, 1,
    };

    float verticesBottomBorder[] = {
        -1.0f, -1.0f+borderY,    0, 0, 0, 1,    0, 1,
        -1.0f, -1.0f,    0, 0, 0, 1,    0, 0,
        1.0f, -1.0f,    0, 0, 0, 1,    1, 0,
        1.0f, -1.0f+borderY,    0, 0, 0, 1,    1, 1,
    };

    float verticesSand[] = {
        -1.0f, -0.76f,    1, 1, 1, 1,    0, 1,
        -1.0f, -1.0f,    1, 1, 1, 1,    0, 0,
        1.0f, -1.0f,    1, 1, 1, 1,    8, 0,
        1.0f, -0.76f,    1, 1, 1, 1,    8, 1,
    };

    float verticesGrassLeft[] = {
        -0.826875f, -0.075f,    1, 1, 1, 1,    0, 1,
        -0.826875f, -0.83f,    1, 1, 1, 1,    0, 0,
        -0.613125f, -0.83f,    1, 1, 1, 1,    1, 0,
        -0.613125f, -0.075f,    1, 1, 1, 1,    1, 1,
    };

    float verticesGrassRight[] = {
        0.6328125f, -0.22f,    1, 1, 1, 1,    0, 1,
        0.6328125f, -0.83f,    1, 1, 1, 1,    0, 0,
        0.8071875f, -0.83f,    1, 1, 1, 1,    1, 0,
        0.8071875f, -0.22f,    1, 1, 1, 1,    1, 1,
    };

    float verticesClosedChest[] = {
        0.194625f, -0.498f,    1, 1, 1, 1,    0, 1,
        0.194625f, -0.84f,    1, 1, 1, 1,    0, 0,
        0.525375f, -0.84f,    1, 1, 1, 1,    1, 0,
        0.525375f, -0.498f,    1, 1, 1, 1,    1, 1,
    };

    float verticesOpenChest[] = {
        0.194625f, -0.218f,    1, 1, 1, 1,    0, 1,
        0.194625f, -0.84f,    1, 1, 1, 1,    0, 0,
        0.525375f, -0.84f,    1, 1, 1, 1,    1, 0,
        0.525375f, -0.218f,    1, 1, 1, 1,    1, 1,
    };

    float verticesCoinLeft[] = {
        0.2784375f, -0.54f,    1, 1, 1, 1,    0, 1,
        0.2784375f, -0.63f,    1, 1, 1, 1,    0, 0,
        0.3290625f, -0.63f,    1, 1, 1, 1,    1, 0,
        0.3290625f, -0.54f,    1, 1, 1, 1,    1, 1,
    };

    float verticesCoinRight[] = {
        0.3909375f, -0.54f,    1, 1, 1, 1,    0, 1,
        0.3909375f, -0.63f,    1, 1, 1, 1,    0, 0,
        0.4415625f, -0.63f,    1, 1, 1, 1,    1, 0,
        0.4415625f, -0.54f,    1, 1, 1, 1,    1, 1,
    };

    float verticesDiamond[] = {
        0.325125f, -0.486f,    1, 1, 1, 1,    0, 1,
        0.325125f, -0.61f,    1, 1, 1, 1,    0, 0,
        0.394875f, -0.61f,    1, 1, 1, 1,    1, 0,
        0.394875f, -0.486f,    1, 1, 1, 1,    1, 1,
    };

    float verticesGoldfish[] = {
        -0.135f, 0.16f,    1, 1, 1, 1,    0, 1,
        -0.135f, -0.16f,    1, 1, 1, 1,    0, 0,
        0.135f, -0.16f,    1, 1, 1, 1,    1, 0,
        0.135f, 0.16f,    1, 1, 1, 1,    1, 1,
    };

    float verticesClownfish[] = {
        -0.1153125f, 0.137f,    1, 1, 1, 1,    0, 1,
        -0.1153125f, -0.137f,    1, 1, 1, 1,    0, 0,
        0.1153125f, -0.137f,    1, 1, 1, 1,    1, 0,
        0.1153125f, 0.137f,    1, 1, 1, 1,    1, 1,
    };

    float verticesBubble[] = {
        -0.01125f, 0.02f,    1, 1, 1, 1,    0, 1,
        -0.01125f, -0.02f,    1, 1, 1, 1,    0, 0,
        0.01125f, -0.02f,    1, 1, 1, 1,    1, 0,
        0.01125f, 0.02f,    1, 1, 1, 1,    1, 1,
    };

    float verticesCrab[] = {
        -0.1125f, 0.133f,    1, 1, 1, 1,    0, 1,
        -0.1125f, -0.133f,    1, 1, 1, 1,    0, 0,
        0.1125f, -0.133f,    1, 1, 1, 1,    1, 0,
        0.1125f, 0.133f,    1, 1, 1, 1,    1, 1,
    };

    float verticesStudent[] = {
        0.34125f, 0.95f,    1, 1, 1, 1,    0, 1,
        0.34125f, 0.78f,    1, 1, 1, 1,    0, 0,
        0.96f, 0.78f,    1, 1, 1, 1,    1, 0,
        0.96f, 0.95f,    1, 1, 1, 1,    1, 1,
    };

    // Za svaki niz pravimo njegov VAO i VBO
    unsigned int VAOWater, VBOWater;
    formVAO(verticesWater, sizeof(verticesWater), VAOWater, VBOWater);
    unsigned int VAOGlass, VBOGlass;
    formVAO(verticesGlass, sizeof(verticesGlass), VAOGlass, VBOGlass);
    unsigned int VAOLeftBorder, VBOLeftBorder;
    formVAO(verticesLeftBorder, sizeof(verticesLeftBorder), VAOLeftBorder, VBOLeftBorder);
    unsigned int VAORightBorder, VBORightBorder;
    formVAO(verticesRightBorder, sizeof(verticesRightBorder), VAORightBorder, VBORightBorder);
    unsigned int VAOBottomBorder, VBOBottomBorder;
    formVAO(verticesBottomBorder, sizeof(verticesBottomBorder), VAOBottomBorder, VBOBottomBorder);
    unsigned int VAOSand, VBOSand;
    formVAO(verticesSand, sizeof(verticesSand), VAOSand, VBOSand);
    unsigned int VAOGrassLeft, VBOGrassLeft;
    formVAO(verticesGrassLeft, sizeof(verticesGrassLeft), VAOGrassLeft, VBOGrassLeft);
    unsigned int VAOGrassRight, VBOGrassRight;
    formVAO(verticesGrassRight, sizeof(verticesGrassRight), VAOGrassRight, VBOGrassRight);
    unsigned int VAOClosedChest, VBOClosedChest;
    formVAO(verticesClosedChest, sizeof(verticesClosedChest), VAOClosedChest, VBOClosedChest);
    unsigned int VAOOpenChest, VBOOpenChest;
    formVAO(verticesOpenChest, sizeof(verticesOpenChest), VAOOpenChest, VBOOpenChest);
    unsigned int VAOCoinLeft, VBOCoinLeft;
    formVAO(verticesCoinLeft, sizeof(verticesCoinLeft), VAOCoinLeft, VBOCoinLeft);
    unsigned int VAOCoinRight, VBOCoinRight;
    formVAO(verticesCoinRight, sizeof(verticesCoinRight), VAOCoinRight, VBOCoinRight);
    unsigned int VAODiamond, VBODiamond;
    formVAO(verticesDiamond, sizeof(verticesDiamond), VAODiamond, VBODiamond);
    unsigned int VAOGoldfish, VBOGoldfish;
    formVAO(verticesGoldfish, sizeof(verticesGoldfish), VAOGoldfish, VBOGoldfish);
    unsigned int VAOClownfish, VBOClownfish;
    formVAO(verticesClownfish, sizeof(verticesClownfish), VAOClownfish, VBOClownfish);
    unsigned int VAOBubble, VBOBubble;
    formVAO(verticesBubble, sizeof(verticesBubble), VAOBubble, VBOBubble);
    unsigned int VAOCrab, VBOCrab;
    formVAO(verticesCrab, sizeof(verticesCrab), VAOCrab, VBOCrab);
    unsigned int VAOStudent, VBOStudent;
    formVAO(verticesStudent, sizeof(verticesStudent), VAOStudent, VBOStudent);
    glClearColor(0.025f, 0.075f, 0.10f, 1.0f);

    double previousTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double initFrameTime = glfwGetTime();
        float dt = (float)(initFrameTime - previousTime);
        previousTime = initFrameTime;
        dt = min(dt, 0.05f);
        glfwPollEvents();
        if (glfwWindowShouldClose(window)) break;
        processInput(window, dt);
        glClear(GL_COLOR_BUFFER_BIT);

        // Redosled je bitan
        drawWater(basicShader, VAOWater);
        drawSand(basicShader, VAOSand);
        drawGrass(basicShader, VAOGrassLeft, VAOGrassRight);
        drawChest(basicShader, VAOClosedChest, VAOOpenChest, VAOCoinLeft, VAOCoinRight, VAODiamond);
        drawGoldfish(basicShader, VAOGoldfish);
        drawClownfish(basicShader, VAOClownfish);
        drawCrab(basicShader, VAOCrab);
        drawBubbles(basicShader, VAOBubble, dt);
        drawGlass(basicShader, VAOGlass);
        // Ivice crtamo posebno preko scene
        drawLeftBorder(basicShader, VAOLeftBorder);
        drawRightBorder(basicShader, VAORightBorder);
        drawBottomBorder(basicShader, VAOBottomBorder);
        drawStudent(basicShader, VAOStudent);
        glfwSwapBuffers(window);

        // Limiter
        while (glfwGetTime() - initFrameTime < 1.0 / 75.0) {}
    }

    glDeleteTextures(11, textures);
    unsigned int buffers[] = {VBOWater, VBOGlass, VBOLeftBorder, VBORightBorder, VBOBottomBorder, VBOSand, VBOGrassLeft, VBOGrassRight, VBOClosedChest, VBOOpenChest, VBOCoinLeft, VBOCoinRight, VBODiamond, VBOGoldfish, VBOClownfish, VBOBubble, VBOCrab, VBOStudent};
    unsigned int vertexArrays[] = {VAOWater, VAOGlass, VAOLeftBorder, VAORightBorder, VAOBottomBorder, VAOSand, VAOGrassLeft, VAOGrassRight, VAOClosedChest, VAOOpenChest, VAOCoinLeft, VAOCoinRight, VAODiamond, VAOGoldfish, VAOClownfish, VAOBubble, VAOCrab, VAOStudent};
    glDeleteBuffers(18, buffers);
    glDeleteVertexArrays(18, vertexArrays);
    glDeleteProgram(basicShader);
    glfwDestroyCursor(cursor);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}








