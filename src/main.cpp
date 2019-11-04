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
#include "vision.h"

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
 vex::motor_group leftGroup = vex::motor_group(leftMotor1, leftMotor2);
 vex::motor_group rightGroup = vex::motor_group(rightMotor1, rightMotor2);
 vex::gyro GYRO = vex::gyro(Brain.ThreeWirePort.A);
 vex::smartdrive base = vex::smartdrive(leftGroup, rightGroup, GYRO,319, 320, 130, distanceUnits::mm, 1);
 int intakeSpeed = 100;
 int armSpeed = 100;
 int leverSpeed = 100;
 int rate = 1;
 bool intakeOn = false;
 bool speed = false;
 bool userControl = true;
 int max(int i, int j, int k) {
   if(i > j && i > k)
    return i;
   else if(j > k)
    return j;
   else
    return k;
 }
 void auton(void) {
   
 }
//  void auton(void) {
//   vision::object *cube;
//   vision::signature colors[3] = {ORANGE, GREEN, PURPLE};
//   cube = &vs.largestObject;
//   for(int j = 0; j < 8; j++) {
//     //finds closest cube
//     vs.takeSnapshot(ORANGE);
//     int orangeWidth = cube-> width;
//     vs.takeSnapshot(GREEN);
//     int greenWidth = cube->width;
//     vs.takeSnapshot(PURPLE);
//     int purpleWidth = cube-> width;
//     int closestWidth = max(orangeWidth, greenWidth, purpleWidth); 
//     int i;
//     if(closestWidth == orangeWidth)
//       i = 0;
//     else if(closestWidth == greenWidth)
//       i = 1;
//     else
//       i = 2;
//     base.turn(right);
//     while(!cube->exists || cube->width < 20) {
//       vs.takeSnapshot(colors[i]);
//     }
//     base.stop(brake);
//     Controller.Screen.clearScreen();
//     Controller.Screen.print("Cube found!");
//     Controller.Screen.newLine();
//     Controller.Screen.print("Moving..."); 
//     //starts moving towards cube 
//     while(cube->width < 250) {
//       if(cube->centerX > 170)
//         base.turn(right);
//       else if(cube->centerX < 130) 
//         base.turn(left);
//       else
//         base.drive(forward);
//       vs.takeSnapshot(colors[i]);
//     }
//     base.stop(brake);
//     Controller.Screen.print("Cube reached.");
//     //intake cube
//     intakeMotor1.spin(forward, intakeSpeed, vex::velocityUnits::pct);
//     intakeMotor2.spin(forward, intakeSpeed, vex::velocityUnits::pct);
//     base.driveFor(forward, 150, mm);
//     }
//  }
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
 void toggleSpeed() {
   if(speed) {
     rate = 1;
     Controller.Screen.print("Now at 100% speed.");
     speed = false;
   }
   else if(!speed) {
     rate = 2;
     Controller.Screen.print("Now at 50% speed.");
     speed = true;
   }
 }
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  vex::task::sleep(1000);
     Brain.Screen.print("User Program has Started.");
     Controller.Screen.clearScreen();
     while(userControl) {
       //chassis movement
       leftMotor1.spin(forward, Controller.Axis3.position()/rate, vex::velocityUnits::pct);
       leftMotor2.spin(forward, Controller.Axis3.position()/rate, vex::velocityUnits::pct);
       rightMotor1.spin(forward, Controller.Axis2.position()/rate, vex::velocityUnits::pct);
       rightMotor2.spin(forward, Controller.Axis2.position()/rate, vex::velocityUnits::pct); 
       //toggle intake
       Controller.ButtonR1.pressed(intake);
       Controller.ButtonR2.pressed(toggleSpeed);
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
     //vs.takeSnapshot(ORANGE);
     //if(vs.largestObject.exists)
}