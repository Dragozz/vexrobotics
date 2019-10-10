// To complete the VEXcode V5 Text project upgrade process, please follow the
// steps below.
// 
// 1. You can use the Robot Configuration window to recreate your V5 devices
//   - including any motors, sensors, 3-wire devices, and controllers.
// 
// 2. All previous code located in main.cpp has now been commented out. You
//   will need to migrate this code to the new "int main" structure created
//   below and keep in mind any new device names you may have set from the
//   Robot Configuration window. 
// 
// If you would like to go back to your original project, a complete backup
// of your original (pre-upgraded) project was created in a backup folder
// inside of this project's folder.

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----
// /*----------------------------------------------------------------------------*/
// /*                                                                            */
// /*    Module:       main.cpp                                                  */
// /*    Author:       C:\Users\delop                                            */
// /*    Created:      Tue Oct 01 2019                                           */
// /*    Description:  V5 project                                                */
// /*                                                                            */
// /*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;
 
 // define your global instances of motors and other devices here
 vex::motor leftMotor1 = vex::motor(vex::PORT1);
 vex::motor leftMotor2 = vex::motor(vex::PORT2);
 vex::motor rightMotor1 = vex::motor(vex::PORT3, true);
 vex::motor rightMotor2 = vex::motor(vex::PORT4, true);
 vex::motor intakeMotor1 = vex::motor(vex::PORT5);
 vex::motor intakeMotor2 = vex::motor(vex::PORT6);
 vex::motor armMotor = vex::motor(vex::PORT7);
 vex::motor leverMotor = vex::motor(vex::PORT8);
 vex::controller Controller = vex::controller();
 
 int intakeSpeed = 100;
 int armSpeed = 100;
 int leverSpeed = 100;
 bool intakeOn = false;
 void intake() {
   if(intakeOn) {
     intakeMotor1.stop();
     intakeMotor2.stop();
     intakeOn = false;
   }
   else if(!intakeOn) {
     intakeMotor1.spin(forward, intakeSpeed, vex::velocityUnits::pct);
     intakeMotor2.spin(forward, intakeSpeed, vex::velocityUnits::pct);
     intakeOn = true;
   }
 }
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  vex::task::sleep(2000);
     Brain.Screen.print("User Program has Started.");
     while(1) {
       //chassis movement
       leftMotor1.spin(forward, Controller.Axis3.position(), vex::velocityUnits::pct);
       leftMotor2.spin(forward, Controller.Axis3.position(), vex::velocityUnits::pct);
       rightMotor1.spin(forward, Controller.Axis2.position(), vex::velocityUnits::pct);
       rightMotor2.spin(forward, Controller.Axis2.position(), vex::velocityUnits::pct); 
       //toggle intake
       Controller.ButtonR1.pressed(intake);
       //manual control for arm
       if(Controller.ButtonL1.pressing()) {
         armMotor.spin(forward, armSpeed, vex::velocityUnits::pct);
       }
       else if(Controller.ButtonL2.pressing()) {
         armMotor.spin(reverse, armSpeed, vex::velocityUnits::pct);
       }
       else
         armMotor.stop();
       //move lever up to 90 degrees (needs finetuning)
       leverMotor.setPosition(0,degrees);
       if(Controller.ButtonUp.pressing()) {
         leverMotor.rotateFor(30, degrees);
       }
       //move lever back down (needs finetuning)
       if(Controller.ButtonDown.pressing()) {
         leverMotor.rotateFor(-30, degrees);
       }
       //manual control for lever
       if(Controller.ButtonX.pressing()) {
         leverMotor.spin(forward, leverSpeed, vex::velocityUnits::pct);
       }
       else if(Controller.ButtonB.pressing()) {
         leverMotor.spin(reverse, leverSpeed, vex::velocityUnits::pct);
       }
       else
         leverMotor.stop();
     }
}