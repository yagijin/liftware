# liftware  
手が震える人に向けたスプーンである，GoogleのLiftwareを自分で作ってみました．  

# liftwareとは  
https://youtu.be/p9oc91GZguA  

LiftwareはGoogleのデバイスで，手が震えて食事に支障が出る方に向けたスプーンです．  
手の振動を軽減して食事を食べやすくします．  

素晴らしいアイデアだと思って興味が湧いたので，実際に作ってみようと思いました．  

## 使用するもの  
今回は簡単のためサーボモータ一つで一軸で作ってみました．  
使用したものは，ArduinoUno,MPU6050(６軸センサ)，サーボモータです．  

## 構成  
1. ６軸センサから得た加速度値（今回はY軸とZ軸）を使用して対地角を計算します．  
2. この際に得た加速度値に対して，ローパスフィルタを適用します．  
3. 今回使用したフィルタは，IIRのバターワースフィルタで係数は3次でサンプリング周波数100Hz,カットオフ周波数5Hzです．  
4. ローパスフィルタによって，スプーンを動かす大きい動きは残り，細かい手の震えは消滅します．  
5. この計算後の値をモータに適用すれば完成です．  

## 実際の動画  

[![liftware](http://img.youtube.com/vi/6sHJKby-OgM/default.jpg)](https://youtu.be/6sHJKby-OgM)

## 震えの振動が打ち消されている様子  
<img width="691" alt="serialplot" src="https://user-images.githubusercontent.com/29916489/71187904-5a77f700-22c3-11ea-80fb-a17e7e9bfcb9.png">