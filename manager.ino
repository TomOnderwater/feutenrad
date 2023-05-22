bool newRoll()
{
  //Serial.println(ABS(est_speed));
  if (!spinning && checkSpin())
  {
    spinning = true;
    return true;
  }
  return false;
}

void spinup()
{
  engageWheel();
  driveMotor(255);
  hastask = true;
  Serial.println("going spinning");
  t.setTimeout([&]() {
    releaseWheel();
    driveMotor(0);
    hastask = false;
  }, 10000);
}

float ABS(float v)
{
  if (v < 0) return -v;
  else return v;
}
bool checkSpin()
{
  return (ABS(est_speed) > 0.4f);
}
//
bool waitEnded()
{
  if (waiting && (ABS(est_speed) < 0.4f))
  {
    Serial.println("waiting ended");
    waiting = false;
    return true;
  }
  return false;
}

void handleSpins()
{
  if (millis() > (timer + 20))
  {
    //first check if there's a task we're doing
    //if there's targets and we weren't spinning
    if (hastask) return;
    if (targetCount - targetsDone > 0)
    {
      if (newRoll())
      {
        t.setTimeout([&]() {
          if (checkSpin())
          {
            getNext();
            waiting = true;
            initDir = spinDir;
            Serial.println("spinning!");
          }
          else spinning = false;
        }, 1000);
      }
    }
    if (spinning)
    {
      if (waitEnded()) guiding = true;
      if (guiding) guideToTarget();
      if (targetReached()) endSpin();
    }
    timer = millis();
  }
}

void getNext()
{
  int count = 0;
  for (int i = 0; i < 8; i++)
  {
    if (turns[targetsDone].fields[i]) count++;
    else break;
  }
  int index = round(random(0, count));
  lastturn = turns[targetsDone];
  spinTarget = lastturn.fields[index];
  targetsDone ++;
  sendState();
}


bool onTarget()
{
  return ((pos == spinTarget) || freeSpin());
}

bool targetReached()
{
  if (guiding && (ABS(est_speed) < 0.03f) && onTarget())
  {
    Serial.println("target reached!");
    guiding = false;
    return true;
  }
  return false;
}

bool freeSpin()
{
  int count = 0;
  for (int i = 0; i < 8; i++)
  {
    if (lastturn.fields[i]) count++;
  }
  return (count == 8);
}
void guideToTarget()
{
  if (freeSpin()) return;
  engageWheel();
  if (!onTarget()) driveMotor(getResponse(spinTarget));
  else
  {
    driveMotor(200 * -initDir);
    t.setTimeout([&]()
    {
      brake();
    }, 250);
  }
  //if (!onTarget()) driveMotor(getResponse(spinTarget));
  //else brake();
}

int getResponse(int t)
{
  int p = distTo(t) * initDir * 125;
  int d = round(est_speed * float(initDir) * -300.0f);
  return constrain(p + d, -255, 255);
}

void endSpin()
{
  Serial.println("completed");
  brake();
  releaseWheel();
  spinning = false;
}

void clearTurns()
{
  targetCount = 0;
  for (int i = 0; i < 10; i++)
  {
    turns[i].t = 'n';
    for (int j = 0; j < 8; j++)
    {
      turns[i].fields[j] = 0;
    }
  }
}
