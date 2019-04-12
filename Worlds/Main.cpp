#include "vex.h"

using namespace vex;

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          VEX Worlds Main File                             */
/*                               Version 1.0                                 */
/*                                Team 750E                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/

bool launching = false;

void pre_auton( void ) { }

void puncherDraw(void) {
    LauncherMotor.rotateFor(directionType::fwd, 500, rotationUnits::deg, 100, velocityUnits::pct);
    LauncherMotor.resetRotation();
}

void gyroTurn(double angle) {
    task::sleep(200);
    
    const double ENDPOINT = g.value(rotationUnits::deg) + angle;
    
    
    double currentValue = g.value(rotationUnits::deg);
    double currentError = currentValue - ENDPOINT;
    double previousError = 0.00;
    double totalError = 0.00;
    const double INTEGRAL_LIMIT = 500.0;
    
    while(fabs(currentError) > 0.10) {
        
        double kP = 0.40;
        double kI = 0.10;
        double kD = 0.50;
        
        previousError = currentError;
        currentError = ENDPOINT - currentValue;
        
        double p = kP * currentError;
        double i = kI * totalError;
        double d = kD * (currentError - previousError) / 0.02;
        
        FrontRightMotor.spin(directionType::fwd, p + i + d, velocityUnits::pct);
        FrontLeftMotor.spin(directionType::rev, p + i + d, velocityUnits::pct);
        BackLeftMotor.spin(directionType::rev, p + i + d, velocityUnits::pct);
        BackRightMotor.spin(directionType::fwd, p + i + d, velocityUnits::pct);
        
        currentValue = g.value(rotationUnits::deg);
        totalError += previousError;
        
        if(totalError > INTEGRAL_LIMIT) totalError = INTEGRAL_LIMIT;
        if(totalError < -INTEGRAL_LIMIT) totalError = -INTEGRAL_LIMIT;
        
        task::sleep(20);
    }
    
    FrontRightMotor.stop(brakeType::brake);
    FrontLeftMotor.stop(brakeType::brake);
    BackLeftMotor.stop(brakeType::brake);
    BackRightMotor.stop(brakeType::brake);
    
}

void turn(double angle) {
    const double finalAngle = angle;
    
    g.startCalibration();
    while(g.isCalibrating()){
        task::sleep(10);
    }
    
    task::sleep(500);
    
    while(g.value(rotationUnits::deg) < finalAngle) {
        FrontLeftMotor.spin(directionType::fwd, 30, velocityUnits::pct);
        FrontRightMotor.spin(directionType::rev, 30, velocityUnits::pct);
        BackLeftMotor.spin(directionType::fwd, 30, velocityUnits::pct);
        BackRightMotor.spin(directionType::rev, 30, velocityUnits::pct);
        controller1.Screen.clearScreen();
        controller1.Screen.setCursor(1, 1);
        controller1.Screen.print(g.value(rotationUnits::deg));
        task::sleep(50);
    }
    
    FrontLeftMotor.stop();
    FrontRightMotor.stop();
    BackLeftMotor.stop();
    BackRightMotor.stop();
}

void driveFor( float tiles , int speed ){
    const float TILE_LENGTH = 12.5;
    float length = tiles * TILE_LENGTH;
    float circum =  3.141592653589 * 4.125;
    float rotations = length / circum;
    float degrees = 360 * rotations;
    
    FrontRightMotor.startRotateFor(degrees, rotationUnits::deg, speed, velocityUnits::pct);
    FrontLeftMotor.startRotateFor(degrees, rotationUnits::deg, speed, velocityUnits::pct);
    BackRightMotor.startRotateFor(degrees, rotationUnits::deg, speed, velocityUnits::pct);
    BackLeftMotor.rotateFor(degrees, rotationUnits::deg, speed, velocityUnits::pct);
}

int autonSelectorRedScreen( void ){
    Brain.Screen.clearScreen();
    
    Brain.Screen.drawRectangle(10, 10, 140, 75, color::red);
    Brain.Screen.drawRectangle(160, 10, 140, 75, color::red);
    
    Brain.Screen.printAt(70, 55, "1");
    Brain.Screen.printAt(220, 55, "2");
    
    while(true){
        if(Brain.Screen.pressing()){
            int xpos = Brain.Screen.xPosition();
            int ypos = Brain.Screen.yPosition();
            
            if(ypos >= 10 && ypos <= 75){
                if(xpos >= 10 && xpos <= 150){
                    return 1;
                }
                else if(xpos >= 160 && xpos <= 300){
                    return 2;
                }
            }
        }
    }
}

int autonSelectorBlueScreen( void ){
    Brain.Screen.clearScreen();
    
    Brain.Screen.drawRectangle(10, 10, 140, 75, color::blue);
    Brain.Screen.drawRectangle(160, 10, 140, 75, color::blue);
    
    Brain.Screen.printAt(70, 55, "1");
    Brain.Screen.printAt(220, 55, "2");
    
    while(true){
        if(Brain.Screen.pressing()){
            int xpos = Brain.Screen.xPosition();
            int ypos = Brain.Screen.yPosition();
            
            if(ypos >= 10 && ypos <= 85){
                if(xpos >= 10 && xpos <= 150){
                    return 3;
                }
                else if(xpos >= 160 && xpos <= 300){
                    return 4;
                }
            }
        }
    }
}

int autonSelectorMainScreen( void ) {
    Brain.Screen.clearScreen();
    
    Brain.Screen.drawRectangle(10, 10, 200, 150, color::red);
    Brain.Screen.drawRectangle(250, 10, 200, 150, color::blue);
    
    Brain.Screen.printAt(90, 50, "Red");
    Brain.Screen.printAt(320, 50, "Blue");
    
    while (true){
        if(Brain.Screen.pressing()){
            int xpos = Brain.Screen.xPosition();
            
            if(xpos >= 10 && xpos <= 210){
                return autonSelectorRedScreen();
            }
            else{
                return autonSelectorBlueScreen();
            }
        }
    }
}

void RedInside ( void ){
    
}

void BlueInside ( void ){
    
}

void RedOutside ( void ){
    
}

void BlueOutside ( void ){
    
}

void autonomous( void ) {
    /*int auton = autonSelectorMainScreen();
     switch(auton){
     case 1:
     break;
     }*/
}



void opDrive( controller::axis left, controller::axis right){
    FrontRightMotor.spin(directionType::fwd, left.value() - right.value(), percentUnits::pct);
    FrontLeftMotor.spin(directionType::fwd, left.value() + right.value(), percentUnits::pct);
    BackRightMotor.spin(directionType::fwd, left.value() - right.value(), percentUnits::pct);
    BackLeftMotor.spin(directionType::fwd, left.value() + right.value(), percentUnits::pct);
}

void launch(controller::button launchButton){
    launching = LauncherMotor.isSpinning();
    if (launchButton.pressing() && !launching) {
        LauncherMotor.startRotateFor(directionType::fwd, 720, rotationUnits::deg, 100, velocityUnits::pct);
    }
}

void intake(controller::button inButton, controller::button outButton){
    if(inButton.pressing()){
        RollerMotor.spin(directionType::rev, 100, velocityUnits::pct);
    }
    else if(outButton.pressing()){
        RollerMotor.spin(directionType::fwd, 100, velocityUnits::pct);
    }
    else{
        RollerMotor.stop();
    }
}

void adjust(controller::button up, controller::button down){
    if(up.pressing()){
        AngleAdjusterMotor.spin(directionType::rev, 100, velocityUnits::pct);
    }
    else if(down.pressing()){
        AngleAdjusterMotor.spin(directionType::fwd, 100, velocityUnits::pct);
    }
    else{
        AngleAdjusterMotor.stop(vex::brakeType::hold);
    }
}


void usercontrol( void ) {
    
    thread puncherThread = thread(puncherDraw);
    
    while (1) {
        
        controller::axis VERTICAL_AXIS = controller1.Axis3;
        controller::axis HORIZONTAL_AXIS = controller1.Axis1;
        
        controller::button LAUNCH_BUTTON = controller1.ButtonR1;
        
        controller::button INTAKE_IN = controller1.ButtonL1;
        controller::button INTAKE_OUT = controller1.ButtonL2;
        
        controller::button ANGLE_UP = controller1.ButtonA;
        controller::button ANGLE_DOWN = controller1.ButtonX;
        
        opDrive(VERTICAL_AXIS, HORIZONTAL_AXIS);
        
        launch(LAUNCH_BUTTON);
        
        intake(INTAKE_IN, INTAKE_OUT);
        
        adjust(ANGLE_UP, ANGLE_DOWN);
        
        task::sleep(20);
    }
}

int main() {
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );
    
    pre_auton();
    
    while(1) {
        task::sleep(100);
    }
}
