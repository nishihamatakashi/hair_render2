#pragma once

#include"hr_math.h"

//通常のカラーマップ読み込み
extern void LoadImage(GLuint &tex, const char *name);

//高さマップの読み込み
extern void LoadHeight(GLuint &tex, const char *name, float nz);

//キューブマップ読み込み
extern void LoadCubeImage(GLuint &tex, const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz);

//テスト用
extern void LoadTestMap(GLuint &tex, const char *name);
