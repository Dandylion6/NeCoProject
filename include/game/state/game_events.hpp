#pragma once


/**
 * Holds game related events. These flags will be reset after every other update.
 */
struct GameEvents final
{
    bool shouldExit = false;
    /**
     * If true, will load the game.
     */
    bool shouldLoad = false;
    /**
     * If true, will re-load the game.
     */
    bool shouldRestart = false;
};