# Aerotech-101SMC2EN-Control

Circuit board and microcontroller firmware to control the Aerotech 101SMC2EN-HM motion control motor.

# Serial Interface

The revamped controller communicates with other devices over USB.

The controller communicates at a baud rate of 9600.
All commands must be terminated with newline (ASCII 012).

The command syntax is as follows:

| command         | behavior                                 |
|-----------------|------------------------------------------|
| E <some chars>  | echo                                     |
| G               | get position                             |
| H               | send home                                |
| M <destination> | move to destination, in mm               |
| Q               | query                                    |
| S               | save current position to on-board memory |
| T <position>    | tell stage what its current position is  |

Some commands cause the stage to send information back.
The returned charachters will always be terminated with newline (ASCII 012).

Get position will return the current position of the stage, in mm.
Query will return one of R (ready), B (busy).

