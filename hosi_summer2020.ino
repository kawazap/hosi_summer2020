#include "Motor.h"
#include <Servo.h>
#include <SoftwareSerial.h>
Servo servo;
#define SMIN 0
#define SMAX 90
#define MYRX 12 //デジタル12番ピンはソフトウェアシリアルRX
#define MYTX 11 //デジタル11番ピンはソフトウェアシリアルTX
int pos = 90;
SoftwareSerial mySerial(MYRX, MYTX);
unsigned char c[8];
unsigned long chksum;
//int direction(int x, int y){
//  double deg, rad;
//  int pattern = 0;
//  x -= 64;
//  y = abs(y-128)-64;
//  rad = atan2((double)y,(double)x);
//  deg = 180 + (rad*180.0/(atan(1.0)*4.0));
//  if(abs(x) > 16 || abs(y)){
//    for(int i = 0; i < 8; i++){
//      if(deg >= 0.0 + (i * 45.0) && deg < 45.0 + (i * 45.0))
//        pattern = i + 1;
//    }
//  }
//}
Motor moA(2,3);
Motor moB(4,5);
Motor moC(7,6);
Motor moD(23,22);
void setup() {
  mySerial.begin(2400);//SBDBTとArduinoは2400bps
  Serial.begin(19200);//シリアルモニター表示
  c[0] = 0x80; //SBDBTからのシリアル信号の１個目は固定。
  servo.attach(9);
  servo.write(pos);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(22,OUTPUT);
  pinMode(23,OUTPUT);
  pinMode(30,OUTPUT);
  pinMode(32,OUTPUT);
}

void Stop(){
 moA.run(0);
 moB.run(0);
 moC.run(0);
 moD.run(0);
}
void up(){
 moA.run(-100);
 moB.run(100);
 moC.run(100);
 moD.run(-100);
}
void right(){
 moA.run(-100);
 moB.run(-100);
 moC.run(100);
 moD.run(100);
}
void  down(){
 moA.run(100);
 moB.run(-100);
 moC.run(-100);
 moD.run(100);
}
void left(){
 moA.run(100);
 moB.run(100);
 moC.run(-100);
 moD.run(-100);
}
void turnR(){
 moA.run(-100);
 moB.run(100);
 moC.run(-100);
 moD.run(100);
}
void changeR(){
 moA.run(100);
 moB.run(100);
 moC.run(0);
 moD.run(0);
}
void turnL(){
 moA.run(100);
 moB.run(-100);
 moC.run(100);
 moD.run(-
 
 100);
}
void changeL(){
 moA.run(0);
 moB.run(0);
 moC.run(100);
 moD.run(100);
}
void loop() {
  //まずは無線からシリアルを読み込む。c[1]とc[2]にキー入力が格納される。
  int i;
  if (mySerial.available() >= 8) { //8byte以上あるかチェック
    if (mySerial.read() == 0x80) { //１byte読み込んで0x80のスタートビットかチェック
      Serial.print(c[0], HEX); //１６進数で数値を表示。
      Serial.print(",");//コンマで区切る。
      for (chksum = c[0], i = 1; i < 8; i++) { //スタートビットは読み込み済みなので、次の７個のデータを読み込む。
        c[i] = mySerial.read();
        if (i < 7) chksum += c[i];
        Serial.print(c[i], HEX); //１６進数で数値を表示。
        Serial.print(",");//}//コンマで区切る。
      }
      if (c[7] == (chksum & 0x7F)) { //ボタン部分のみのチェックサムを簡易計算してみる。
        Serial.println("check sum OK !");//チェックサムOKを表示。

      }else {
        Serial.println("check sum * * ERROR * *");//ダメならエラーを表示。
      }
      if (c[1] == 0x00 ) { //何も押されていなければ静止
        if (c[2] == 0x00 ) { //何も押されていなければ静止

          Serial.println("* STOP *");
          Stop();
        }
      }


      if ((c[2] & 0x01) == 0x01 && (c[2] & 0x02) == 0x02) {
        //if ((c[2] & 0x03 ) == 0x03 ) { //Start(上下同時押しはないと言う前提で書いてるので、注意！）
        Serial.println("Start");
      } else  if ((c[2] & 0x04) == 0x04 && (c[2] & 0x08) == 0x08) {//左右同時押しはないと言う前提で書いてるので、注意！）
        // if ((c[2] & 0x0C ) == 0x0C ) { //Select
        Serial.println("Select");
      } else {
        if ((c[2] & 0x01) == 0x01 ) { //上
          Serial.println("↑Up");    
          up();
        }
        else if ((c[2] & 0x02) == 0x02 ) { //下
          Serial.println("↓Down");
          down();
        }
        else if ((c[2] & 0x03 ) == 0x03 ) { //Start
          Serial.println("Start");
        }
        else if ((c[2] & 0x04 ) == 0x04 ) { //右
          Serial.println("→Right");
          right();
        }
        else if ((c[2] & 0x08 ) == 0x08 ) { //左
          Serial.println("←Left");
          left();
        }else  if ((c[2] & 0x10 ) == 0x10 ) { //三角
          //Serial.println("Triangle△");
          digitalWrite(30,HIGH);
          digitalWrite(32,LOW);
        }
       else  if ((c[2] & 0x20 ) == 0x20 ) { //バツ
          //Serial.println("×Cross");
           digitalWrite(30,LOW);
           digitalWrite(32,HIGH);
          }
        else if ((c[2] & 0x40 ) == 0x40 ) { //マル
          //Serial.println("○Circle");
           
          servo.write(180);
          delay(40);
        }
        else if ((c[1] & 0x01 ) == 0x01 ) { //四角
          //Serial.println("□Square");

          servo.write(0);
          delay(40);
        }
       else  if ((c[1] & 0x02 ) == 0x02 ) { //L1
          Serial.println("L1");
          turnL();
        }
        else if ((c[1] & 0x04 ) == 0x04 ) { //L2
          Serial.println("L2");
          changeL();
        }
        else if ((c[1] & 0x08 ) == 0x08 ) { //R1
          Serial.println("R1");
          turnR();
        }
        else if ((c[1] & 0x10 ) == 0x10 ) { //R2
          Serial.println("R2");
          changeR();
        }  
      }
    }
  }
}
