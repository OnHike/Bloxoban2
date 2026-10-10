#pragma once
#include "SDL3/SDL_render.h"
#include "spriteLibrary.h"

struct Cutscene
{
    bool active;
    Sprite* sprite;          // Spritesheet; every cell is one frame.
    int frame;
    const float* frameDurations; // Seconds each frame is shown, indexed by frame.
    int frameDurationCount;
    float timer;
};

// Starts playing every frame of `sprite`. `frameDurations` must outlive the cutscene
// (use a static array); if it has fewer entries than frames, the last one is reused.
void StartCutscene(Cutscene* scene, Sprite* sprite, const float* frameDurations, int frameDurationCount);

// Advances the cutscene. Returns true while it is still playing.
bool UpdateCutscene(Cutscene* scene, float dt);

// Draws a dimmed overlay plus the current frame on top of whatever is already rendered.
void DrawCutscene(
    const Cutscene* scene,
    SDL_Renderer* renderer,
    Sprite* blackSprite);