
//fish

/* 
 * !WARNING!
 * Change "shift" variable ONLY if physicaly present with the code
 * The variable is responsible for the time before fish changes position
 * If shift TO BIG -> DAMAGE to physical gear system may occur
 * 
 * To big of a number forces gears to change to slowly and generates more force
 * This makes the mechanism grind on the gears if its already at its limit, damaging the fish
 * 
 * TIPS:
 * When gear system INSIDE fish -> "shift" between 50 - 150
 * When gear system NOT inside fish -> "shift" between 20 - 50
 */

int shift = 150; // 50 når fisken er IKKE på // 150 når fisken er på

// Normal variables
int Left = 2;
int Right = 4;
int t = 50; //was 50

//leds
#define led 6


//radio

/*
* This is definitions for the radio
* The radio is used to control the robot and fish
* The radio is connected to the following pins:
* - A0: radio ch 5
* - A1: radio ch 1
* - A2: radio ch 2
* - A3: radio ch 3
* - A4: radio ch 4
*/

#define pwm1_pin A0 //radio ch 5
#define pwm2_pin A1 //radio ch 1
#define pwm3_pin A2 //radio ch 2
#define pwm4_pin A3 //radio ch 3
#define pwm5_pin A4 //radio ch 4

int lastPwm[] = {0, 0, 0, 0, 0};

int radio[] = {0, 0, 0, 0, 0};


//motor
#define motor1_dir 8
#define motor1_dir2 7
#define motor2_dir 13
#define motor2_dir2 12

#define motor_pwm 9

//drive
int driveState = 0;


//pump
#define pump 5

void setup() {
    //fish
    pinMode(Left, OUTPUT);
    pinMode(Right, OUTPUT);

    //leds
    pinMode(led, OUTPUT);

    //radio
    pinMode(pwm1_pin, INPUT);
    pinMode(pwm2_pin, INPUT);
    pinMode(pwm3_pin, INPUT);
    pinMode(pwm4_pin, INPUT);
    pinMode(pwm5_pin, INPUT);

    //serial
    Serial.begin(9600);


    //motor pins output
    pinMode(motor1_dir, OUTPUT);
    pinMode(motor1_dir2, OUTPUT);
    pinMode(motor2_dir, OUTPUT);
    pinMode(motor2_dir2, OUTPUT);

    pinMode(motor_pwm, OUTPUT);

    //pump
    pinMode(pump, OUTPUT);
    pinMode(10, OUTPUT);
    digitalWrite(10, LOW); //enables pump

}

void loop() {
    //only flap fish if switch is flipped (can be changed to depend on throttle)
    if (pwmRead(0) > 0){
        fish();
    }else{
        digitalWrite(led, HIGH);
    }

    //pump
    if (pwmRead(1) > 100 || pwmRead(1) < -100){
        digitalWrite(pump, HIGH);
    }else{
        digitalWrite(pump, LOW);
    }
    

    //drive
    int speed = pwmRead(3);
    int dir = pwmRead(4);
    drive(speed, dir); //drive motor
    

}

//reads radio channel values and returns it as an int from -255 to 255
int pwmRead(int pin)
{
    //read pwm pulse width for each pin
    int pwm = 0;
    switch (pin)
    {
    case 0:
        //measure pulse length
        pwm = pulseIn(pwm1_pin, HIGH, 100000);
        break;
    case 1:
        pwm = pulseIn(pwm2_pin, HIGH, 100000);
        break;
    case 2:
        pwm = pulseIn(pwm3_pin, HIGH, 100000);
        break;
    case 3:
        pwm = pulseIn(pwm4_pin, HIGH, 100000);
        break;
    case 4:
        pwm = pulseIn(pwm5_pin, HIGH, 100000);
        break;
    }

    Serial.print("raw value: ");

    Serial.print(pwm);

    Serial.print("");

    // //some basic smoothing
    // if (pwm > lastPwm[pin])
    // {
    //     pwm = lastPwm[pin] + (pwm - lastPwm[pin]) / 2;
    // }
    // else
    // {
    //     pwm = lastPwm[pin] - (lastPwm[pin] - pwm) / 2;
    // }

    //check if pwm is within range of 1100-1900
    if (pwm < 1100)
    {
        pwm = 1100;
    }
    else if (pwm > 1900)
    {
        pwm = 1900;
    }
    
    lastPwm[pin] = pwm;

    //implement deadzone around centre
    int deadzone = 80;
    if (pwm < 1500 + deadzone && pwm > 1500 - deadzone)
    {
        pwm = 1500;
    }

    //map to more readable range and check if connected
    pwm = map(pwm, 1100, 1900, -255, 255);  
    
    //debug print
    Serial.print("Pwm on pin: ");
    Serial.print(pin);
    Serial.print(" is: ");
    Serial.println(pwm);

    return pwm;
}

//speed -255 - 255 dir Expects value between -255 and 255
void drive(int speed, int dir) 
{   
    if (speed > 0)
    {
        //forwards
        digitalWrite(motor1_dir, HIGH);
        digitalWrite(motor1_dir2, LOW);

        digitalWrite(motor2_dir, HIGH);
        digitalWrite(motor2_dir2, LOW);
        analogWrite(motor_pwm, speed);
    }else if(speed < 0){
        //backwards
        digitalWrite(motor1_dir2, HIGH);
        digitalWrite(motor1_dir, LOW);

        digitalWrite(motor2_dir2, HIGH);
        digitalWrite(motor2_dir, LOW);
        analogWrite(motor_pwm, -speed);
    }else{
        if (dir > 0)
        {
            //turn left
            digitalWrite(motor1_dir, HIGH);
            digitalWrite(motor1_dir2, LOW);

            digitalWrite(motor2_dir2, HIGH);
            digitalWrite(motor2_dir, LOW);
            analogWrite(motor_pwm, dir);
        }else if (dir < 0)
        {
            //turn right
            digitalWrite(motor1_dir2, HIGH);
            digitalWrite(motor1_dir, LOW);

            digitalWrite(motor2_dir, HIGH);
            digitalWrite(motor2_dir2, LOW);
            analogWrite(motor_pwm, -dir);
        }else{
            analogWrite(motor_pwm, 0);
        }
    }


}

//speed is from 0-255
void fish(){
    digitalWrite(Left, HIGH);
    digitalWrite(Right, LOW);
    digitalWrite(led, HIGH);
    delay(shift);    digitalWrite(Left, LOW);
    digitalWrite(Right, LOW);
    digitalWrite(led, LOW);
    delay(t);    digitalWrite(Left, LOW);
    digitalWrite(Right, HIGH);
    digitalWrite(led, HIGH);
    delay(shift);    digitalWrite(Left, LOW);
    digitalWrite(Right, LOW);
    digitalWrite(led, LOW);
    delay(t);
}