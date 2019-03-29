#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"

using namespace vex;

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          VEX Worlds Main File                             */
/*                               Version 1.0                                 */
/*                                Team 750E                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/

const int FRONT_RIGHT = PORT1;
const int FRONT_LEFT = PORT2;
const int BACK_RIGHT = PORT3;
const int BACK_LEFT = PORT4;

const int LAUNCHER = PORT5;

const int ANGLE_ADJUST = PORT6;

const int ROLLER_MOTOR = PORT7;



brain Brain;

competition Competition;

controller controller1 = controller();

motor FrontRightMotor(FRONT_RIGHT, true);
motor FrontLeftMotor(FRONT_LEFT, false);
motor BackRightMotor(BACK_RIGHT, true);
motor BackLeftMotor(BACK_LEFT, false);

motor LauncherMotor(LAUNCHER, false);

motor AngleAdjusterMotor(ANGLE_ADJUST, false);

motor RollerMotor(ROLLER_MOTOR, false);
