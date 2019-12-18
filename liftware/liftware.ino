//2019/12/08 手の震えを軽減するスプーンの作成（簡単のため今回は一軸）
//This code is written by Jin Yagi.

#include <Servo.h>
#include <Wire.h>

//簡単のためhttps://github.com/tttapa/Filtersをお借りしました．
#include <IIRFilter.h>

//６軸センサ（MPU6050）の設定
#define MPU6050_ADDR 0x68
#define MPU6050_AX  0x3B
#define MPU6050_AY  0x3D
#define MPU6050_AZ  0x3F
#define MPU6050_TP  0x41
#define MPU6050_GX  0x43
#define MPU6050_GY  0x45
#define MPU6050_GZ  0x47

short int AccX, AccY, AccZ;
short int Temp;
short int GyroX, GyroY, GyroZ;
int culcNum;

//サンプリング周波数100Hz，カットオフ周波数5Hzのバターワースフィルタ（ローパス）
//Matlabにて設計
const double b_coefficients[] = { 0.0201, 0.0402, 0.0201 };
const double a_coefficients[] = { 1.0000, -1.5610, 0.6414 };
IIRFilter iir(b_coefficients, a_coefficients);
IIRFilter iir2(b_coefficients, a_coefficients);

Servo myservo;

void setup() {
  Serial.begin(115200);
  Serial.println("### Serial started");
  Wire.begin();
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();
  myservo.attach(13);
  myservo.write(0);
}

void loop() {
  //６軸センサの処理
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_AX);
  Wire.endTransmission();
  Wire.requestFrom(MPU6050_ADDR, 14);
  
  AccX = Wire.read() << 8;  AccX |= Wire.read();
  AccY = Wire.read() << 8;  AccY |= Wire.read();
  AccZ = Wire.read() << 8;  AccZ |= Wire.read();
  Temp = Wire.read() << 8;  Temp |= Wire.read();
  GyroX = Wire.read() << 8; GyroX |= Wire.read();
  GyroY = Wire.read() << 8; GyroY |= Wire.read();
  GyroZ = Wire.read() << 8; GyroZ |= Wire.read();

  delay(10);//これによりサンプリング周波数100Hzとなる

  //フィルタを適用してフィルタを掛けた際の減衰を補正
  //今回はY,Z軸の加速度値のみを使用
  double filtered = iir.filter(AccY)*2.3;
  double filtered2 = iir2.filter(AccZ)*2.3;

  //正規化
  double acc_y = filtered / 16384.0;
  double acc_z = filtered2 / 16384.0;

  //加速度値から対地角に変換
  double acc_angle_y = atan2(acc_y, acc_z) * 360 / 2.0 / PI;

  //データの表示（ArduinoIDEのシリアルプロッタにはこれで描画される）
  double value = round(acc_angle_y);
  Serial.print("  ");Serial.print(value);Serial.println("");

  //サーボに計算した値を適用
  myservo.write((int)value);

  //フィルタを掛ける前と比較したいため，掛ける前のデータでも同じ処理をする
  double raw_acc_y = (double)AccY / 16384.0;
  double raw_acc_z = (double)AccZ / 16384.0;
  double raw_acc_angle_y = atan2(raw_acc_y, raw_acc_z) * 360 / 2.0 / PI;
  Serial.print("  "); Serial.print(round(raw_acc_angle_y));
}
