#include "input/touch.h"

XPT2046_Touchscreen ts(CS_PIN);
void touch_setup()
{
    ts.begin();
    ts.setRotation(1);
}

void touch_update()
{
    if (ts.touched())
    {
        neotrellisPressed[TRELLIS_BUTTON_ENTER] = true;
        TS_Point p = ts.getPoint();
        pixelTouchX = map(p.x, TS_MINX, TS_MAXX, 0, 480-STEP_FRAME_W);
        if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER)
            gridTouchY = map(p.y, TS_MINY, TS_MAXY, 0, 8);
        else
            gridTouchY = map(p.y, TS_MINY, TS_MAXY, 0, 14);
    }
}
