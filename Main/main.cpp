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


void turn( float degrees ){
    const float TURNING_DIAMETER = 17.5;
    float turningRatio = TURNING_DIAMETER / WHEEL_DIAMETER;
    int turnSpeed = 60;

    RightMotorFront.startRotateFor(degrees * turningRatio / 2, rotationUnits::deg, turnSpeed, velocityUnits::pct);
    LeftMotorFront.startRotateFor(-degrees * turningRatio / 2, rotationUnits::deg, turnSpeed, velocityUnits::pct);
    RightMotorBack.startRotateFor(degrees * turningRatio / 2, rotationUnits::deg, turnSpeed, velocityUnits::pct);
    LeftMotorBack.rotateFor(-degrees * turningRatio / 2, rotationUnits::deg, turnSpeed, velocityUnits::pct);
}


void shoot( void ){
    LauncherMotor.rotateFor(1.4, timeUnits::sec, 100, velocityUnits::pct);
}

/*****AUTON SELECTOR*****/

int selectAuton() {
    Brain.Screen.clearScreen();

    Brain.Screen.drawRectangle(10, 10, 140, 50, color::red);
    Brain.Screen.drawRectangle(160, 10, 140, 50, color::red);
    Brain.Screen.drawRectangle(310, 10, 140, 50, color::red);

    Brain.Screen.drawRectangle(10, 80, 140, 50, color::blue);
    Brain.Screen.drawRectangle(160, 80, 140, 50, color::blue);
    Brain.Screen.drawRectangle(310, 80, 140, 50, color::blue);

    Brain.Screen.printAt(31, 35, "Front Flag");
    Brain.Screen.printAt(183, 35, "Front Plat");
    Brain.Screen.printAt(360, 35, "Back");

    Brain.Screen.printAt(31, 105, "Front Flag");
    Brain.Screen.printAt(185, 105, "Front Plat");
    Brain.Screen.printAt(360, 105, "Back");

    while(true) {
        if(Brain.Screen.pressing()) {
            int xPos = Brain.Screen.xPosition();
            int yPos = Brain.Screen.yPosition();

            if(yPos >= 10 && yPos <= 60) {
                if(xPos >= 10 && xPos <= 150) {
                    return 1;
                }
                else if(xPos >= 160 && xPos <= 300) {
                    return 2;
                }
                else if(xPos >= 310 && xPos <= 450){
                    return 3;
                }
            }
            else if(yPos >= 80 && yPos <= 130) {
                if(xPos >= 10 && xPos <= 150) {
                    return 4;
                }
                else if(xPos >= 160 && xPos <= 300) {
                    return 5;
                }
                else if(xPos >= 310 && xPos <= 450){
                    return 6;
                }
            }
        }
    }
}

/*****BLUE INSIDE AUTON*****/

void BlueInsidePlatform( void ){
    shoot(); //shoot high flag
    RollerMotor.spin(directionType::fwd,100,velocityUnits::pct); //turn on roller
    turn(160.0); //turn left to face cap w ball
    task::sleep(300); //stop for 0.3 seconds to avoid drifting
    driveFor(3.0, 100); //drive for 3 tiles to get ball
    driveFor(0.6, 40); //drive slowly to approach ball
    driveFor(-4.5, 100); //drive back and hit wall to align bot
    driveFor(0.48, 100); //drive slowly forward to avoid hitting wall when turning
    task::sleep(300); //sleep for 0.3 seconds
    turn(-170.0); //turn right to face flags
    task::sleep(300); //sleep for 0.3 seconds
    driveFor(2.4, 100); //drive forwards
    task::sleep(300); //sleep for 0.3 seconds
    RollerMotor.stop(); //stops roller
    shoot(); //shoot
    turn(-20.0);
    driveFor(1.6, 60); //drive slowly into low flag and align w wall
    driveFor(-5.4, 90); //drive backwards for platform
    turn(-153.0); //turn so that back is facing platform
    driveFor(-6.0, 100); //drive into platform
}

void BlueInsideLowFlag( void ){
    shoot(); //shoot high flag
    RollerMotor.spin(directionType::fwd,100,velocityUnits::pct); //turn on roller
    turn(150.0); //turn left to face cap w ball
    task::sleep(300); //stop for 0.3 seconds to avoid drifting
    driveFor(3.0, 100); //drive for 3 tiles to get ball
    driveFor(0.6, 40); //drive slowly to approach ball
    driveFor(-4.5, 100); //drive back and hit wall to align bot
    driveFor(0.48, 100); //drive slowly forward to avoid hitting wall when turning
    task::sleep(300); //sleep for 0.3 seconds
    turn(-165.0); //turn right to face flags
    task::sleep(300); //sleep for 0.3 seconds
    driveFor(2.4, 100); //drive forwards
    task::sleep(300); //sleep for 0.3 seconds
    RollerMotor.stop(); //stops roller
    shoot(); //shoot
    turn(-20.0);
    driveFor(1.6, 60); //drive slowly into low flag and align w wall
    driveFor(2.0, 40); //drive backwards for medium flag
}


/*****OUTSIDE AUTON*****/

void Outside ( void ){
    RollerMotor.spin(directionType::rev,100,velocityUnits::pct);
    driveFor(3.6, 40);
}


/*****RED INSIDE AUTON*****/

void RedInsidePlatform( void ){
    shoot(); //shoot high flag
    RollerMotor.spin(directionType::fwd,100,velocityUnits::pct); //turn on roller
    turn(-160.0); //turn left to face cap w ball
    task::sleep(300); //stop for 0.3 seconds to avoid drifting
    driveFor(3.0, 100); //drive for 3 tiles to get ball
    driveFor(0.6, 40); //drive slowly to approach ball
    driveFor(-4.5, 100); //drive back and hit wall to align bot
    driveFor(0.41, 100); //drive slowly forward to avoid hitting wall when turning
    task::sleep(300); //sleep for 0.3 seconds
    turn(140.0); //turn right to face flags
    driveFor(2.55, 100); //drive forwards to hit medium flag
    shoot(); //shoot
    task::sleep(300); //sleep for 0.3 seconds
    RollerMotor.stop(); //stop roller motor
    driveFor(-4.36, 100); //drive back to reach platform
    task::sleep(300); //sleep for 0.3 seconds
    turn(135.0); //turn so that back is facing platform
    task::sleep(300);
    driveFor(-5.0, 100); //drive into platform

}

void RedInsideLowFlag( void ){
    shoot(); //shoot high flag
    RollerMotor.spin(directionType::fwd,100,velocityUnits::pct); //turn on roller
    turn(-160.0); //turn left to face cap w ball
    task::sleep(300); //stop for 0.3 seconds to avoid drifting
    driveFor(3.0, 100); //drive for 3 tiles to get ball
    driveFor(0.6, 40); //drive slowly to approach ball
    driveFor(-4.5, 100); //drive back and hit wall to align bot
    driveFor(0.41, 100); //drive slowly forward to avoid hitting wall when turning
    task::sleep(300); //sleep for 0.3 seconds
    turn(140.0); //turn right to face flags
    task::sleep(300); //sleep for 0.3 seconds
    driveFor(0.5, 40); //drive slowly initially
    turn(22.0); //turn to face low flag
    task::sleep(300); //sleep for 0.3 seconds
    driveFor(2.7, 100); //drive forwards
    task::sleep(300); //sleep for 0.3 seconds
    RollerMotor.stop(); //stops roller
    driveFor(1.6, 60); //drive slowly into low flag and align w wall
    driveFor(-2.0, 40); //drive backwards for medium flag
    turn(-27.0); //turn to aim for medium flag
    shoot(); //shoot
    turn(22.0);
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
    RightMotorFront.spin(directionType::fwd, (left.value() - right.value()), velocityUnits::pct);
    LeftMotorFront.spin(directionType::fwd, (left.value() + right.value()), velocityUnits::pct);
    RightMotorBack.spin(directionType::fwd, (left.value() - right.value()), velocityUnits::pct);
    LeftMotorBack.spin(directionType::fwd, (left.value() + right.value()), velocityUnits::pct);
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

    if(slowUp.pressing()){
        RightMotorFront.spin(directionType::fwd, 35, velocityUnits::pct);
        LeftMotorFront.spin(directionType::fwd, 35, velocityUnits::pct);
        RightMotorBack.spin(directionType::fwd, 35, velocityUnits::pct);
        LeftMotorBack.spin(directionType::fwd, 35, velocityUnits::pct);
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

    switch(selectAuton()) {
        case 1:
            comp.autonomous( RedInsideLowFlag );
            break;
        case 2:
            comp.autonomous( RedInsidePlatform );
            break;
        case 3:
            comp.autonomous( Outside );
            break;
        case 4:
            comp.autonomous( BlueInsideLowFlag );
            break;
        case 5:
            comp.autonomous( BlueInsidePlatform );
            break;
        case 6:
            comp.autonomous( Outside );
            break;
    }

    Brain.Screen.clearScreen();

    comp.drivercontrol( usercontrol );

    while(1) {
        task::sleep(100);
    }
}
