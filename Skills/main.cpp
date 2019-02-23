/**************************************************
 *
 *
 * Team: 750E
 * Game: Turning Point
 * Main File
 *
 *
 ***************************************************/


#include "robot-config.h"

//using namespace for cleaner code
using namespace vex;

//constant for wheel diameter
const float WHEEL_DIAMETER = 4.125;

//constant for DriverSkills
const bool DriverSkills = true;


/*****RUMBLE THREAD*****/
void rumbleTimer(void) {
    task::sleep(20000);
    Controller1.rumble("-");
    task::sleep(20000);
    Controller1.rumble("--");
    task::sleep(10000);
    Controller1.rumble("---");
}


void pre_auton( void ) {
    
}


/**************************************************
 *
 * Auton methods
 * driveFor();
 * turn();
 * shoot();
 *
 **************************************************/


void driveFor( float tiles , int speed ){
    const float TILE_LENGTH = 12.5;
    float length = tiles * TILE_LENGTH;
    float circum =  3.141592653589 * WHEEL_DIAMETER;
    float rotations = length / circum;
    float degrees = 360 * rotations;
    
    RightMotorFront.startRotateFor(degrees, rotationUnits::deg, speed, velocityUnits::pct);
    LeftMotorFront.startRotateFor(degrees, rotationUnits::deg, speed, velocityUnits::pct);
    RightMotorBack.startRotateFor(degrees, rotationUnits::deg, speed, velocityUnits::pct);
    LeftMotorBack.rotateFor(degrees, rotationUnits::deg, speed, velocityUnits::pct);
}


void turn( float degrees , int turnSpeed){
    const float TURNING_DIAMETER = 17.5;
    float turningRatio = TURNING_DIAMETER / WHEEL_DIAMETER;
    
    RightMotorFront.startRotateFor(degrees * turningRatio / 2, rotationUnits::deg, turnSpeed, velocityUnits::pct);
    LeftMotorFront.startRotateFor(-degrees * turningRatio / 2, rotationUnits::deg, turnSpeed, velocityUnits::pct);
    RightMotorBack.startRotateFor(degrees * turningRatio / 2, rotationUnits::deg, turnSpeed, velocityUnits::pct);
    LeftMotorBack.rotateFor(-degrees * turningRatio / 2, rotationUnits::deg, turnSpeed, velocityUnits::pct);
}


void shoot( void ){
    LauncherMotor.rotateFor(1.4, timeUnits::sec, 100, velocityUnits::pct);
}

/*****PROGRAMMING SKILLS*****/

void ProgrammingSkills( void ) {
    driveFor(3.0, 65); //drive for 3 tiles to get ball
    RollerMotor.startRotateFor(-1000, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(0.6, 65); //drive slowly to approach ball
    RollerMotor.rotateFor(1000, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(-5.0, 65); //drive back and hit wall to align bot
    task::sleep(200);
    driveFor(0.48, 100); //drive slowly forward to avoid hitting wall when turning
    task::sleep(300);
    turn(155.0, 35);
    shoot();
    task::sleep(601);
    RollerMotor.startRotateFor(4000, rotationUnits::deg, 100, velocityUnits::pct);
    task::sleep(2000);
    driveFor(2.35, 65);
    task::sleep(200);
    RollerMotor.startRotateFor(-600, rotationUnits::deg,100,velocityUnits::pct);
    shoot();
    turn(35.0, 50);
    driveFor(1.6, 75); //drive slowly into low flag and align w wall
    driveFor(-0.4,75);
    turn(-45.0, 50);
    driveFor(-2.01, 65);
    turn(-150.0, 35);
    driveFor(-0.6, 35);
    RollerMotor.spin(directionType::rev, 100, velocityUnits::pct);
    driveFor(2.5, 65);
    RollerMotor.stop();
    driveFor(-4.2, 45);
    driveFor(0.48, 45);
    turn(-150.0, 35);
    driveFor(2.75, 45);
    turn(150.0, 35);
    driveFor(-0.9, 25);
    driveFor(5.6, 75);
}

/*************************************************
 *
 * Operator Control Methods
 * drive();
 * intake();
 * slowDrive();
 * launch();
 *
 *
 *************************************************/

void drive(controller::axis left, controller::axis right){
    RightMotorFront.spin(directionType::fwd, (left.value() - (0.85 * right.value())), velocityUnits::pct);
    LeftMotorFront.spin(directionType::fwd, (left.value() + (0.85 * right.value())), velocityUnits::pct);
    RightMotorBack.spin(directionType::fwd, (left.value() - (0.85 * right.value())), velocityUnits::pct);
    LeftMotorBack.spin(directionType::fwd, (left.value() + (0.85 * right.value())), velocityUnits::pct);
}

void intake(controller::button in, controller::button out){
    if(in.pressing()){
        RollerMotor.spin(directionType::rev, 100, velocityUnits::pct);
    }
    else if(out.pressing()){
        RollerMotor.spin(directionType::fwd, 100, velocityUnits::pct);
    }
    else{
        RollerMotor.stop();
    }
}

void slowDrive(controller::button slowLeft, controller::button slowRight, controller::button slowUp){
    if(slowLeft.pressing()){
        RightMotorFront.spin(directionType::fwd, 7, velocityUnits::pct);
        LeftMotorFront.spin(directionType::rev, 7, velocityUnits::pct);
        RightMotorBack.spin(directionType::fwd, 7, velocityUnits::pct);
        LeftMotorBack.spin(directionType::rev, 7, velocityUnits::pct);
    }
    else if(slowRight.pressing()){
        RightMotorFront.spin(directionType::rev, 7, velocityUnits::pct);
        LeftMotorFront.spin(directionType::fwd, 7, velocityUnits::pct);
        RightMotorBack.spin(directionType::rev, 7, velocityUnits::pct);
        LeftMotorBack.spin(directionType::fwd, 7, velocityUnits::pct);
    }
    // This is used for the first back cap for the current 27-point path
    if(slowUp.pressing()){
        RightMotorFront.spin(directionType::fwd, 40, velocityUnits::pct);
        LeftMotorFront.spin(directionType::fwd, 40, velocityUnits::pct);
        RightMotorBack.spin(directionType::fwd, 40, velocityUnits::pct);
        LeftMotorBack.spin(directionType::fwd, 40, velocityUnits::pct);
    }
}

void launch(controller::button launchButton){
    if(launchButton.pressing()){
        LauncherMotor.spin(directionType::fwd, 100, velocityUnits::pct);
    }
    else{
        LauncherMotor.stop();
    }
}


/*****OPERATOR CONTROL*****/

void usercontrol( void ) {
    
    if(DriverSkills) {
        thread rumbleThread = thread(rumbleTimer);
    }
    
    while (1) {
        
        controller::axis VERTICAL_AXIS = Controller1.Axis3;
        controller::axis HORIZONTAL_AXIS = Controller1.Axis1;
        
        controller::button SLOW_LEFT = Controller1.ButtonLeft;
        controller::button SLOW_RIGHT = Controller1.ButtonRight;
        controller::button SLOW_UP = Controller1.ButtonUp;
        
        controller::button INTAKE_IN = Controller1.ButtonL1;
        controller::button INTAKE_OUT = Controller1.ButtonL2;
        
        controller::button LAUNCH_BUTTON = Controller1.ButtonR1;
        
        drive(VERTICAL_AXIS, HORIZONTAL_AXIS);
        
        launch(LAUNCH_BUTTON);
        
        intake(INTAKE_IN, INTAKE_OUT);
        
        slowDrive(SLOW_LEFT, SLOW_RIGHT, SLOW_UP);
        
        task::sleep(20);
    }
}


/*****MAIN METHOD*****/

int main() {
    
    pre_auton();
    
    comp.autonomous(ProgrammingSkills);
    comp.drivercontrol(usercontrol);
    
    
    while(1) {
        task::sleep(100);
    }
}
