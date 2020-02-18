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
#include <math.h>
//#include "vision.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
vex::motor leftMotor1 = vex::motor(vex::PORT8);
vex::motor leftMotor2 = vex::motor(vex::PORT10);
vex::motor rightMotor1 = vex::motor(vex::PORT7, true);
vex::motor rightMotor2 = vex::motor(vex::PORT9, true);
vex::motor intakeMotor1 = vex::motor(vex::PORT6); 
vex::motor intakeMotor2 = vex::motor(vex::PORT3, true); 
vex::motor armMotor = vex::motor(vex::PORT1, vex::gearSetting::ratio36_1); //torque motor
vex::motor leverMotor = vex::motor(vex::PORT2, vex::gearSetting::ratio36_1); //torque motor
vex::controller Controller = vex::controller(vex::controllerType::primary);
vex::controller Controller2 = vex::controller(vex::controllerType::partner);
vex::motor_group leftGroup = vex::motor_group(leftMotor1, leftMotor2);
vex::motor_group rightGroup = vex::motor_group(rightMotor1, rightMotor2);
vex::inertial GYRO = vex::inertial(vex::PORT22);
//vex::gyro GYRO = vex::gyro(Brain.ThreeWirePort.A);
vex::drivetrain chassis = vex::drivetrain(leftGroup, rightGroup);
int intakeSpeed = 100;
int chassisSpeed = 100;
int armSpeed = 100;
double leverSpeed = 35;
int rate = 1;
int leverRate = 4;
bool intakeOn = false;
bool speed = false;
bool userControl = true;
  // function to calculate biggest of 3 numbers
  // int max(int i, int j, int k) {
  //   if(i > j && i > k)
  //     return i;
  //   else if(j > k)
  //     return j;
  //   else
  //     return k;
  // }
  // function to toggle intake
  // void intake() {
  //   if(intakeOn) {
  //     intakeMotor1.stop();
  //     intakeMotor2.stop();
  //     intakeOn = false;
  //   }
  //   else if(!intakeOn) {
  //     intakeMotor1.spin(forward, intakeSpeed, vex::velocityUnits::pct);
  //     intakeMotor2.spin(forward, intakeSpeed, vex::velocityUnits::pct);
  //     intakeOn = true;
  //   }
  // }
 void leftPIDController(int setpoint) {
   double Kp = 0.0;
   double Ki = 0.0;
   double Kd = 0.0;
   double error;
   double integral = 0;
   double derivative;
   double prevError = 0;
   double power = 100;
   while(true) { //change true to some condition later
     leftGroup.setVelocity(power, pct);
     error = setpoint - leftGroup.position(rev);
     integral += error;
     if(error == 0 || leftGroup.position(rev) > setpoint)
        integral = 0;
     if(abs(error) > 40)
        integral = 0;
     derivative = error - prevError;
     prevError = error;
     power = error*Kp + integral*Ki + derivative*Kd;
     wait(20, msec);
   }
 }
 void rightPIDController(int setpoint) {
   double Kp = 0.0;
   double Ki = 0.0;
   double Kd = 0.0;
   double error;
   double integral = 0;
   double derivative;
   double prevError = 0;
   double power = 100;
   while(true) { //change true to some condition later
     rightGroup.setVelocity(power, pct);
     error = setpoint - leftGroup.position(rev);
     integral += error;
     if(error == 0 || leftGroup.position(rev) > setpoint)
        integral = 0;
     if(abs(error) > 40)
        integral = 0;
     derivative = error - prevError;
     prevError = error;
     power = error*Kp + integral*Ki + derivative*Kd;
     wait(20, msec);
   }
 }
int slewRate(int lYRequested, int rYRequested) {
  int lYLastSent = 0, rYLastSent = 0;
  int lY, rY;
  int slewRateLimit = 15, threshold = 15;
  bool slowMode = false;
  while(true){
    lYRequested = leftGroup.velocity(velocityUnits::pct);
    rYRequested = rightGroup.velocity(velocityUnits::pct);
    if(abs(lYRequested - lYLastSent) > slewRateLimit){
        if(lYRequested > lYLastSent) {
          lY += slewRateLimit;
        }else{
          lY -= slewRateLimit;
        }
    }else{
      lY = (lYRequested == 0) ? 0 : lY;
    }

    lYLastSent = lY;

    if(abs(rYRequested - rYLastSent) > slewRateLimit) {
      if(rYRequested > rYLastSent) {
        rY+= slewRateLimit;
      }else{
        rY-=slewRateLimit;
      }
    }else{
      rY = (rYRequested == 0) ? 0 : rY;
    }

    rYLastSent = rY;
  
    leftGroup.setVelocity((abs(lY) > threshold) ? lY: 0, velocityUnits::pct);
    rightGroup.setVelocity((abs(rY) > threshold) ? rY: 0, velocityUnits::pct);
    wait(20, msec);

  }
  return 0;

}

// function to toggle speed from 100% to 50% speed and vice versa
 void toggleSpeed() {
   if(speed) {
     rate = 1;
     Controller.Screen.clearScreen();
     Controller.Screen.setCursor(1, 1);
     Controller.Screen.newLine();
     Controller.Screen.print("\nNow at 100% speed.");
     speed = false;
   }
   else if(!speed) {
     rate = 4;
     Controller.Screen.clearScreen();
     Controller.Screen.setCursor(1, 1);
     Controller.Screen.newLine();
     Controller.Screen.print("\nNow at 25% speed.");
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
  // GYRO.calibrate();
  // while(GYRO.isCalibrating()){
  //   wait(100, msec);
  // }
  // GYRO.resetHeading();
  // GYRO.resetRotation();
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
double circumferenceOfRobot = 259.338;

double cube_joystick_val(double input){
    return pow(input/100.0, 3.0)*100.0;
}
void moveForward(double distance, int speed){
  chassis.setDriveVelocity(speed, percentUnits::pct);
  chassis.driveFor(directionType::fwd, distance, distanceUnits::in, true);
}
void turnRight(double distance) {
  chassis.turnFor(distance, rotationUnits::deg, true);
}
void turnLeft(double distance) {
  chassis.turnFor(-distance, rotationUnits::deg, true);
}
void turn(bool right, double revolutions){
  double revo = revolutions;
  if(!right)
    revo = -revolutions;
  leftMotor1.rotateFor(revo, rev, false);
  leftMotor2.rotateFor(revo, rev, false);
  rightMotor1.rotateFor(-revo, rev, false);
  rightMotor2.rotateFor(-revo, rev);
}
void intakeCubes(){
  intakeMotor1.spin(forward, intakeSpeed, vex::velocityUnits::pct);
  intakeMotor2.spin(forward, intakeSpeed, vex::velocityUnits::pct);
}
void releaseCubes() {
  intakeMotor1.rotateFor(-0.7, rev, intakeSpeed/2, vex::velocityUnits::pct, false);
  intakeMotor2.rotateFor(-0.7, rev, intakeSpeed/2, vex::velocityUnits::pct, false);
}
void releaseCubesSlow() {
  intakeMotor1.rotateFor(-0.1, rev, intakeSpeed/6, vex::velocityUnits::pct, false);
  intakeMotor2.rotateFor(-0.1, rev, intakeSpeed/6, vex::velocityUnits::pct, false);
}
void moveLeverUp(double speed) {
  leverMotor.setVelocity(speed, percentUnits::pct);
  //intakeMotor1.spin(reverse, intakeSpeed/8, vex::velocityUnits::pct);
  //intakeMotor2.spin(reverse, intakeSpeed/8, vex::velocityUnits::pct);
  leverMotor.rotateTo(1.57, rev, true);
}
void moveLeverDown(double speed) {
  leverMotor.setVelocity(speed, percentUnits::pct);
  leverMotor.rotateTo(0, rev, false);
}
void moveArms(double speed, double distance, bool wait) {
  armMotor.rotateTo(distance, rev, speed, velocityUnits::pct, wait);
}
void autonomous(void) { //original auton (at BOTB)
  moveArms(armSpeed, -1, true);
  moveArms(armSpeed, 0, true);
  intakeCubes();
  moveForward(35, 30);
  turnLeft(100);
  intakeMotor1.stop();
  intakeMotor2.stop();
  moveForward(24, 30);
  vex::task::sleep(500);
  releaseCubes();
  //turnRight(27);
  //moveForward(11, 30);
  //moveForward(-3, 60);
  releaseCubes();
  moveLeverUp(leverSpeed);
  vex::task::sleep(250);
  intakeMotor1.stop();
  intakeMotor2.stop();
  moveForward(1, 5);
  vex::task::sleep(250);
  intakeMotor1.spin(reverse, intakeSpeed/8, vex::velocityUnits::pct);
  intakeMotor2.spin(reverse, intakeSpeed/8, vex::velocityUnits::pct);
  moveForward(-15, 30);
  moveLeverDown(leverSpeed/4);
}
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
    leftMotor1.spin(forward, Controller.Axis3.position()/1.1/rate, vex::velocityUnits::pct);
    leftMotor2.spin(forward, Controller.Axis3.position()/1.1/rate, vex::velocityUnits::pct);
    rightMotor1.spin(forward, Controller.Axis2.position()/1.1/rate, vex::velocityUnits::pct);
    rightMotor2.spin(forward, Controller.Axis2.position()/1.1/rate, vex::velocityUnits::pct); 
    //task::resume(slewRate(Controller.Axis3.position(), Controller.Axis2.position()));
    //vex::task driving(slewRate(Controller.Axis3.position(), Controller.Axis2.position()));
    //toggle intake
    if(Controller.ButtonR1.pressing()) { //intake in
      intakeMotor1.spin(forward, intakeSpeed, vex::velocityUnits::pct);
      intakeMotor2.spin(forward, intakeSpeed, vex::velocityUnits::pct);
    }
    else if(Controller.ButtonR2.pressing()) { //intake out
      intakeMotor1.spin(reverse, intakeSpeed, vex::velocityUnits::pct);
      intakeMotor2.spin(reverse, intakeSpeed, vex::velocityUnits::pct);
    }
    else {
      intakeMotor1.stop();
      intakeMotor2.stop();
    }
    Controller.ButtonA.pressed(toggleSpeed);
    //manual control for arm
    if(Controller.ButtonL1.pressing()) { //arm up
      armMotor.spin(reverse, armSpeed, vex::velocityUnits::pct);
    }
    else if(Controller.ButtonL2.pressing()) { //arm down
      armMotor.spin(forward, armSpeed, vex::velocityUnits::pct);
    }
    else {
      armMotor.stop(brakeType::hold);
    }
    // //move lever up to 80 degrees 
    // if(Controller2.ButtonR1.pressing()) {
    //   moveLeverUp(leverSpeed);
    // }
    // //move lever back down to origin
    // else if(Controller2.ButtonR2.pressing()) {
    //   moveLeverDown(leverSpeed);
    // }
    //move arm up to preset height
    if(Controller.ButtonLeft.pressing()) { //small & medium tower
      moveArms(armSpeed, -0.9, true);
    }
    if(Controller.ButtonRight.pressing()) { //lever preset to stack
      moveLeverUp(leverSpeed);
    }
    if(Controller.ButtonUp.pressing()) { //tall tower
      moveArms(armSpeed, -1.25, true);
    }
    //move arm back down
    if(Controller.ButtonDown.pressing()) {
      moveArms(armSpeed, 0, true);
    }
    //manual control for lever
    if(Controller.ButtonX.pressing()) {
      if(leverMotor.position(rev) < 1.1)
        leverMotor.spin(forward, leverSpeed, velocityUnits::pct);
      else 
        leverMotor.spin(forward, leverSpeed/1.5, velocityUnits::pct);
    }
    else if(Controller.ButtonB.pressing()) {
      leverMotor.spin(reverse, leverSpeed, velocityUnits::pct);
    }
    else {
      leverMotor.stop(brakeType::hold);
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
