/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */


//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio

// MOTOR PARAMETERS
#define   inA         2
#define   inB         3
#define   inC         4
#define   inD         7
#define   enA         5
#define   enB         6

int leftBaseSpeed   = 80;
int rightBaseSpeed  = 80;
int turnSpeed = 60 ;
int maxSpeed        = 255;

void wheel(int leftSpeed, int rightSpeed);
int readSensor(void);


// INFRARED PARAMETERS
#define integral_limit 50
int integral[integral_limit], intersection = 0;

int lineFollow(void);
int readSensor(void);
void maze(void);
#define Rturn 500


//SONAR PARAMETERS
#define trigF 8
#define trigL 10
#define echoF 11
#define echoL 13

long duration1, duration3;
int distF, distL;

int distance(void);
void wallFollow(void);


