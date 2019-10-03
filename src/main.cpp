/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\delop                                            */
/*    Created:      Tue Oct 01 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;

// define your global instances of motors and other devices here
vex::motor leftMotor1 = vex::motor(vex::PORT1);
vex::motor leftMotor2 = vex::motor(vex::PORT2);
vex::motor rightMotor1 = vex::motor(vex::PORT3, true);
vex::motor rightMotor2 = vex::motor(vex::PORT4, true);
vex::motor intakeMotor1 = vex::motor(vex::PORT5);
vex::motor intakeMotor2 = vex::motor(vex::PORT6);
vex::motor armMotor = vex::motor(vex::PORT7);
vex::motor angleMotor = vex::motor(vex::PORT8);
vex::controller Controller = vex::controller();

int intakeSpeed = 100;
int armSpeed = 100;
int main() {
    vex::task::sleep(2000);
    Brain.Screen.print("User Program has Started.");
    while(1) {
      leftMotor1.spin(vex::directionType::fwd, Controller.Axis3.position(), vex::velocityUnits::pct);
      leftMotor2.spin(vex::directionType::fwd, Controller.Axis3.position(), vex::velocityUnits::pct);
      rightMotor1.spin(vex::directionType::fwd, Controller.Axis2.position(), vex::velocityUnits::pct);
      rightMotor2.spin(vex::directionType::fwd, Controller.Axis2.position(), vex::velocityUnits::pct); 
      if(Controller.ButtonR1.pressing()) {
        intakeMotor1.spin(vex::directionType::fwd, intakeSpeed, vex::velocityUnits::pct);
      }
      else{
        intakeMotor1.stop();
      }
      if(Controller.ButtonL1.pressing()) {
        armMotor.spin(vex::directionType::fwd, armSpeed, vex::velocityUnits::pct);
      }
    }
}
