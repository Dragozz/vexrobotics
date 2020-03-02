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
#include <string.h>
#include <iostream>
#include <vex_triport.h>
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
vex::inertial GYRO = vex::inertial(vex::PORT4);
triport potPort(PORT22);
vex::pot potent = vex::pot(potPort.B);
triport cubePort(PORT22);
vex::line cubeSensor = vex::line(cubePort.A); //change this
vex::drivetrain chassis = vex::drivetrain(leftGroup, rightGroup);
int lYRequested, rYRequested = 0;
int intakeSpeed = 100;
int chassisSpeed = 100;
int armSpeed = 100;
double leverSpeed = 35;
int rate = 1;
int leverRate = 4;
bool intakeOn = false;
bool speed = false;
bool userControl = true;
int autonomousSelection = -1;
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
typedef struct _button {
  int xpos;
  int ypos;
  int width;
  int height;
  bool state;
  vex::color offColor;
  vex::color onColor;
  const char *label;
} button;
// Button array definitions for each software button. The purpose ofeach button data structure
// is defined above. The array size can be extended, so you can have as many buttons as you
// wish as long as it fits.
button buttons[] = { 
  { 30, 30, 60, 60, false, 0xE00000, 0x0000E0, "bBlue" },
  { 150, 30, 60, 60, false, 0xE00000, 0x0000E0, "fBlue" },
  { 270, 30, 60, 60, false, 0xE00000, 0x0000E0, "bRed" },
  { 390, 30, 60, 60, false, 0xE00000, 0x0000E0, "fRed" }
};
// forward ref
void displayButtonControls( int index, bool pressed );

/*-----------------------------------------------------------------------------*/
/** @brief      Check if touch is inside button                                */
/*-----------------------------------------------------------------------------*/
int findButton(  int16_t xpos, int16_t ypos ) {
    int nButtons = sizeof(buttons) / sizeof(button);

    for( int index=0;index < nButtons;index++) {
      button *pButton = &buttons[ index ];
      if( xpos < pButton->xpos || xpos > (pButton->xpos + pButton->width) )
        continue;

      if( ypos < pButton->ypos || ypos > (pButton->ypos + pButton->height) )
        continue;

      return(index);
    }
    return (-1);
}

/*-----------------------------------------------------------------------------*/
/** @brief      Init button states                                             */
/*-----------------------------------------------------------------------------*/
void initButtons() {
    int nButtons = sizeof(buttons) / sizeof(button);

    for( int index=0;index < nButtons;index++) {
      buttons[index].state = false;
    }
}

/*-----------------------------------------------------------------------------*/
/** @brief      Screen has been touched                                        */
/*-----------------------------------------------------------------------------*/
void userTouchCallbackPressed() {
    int index;
    int xpos = Brain.Screen.xPosition();
    int ypos = Brain.Screen.yPosition();

    if( (index = findButton( xpos, ypos )) >= 0 ) {
      displayButtonControls( index, true );
    }

}

/*-----------------------------------------------------------------------------*/
/** @brief      Screen has been (un)touched                                    */
/*-----------------------------------------------------------------------------*/
void userTouchCallbackReleased() {
    int index;
    int xpos = Brain.Screen.xPosition();
    int ypos = Brain.Screen.yPosition();

    if( (index = findButton( xpos, ypos )) >= 0 ) {
      // clear all buttons to false, ie. unselected
      //      initButtons(); 

      // now set this one as true
      if( buttons[index].state == true) {
      buttons[index].state = false; }
      else    {
      buttons[index].state = true;}

      // save as auton selection
      autonomousSelection = index;

      displayButtonControls( index, false );
    }
}

/*-----------------------------------------------------------------------------*/
/** @brief      Draw all buttons                                               */
/*-----------------------------------------------------------------------------*/
void displayButtonControls( int index, bool pressed ) {
    vex::color c;
    Brain.Screen.setPenColor( vex::color(0xe0e0e0) );

    for(int i=0;i<sizeof(buttons)/sizeof(button);i++) {

      if( buttons[i].state )
        c = buttons[i].onColor;
      else
        c = buttons[i].offColor;

      Brain.Screen.setFillColor( c );

      // button fill
      if( i == index && pressed == true ) {
        Brain.Screen.drawRectangle( buttons[i].xpos, buttons[i].ypos, buttons[i].width, buttons[i].height, c );
      }
      else
        Brain.Screen.drawRectangle( buttons[i].xpos, buttons[i].ypos, buttons[i].width, buttons[i].height );

      // outline
      Brain.Screen.drawRectangle( buttons[i].xpos, buttons[i].ypos, buttons[i].width, buttons[i].height, vex::color::transparent );

// draw label
      if(  buttons[i].label != NULL )
        Brain.Screen.printAt( buttons[i].xpos + 8, buttons[i].ypos + buttons[i].height - 8, buttons[i].label );
    }
}
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
// void slewRate() {
//   int lYLastSent = 0, rYLastSent = 0;
//   int lY, rY;
//   int slewRateLimit = 15, threshold = 15;
//   bool slowMode = false;
//   while(true){
//     lYRequested = leftGroup.velocity(velocityUnits::pct);
//     rYRequested = rightGroup.velocity(velocityUnits::pct);
//     if(abs(lYRequested - lYLastSent) > slewRateLimit){
//         if(lYRequested > lYLastSent) {
//           lY += slewRateLimit;
//         }else{
//           lY -= slewRateLimit;
//         }
//     }else{
//       lY = (lYRequested == 0) ? 0 : lY;
//     }

//     lYLastSent = lY;

//     if(abs(rYRequested - rYLastSent) > slewRateLimit) {
//       if(rYRequested > rYLastSent) {
//         rY+= slewRateLimit;
//       }else{
//         rY-=slewRateLimit;
//       }
//     }else{
//       rY = (rYRequested == 0) ? 0 : rY;
//     }

//     rYLastSent = rY;
  
//     leftGroup.setVelocity((abs(lY) > threshold) ? lY: 0, velocityUnits::pct);
//     rightGroup.setVelocity((abs(rY) > threshold) ? rY: 0, velocityUnits::pct);
//     wait(20, msec);

//   }

// }

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
  GYRO.calibrate();
  while(GYRO.isCalibrating()){
    wait(100, msec);
  }
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
int autonSlewLimit = 25;
void moveForward(double distance, int speed){
  chassis.setDriveVelocity(speed, percentUnits::pct);
  double offset = 2;
  while(offset!=0){
    offset = speed- chassis.velocity(percentUnits::pct);
    if(offset >= autonSlewLimit){
      chassis.setDriveVelocity(chassis.velocity(percentUnits::pct)+autonSlewLimit, percentUnits::pct);
    }else if(offset <= -autonSlewLimit){
      chassis.setDriveVelocity(chassis.velocity(percentUnits::pct) + autonSlewLimit, percentUnits::pct);
    }else{
      chassis.setDriveVelocity(chassis.velocity(percentUnits::pct)+offset, percentUnits::pct);
    }
    wait(20, msec);
  }
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
  intakeMotor1.spin(forward, intakeSpeed, vex::percentUnits::pct);
  intakeMotor2.spin(forward, intakeSpeed, vex::percentUnits::pct);
}
void releaseCubes() {
  intakeMotor1.rotateFor(-0.85, rev, intakeSpeed/2, vex::velocityUnits::pct, false);
  intakeMotor2.rotateFor(-0.85, rev, intakeSpeed/2, vex::velocityUnits::pct, false);
}
void releaseCubesSlow() {
  intakeMotor1.rotateFor(-0.1, rev, intakeSpeed/6, vex::velocityUnits::pct, false);
  intakeMotor2.rotateFor(-0.1, rev, intakeSpeed/6, vex::velocityUnits::pct, false);
}
void moveLeverUp() {
  double targetPos = 70;
  double scalar = 2;
  while(potent.angle() < 70) { //lever preset to stack
    double currentPos = potent.angle();
    double error = targetPos - currentPos;
    double speed = error*scalar;
    if(speed < 10)
      speed = 10;
    leverMotor.spin(forward, speed, percentUnits::pct);
  }
}
void moveLeverDown(double speed) {
  leverMotor.setVelocity(speed, percentUnits::pct);
  leverMotor.rotateTo(0, rev, false);
}
void moveArms(double speed, double distance, bool wait) {
  armMotor.rotateTo(distance, rev, speed, velocityUnits::pct, wait);
}
// void setRobotAngleFwd(double angleSet, double averageSpeed) {
//   errorH = angleSet - robotAngle;
//   leftGroup.spin(forward, (averageSpeed + errorH*kpAngle, pct));
//   rightGroup.spin(forward, (averageSpeed - errorH*kpAngle, pct));
// }
// void setRobotAngleRev(double angleSet, double averageSpeed) {
//   errorH = robotAngle - angleSet;;
//   leftGroup.spin(reverse, (averageSpeed + errorH*kpAngle, pct));
//   rightGroup.spin(reverse, (averageSpeed - errorH*kpAngle, pct));
// }
void turnRightGyro(int deg) {
  GYRO.resetRotation();
  while(GYRO.rotation(degrees) <= deg) {
    // leftGroup.spin(forward);
    // rightGroup.spin(reverse);
    chassis.turn(right);
  }
  // leftGroup.stop();
  // rightGroup.stop();
  chassis.stop();
}
void turnLeftGyro(int deg) {
  GYRO.resetRotation();
  while(GYRO.rotation(degrees) >= -deg) {
    // leftGroup.spin(reverse);
    // rightGroup.spin(forward);
    chassis.turn(left);
  }
  // leftGroup.stop();
  // rightGroup.stop();
  chassis.stop();
}
int autonSlewLimit = 20;
void moveForwardSlew(double distance){
    double speed = 0;
    leftGroup.rotateFor(distance, rev, speed, velocityUnits::pct, false);
    rightGroup.rotateFor(distance, rev, speed, velocityUnits::pct, false);
    while(leftGroup.position(rev) < distance/2) {
      leftGroup.setVelocity(leftGroup.velocity(velocityUnits::pct) + autonSlewLimit, velocityUnits::pct);
      rightGroup.setVelocity(rightGroup.velocity(velocityUnits::pct) + autonSlewLimit, velocityUnits::pct);
      speed += autonSlewLimit;
      wait(20, msec);
    }
    while(leftGroup.position(rev) < distance) {
      leftGroup.setVelocity(leftGroup.velocity(velocityUnits::pct) - autonSlewLimit, velocityUnits::pct);
      rightGroup.setVelocity(rightGroup.velocity(velocityUnits::pct) - autonSlewLimit, velocityUnits::pct);
      speed -= autonSlewLimit;
      wait(20, msec);
    }
}
void moveBackwardSlew(double distance, int speed){
  chassis.setDriveVelocity(speed, percentUnits::pct);
  chassis.driveFor(directionType::fwd, distance, distanceUnits::in, true);
  double offset = 2;
  while(offset!=0){
    offset = speed- chassis.velocity(percentUnits::pct);
    if(offset >= autonSlewLimit){
      chassis.setDriveVelocity(chassis.velocity(percentUnits::pct) + autonSlewLimit, percentUnits::pct);
    }else if(offset <= -autonSlewLimit){
      chassis.setDriveVelocity(chassis.velocity(percentUnits::pct) - autonSlewLimit, percentUnits::pct);
    }else{
      chassis.setDriveVelocity(chassis.velocity(percentUnits::pct)+offset, percentUnits::pct);
    }
    wait(20, msec);
  }
}
void intakeOneCube() {
  while(true) {
    double sensorValue = cubeSensor.value(analogUnits::range12bit);
    Controller.Screen.print(sensorValue);
    Controller.Screen.newLine();
    if(sensorValue < 2800 && sensorValue > 1) {
      Controller.Screen.print(sensorValue && "IN IFFFF");
      intakeMotor1.stop();
      intakeMotor2.stop();
      break;
    }
    else {
      intakeMotor1.spin(forward, intakeSpeed, vex::velocityUnits::pct);
      intakeMotor2.spin(forward, intakeSpeed, vex::velocityUnits::pct);
      chassis.drive(forward, 10, velocityUnits::pct);
    }
  }
}
void autonomous(void) {
  // GYRO.calibrate();
  // while(GYRO.isCalibrating()){
  //   wait(100, msec);
  // }
  // moveArms(armSpeed, -1, true);
  // moveArms(armSpeed, 0, true);
  intakeCubes();
    moveForward(60, 50);
    turnRightGyro(137);
    intakeMotor1.stop();
    intakeMotor2.stop();
    moveForward(42, 50);
    vex::task::sleep(500);
    releaseCubes();
    moveLeverUp();
    releaseCubes();
    moveForward(-10, 35);
    vex::task::sleep(250);
    moveLeverDown(70);
  //
  // intakeCubes();
  // moveForward(20.4, 40);
  // turnRightGyro(90);
  // moveForward(18, 40);
  // turnRightGyro(45);
  // intakeMotor1.stop();
  // intakeMotor2.stop();
  // moveForward(11, 40);
  // releaseCubes();
  // moveLeverUp();
  // releaseCubes();
  // moveForward(-10, 35);
}
void autonomouss(void) {
  bool bBlue = buttons[0].state;
  bool fBlue = buttons[1].state;
  bool bRed = buttons[2].state;
  bool fRed = buttons[3].state;
  if(bBlue) { //everything tested except arms at beginning
    moveArms(armSpeed, -1, true);
    moveArms(armSpeed, 0, true);
    intakeCubes();
    moveForward(55, 50);
    turnLeftGyro(136);
    intakeMotor1.stop();
    intakeMotor2.stop();
    moveForward(37, 50);
    vex::task::sleep(500);
    releaseCubes();
    moveLeverUp();
    releaseCubes();
    moveForward(-10, 35);
  }
  else if(fBlue) {
    moveArms(armSpeed, -1, true);
    moveArms(armSpeed, 0, true);
    intakeCubes();
    moveForward(20.4, 30);
    turnRight(61);
    moveForward(18, 30);
    vex::task::sleep(500);
    intakeMotor1.stop();
    intakeMotor2.stop();
    releaseCubes();
    turnRight(27);
    moveForward(11, 30);
    moveLeverUp();
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
  else if(bRed) {
    moveArms(armSpeed, -1, true);
    moveArms(armSpeed, 0, true);
    intakeCubes();
    moveForward(55, 50);
    turnRightGyro(136);
    intakeMotor1.stop();
    intakeMotor2.stop();
    moveForward(37, 50);
    vex::task::sleep(500);
    releaseCubes();
    moveLeverUp();
    releaseCubes();
    moveForward(-10, 35);
  }
  else if(fRed) {
    moveArms(armSpeed, -1, true);
    moveArms(armSpeed, 0, true);
    intakeCubes();
    moveForward(20.4, 30);
    turnLeft(61);
    moveForward(18, 30);
    vex::task::sleep(500);
    intakeMotor1.stop();
    intakeMotor2.stop();
    releaseCubes();
    turnLeft(27);
    moveForward(11, 30);
    //moveForward(-3, 60);
    moveLeverUp();
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
int slewRateLimit = 35;
double leftSlew(double axis){
  double difference = axis-leftGroup.velocity(velocityUnits::pct);
  if(difference >= slewRateLimit) {
    return leftGroup.velocity(velocityUnits::pct) + slewRateLimit;
  }
  else if(difference <= -slewRateLimit) {
    return leftGroup.velocity(velocityUnits::pct) - slewRateLimit;
  }
  else {
    return leftGroup.velocity(velocityUnits::pct) + difference;
  }
}
double rightSlew(double axis) {
  double difference = axis-rightGroup.velocity(velocityUnits::pct);
  if(difference >= slewRateLimit) {
     return rightGroup.velocity(velocityUnits::pct) + slewRateLimit;
  }
  else if(difference <= -slewRateLimit) {
    return rightGroup.velocity(velocityUnits::pct) - slewRateLimit;
  }
  else {
    return rightGroup.velocity(velocityUnits::pct) + difference;
  }
}
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
    leftGroup.spin(forward, Controller.Axis3.position(),vex::velocityUnits::pct);
    rightGroup.spin(forward, Controller.Axis2.position(),vex::velocityUnits::pct);
    // leftMotor1.spin(forward, Controller.Axis3.position(), vex::velocityUnits::pct);
    // leftMotor2.spin(forward, Controller.Axis3.position(), vex::velocityUnits::pct);
    // rightMotor1.spin(forward, Controller.Axis2.position(), vex::velocityUnits::pct);
    // rightMotor2.spin(forward, Controller.Axis2.position(), vex::velocityUnits::pct); 
    // task::resume(slewRate(Controller.Axis3.position(), Controller.Axis2.position()));
    //vex::thread(slewRate).detach();
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
      moveArms(armSpeed, -0.95, true);
    }
    // while(Controller.ButtonRight.pressing() && potent.angle() < 70) {
    //   leverMotor.spin(forward, leverSpeed, percentUnits::pct);
    // }
    if(Controller.ButtonUp.pressing()) { //tall tower
      moveArms(armSpeed, -1.25, true);
    }
    //move arm back down
    if(Controller.ButtonDown.pressing()) {
      moveArms(armSpeed, 0, true);
    }
    //manual control for lever
    double targetPos = 70;
    double scalar = 2;
    if(Controller.ButtonX.pressing()) {
      while(Controller.ButtonX.pressing() && potent.angle() < 70) { //lever preset to stack
        double currentPos = potent.angle();
        double error = targetPos - currentPos;
        double speed = error*scalar;
        if(speed < 10)
          speed = 10;
        leverMotor.spin(forward, speed, percentUnits::pct);
      }
    }
    else if(Controller.ButtonB.pressing()) {
      leverMotor.spin(reverse, 100, velocityUnits::pct);
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
//vex::thread *ourThread;
int main() {
  // Set up callbacks for autonomous and driver control periods.
  // lYRequested = Controller.Axis3.position();
  // rYRequested = Controller.Axis2.position();
  //ourThread = new vex::thread(slewRate);
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
// register events for button selection
    Brain.Screen.pressed( userTouchCallbackPressed );
    Brain.Screen.released( userTouchCallbackReleased );

    // make nice background
    Brain.Screen.setFillColor( vex::color(0x404040) );
    Brain.Screen.setPenColor( vex::color(0x404040) );
    Brain.Screen.drawRectangle( 0, 0, 480, 120 );
    Brain.Screen.setFillColor( vex::color(0x808080) );
    Brain.Screen.setPenColor( vex::color(0x808080) );
    Brain.Screen.drawRectangle( 0, 120, 480, 120 );

    // initial display
    displayButtonControls( 0, false );

    while(1) {
        // Allow other tasks to run
        if( !Competition.isEnabled() )
            Brain.Screen.setFont(fontType::mono40);
        Brain.Screen.setFillColor( vex::color(0xFFFFFF) );

        Brain.Screen.setPenColor( vex::color(0xc11f27));
        Brain.Screen.printAt( 0,  135, "  XLR8  " );
        this_thread::sleep_for(10);
    }
  // Run the pre-autonomous function.
  pre_auton();
  

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
