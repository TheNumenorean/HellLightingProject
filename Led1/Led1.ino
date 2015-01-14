#define LED1 7
#define LED2 6
#define LED3 5

int a;

void setup(){
  
  a = 1000;
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}

void loop(){
  
  
  
  digitalWrite(LED1,HIGH);
  delay(a);
  digitalWrite(LED2,HIGH);
  delay(a);
  digitalWrite(LED3,HIGH);
  delay(a);
  digitalWrite(LED1,LOW);
  delay(a);
  digitalWrite(LED2,LOW);
  delay(a);
  digitalWrite(LED3,LOW);
  delay(a);
  
  if (a > 100)
    a = a - 100;
  else if (a > 50) 
    a = a - 10;
  else if (a > 0)
    a = a - 5;
  else {
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED3,HIGH);
    delay(1000);
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    delay(1000);
    a = 1000;
  }
}
