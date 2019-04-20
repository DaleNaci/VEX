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
bool performingAction = false;
bool puncherDrawback = true;

void pre_auton( void ) { }

void wait(int time) {
    task::sleep(time);
}

void puncherDraw(void) {
    LauncherMotor.rotateFor(directionType::fwd, 500, rotationUnits::deg, 100, velocityUnits::pct);
    LauncherMotor.resetRotation();
    puncherDrawback = true;
}

void gyroTurn(double angle) {
    
    const double ENDPOINT = g.value(rotationUnits::deg) + angle;
    
    double currentValue = g.value(rotationUnits::deg);
    double currentError = ENDPOINT - currentValue;
    double previousError = 0.00;
    
    while(fabs(currentError) > 2.50) {
        
        controller1.Screen.setCursor(1, 1);
        controller1.Screen.print(currentError);
        
        double kP = 0.4;
        double kD = 0.3;
        
        previousError = currentError;
        currentError = ENDPOINT - currentValue;
        
        double p = kP * currentError;
        double d = kD * (currentError - previousError);
        
        FrontRightMotor.spin(directionType::rev, p + d, velocityUnits::pct);
        FrontLeftMotor.spin(directionType::fwd, p + d, velocityUnits::pct);
        BackLeftMotor.spin(directionType::fwd, p + d, velocityUnits::pct);
        BackRightMotor.spin(directionType::rev, p + d, velocityUnits::pct);
        
        currentValue = g.value(rotationUnits::deg);
        
        task::sleep(20);
    }
    
    FrontRightMotor.stop(brakeType::brake);
    FrontLeftMotor.stop(brakeType::brake);
    BackLeftMotor.stop(brakeType::brake);
    BackRightMotor.stop(brakeType::brake);
    
}

/*void encoderPID(double angle){
 double average = (FrontLeftMotor.value() + FrontRightMotor.value() + BackLeftMotor.value() + BackRightMotor.value()) / 4.0;
 const double ENDPOINT = (average + angle);
 
 double currentValue = average;
 double currentError = ENDPOINT - currentValue;
 
 while(fabs(currentError) > 2.5){
 controller1.Screen.setCursor(1, 1);
 controller1.Screen.print(currentError);
 
 double kP = 0.1;
 //double kD = 0.3;
 
 //previousError = currentError;
 currentError = ENDPOINT - currentValue;
 
 double p = kP * currentError;
 //double d = kD * (currentError - previousError);
 
 FrontRightMotor.spin(directionType::fwd, p, velocityUnits::pct);
 FrontLeftMotor.spin(directionType::fwd, p, velocityUnits::pct);
 BackLeftMotor.spin(directionType::fwd, p, velocityUnits::pct);
 BackRightMotor.spin(directionType::fwd, p, velocityUnits::pct);
 
 currentValue = (FrontLeftMotor.value() + FrontRightMotor.value() + BackLeftMotor.value() + BackRightMotor.value()) / 4.0;
 
 task::sleep(20);
 }
 }*/

void turn(double angle, bool reversed) {
    int multiplier = 1;
    if (reversed) {
        multiplier = -1;
    }
    const double error_multiplier = .255;
    const double finalAngle = angle * multiplier * error_multiplier;
    
    g.startCalibration();
    while(g.isCalibrating()){
        task::sleep(10);
    }
    
    task::sleep(500);
    if (reversed) {
        while(g.value(rotationUnits::deg) > finalAngle) {
            FrontLeftMotor.spin(directionType::rev, 30, velocityUnits::pct);
            FrontRightMotor.spin(directionType::fwd, 30, velocityUnits::pct);
            BackLeftMotor.spin(directionType::rev, 30, velocityUnits::pct);
            BackRightMotor.spin(directionType::fwd, 30, velocityUnits::pct);
            controller1.Screen.clearScreen();
            controller1.Screen.setCursor(1, 1);
            controller1.Screen.print(g.value(rotationUnits::deg));
            task::sleep(50);
        }
    } else {
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

void startRollers(bool reversed) {
    int multiplier = 1;
    if (reversed) {
        multiplier = -1;
    }
    RollerMotor.setVelocity(100 * multiplier, velocityUnits::pct);
}

void stopRollers() {
    RollerMotor.setVelocity(0, velocityUnits::pct);
}

void rotateRollers(int rot) {
    RollerMotor.startRotateFor(directionType::fwd, rot, rotationUnits::deg);
}

void changeAngle(double rot) {
    AngleAdjusterMotor.rotateFor(directionType::fwd, rot, rotationUnits::deg);
}

void autonLaunch() {
    LauncherMotor.rotateFor(directionType::fwd, 720, rotationUnits::deg, 100, velocityUnits::pct);
}


int autonSelectorRedScreen( void ){
    Brain.Screen.clearScreen();
    
    Brain.Screen.drawRectangle(10, 10, 140, 75, color::red);
    Brain.Screen.drawRectangle(160, 10, 140, 75, color::red);
    
    Brain.Screen.printAt(30, 55, "Red Inside");
    Brain.Screen.printAt(180, 55, "Red Outside");
    
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
    
    Brain.Screen.printAt(30, 55, "Blue Inside");
    Brain.Screen.printAt(180, 55, "Blue Outside");
    
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
    //1
    puncherDraw();
}

void BlueInside ( void ){
    //3
    puncherDraw();
}

void RedOutside ( void ){
    //2
    puncherDraw();
}

void BlueOutside ( void ){
    //4
    puncherDraw();
}


int autonomous( void ) {
    int auton = autonSelectorMainScreen();
    return auton;
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
        LauncherMotor.startRotateFor(directionType::fwd, 750, rotationUnits::deg, 100, velocityUnits::pct);
    }
    launching = LauncherMotor.isSpinning();
    if(!launching && !puncherDrawback){
        LauncherMotor.stop();
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

//TEST ALL FEATURES
void testAuton ( void ){
    driveFor(3.0, 50);
    gyroTurn(90.0);
    rotateRollers(1000);
    changeAngle(40);
    autonLaunch();
}

//WORK ON
void doubleShot(controller::button up) {
    if (!performingAction && up.pressing()) {
        performingAction = true;
        // DOUBLE SHOT
        performingAction = false;
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
        
        controller::button DOUBLE_SHOT = controller1.ButtonUp;
        
        opDrive(VERTICAL_AXIS, HORIZONTAL_AXIS);
        
        launch(LAUNCH_BUTTON);
        
        intake(INTAKE_IN, INTAKE_OUT);
        
        adjust(ANGLE_UP, ANGLE_DOWN);
        
        doubleShot(DOUBLE_SHOT);
        
        task::sleep(20);
    }
}

int main() {
    int auton = autonomous();
    switch(auton){
        case 1:
            Competition.autonomous( RedInside );
            break;
            
        case 2:
            Competition.autonomous( RedOutside );
            break;
            
        case 3:
            Competition.autonomous( BlueInside );
            break;
            
        case 4:
            Competition.autonomous( BlueOutside );
            break;
    }
    
    Brain.Screen.clearScreen();
    
    Competition.drivercontrol( usercontrol );
    
    pre_auton();
    
    while(1) {
        task::sleep(100);
    }
}
