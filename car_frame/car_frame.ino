#include "car.h"

Car car;
State<Car> driveToWall;
State<Car> driveAlongWall;
State<Car> driveAroundCup;
State<Car> driveToCup;
State<Car> driveBack;
StateMachine<Car> SM;

void setup()
{
  
  driveToWall = State<Car>(
    car,
    [](Car &resource){ resource.probeAngle(0); resource.probeAngle(180); delay(1000); resource.probeAngle(90); delay(1000); return; },
    [](Car &resource){ if(resource.checkDistance() > .1){resource.driveForward(.1);} else {SM.setState(driveAlongWall);} },
    [](Car &resource){ resource.turnRight(); resource.alignLeft(); }
  );
  driveAlongWall = State<Car>(
    car,
    [](Car &resource){ resource.probeAngle(0); delay(1000); return; },
    [](Car &resource){ if(resource.checkDistance() < .2){resource.driveForward(.1);} else {resource.driveForward(.1); SM.setState(driveAroundCup);} },
    [](Car &resource){ resource.turnLeft(); }
  );
  driveAroundCup = State<Car>(
    car,
    [](Car &resource){ resource.driveForward(.05); while(resource.checkDistance() > .2){resource.driveForward(.02);} },
    [](Car &resource){ for (int i = 0; i < 2; i++){while(resource.checkDistance() < .2){resource.driveForward(.02);} resource.driveForward(.05); resource.turnLeft(); while(resource.checkDistance() > .2){resource.driveForward(.02);}} resource.probeAngle(90); while(resource.checkDistance() > .05){resource.driveForward(.01);}SM.setState(driveToCup); },
    [](Car &resource){ resource.turnRight(); resource.alignLeft(); }
  );
  driveToCup = State<Car>(
    car,
    [](Car &resource){ resource.probeAngle(90); },
    [](Car &resource){ if(resource.checkDistance() > .2){resource.driveForward(.1);} else if (resource.checkDistance() > .1){resource.driveForward(.05, 100);} else if(resource.checkDistance() > .05){resource.driveForward(.01, 50);} else {if(resource.isRed()){resource.alert(); SM.setState(driveBack);}} },
    [](Car &resource){ resource.driveBackward(0.1); resource.turnRight(180); }
  );
  driveBack = State<Car>(
    car,
    [](Car &resource) {return;},
    [](Car &resource) {return;},
    [](Car &resource) {return;}
  );
}

void loop()
{
  SM.doState();
}

