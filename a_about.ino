// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)
//
//
// DrainBloodPlayerTurnsv19 is the last working mode . (incorporates the center leds, and has all players lined up correctly)
//DrainBloodPlayerTurnsv20 is moded pass turn button to update the Modes. And adds gameMode for the center circle
//DrainBloodPlayerTurnsv21 Adds in the free for all, and the 2 Headed Giant game modes
//DrainBloodPlayerTurnsv22 Moves the buttons arounds so they are more intuitive for the 2 headed giant game modes
//DrainBloodPlayerTurnsv23 Adds game mode 3 and 4 for Photographing the table. (3 = all red, 4 = multi colors)
//DrainBloodPlayerTurnsv24 Adds in game mode for up to 6 players
//. v24 is the last stable version (it is what is currently in the table)
//DrainBloodPlayerTurnsv25 Makes the death animation drain blood flow symetrically for all players
