#pragma once

struct Input {
    bool down    [256] = {};
	bool pressed [256] = {};
	bool released[256] = {};

    bool mouseDown    [3] = {};
    bool mousePressed [3] = {};
    bool mouseReleased[3] = {};

    float mouseDX = 0;
    float mouseDY = 0;
    float mouseScrollDelta = 0;
    int lastMouseX = 0, lastMouseY = 0;

    void Flush() { mouseDX = 0; mouseDY = 0; mouseScrollDelta = 0; }
    void Update() {
        for (int i = 0; i < 256; i++) {
            pressed[i]  = (!prev[i] &&  down[i]);
            released[i] = ( prev[i] && !down[i]);
            prev[i] = down[i];
        }
        for (int i = 0; i < 3; i++) {
            mousePressed[i] = (!mousePrev[i] && mouseDown[i]);
            mouseReleased[i] = (mousePrev[i] && !mouseDown[i]);
            mousePrev[i] = mouseDown[i];
        }
    }
private:
    bool prev[256] = {};
    bool mousePrev[3] = {};
};