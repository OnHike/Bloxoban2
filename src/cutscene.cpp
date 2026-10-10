#include "cutscene.h"
#include "common.h"
#include "rendering.h"

static float GetFrameDuration(const Cutscene* scene)
{
    int i = scene->frame < scene->frameDurationCount ? scene->frame : scene->frameDurationCount - 1;
    return scene->frameDurations[i];
}

void StartCutscene(Cutscene* scene, Sprite* sprite, const float* frameDurations, int frameDurationCount)
{
    scene->sprite = sprite;
    scene->frame = 0;
    scene->frameDurations = frameDurations;
    scene->frameDurationCount = frameDurationCount;
    scene->timer = 0;
    scene->active = sprite != nullptr && GetSpriteCount(sprite) > 0 && frameDurations != nullptr && frameDurationCount > 0;
}

bool UpdateCutscene(Cutscene* scene, float dt)
{
    if (!scene->active)
    {
        return false;
    }

    scene->timer += dt;
    while (scene->timer >= GetFrameDuration(scene))
    {
        scene->timer -= GetFrameDuration(scene);
        scene->frame++;
        if (scene->frame >= GetSpriteCount(scene->sprite))
        {
            scene->active = false;
            return false;
        }
    }
    return true;
}

void DrawCutscene(
    const Cutscene* scene,
    SDL_Renderer* renderer,
    Sprite* blackSprite)
{
    if (!scene->active)
    {
        return;
    }

    RenderSprite_World(
        blackSprite,
        renderer,
        nullptr,
        0,
        0,
        SCREEN_WIDTH,
        0.5f);

    int cellW = scene->sprite->width / scene->sprite->sprite_count_x;
    int cellH = scene->sprite->height / scene->sprite->sprite_count_y;
    if (GetSpriteCount(scene->sprite) <= 1)
    {
        cellW = scene->sprite->width;
        cellH = scene->sprite->height;
    }

    // Fit the frame to the screen, centred (the sprite pivot is applied by the renderer).
    float scale = SCREEN_WIDTH / (float)(cellW * UPSCALE_FACTOR);
    float scaleY = SCREEN_HEIGHT / (float)(cellH * UPSCALE_FACTOR);
    if (scaleY < scale) scale = scaleY;
    float x = (SCREEN_WIDTH - cellW * UPSCALE_FACTOR * scale) / 2.0f + scene->sprite->pivot_x * UPSCALE_FACTOR * scale;
    float y = (SCREEN_HEIGHT - cellH * UPSCALE_FACTOR * scale) / 2.0f + scene->sprite->pivot_y * UPSCALE_FACTOR * scale;

    RenderSprite_World(
        SpriteRenderInfo(scene->frame, scene->sprite),
        renderer,
        nullptr,
        x,
        y,
        scale);
}
