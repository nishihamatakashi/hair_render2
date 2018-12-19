#pragma once

/*
hair_renderで使う数学式，処理
*/

#include "hr_type.h"

//xをlow～highでクランプ
#define CLAMP(x, low, high) (x > high)? high : (x < low)? low : x

//行列情報
typedef struct
{
	float model[16];
	float view[16];
	float proj[16];
	float adjoint[16];
	float clip[16];
	float texture[16];
}Matrix;


//ある数字列をテクスチャに格納したときのサイズを計算(2^nで)
extern void calcResolution(const int n, int* size);

//マウスの(2D)移動から移動方向を算出する
extern void mouseTranslate(float x, float y, float *dir, float *p);

//マウスの(2D)移動から回転を算出する
extern void mouseRotate(float x, float y, float *dir, float *p0, float *p1);

//マウスのホイール移動から移動方向を算出する
extern void mouseZoom(float w, float *dir, float *p);

//座標値から空間登録するハッシュ関数
extern int setSpace_Hash(float x, float y, float z, int cell_num, float length);

//距離関数
extern float distance(float x, float y, float z);

//正規化関数
extern void Normalize(float *p);

//回転行列ではなく，座標を算出
extern void loadRotateSimple(float *p, float *p1, float *n, float a);

//球面線形保管
extern float *slerp(float *q1, float *q2, float *q3, float t);

//クォータニオンを算出
extern float *qmake(float *n, float a, float *q);

//単位行列化
extern float *loadIdentity(float *m);

//行列の掛け算
extern float *multiply(const float *m0, const float *m1, float *m);

//随伴行列の算出
extern float *adjoint(float *m0, float *m1);


//移動率から行列を算出(mが単位行列の時)
extern float *loadTranslate(float x, float y, float z, float *m);

//クォータニオンを計算(mが単位行列の時)
extern float *loadQuaternion(float *q, float *m);

//任意軸回転(mが単位行列の時)
extern float *loadRotate(float a, float *n, float *m);

//オイラー角回転(mが単位行列の時)
extern float *loadEulerRotate(const float *r, float *m);

//拡大縮小(mが単位行列の時)
extern float *loadScale(float x, float y, float z, float *m);

//ビュー変換行列を算出(mが単位行列の時)
extern float *loadLookat(const float *e, const float *g, const float *u, float *m);

//ビュー変換行列(Skybox用)を算出(mが単位行列の時)
extern float *loadSkyLookat(const float *e,const float *g,const float *u, float *rv);

//正視投影変換行列を算出(mが単位行列の時)
extern float *loadOrthogonal(float left, float right, float bottom, float top, float zNear, float zFar, float *m);

//透視投影変換行列を算出(mが単位行列の時)
extern float *loadFrustum(float left, float right, float bottom, float top, float zNear, float zFar, float *m);

//画角を用いた透視変換行列を算出(mが単位行列の時)
extern float *loadPerspective(float fovy, float aspect, float zNear, float zFar, float *m);

//移動率から行列を算出
extern float *Translate(float x, float y, float z, float *m);

//拡大縮小(mが単位行列の時)
extern float *Scale(float x, float y, float z, float *m);

//オイラー角回転
extern float *EulerRotate(const float *r, float *m);

//任意軸回転
extern float *Rotate(float a, float *n, float *m);

//ビュー変換行列を算出
extern float *Lookat(float *e, float *g, float *u, float *m);

//クォータニオンを計算
extern float *Quaternion(float *q, float *m);

//逆行列を算出する
extern float *Inverse(float *m0, float *m1);

//オブジェクトにある情報からmodel行列を算出する
extern float *CalcModelMatrix(float *t, float *r, float *s, float *m);