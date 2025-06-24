#include "frame.h"

Frame createFrame(SDL_Renderer *r, SDL_Color bg, SDL_Color borderColor, int radius)
{
    Frame frame = {
        .renderer = r,
        .bgColor = bg,
        .borderColor = borderColor,
        .borderRadius = radius};

    return frame;
}

void renderFrame(Frame *frame)
{
    int x1 = frame->rect.x;
    int y1 = frame->rect.y;

    int x2 = frame->rect.x + frame->rect.w;
    int y2 = frame->rect.y + frame->rect.h;

    roundedBoxRGBA(
        frame->renderer,
        x1, y1,
        x2, y2,
        frame->borderRadius,
        frame->bgColor.r, frame->bgColor.g, frame->bgColor.b, frame->bgColor.a);

    roundedRectangleRGBA(
        frame->renderer,
        x1, y1,
        x2, y2,
        frame->borderRadius,
        frame->borderColor.r, frame->borderColor.g, frame->borderColor.b, frame->borderColor.a);
}