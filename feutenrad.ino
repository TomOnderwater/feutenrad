#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include "config.h"
#include "web.h"

#include <AsyncTimer.h>
AsyncTimer t;

#include <Servo.h>

WebsocketsClient client;

#define SERVOPIN D1
#define ENGAGED 2000
#define FREE 500

// MOTOR
#define EN D2
#define L_PWM D3
#define R_PWM D4

#define MAXSPEED 100

// ENCODER
#define SS1   D8
#define SS2   D7
#define SS4   D6
#define SS5   D5
bool block, pblock;
int count = 0;
int spinDir = 1;
int pos, pPos;

unsigned long lastTime, lastSwitch;
float p_est_speed = 0.0f, est_speed = 0.0f; //rough speed
float p_spd = 0.0f, spd = 0.0f;

#define FULLANGLE 576 * 2
#define EIGHTANGLE 144
#define BLOCKANGLE 18
#define STEPANGLE 9

const float BLOCKFRACTION = float(BLOCKANGLE) / float(FULLANGLE);

unsigned long timer;

Servo servo;

// GAME RELATED

struct turn {
  char t;
  uint8_t fields[8];
};

struct turn lastturn;
struct turn turns[10];

int targetCount = 0;
int targetsDone = 0;
bool spinning = false;
uint8_t spinTarget = 0;
int initDir = 1;

//TASKS
bool hastask = false;
bool waiting = false;
bool guiding = false;
bool rotating = false;

const uint8_t greyvalues[] =
{
  7, 3, 1, 5, 4, 0, 2, 6
};

void setup() {
  Serial.begin(115200);
  timer = millis();

  randomSeed(analogRead(A0)); //new seed

  clearTurns();

  setupMotor();
  enableMotor();

  //driveMotor(-255);

  servo.attach(SERVOPIN);
  releaseWheel();

  configureAP();

  configureEncoder();

  Serial.println("Setup Completed");
}

void updateLoop()
{
  t.handle();
  //update the  wheel at ALL times, as fast as possible
  updateWheel();
  handleSpins();
}

void loop() {
  updateLoop();
  client = server.accept();
  client.onMessage(handle_message);
  while (client.available()) {
    client.poll();
    updateLoop();
  }
}
