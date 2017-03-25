#include "car.h"
#include "FSM.h"

Car car;
State<Car> driveToWall;
State<Car> driveAlongWall;
State<Car> driveAroundCup;
State<Car> driveToCup;
State<Car> driveBack;
State<Car> avoidOutTest;

State<Car> unitTest;
State<Car> alongOutTest;
State<Car> aroundTest;
State<Car> findTest;
State<Car> alongBackTest;
State<Car> avoidBackTest;
State<Car> finalCountdownTest;
State<Car> endTest;

StateMachine<Car> SM;
int angleTurned = 0;

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
    [](Car &resource){;if(resource.getDistance() > 6){resource.driveForward(.01); resource.distanceForward += .01;} else {SM.setState(alongOutTest);}return;},
    [](Car &resource){return;}
  );

  alongOutTest = State<Car>(
    &car,
    [](Car &resource){resource.turnRight();resource.probeAngle(180);return;},
    [](Car &resource){if(resource.getDistance() < 4){resource.turnRight(2);} else if(resource.getDistance() > 6 && resource.getDistance() < 9){resource.turnLeft(2);} else if(resource.getDistance() > 9){SM.setState(avoidOutTest);} resource.driveForward(.01); resource.distanceRight += .01;return;},
    [](Car &resource){resource.driveForward(.07);resource.distanceRight += .07; resource.turnLeft(); resource.driveForward(.19); return;}
  );

  avoidOutTest = State<Car>(
    &car, 
    [](Car &resource){},
    [](Car &resource){resource.probeAngle(180); if(resource.getDistance()>6){resource.turnLeft(13);} resource.driveForward(.025);resource.probeAngle(90); if(resource.getDistance() < 8){SM.setState(findTest);}},
    [](Car &resource){resource.turnRight(25);resource.driveForward(.06);resource.turnRight(32.5);}
  );

  findTest = State<Car>(
    &car,
    [](Car &resource){},
    [](Car &resource){resource.probeAngle(90);delay(100);if(resource.getDistance() < 5){resource.alert();SM.setState(alongBackTest);return;}resource.probeAngle(180);delay(100);if(resource.getDistance() < 2){resource.turnRight(7);} else if(resource.getDistance() > 4 && resource.getDistance() < 9){resource.turnLeft(7);}resource.driveForward(.02); resource.distanceLeft += .02;},
    [](Car &resource){return;}
  );

  alongBackTest = State<Car>(
    &car,
    [](Car &resource){resource.probeAngle(0);resource.turnRight(-83);resource.driveBackward(.04);delay(250);resource.probeAngle(170);delay(250);if(resource.getDistance() > 8){resource.probeAngle(0);resource.driveForward(.035);resource.turnLeft(90);resource.driveBackward(.09);} else {resource.probeAngle(0);resource.driveForward(.00);resource.turnRight(-90);resource.driveForward(.14);angleTurned += 180; SM.setState(avoidBackTest);}},
    [](Car &resource){resource.probeAngle(90);delay(100);if(resource.getDistance() < 8){SM.setState(avoidBackTest);return;}resource.probeAngle(0);delay(100);if(resource.getDistance() < 2){resource.turnLeft(4);} else if(resource.getDistance() > 3 && resource.getDistance() < 9){resource.turnRight(4);}resource.driveForward(.02);},
    [](Car &resource){}
  );

  avoidBackTest = State<Car>(
    &car,
    [](Car &resource){resource.turnLeft(90-angleTurned/2);angleTurned+=(90-angleTurned/2);resource.probeAngle(0);},
    [](Car &resource){delay(50);if(resource.getDistance() > 9 && angleTurned < 300){resource.turnRight(min(15,300-angleTurned));angleTurned += min(15,300-angleTurned);} else if(angleTurned >= 300){SM.setState(finalCountdownTest);} resource.driveForward(.025);},
    [](Car &resource){}
  );

  finalCountdownTest = State<Car>(
    &car,
    [](Car &resource){resource.driveForward(.15);resource.turnRight(90);resource.probeAngle(0);resource.driveForward(.05);resource.distanceRight -= .05;},
    [](Car &resource){if(resource.getDistance() < 3){resource.turnLeft(5);} else if(resource.getDistance() > 5){resource.turnRight(5);}if(resource.distanceRight > 0){resource.driveForward(.01);resource.distanceRight -= .01;} else {SM.setState(endTest);}},
    [](Car &resource){resource.turnLeft(90);resource.driveForward(resource.distanceForward);resource.turnLeft(90);resource.turnLeft(90);}
  );

  endTest = State<Car>(
    &car,
    [](Car &resource){},
    [](Car &resource){},
    [](Car &resource){}
  );
  
  SM = StateMachine<Car>(unitTest);
  Serial.begin(9600);
}

void loop()
{
  SM.doState();
}

