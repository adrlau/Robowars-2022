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

int shift = 150; // 50 når fisken er IKKE på

// Normal variables
int Left = 2;
int Right = 4;
int t = 50;

void setup() {
  pinMode(Left, OUTPUT);
  pinMode(Right, OUTPUT);
}

void loop() {
  digitalWrite(Left, HIGH);
  digitalWrite(Right, LOW);
  delay(shift);

  digitalWrite(Left, LOW);
  digitalWrite(Right, LOW);
  delay(t);

  digitalWrite(Left, LOW);
  digitalWrite(Right, HIGH);
  delay(shift);

  digitalWrite(Left, LOW);
  digitalWrite(Right, LOW);
  delay(t);
}
