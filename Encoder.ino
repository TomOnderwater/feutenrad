
void configureEncoder()
{
  pinMode(SS1, INPUT);
  pinMode(SS2, INPUT);
  pinMode(SS4, INPUT);
  pinMode(SS5, INPUT);

  // get pos
  lastTime = millis();
  lastSwitch = millis();
  block = digitalRead(SS1);
  pblock = block;
  pos = getPos();
  pPos = pos;
}

void printDebug()
{
  Serial.print("raw (guessed): ");
  Serial.print(wheelAngle());
  Serial.print(", pos (known): ");
  Serial.print(pos);
  Serial.println();
}

void updateWheel()
{
  checkCount();
  int testpos = getPos();
  if (pos != testpos)
  {
    //check if it is a neighbour, otherwise don't change pos
    int previous = pos == 1 ? 8 : pos - 1;
    int next = pos == 8 ? 1 : pos + 1;
    if (testpos == previous)
    {
      //pos is real and spinning down
      pPos = pos;
      pos = testpos;
      spinDir = -1;
      logSwitch();
      resetCount();
    }
    else if (testpos == next)
    {
      //pos is real and spinning up
      pPos = pos;
      pos = testpos;
      spinDir = 1;
      logSwitch();
      resetCount();
    }
  }
  if (millis() - lastSwitch > 2000) est_speed = 0.0f;
}

int distTo(int t)
{
  if (initDir == 1)
  {
    if (t > pos) return (t - pos);
    else return (t + (8 - pos));
  }
  else
  {
    //5 < 3 = 2
    if (t < pos) return pos - t;
    //
    else return (pos + (8 - t));
  }
}

void logSwitch()
{
  unsigned long timePassed = millis() - lastSwitch;
  lastSwitch = millis();
  if (timePassed < 2000)
  {
    est_speed = 125.0f / float(timePassed);
  }
}

int wheelAngle()
{
  int myangle = ((pos - 1) * EIGHTANGLE);
  if (spinDir == -1) myangle += EIGHTANGLE;
  if (count <= 1) myangle += spinDir * count * STEPANGLE;
  else myangle += STEPANGLE + (spinDir * count * BLOCKANGLE);
  return myangle;
}

bool checkCount()
{
  block = digitalRead(SS1);
  if (pblock != block)
  {
    pblock = block;
    count ++;
    return true;
  }
  return false;
}

float interpolate(float a, float b, int timethen, int timenow)
{
  if (timethen == 0) return a;
  float growth = (b - a) / float(timethen);
  return a + growth * float(timenow);
}

void resetCount()
{
  count = 0;
}

int getPos()
{
  uint8_t myRead = 0;

  myRead |= digitalRead(SS2) << 2;
  myRead |= digitalRead(SS4) << 1;
  myRead |= digitalRead(SS5);

  for (int i = 0; i < 8; i++)
  {
    if (myRead == greyvalues[i]) return i + 1;
  }
  return 0;
}
