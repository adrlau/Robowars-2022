//motor pins
#define motor1_dir 6
#define motor1_dir2 7
#define motor1_pwm 8
#define motor2_dir 9
#define motor2_dir2 10
#define motor2_pwm 11

//pwm read pins
#define pwm1_pin A0
#define pwm2_pin A1
#define pwm3_pin A2
#define pwm4_pin A3
#define pwm5_pin A4

void setup()
{
    //motor pins output
    pinMode(motor1_dir, OUTPUT);
    pinMode(motor1_dir2, OUTPUT);
    pinMode(motor1_pwm, OUTPUT);
    pinMode(motor2_dir, OUTPUT);
    pinMode(motor2_dir2, OUTPUT);
    pinMode(motor2_pwm, OUTPUT);

    //pwm read pins input
    pinMode(pwm1_pin, INPUT);
    pinMode(pwm2_pin, INPUT);
    pinMode(pwm3_pin, INPUT);
    pinMode(pwm4_pin, INPUT);
    pinMode(pwm5_pin, INPUT);


int pwmRead(int pin)
{
    int pwm = 0;
    switch (pin)
    {
    case 0:
        pwm = analogRead(pwm1_pin);
        break;
    case 1:
        pwm = analogRead(pwm2_pin);
        break;
    case 2:
        pwm = analogRead(pwm3_pin);
        break;
    case 3:
        pwm = analogRead(pwm4_pin);
        break;
    case 4:
        pwm = analogRead(pwm5_pin);
        break;
    }
    pwm = map(pwm, 1000, 2000, -255, 255);
    return pwm;
}

//drives the motors in tank drive mode. Expects value between -255 and 255
void drive(int motor1, int motor2)
{
    //motor1
    if (motor1 < 0)
    {
        digitalWrite(motor1_dir, LOW);
        digitalWrite(motor1_dir2, HIGH);
        analogWrite(motor1_pwm, -motor1);
    }
    else
    {
        digitalWrite(motor1_dir, HIGH);
        digitalWrite(motor1_dir2, LOW);
        analogWrite(motor1_pwm, motor1);
    }

    //motor 2
    if (motor2 < 0)
    {
        digitalWrite(motor2_dir, LOW);
        digitalWrite(motor2_dir2, HIGH);
        analogWrite(motor2_pwm, -motor2);
    }
    else
    {
        digitalWrite(motor2_dir, HIGH);
        digitalWrite(motor2_dir2, LOW);
        analogWrite(motor2_pwm, motor2);
    }
}

void loop()
{
    //mixing the pwm signals with master speed and direction to speed per motor.
    int motor1 = pwmRead(0)+pwmRead(1);
    int motor2 = pwmRead(2)-pwmRead(1);
    drive(motor1, motor2);
}