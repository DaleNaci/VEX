/**************************************************
*
*
* Team: 750E
* Game: Turning Point
* Header File
*
*
***************************************************/

//uses namespace for cleaner code;
using namespace vex;

/**************************************************/
/*Motor Port Declaration Panel*/

const int FRONT_RIGHT_PORT = PORT17;
const int BACK_RIGHT_PORT = PORT10;
const int FRONT_LEFT_PORT = PORT19;
const int BACK_LEFT_PORT = PORT18;

const int ROLLER_PORT = PORT20;

const int LAUNCHER_PORT = PORT2;

/***************************************************/

/**************************************************/
/*Initializing Panel*/

brain Brain = brain();

controller Controller1 = controller();

competition comp = competition();

motor RightMotorFront = motor(FRONT_RIGHT_PORT, true);
motor RightMotorBack = motor(BACK_RIGHT_PORT, true);
motor LeftMotorFront = motor(FRONT_LEFT_PORT, false);
motor LeftMotorBack = motor(BACK_LEFT_PORT, false);

motor RollerMotor = motor(ROLLER_PORT, false);

motor LauncherMotor = motor(LAUNCHER_PORT, true);
/**************************************************/
