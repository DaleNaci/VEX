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

void pre_auton( void ) {

}

int autonSelectorRedScreen( void ){
  Brain.Screen.clearScreen();

  Brain.Screen.drawRectangle(10, 10, 140, 75, color::red);
  Brain.Screen.drawRectangle(160, 10, 140, 75, color::red);

  Brain.Screen.printAt(31, 85, "1");
  Brain.Screen.printAt(183, 35, "2");

  while(true){
    if(Brain.Screen.pressing()){
      int xpos = Brain.Screen.xPosition();
      int ypos = Brain.Screen.yPosition();

      if(ypos >= 10 && ypos <= 85){
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

  Brain.Screen.printAt(31, 85, "1");
  Brain.Screen.printAt(183, 35, "2");

  while(true){
    if(Brain.Screen.pressing()){
      int xpos = Brain.Screen.xPosition();
      int ypos = Brain.Screen.yPosition();

      if(ypos >= 10 && ypos <= 85){
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

int autonSelectorMainScreen( void ) {
  Brain.Screen.clearScreen();

  Brain.Screen.drawRectangle(10, 10, 200, 150, color::red);
  Brain.Screen.drawRectangle(10, 170, 200, 150, color::blue);

  Brain.Screen.printAt(50, 50, "Red");
  Brain.Screen.printAt(250, 50, "Blue");

  while (true){
    if(Brain.Screen.pressing()){
      int xpos = Brain.Screen.xPosition();

      if(xpos >= 10 && xpos <= 160){
        return autonSelectorRedScreen();
      }
      else{
        return autonSelectorBlueScreen();
      }
    }
  }
}

void autonomous( void ) {
  int auton = autonSelectorMainScreen();
  switch(auton){
    case 1:
      break;
  }
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
      LauncherMotor.startRotateFor(directionType::fwd, 360, rotationUnits::deg);
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

  // 180 NUMBER MIGHT NEED FINE-TUNING
  LauncherMotor.startRotateFor(directionType::fwd, 180, rotationUnits::deg);
  LauncherMotor.resetRotation();

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
    //Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );

    pre_auton();

    while(1) {
      task::sleep(100);
    }
}
