#pragma once

struct CVector3f {
public:
  float x;
  float y;
  float z;

  inline CVector3f() : x(0), y(0), z(0) {};

  inline CVector3f(float x, float y, float z) : x(x), y(y), z(z) {};
};

struct CQuaternion {
public:
  float w;
  float x;
  float y;
  float z;

  inline CQuaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {};
};

struct CAxisAngle : CVector3f {
  inline CAxisAngle() : CVector3f() {}
  inline CAxisAngle(float x, float y, float z) : CVector3f(x, y, z) {}
};

class CTransform4f {
public:
  union {
    float matrix[12];
    float mtx[3][4];
    struct {
      float row1[3];
      float x;
      float row2[3];
      float y;
      float row3[3];
      float z;
    };
  };

  static constexpr inline CTransform4f Identity() {
    CTransform4f res;
    float mat[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    res.matrix[0] = mat[0];
    res.matrix[1] = mat[1];
    res.matrix[2] = mat[2];
    res.matrix[3] = mat[3];
    res.matrix[4] = mat[4];
    res.matrix[5] = mat[5];
    res.matrix[6] = mat[6];
    res.matrix[7] = mat[7];
    res.matrix[8] = mat[8];
    res.matrix[9] = mat[9];
    res.matrix[10] = mat[10];
    res.matrix[11] = mat[11];

    return res;
  }

  static CTransform4f Translate(float x, float y, float z);
  CTransform4f RotateX(float theta);
  CTransform4f RotateY(float theta);
  CTransform4f RotateZ(float theta);

  inline CVector3f origin() { return CVector3f(matrix[3], matrix[3 + 4], matrix[3 + 8]); };
};