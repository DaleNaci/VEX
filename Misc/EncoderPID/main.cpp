#include "robot-config.h"

const float DIAMETER = 4.0;
const float CIRCUMFERENCE = DIAMETER * 3.1415269;

int inchToDegrees ( int inches ){
    float revolutions = inches / CIRCUMFERENCE;
    return revolutions * 360;
}

void pre_auton( void ){
    RightMotorFront.resetRotation();
}

void encoderDrive( double angle ){
    vex::task::sleep(200);
    
    const double ENDPOINT = RightMotorFront.rotation(vex::rotationUnits::deg) + angle;
    
    double currentValue = RightMotorFront.rotation(vex::rotationUnits::deg);
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
        
        RightMotorFront.spin(vex::directionType::fwd, p + i + d, vex::velocityUnits::pct);
        LeftMotorFront.spin(vex::directionType::rev, p + i + d, vex::velocityUnits::pct);
        RightMotorBack.spin(vex::directionType::fwd, p + i + d, vex::velocityUnits::pct);
        LeftMotorBack.spin(vex::directionType::rev, p + i + d, vex::velocityUnits::pct);
        
        currentValue = RightMotorFront.rotation(vex::rotationUnits::deg);
        totalError += previousError;
        
        if(totalError > INTEGRAL_LIMIT) totalError = INTEGRAL_LIMIT;
        if(totalError < -INTEGRAL_LIMIT) totalError = -INTEGRAL_LIMIT;
        
        vex::task::sleep(20);
    }
}

void autonomous ( void ){
    encoderDrive(10000);
}

void usercontrol( void ){
    
}

int main() {
    
    pre_auton();
    
    comp.autonomous( autonomous );
    comp.drivercontrol( usercontrol );
    
    
    while(true){
        vex::task::sleep(20);
    }
}
