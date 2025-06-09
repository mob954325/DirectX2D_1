#include "pch.h"
#include "Input.h"

void Input::Update() {
    memcpy_s(prevState, sizeof(prevState), currState, sizeof(currState));
    for (int i = 0; i < 256; i++) {
        currState[i] = GetAsyncKeyState(i);
    }
}

bool Input::IsKeyDown(int vKey) {
    return (currState[vKey] & 0x8000) != 0;
}

bool Input::IsKeyPressed(int vKey) {
    return (!(prevState[vKey] & 0x8000) && (currState[vKey] & 0x8000));
}

bool Input::IsKeyReleased(int vKey) {
    return ((prevState[vKey] & 0x8000) && !(currState[vKey] & 0x8000));
}