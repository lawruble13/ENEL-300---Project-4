#include "car.h"
#include "FSM.h"

Car car;
State<Car> driveToWall;
State<Car> driveAlongWall;
State<Car> driveAroundCup;
State<Car> driveToCup;
State<Car> driveBack;

State<Car> unitTest;
State<Car> alongTest;
State<Car> aroundTest;
State<Car> findTest;

StateMachine<Car> SM;

void setup()
{
  pinMode(2, OUTPUT);
  car.attachServos(11,12);
  car.probeAttach(A0, 13);
  car.setAdj(1.0,0.625);
  car.colorAttach(5,4,3, A1);
  car.setBuzzer(6);
  driveToWall = State<Car>(
    &car,
    [](Car &resource){ resource.probeAngle(0); delay(1000); resource.probeAngle(180); delay(1000); resource.probeAngle(90); delay(1000); return; },
    [](Car &resource){ if(resource.checkDistance() > .1){resource.driveForward(.1);} else {SM.setState(driveAlongWall);} },
    [](Car &resource){ resource.turnRight(); resource.alignLeft(); }
  );
  driveAlongWall = State<Car>(
    &car,
    [](Car &resource){ resource.probeAngle(0); delay(1000); return; },
    [](Car &resource){ if(resource.checkDistance() < .2){resource.driveForward(.1);} else {resource.driveForward(.1); SM.setState(driveAroundCup);} },
    [](Car &resource){ resource.turnLeft(); }
  );
  driveAroundCup = State<Car>(
    &car,
    [](Car &resource){ resource.driveForward(.05); while(resource.checkDistance() > .2){resource.driveForward(.02);} },
    [](Car &resource){ for (int i = 0; i < 2; i++){while(resource.checkDistance() < .2){resource.driveForward(.02);} resource.driveForward(.05); resource.turnLeft(); while(resource.checkDistance() > .2){resource.driveForward(.02);}} resource.probeAngle(90); while(resource.checkDistance() > .05){resource.driveForward(.01);}SM.setState(driveToCup); },
    [](Car &resource){ resource.turnRight(); resource.alignLeft(); }
  );
  driveToCup = State<Car>(
    &car,
    [](Car &resource){ resource.probeAngle(90); },
    [](Car &resource){ if(resource.checkDistance() > .2){resource.driveForward(.1);} else if (resource.checkDistance() > .1){resource.driveForward(.05, 100);} else if(resource.checkDistance() > .05){resource.driveForward(.01, 50);} else {if(resource.isRed()){resource.alert(); SM.setState(driveBack);}} },
    [](Car &resource){ resource.driveBackward(0.1); resource.turnRight(180); }
  );
  driveBack = State<Car>(
    &car,
    [](Car &resource) {resource.driveForward(resource.distanceLeft);resource.turnRight();resource.driveForward(.49);resource.turnRight();resource.driveForward(resource.distanceRight-.14);resource.turnLeft();resource.driveForward(resource.distanceForward-.1);resource.turnRight();resource.turnRight();return;},
    [](Car &resource) {return;},
    [](Car &resource) {return;}
  );

  unitTest = State<Car>(
    &car,
    [](Car &resource){resource.probeAngle(0);resource.probeAngle(180);resource.probeAngle(90);resource.driveForward(.1);resource.distanceForward += .1; return;},
    [](Car &resource){;if(resource.getDistance() > 6){resource.driveForward(.01); resource.distanceForward += .01;} else {SM.setState(alongTest);}return;},
    [](Car &resource){return;}
  );

  alongTest = State<Car>(
    &car,
    [](Car &resource){resource.turnRight();resource.probeAngle(180);return;},
    [](Car &resource){if(resource.getDistance() < 4){resource.turnRight(2);} else if(resource.getDistance() > 6 && resource.getDistance() < 9){resource.turnLeft(2);} else if(resource.getDistance() > 9){SM.setState(findTest);} resource.driveForward(.05); resource.distanceRight += .05;return;},
    [](Car &resource){resource.driveForward(.07);resource.distanceRight += .07; resource.turnLeft(); resource.driveForward(.33);resource.turnLeft(); return;}
  );

  findTest = State<Car>(
    &car,
    [](Car &resource){resource.driveForward(.2);resource.distanceLeft+=.2;},
    [](Car &resource){if(resource.getDistance() > 8){resource.driveForward(.02);resource.distanceLeft+=.02;}else {resource.alert(); SM.setState(driveBack);}},
    [](Car &resource){resource.turnRight();resource.turnRight();return;}
  );

  
  SM = StateMachine<Car>(unitTest);
  Serial.begin(9600);
}

void loop()
{
  SM.doState();
}

