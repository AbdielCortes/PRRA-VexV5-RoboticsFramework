#include "movement.h"
#include "util.h"
#include "vex.h"

using namespace auton;

void Auton::move_group(vex::motor_group mg, double pow,
                       vex::velocityUnits units) {
  // assuming mg has same amount of motors on both sides

  mg.spin(directionType::fwd, pow, units);
}

void Auton::move_group_double(
    vex::motor_group left_mg, vex::motor_group right_mg, double pow,
    velocityUnits vel) { // autonomous movement of base indefinetely
  move_group(left_mg, pow, vel);
  move_group(right_mg, pow, vel);
}
/*************************************
TODO these functions have been copy pasted from older projects,
they still  need to be translated into this library's design language
*************************************/

void Auton::move_group_for(vex::motor_group left_mg, vex::motor_group right_mg,
                           double lim, vex::rotationUnits rot_units,
                           double speed, vex::velocityUnits vel_units) {
  // void motorRotateFor(vex::motor mL, vex::motor mR, float rotation,
  // rotationUnits units, int speed, velocityUnits units_v){
  left_mg.resetRotation();
  right_mg.resetRotation();

  double dir = 1;
  if (lim < 0)
    dir = -dir;

  lim = abs(lim);
  while (
      (lim - abs((left_mg.rotation(rot_units) + right_mg.rotation(rot_units)) /
                 2)) >= 0) {
    // find encoder difference
    double dVel =
        abs(left_mg.velocity(vel_units)) - abs(right_mg.velocity(vel_units));

    // depending on which motor is slower substract the speed difference from
    // faster motor
    if (dVel > 0) { // mL faster
      //  moveBaseL(dir*(speed - dVel), units_v);
      //  moveBaseR(dir*speed, units_v);

      move_group(left_mg, dir * (speed - dVel), vel_units);
      move_group(right_mg, dir * speed, vel_units);
    } else if (dVel < 0) { // mR faster
      //  moveBaseL(dir*speed, units_v);
      //  moveBaseR(dir*(speed - dVel), units_v);

      move_group(left_mg, dir * (speed - dVel), vel_units);
      move_group(right_mg, dir * speed, vel_units);
    } else { // speed is equal
      //  moveBaseL(dir*speed, units_v);
      //  moveBaseR(dir*speed, units_v);

      move_group(left_mg, dir * speed, vel_units);
      move_group(right_mg, dir * speed, vel_units);
    }

    task::sleep(5);
  }

  group_stop(left_mg, right_mg);
  // }
}

void Auton::move_group_for_bumper(vex::triport::port &sensor_port,
                                  vex::motor_group left_mg,
                                  vex::motor_group right_mg, double speed,
                                  vex::velocityUnits vel_units) {
  // int orig_val = bump.value();
  bumper bump = bumper(sensor_port);
  while (!bump.pressing()) {
    // while(1){
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(bump.value());

    move_group(left_mg, speed, vel_units);

    task::sleep(5);
  }

  group_stop(left_mg, right_mg);
}

// Moves motor groups in separete directions, speed sing dictates direction
void Auton::mech_rotate(vex::motor_group left_mg, vex::motor_group right_mg,
                        double lim, vex::rotationUnits rot_units, double speed,
                        vex::velocityUnits vel_units) {
  //   left_mg.resetRotation();
  // right_mg.resetRotation();
  //   if(lim < 0){
  //     speed = -speed;
  // }
  left_mg.rotateFor(lim, rot_units, abs(speed), vel_units,
                    false); // to change direction, change sign of input speed
  right_mg.rotateFor(-lim, rot_units, abs(speed), vel_units);

  group_stop(left_mg, right_mg);
}

void Auton::mech_rotate_gyro(vex::triport::port &sensor_port,
                             vex::motor_group left_mg,
                             vex::motor_group right_mg, 
                             double deg,
                             double speed, vex::velocityUnits vel_units) {

  gyro sensor = gyro(sensor_port);
  util::gyro_calibrate(sensor);
  task::sleep(1000); // jic
  sensor.resetHeading();
  // sensor.setHeading(0.0, rot_units);

  if (deg < 0) {//ccw
    deg = 360 + deg;
    sensor.setHeading(359.0, rotationUnits::deg);

    while (sensor.heading(rotationUnits::deg) >= deg) {
      move_group(left_mg, -speed, vel_units);
      move_group(right_mg, speed, vel_units);

      Brain.Screen.setCursor(1, 1);
      Brain.Screen.print(sensor.heading(rotationUnits::deg));

      task::sleep(5);
    }
  } else {//cw
    // if(rot_units == rotationUnits::deg) sensor.setHeading(359.0, rot_units);
    while (sensor.heading(rotationUnits::deg) <= deg) {
      move_group(left_mg, speed, vel_units);
      move_group(right_mg, -speed, vel_units);

      Brain.Screen.setCursor(1, 1);
      Brain.Screen.print(sensor.heading(rotationUnits::deg));

      task::sleep(5);
    }
  }

  group_stop(left_mg, right_mg, brakeType::hold);
}

// TODO define remaining methods from header file
//      Most of these methods can reuse the definitions from old_code.cpp

// Stops motor groups, depending on brake type can be used for base or arm
void Auton::group_stop(vex::motor_group left_mg, vex::motor_group right_mg,
                       brakeType brake_type) {
  left_mg.stop(brake_type);
  right_mg.stop(brake_type);
}