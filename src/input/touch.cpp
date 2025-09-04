#include "input/touch.h"

XPT2046_Touchscreen ts(CS_PIN, 15);

bool tsTouched = false;

void touch_setup()
{
    ts.begin();
    ts.setRotation(1);
}

void touch_update()
{
    if (ts.touched())
    {
        //
        TS_Point p = ts.getPoint();
        int pX = map(p.x, TS_MINX, TS_MAXX, 0, 480 - STEP_FRAME_W / 2);
        if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER || activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
        {
            if (pX < SEQ_GRID_RIGHT)
            {
                tsTouched = true;
                pixelTouchX = map(p.x, TS_MINX, TS_MAXX, 0, 480 - STEP_FRAME_W / 2);
                if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER)
                    gridTouchY = map(p.y, TS_MINY + 200, TS_MAXY - 400, 0, 8);
                else
                    gridTouchY = map(p.y, TS_MINY, TS_MAXY, 0, 14);
            }
            else if (pX >= SEQ_GRID_RIGHT)
            {
                lastPotRow = map(p.y, TS_MINY + 200, TS_MAXY - 400, 0, 3);
                change_plugin_row = true;
                change_row = true;
            }
        }
        else
        {
            if (pX < SEQ_GRID_RIGHT)
            {
                tsTouched = true;
                parameterTouchX = constrain(map(p.x, TS_MINX + 200, TS_MAXX - 600, 0, 3), 0, 3);
                parameterTouchY = constrain(map(p.y, TS_MINY + 150, TS_MAXY - 150, 127, 0), 0, 127);
                Serial.printf("parameterTouchX: %d, Y: %d\n", parameterTouchX, parameterTouchY);
            }
            else if (pX >= SEQ_GRID_RIGHT)
            {
                lastPotRow = map(p.y, TS_MINY + 200, TS_MAXY - 400, 0, 3);
                change_plugin_row = true;
                change_row = true;
            }
        }
    }
    if (!ts.touched() && tsTouched)
        tsTouched = false;
}
