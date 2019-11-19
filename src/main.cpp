/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
//#include "vision.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
vex::motor leftMotor1 = vex::motor(vex::PORT1);
vex::motor leftMotor2 = vex::motor(vex::PORT2);
vex::motor rightMotor1 = vex::motor(vex::PORT3, true);
vex::motor rightMotor2 = vex::motor(vex::PORT4, true);
vex::motor intakeMotor1 = vex::motor(vex::PORT5);
vex::motor intakeMotor2 = vex::motor(vex::PORT6, true);
vex::motor armMotor = vex::motor(vex::PORT7);
vex::motor leverMotor = vex::motor(vex::PORT8);
vex::controller Controller = vex::controller();
vex::motor_group leftGroup = vex::motor_group(leftMotor1, leftMotor2);
vex::motor_group rightGroup = vex::motor_group(rightMotor1, rightMotor2);
//vex::gyro GYRO = vex::gyro(Brain.ThreeWirePort.A);
//vex::smartdrive base = vex::smartdrive(leftGroup, rightGroup, GYRO, 319, 320, 130, distanceUnits::mm, 1);
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
     Controller.Screen.clearScreen();
     Controller.Screen.print("Now at 100% speed.");
     speed = false;
   }
   else if(!speed) {
     rate = 2;
     Controller.Screen.clearScreen();
     Controller.Screen.print("Now at 50% speed.");
     speed = true;
   }
 }
/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // GYRO.startCalibration();
  // while(GYRO.isCalibrating()){}
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  // //while(GYRO.value(rotationUnits::deg) < 90)
  // base.driveFor(directionType::fwd, 609.6, distanceUnits::mm);
  // base.turnFor(180, degrees);
  // armMotor.rotateFor(90, rotationUnits::rev);
  // base.turnFor(180, degrees);
  // vex::task::sleep(1000);
  // armMotor.rotateFor(-90, rotationUnits::rev);
  // base.driveFor(directionType::fwd, 100, distanceUnits::mm);
  // intakeMotor1.rotateFor(500, rotationUnits::rev);
  // intakeMotor2.rotateFor(500, rotationUnits::rev);
  // base.turnFor(-90, degrees);
  // base.driveFor(directionType::fwd, 1217, distanceUnits::mm);
  // base.turnFor(-90, degrees);
  // base.driveFor(directionType::fwd, 1217, distanceUnits::mm);
  // leverMotor.rotateFor(45, degrees);
}
//
//  void auton(void) { //auton with vs
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
//
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  Brain.Screen.print("User Program has Started.");
  Controller.Screen.clearScreen();
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................
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
    //move lever up to 90 degrees (needs finetuning)
    //leverMotor.setPosition(0,degrees);
    if(Controller.ButtonUp.pressing()) {
      leverMotor.rotateFor(-1000, rev, leverSpeed, vex::velocityUnits::pct, false);
      //leverMotor.resetPosition();
    }
    //move lever back down (needs finetuning)
    if(Controller.ButtonDown.pressing()) {
      leverMotor.rotateFor(1000, rev, leverSpeed, vex::velocityUnits::pct, false);
      //leverMotor.resetPosition();
    }
    //manual control for lever
    if(Controller.ButtonX.pressing()) {
      leverMotor.spin(reverse, leverSpeed, vex::percentUnits::pct);
    }
    else if(Controller.ButtonB.pressing()) {
      leverMotor.spin(forward, leverSpeed, vex::percentUnits::pct);
    }
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
