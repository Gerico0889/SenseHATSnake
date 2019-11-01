#!/usr/bin/env python

import sys
from sense_hat import SenseHat

sense = SenseHat()


status = sys.argv[1]
score = sys.argv[2]


if (status == "GAME_OVER") :
    message = "Game Over - Score: %s" %(score)

elif (status == "GAME_STOP") :
    message = "You have stopped the game"

sense.show_message(message, 0.04)
