
void engageWheel()
{
  servo.writeMicroseconds(ENGAGED);
}
void releaseWheel()
{
  servo.writeMicroseconds(FREE);
}
void setupMotor()
{
  pinMode(EN, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(R_PWM, OUTPUT);
}
void enableMotor() {
  digitalWrite(EN, HIGH);
}
void disableMotor()
{
  digitalWrite(EN, LOW);
  analogWrite(L_PWM, 0);
  analogWrite(R_PWM, 0);
}
void brake()
{
  digitalWrite(EN, HIGH);
  analogWrite(L_PWM, 0);
  analogWrite(R_PWM, 0);
}
void driveMotor(int spd)
{
  enableMotor();
  if (spd == 0) {
    brake();
  }
  else if (spd > 0)
  {
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, spd);
  }
  else {
    analogWrite(L_PWM, 0);
    analogWrite(R_PWM, abs(spd));
  }
}
