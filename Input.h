#pragma once

struct Input {
    bool keys[256] = {};
    bool mouseButtons[3] = {};
    float mouseDX = 0;
    float mouseDY = 0;
    float mouseScrollDelta = 0;
    int lastMouseX = 0, lastMouseY = 0;

    void Flush() { mouseDX = 0; mouseDY = 0; mouseScrollDelta = 0; }
};