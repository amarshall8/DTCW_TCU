# DTCW TCU
Temperature control unit (TCU) code for the Digital Temperature Controlled Worksurface\
Reads commands from LCD front panel over UART and controls the half-bridge PID loop plus other hardware devices accordingly.\
This is primarily to take computational load off of the LCD menu system and avoid issues of the control loop being broken by menu code.
