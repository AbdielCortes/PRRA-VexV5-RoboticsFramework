#ifndef BIGBOT_H
#define BIGBOT_H

#include "robot.h"

//Whoever uses library only needs to make this file to create their robot instance (inheriting from robot class)
class BigBot : public robot::Bot{
  private:

  motor BaseLeft1 = motor(PORT5, ratio18_1, false);
  motor BaseLeft2 = motor(PORT12, ratio18_1, false);
  motor BaseLeft3 = motor(PORT15, ratio18_1, false);
  motor BaseRight1 = motor(PORT4, ratio18_1, false);
  motor BaseRight2 = motor(PORT9, ratio18_1, false);
  motor BaseRight3 = motor(PORT10, ratio18_1, false);
  motor RampL = motor(PORT14, ratio18_1, false);
  motor RampR = motor(PORT20, ratio18_1, false);
  motor RollerL1 = motor(PORT1, ratio18_1, false);
  motor RollerL2 = motor(PORT2, ratio18_1, false);
  motor RollerR1 = motor(PORT18, ratio18_1, false);
  motor RollerR2 = motor(PORT19, ratio18_1, false);
  // led Piston = led(Brain.ThreeWirePort.A);
  triport t = triport( PORT22 );
  digital_out Piston = led(t.A);

  vex::motor_group base_left= motor_group(BaseLeft1, BaseLeft2, BaseLeft3);
  vex::motor_group base_right= motor_group(BaseRight1, BaseRight2, BaseRight3);

  vex::motor_group ramp_l= motor_group(RampL);
  vex::motor_group ramp_r= motor_group(RampR);

  vex::motor_group rollers_l= motor_group(RollerL1, RollerL2);
  vex::motor_group rollers_r= motor_group(RollerR1, RollerR2);

  // vex::digital_out digi[1] = {Piston};
  // robot::DigitalOutGroup pist = robot::DigitalOutGroup(digi, digi);
  // robot::DigitalOutGroup piston_group = pist;

  public:
    BigBot(util::Position& _pos);
    void driver() override;
    void auton() override;
    void move_base(double pow,
           velocityUnits vel = velocityUnits::pct) override;
    void rotate_base(double pow, velocityUnits vel = velocityUnits::pct) override;
    void grab(bool intake = true, float revs = 10.0) override;
};

#endif