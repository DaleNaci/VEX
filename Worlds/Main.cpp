#include "vex.h"
#include <sstream>

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

bool settingShot = false;

float autonTime = 0.0;

void pre_auton( void ) { }

void wait(int time) {
    task::sleep(time);
}

void puncherDraw(void) {
    LauncherMotor.rotateFor(directionType::fwd, 500, rotationUnits::deg, 100, velocityUnits::pct);
    LauncherMotor.resetRotation();
    puncherDrawback = true;
}

void autonPuncherDraw(void) {
    LauncherMotor.startRotateFor(directionType::fwd, 500, rotationUnits::deg, 100, velocityUnits::pct);
}

void gyroTurn(double angle) {
    
    const double ENDPOINT = g.value(rotationUnits::deg) + angle;
    
    double currentValue = g.value(rotationUnits::deg);
    double currentError = ENDPOINT - currentValue;
    double previousError = 0.00;
    
    while(fabs(currentError) > 1.80) {
        
        controller1.Screen.setCursor(1, 1);
        controller1.Screen.print(currentError);
        
        double kP = 0.4;
        double kD = 0.3;
        
        // ARNAV'S CONSTANTS
        // kP = 0.4
        // kD = 0.3
        
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

void turnNonGyro( float degrees ){
    const float errorFix = -4.0;
    int turnSpeed = 30;
    
    FrontRightMotor.startRotateFor(degrees * errorFix, rotationUnits::deg, turnSpeed, velocityUnits::pct);
    FrontLeftMotor.startRotateFor(-degrees * errorFix, rotationUnits::deg, turnSpeed, velocityUnits::pct);
    BackRightMotor.startRotateFor(degrees * errorFix, rotationUnits::deg, turnSpeed, velocityUnits::pct);
    BackLeftMotor.rotateFor(-degrees * errorFix, rotationUnits::deg, turnSpeed, velocityUnits::pct);
}

void driveFor( float tiles, int speed ){
    const float TILE_LENGTH = 24;
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
    RollerMotor.setVelocity(100, velocityUnits::pct);
    RollerMotor.startRotateFor(directionType::fwd, rot, rotationUnits::deg);
}

void rotateRollersRev(int rot) {
    RollerMotor.setVelocity(100, velocityUnits::pct);
    RollerMotor.startRotateFor(directionType::rev, rot, rotationUnits::deg);
}

void changeAngle(double rot) {
    AngleAdjusterMotor.rotateFor(directionType::fwd, rot, rotationUnits::deg, 100, velocityUnits::pct);
}

void autonLaunch() {
    LauncherMotor.rotateFor(directionType::fwd, 720, rotationUnits::deg, 100, velocityUnits::pct);
}

void autonShortLaunch() {
    LauncherMotor.rotateFor(directionType::fwd, 220, rotationUnits::deg, 100, velocityUnits::pct);
}

void startChangeAngle(double rot) {
    AngleAdjusterMotor.startRotateFor(directionType::fwd, rot, rotationUnits::deg);
}

void doubleShotChangeAngle(double rot) {
    AngleAdjusterMotor.rotateFor(directionType::fwd, rot, rotationUnits::deg, 100, velocityUnits::pct);
}

void doubleShotLauncher() {
    LauncherMotor.startRotateFor(directionType::fwd, 720, rotationUnits::deg, 100, velocityUnits::pct);
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
    autonPuncherDraw();
    startChangeAngle(-180);
    driveFor(1.5, 55);
    rotateRollers(1450);
    wait(90);
    driveFor(-1.72, 56);
    wait(24);
    driveFor(0.1, 20);
    rotateRollers(-540);
    gyroTurn(-88.9);
    wait(40);
    autonLaunch();
    wait(40);
    rotateRollers(1800);
    changeAngle(-220);
    wait(700);
    autonLaunch();
    wait(30);
    startChangeAngle(45);
    rotateRollers(2400);
    turnNonGyro(-3.40);
    wait(40);
    driveFor(1.89, 70);
    rotateRollers(1600);
    startChangeAngle(60);
    wait(50);
    turnNonGyro(-3.40);
    wait(20);
    driveFor(-1.03, 67); // THIS CODE DOESN'T GO FOR CAP
    turnNonGyro(54.0);
    rotateRollers(2000);
    wait(30);
    autonLaunch();
    /*turnNonGyro(30.0);
     rotateRollers(-2000);
     driveFor(1.5, 50);*/
}

void BlueInside ( void ){
    //3
    autonPuncherDraw();
    startChangeAngle(-180);
    driveFor(1.5, 52);
    rotateRollers(1420);
    wait(100);
    driveFor(-1.72, 54);
    wait(20);
    driveFor(0.1, 20);
    rotateRollers(-800);
    gyroTurn(87.2);
    wait(150);
    autonLaunch();
    rotateRollers(1650);
    changeAngle(-220);
    wait(280);
    autonLaunch();
    wait(35);
    startChangeAngle(45);
    rotateRollers(2400);
    turnNonGyro(3.4);
    driveFor(1.89, 68);
    rotateRollers(1600);
    startChangeAngle(60);
    wait(50);
    turnNonGyro(3.4);
    wait(100);
    driveFor(-1.03, 67);
    wait(50);
    turnNonGyro(-75);
    rotateRollers(2000);
    startChangeAngle(80);
    wait(30);
    rotateRollers(500);
    wait(30);
    autonPuncherDraw();
    autonLaunch();
}

void RedOutsideMiddle ( void ){
    //2
    driveFor(1.4, 30);
    startChangeAngle(-180);
    autonPuncherDraw();
    rotateRollers(800);
    wait(500);
    driveFor(-.34, 20);
    wait(500);
    gyroTurn(-83.1);
    autonLaunch();
    rotateRollers(2000);
    startChangeAngle(-180);
    wait(2000);
    autonShortLaunch();
    driveFor(-.9, 70);
    wait(100);
    turnNonGyro(95);
    rotateRollers(-3000);
    driveFor(.83, 30);
    driveFor(-.53, 30);
}

void RedOutsideDescore ( void ){
    //2
    driveFor(1.5, 30);
    startChangeAngle(-30);
    autonPuncherDraw();
    startChangeAngle(-190);
    rotateRollers(800);
    wait(500);
    driveFor(-.34, 20);
    wait(500);
    gyroTurn(-57.9);
    autonLaunch();
    rotateRollers(2000);
    startChangeAngle(-167);
    wait(2000);
    autonLaunch();
    driveFor(-.9, 70);
    wait(300);
    turnNonGyro(63);
    rotateRollers(-3000);
    driveFor(0.9, 40);
    driveFor(-0.5, 40);
}


void BlueOutsideMiddle ( void ){
    //4
    driveFor(1.4, 30);
    startChangeAngle(-30);
    autonPuncherDraw();
    startChangeAngle(-220);
    rotateRollers(800);
    wait(500);
    driveFor(-.34, 20);
    wait(500);
    gyroTurn(82.1);
    autonLaunch();
    rotateRollers(2000);
    startChangeAngle(-180);
    wait(2000);
    autonShortLaunch();
    driveFor(-.9, 70);
    wait(300);
    turnNonGyro(-98);
    rotateRollers(-3000);
    driveFor(0.9, 30);
    driveFor(-0.5, 30);
}

void BlueOutsideDescore ( void ){
    //4
    driveFor(1.5, 30);
    startChangeAngle(-30);
    autonPuncherDraw();
    startChangeAngle(-190);
    rotateRollers(800);
    wait(500);
    driveFor(-.34, 20);
    wait(500);
    gyroTurn(60.1);
    autonLaunch();
    rotateRollers(2000);
    startChangeAngle(-167);
    wait(2000);
    autonLaunch();
    driveFor(-.9, 70);
    wait(300);
    turnNonGyro(-68);
    rotateRollers(-3000);
    driveFor(0.9, 40);
    driveFor(-0.5, 40);
}


int autonomous( void ) {
    int auton = autonSelectorMainScreen();
    return auton;
}



void opDrive( controller::axis left, controller::axis right){
    FrontRightMotor.spin(directionType::fwd, left.value() -  right.value(), percentUnits::pct);
    FrontLeftMotor.spin(directionType::fwd, left.value() +  right.value(), percentUnits::pct);
    BackRightMotor.spin(directionType::fwd, left.value() - right.value(), percentUnits::pct);
    BackLeftMotor.spin(directionType::fwd, left.value() + right.value(), percentUnits::pct);
}

void launch(controller::button launchButton){
    launching = LauncherMotor.isSpinning();
    if (launchButton.pressing() && !launching) {
        LauncherMotor.startRotateFor(directionType::fwd, 720, rotationUnits::deg, 100, velocityUnits::pct);
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
    else if(!performingAction && !settingShot){
        AngleAdjusterMotor.stop(vex::brakeType::hold);
    }
}

void finishDS( void ){
    doubleShotChangeAngle(-340);
    performingAction = false;
}

//WORK ON
void doubleShot(controller::button R2) {
    if (!performingAction && R2.pressing()) {
        performingAction = true;
        autonShortLaunch();
        thread changing = thread(finishDS);
        autonPuncherDraw();
        int i = 0;
        while (performingAction && i < 20) {
            i++;
            wait(50);
        }
        performingAction = false;
    }
}

void settingHighFlag( void ) {
    changeAngle(400);
    settingShot = false;
}

void settingMidFlag ( void ) {
    changeAngle(-110);
}

void backDoubleShot(controller::button left) {
    if (!performingAction && left.pressing()) {
        performingAction = true;
        autonShortLaunch();
        thread setMidFlag = thread(settingMidFlag);
        autonPuncherDraw();
        int i = 0;
        while (performingAction && i < 20) {
            i++;
            wait(50);
        }
        performingAction = false;
        
    }
}

void backShot(controller::button up) {
    if (!performingAction && !settingShot && up.pressing()) {
        settingShot = true;
        thread setHighFlag = thread(settingHighFlag);
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
        
        controller::button DOUBLE_SHOT = controller1.ButtonR2;
        
        controller::button BACK_HIGH_FLAG = controller1.ButtonUp;
        controller::button BACK_DOUBLE_SHOT = controller1.ButtonLeft;
        
        opDrive(VERTICAL_AXIS, HORIZONTAL_AXIS);
        
        launch(LAUNCH_BUTTON);
        
        intake(INTAKE_IN, INTAKE_OUT);
        
        adjust(ANGLE_UP, ANGLE_DOWN);
        
        doubleShot(DOUBLE_SHOT);
        
        backShot(BACK_HIGH_FLAG);
        
        backDoubleShot(BACK_DOUBLE_SHOT);
        
        task::sleep(20);
    }
}

int main() {
    // int auton = autonomous();
    // switch(auton){
    //     case 1:
    //         Competition.autonomous( RedInside );
    //         break;
    
    //     case 2:
    //         Competition.autonomous( RedOutside );
    //         break;
    
    //     case 3:
    //         Competition.autonomous( BlueInside );
    //         break;
    
    //     case 4:
    //         Competition.autonomous( BlueOutside );
    //         break;
    // }
    
    Competition.autonomous ( BlueOutsideDescore );
    
    Brain.Screen.clearScreen();
    
    Competition.drivercontrol( usercontrol );
    
    pre_auton();
    
    while(1) {
        task::sleep(100);
    }
}
