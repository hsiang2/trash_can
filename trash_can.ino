#include <Servo.h>

int TRIGGER_DISTANCE = 10;  //定義小於幾公分距離時, 就要打開蓋子
boolean DOOR_OPENED = 0; //目前蓋子是打開的嗎?

#define signalPin 12
Servo myservo;

void setup()
{
  Serial.begin(115200);
  myservo.attach(13); // 連接數位腳位13，伺服馬達的訊號線
}

int distance() {
  long cm;
  pinMode(signalPin, OUTPUT);
  digitalWrite(signalPin, LOW);
  delayMicroseconds(5);
  digitalWrite(signalPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(signalPin, LOW);
  pinMode(signalPin, INPUT);
  cm = pulseIn(signalPin, HIGH)/58;    
  delay(250);
  return cm;
}

//處理手接近時的動作
boolean handNear() {
    if(distance()<TRIGGER_DISTANCE){
      return 1;
     }
    else {
      return 0;
    }
  }

//處理手移開時的動作
boolean handLeave() {
    if(distance()>TRIGGER_DISTANCE) {
      return 1;
     }
     else{
      return 0;
     }
}

//關閉桶蓋的動作
void closeDoor(int numSpeed, int numDelay) {
  for(int i = 0; i <= 90; i+=numSpeed){
    myservo.write(i); // 使用write，傳入角度，從0度轉到180度
    DOOR_OPENED = 0;
    delay(numDelay);
  }
}

//打開桶蓋的動作
void openDoor(int numSpeed, int numDelay) {
  for(int i = 90; i >= 0; i-=numSpeed){
    myservo.write(i);// 使用write，傳入角度，從180度轉到0度
    DOOR_OPENED = 1;
    delay(numDelay);
  }
}

void loop()
{   
  //如果手還在垃圾筒上 (小於定義距離)
  if(handNear()==1) {
        openDoor(3, 10);
      do{
        delay(500);
        //如果不在垃圾筒上了 (大於定義距離)
        if(handLeave()==1)  closeDoor(2,15);
      } while (DOOR_OPENED==1);
  }
  delay(500);
}
