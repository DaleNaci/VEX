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

//pre_auton
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

//driveFor Method arguements: (distance, speed)
void driveFor( float tiles , int speed ){
    //calculates amount of required degrees based on wheel circumference and desired tile length
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

//turn Method arguments: (angle)
void turn( float degrees ){
    //calculates actual amount of degrees required based on desired angle and turning diameter
    const float TURNING_DIAMETER = 17.5;
    float turningRatio = TURNING_DIAMETER / WHEEL_DIAMETER;
    int turnSpeed = 45;
    
    RightMotorFront.startRotateFor(degrees * turningRatio / 2, rotationUnits::deg, turnSpeed, velocityUnits::pct);
    LeftMotorFront.startRotateFor(-degrees * turningRatio / 2, rotationUnits::deg, turnSpeed, velocityUnits::pct);
    RightMotorBack.startRotateFor(degrees * turningRatio / 2, rotationUnits::deg, turnSpeed, velocityUnits::pct);
    LeftMotorBack.rotateFor(-degrees * turningRatio / 2, rotationUnits::deg, turnSpeed, velocityUnits::pct);
}

//shoot Method
void shoot( void ){
    //runs launcherMotor for 1.4 seconds (enough to shoot ball)
    LauncherMotor.rotateFor(1.4, timeUnits::sec, 100, velocityUnits::pct);
}

/*****AUTON SELECTOR*****/

int selectAuton() {
    Brain.Screen.clearScreen();
    
    //draws 6 rectangles for options for auton
    
    Brain.Screen.drawRectangle(10, 10, 140, 75, color::red);
    Brain.Screen.drawRectangle(160, 10, 140, 75, color::red);
    Brain.Screen.drawRectangle(310, 10, 140, 75, color::red);
    
    Brain.Screen.drawRectangle(10, 100, 140, 75, color::blue);
    Brain.Screen.drawRectangle(160, 100, 140, 75, color::blue);
    Brain.Screen.drawRectangle(310, 100, 140, 75, color::blue);
    
    //labels options
    
    Brain.Screen.printAt(31, 35, "Front Flag");
    Brain.Screen.printAt(183, 35, "Back Plat");
    Brain.Screen.printAt(360, 35, "Back");
    
    Brain.Screen.printAt(31, 130, "Front Flag");
    Brain.Screen.printAt(185, 130, "Back Plat");
    Brain.Screen.printAt(360, 130, "Back");
    
    //takes user input and returns a number from 1-6 corresponding to what auton is selected
    
    while(true) {
        if(Brain.Screen.pressing()) {
            int xPos = Brain.Screen.xPosition();
            int yPos = Brain.Screen.yPosition();
            
            if(yPos >= 10 && yPos <= 85) {
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
            else if(yPos >= 100 && yPos <= 185) {
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
    driveFor(3.0, 65); //drive for 3 tiles to get ball
    RollerMotor.startRotateFor(720, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(0.6, 65); //drive slowly to approach ball
    driveFor(-5.0, 65); //drive back and hit wall to align bot
    task::sleep(200);
    driveFor(0.48, 100); //drive slowly forward to avoid hitting wall when turning
    task::sleep(300);
    turn(-168.0);
    shoot();
    task::sleep(601);
    RollerMotor.startRotateFor(4000, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(2.35, 65);
    task::sleep(200);
    shoot();
    turn(-20.0);
    driveFor(1.6, 75); //drive slowly into low flag and align w wall
    driveFor(-1.0, 50);
    turn(20.0);
    driveFor(-4.9, 90); //drive backwards for platform
    turn(-153.0); //turn so that back is facing platform
    driveFor(-6.1, 100); //drive into platform
}

void BlueInsideLowFlag( void ){
    driveFor(3.0, 65); //drive for 3 tiles to get ball
    RollerMotor.startRotateFor(720, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(0.6, 65); //drive slowly to approach ball
    driveFor(-5.0, 65); //drive back and hit wall to align bot
    task::sleep(200);
    driveFor(0.48, 100); //drive slowly forward to avoid hitting wall when turning
    task::sleep(300);
    turn(-164.0);
    shoot();
    task::sleep(601);
    RollerMotor.startRotateFor(4000, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(2.35, 65);
    task::sleep(200);
    RollerMotor.startRotateFor(-600, rotationUnits::deg,100,velocityUnits::pct);
    shoot();
    turn(-20.0);
    driveFor(1.6, 75); //drive slowly into low flag and align w wall
    driveFor(-0.4,75);
}


/*****OUTSIDE AUTON*****/

void BlueOutsidePlat ( void ){
    RollerMotor.startRotateFor(-3500, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(3.8, 65); //drive to flip cap
    task::sleep(300);
    driveFor(-1.7, 65);
    turn(-120.0);
    task::sleep(300);
    driveFor(1.7,65);
    RollerMotor.startRotateFor(1300,rotationUnits::deg, 1000, velocityUnits::pct);
    driveFor(-1.6, 65);
    shoot();
    turn(-80.0);
    task::sleep(300);
    driveFor(4.3, 65);
}

void RedOutsidePlat ( void ){
    RollerMotor.startRotateFor(-3500, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(3.9, 65); //drive to flip cap
    task::sleep(300);
    driveFor(-1.8, 65);
    turn(120.0);
    task::sleep(300);
    driveFor(1.7,65);
    RollerMotor.startRotateFor(1300,rotationUnits::deg, 1000, velocityUnits::pct);
    driveFor(-1.2, 65);
    shoot();
    turn(75.0);
    task::sleep(300);
    driveFor(4.4, 65);
}

void BlueOutside( void ){
    RollerMotor.startRotateFor(-3500, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(3.8, 65); //drive to flip cap
    task::sleep(300);
    driveFor(-1.7, 65);
    turn(-120.0);
    task::sleep(300);
    driveFor(1.7,65);
    RollerMotor.startRotateFor(1500,rotationUnits::deg, 1000, velocityUnits::pct);
    driveFor(-1.6, 65);
}

void RedOutside( void ){
    RollerMotor.startRotateFor(-3500, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(3.9, 65); //drive to flip cap
    task::sleep(300);
    driveFor(-1.8, 65);
    turn(120.0);
    task::sleep(300);
    driveFor(1.7,65);
    RollerMotor.startRotateFor(750,rotationUnits::deg, 1000, velocityUnits::pct);
    driveFor(-1.2, 65);
}

/*****RED INSIDE AUTON*****/

void RedInsidePlatform( void ){
    driveFor(3.0, 100); //drive for 3 tiles to get ball
    RollerMotor.startRotateFor(720, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(0.6, 65); //drive slowly to approach ball
    driveFor(-2.25, 100); //drive back and hit wall to align bot
    task::sleep(200);
    driveFor(-2.25, 50);
    driveFor(0.34, 100); //drive slowly forward to avoid hitting wall when turning
    task::sleep(300);
    turn(149.5);
    shoot();
    task::sleep(601);
    driveFor(1.2, 100);
    RollerMotor.startRotateFor(4000, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(1.1, 100);
    task::sleep(200);
    shoot();
    turn(32.0);
    driveFor(1.6, 60); //drive slowly into low flag and align w wall
    driveFor(-1.0, 50);
    driveFor(-5.0, 90); //drive backwards for platform
    turn(143.0); //turn so that back is facing platform
    driveFor(-8.0, 100); //drive into platform
    
}

void RedInsideLowFlag( void ){
    driveFor(3.0, 65); //drive for 3 tiles to get ball
    RollerMotor.startRotateFor(720, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(0.6, 65); //drive slowly to approach ball
    driveFor(-5.0, 65); //drive back and hit wall to align bot
    task::sleep(200);
    driveFor(0.48, 100); //drive slowly forward to avoid hitting wall when turning
    task::sleep(300);
    turn(150.0);
    shoot();
    task::sleep(601);
    RollerMotor.startRotateFor(4000, rotationUnits::deg, 100, velocityUnits::pct);
    task::sleep(2000);
    driveFor(2.35, 65);
    task::sleep(200);
    RollerMotor.startRotateFor(-600, rotationUnits::deg,100,velocityUnits::pct);
    shoot();
    turn(35.0);
    driveFor(1.6, 75); //drive slowly into low flag and align w wall
    driveFor(-0.4,75);
}

/*****SECRET AUTON*****/

void RedSecretAuton(void){
    driveFor(2.7, 65); //drive for 3 tiles to get ball
    RollerMotor.startRotateFor(720, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(0.6, 65); //drive slowly to approach ball
    driveFor(-5.0, 65); //drive back and hit wall to align bot
    task::sleep(200);
    driveFor(0.33, 100); //drive slowly forward to avoid hitting wall when turning
    task::sleep(300);
    turn(158.0); //turn towards flags
    task::sleep(200);
    driveFor(3.7,75);//get low flags
    task::sleep(200);
    driveFor(-3.7,75);//go back
    task::sleep(250);
    turn(-91.0);//turn towards middle flags
    shoot();
    RollerMotor.startRotateFor(2500, rotationUnits::deg, 100, velocityUnits::pct);
    turn(75.0);//turn back
    task::sleep(800);
    RollerMotor.startRotateFor(2500, rotationUnits::deg, 100, velocityUnits::pct);
    task::sleep(2150);
    shoot();
}

void BlueSecretAuton(void){
    driveFor(2.7, 65); //drive for 3 tiles to get ball
    RollerMotor.startRotateFor(720, rotationUnits::deg, 100, velocityUnits::pct);
    driveFor(0.6, 65); //drive slowly to approach ball
    driveFor(-5.0, 65); //drive back and hit wall to align bot
    task::sleep(200);
    driveFor(0.46, 100); //drive slowly forward to avoid hitting wall when turning
    task::sleep(300);
    turn(-170.0);//turn towards flags
    task::sleep(200);
    driveFor(3.7,75);//get low flag
    task::sleep(200);
    driveFor(-3.7,75);//go back
    task::sleep(250);
    turn(85.0);//turn towards middle flags
    shoot();
    RollerMotor.startRotateFor(2500, rotationUnits::deg, 100, velocityUnits::pct);
    turn(-80.0);//turn back
    task::sleep(2150);
    shoot();
    turn(85.0);
}

/*************************************************
 *
 * Operator Control Methods
 * drive();
 * intake();
 * launch();
 *
 *
 *************************************************/

//drive method for operator control arguements: (left button, right button)
void drive(controller::axis left, controller::axis right){
    RightMotorFront.spin(directionType::fwd, (left.value() - right.value()*.85), velocityUnits::pct);
    LeftMotorFront.spin(directionType::fwd, (left.value() + right.value()*.85), velocityUnits::pct);
    RightMotorBack.spin(directionType::fwd, (left.value() - right.value()*.85), velocityUnits::pct);
    LeftMotorBack.spin(directionType::fwd, (left.value() + right.value()*.85), velocityUnits::pct);
}

//intake method for operator control arguments: (in button, out button)
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

//launch method for operator control arguments: (launch button)
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
        
        //declaration for buttons used on operator control
        
        controller::axis VERTICAL_AXIS = Controller1.Axis3;
        controller::axis HORIZONTAL_AXIS = Controller1.Axis1;
        
        controller::button INTAKE_IN = Controller1.ButtonL1;
        controller::button INTAKE_OUT = Controller1.ButtonL2;
        
        controller::button LAUNCH_BUTTON = Controller1.ButtonR1;
        
        drive(VERTICAL_AXIS, HORIZONTAL_AXIS);
        
        launch(LAUNCH_BUTTON);
        
        intake(INTAKE_IN, INTAKE_OUT);
        
        task::sleep(20);
    }
}


/*****MAIN METHOD*****/

int main() {
    
    pre_auton();
    
    //returns selected auton in int
    int auton = selectAuton();
    
    Brain.Screen.clearScreen();
    
    task::sleep(1000);
    
    //if red flag is selected, ask user for option of secret auton
    if (auton == 1) {
        Brain.Screen.print("Are you sure? - Red Flag");
        Brain.Screen.drawRectangle(10, 100, 140, 75, color::red);
        Brain.Screen.printAt(31, 130, "Yes");
        Brain.Screen.drawRectangle(310, 100, 140, 75, color::red);
        Brain.Screen.printAt(360, 130, "No");
        while(true) {
            if(Brain.Screen.pressing()) {
                int xPos = Brain.Screen.xPosition();
                int yPos = Brain.Screen.yPosition();
                
                if (yPos >= 100 && yPos <= 185 && xPos >= 10 && xPos <= 150) {
                    comp.autonomous( RedInsideLowFlag );
                    break;
                }
                if (yPos >= 100 && yPos <= 185 && xPos >= 310 && xPos <= 450) {
                    comp.autonomous( RedSecretAuton );
                    break;
                }
            }
        }
    }
    //if blue flag is selected, ask user for secret auton
    if (auton == 4) {
        Brain.Screen.print("Are you sure? - Blue Flag");
        Brain.Screen.drawRectangle(10, 100, 140, 75, color::blue);
        Brain.Screen.printAt(31, 130, "Yes");
        Brain.Screen.drawRectangle(310, 100, 140, 75, color::blue);
        Brain.Screen.printAt(360, 130, "No");
        while(true) {
            if(Brain.Screen.pressing()) {
                int xPos = Brain.Screen.xPosition();
                int yPos = Brain.Screen.yPosition();
                
                if (yPos >= 100 && yPos <= 185 && xPos >= 10 && xPos <= 150) {
                    comp.autonomous( BlueInsideLowFlag );
                    break;
                }
                if (yPos >= 100 && yPos <= 185 && xPos >= 310 && xPos <= 450) {
                    comp.autonomous( BlueSecretAuton );
                    break;
                }
            }
        }
    }
    else {
        //switch auton to run correct autonomous
        switch(auton) {
            case 2: comp.autonomous( RedOutsidePlat );         break;
            case 3: comp.autonomous( RedOutside );  break;
            case 5: comp.autonomous( BlueOutsidePlat ); break;
            case 6: comp.autonomous( BlueOutside );break;
        }
    }
    
    Brain.Screen.clearScreen();
    
    //switch auton to print auton title on screen
    switch(auton) {
        case 1: Brain.Screen.print("Red Flag");     break;
        case 2: Brain.Screen.print("Red Outside Plat");     break;
        case 3: Brain.Screen.print("Red Outside No Plat");  break;
        case 4: Brain.Screen.print("Blue Flag");    break;
        case 5: Brain.Screen.print("Blue Outside Plat");    break;
        case 6: Brain.Screen.print("Blue Outside No Plat"); break;
    }
    
    comp.drivercontrol( usercontrol );
    
    while(1) {
        task::sleep(100);
    }
}
