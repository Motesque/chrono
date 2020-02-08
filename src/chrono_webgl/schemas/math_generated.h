// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_MATH_CHRONO_WEBGL_H_
#define FLATBUFFERS_GENERATED_MATH_CHRONO_WEBGL_H_

#include "flatbuffers/flatbuffers.h"

namespace chrono {
namespace webgl {

struct Vector3f;

struct Color;

struct Vector3d;

struct Matrix3x3f;

struct Matrix3x3d;

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Vector3f FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;
  float z_;

 public:
  Vector3f() {
    memset(static_cast<void *>(this), 0, sizeof(Vector3f));
  }
  Vector3f(float _x, float _y, float _z)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        z_(flatbuffers::EndianScalar(_z)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  float z() const {
    return flatbuffers::EndianScalar(z_);
  }
};
FLATBUFFERS_STRUCT_END(Vector3f, 12);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Color FLATBUFFERS_FINAL_CLASS {
 private:
  float r_;
  float g_;
  float b_;
  float a_;

 public:
  Color() {
    memset(static_cast<void *>(this), 0, sizeof(Color));
  }
  Color(float _r, float _g, float _b, float _a)
      : r_(flatbuffers::EndianScalar(_r)),
        g_(flatbuffers::EndianScalar(_g)),
        b_(flatbuffers::EndianScalar(_b)),
        a_(flatbuffers::EndianScalar(_a)) {
  }
  float r() const {
    return flatbuffers::EndianScalar(r_);
  }
  float g() const {
    return flatbuffers::EndianScalar(g_);
  }
  float b() const {
    return flatbuffers::EndianScalar(b_);
  }
  float a() const {
    return flatbuffers::EndianScalar(a_);
  }
};
FLATBUFFERS_STRUCT_END(Color, 16);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(8) Vector3d FLATBUFFERS_FINAL_CLASS {
 private:
  double x_;
  double y_;
  double z_;

 public:
  Vector3d() {
    memset(static_cast<void *>(this), 0, sizeof(Vector3d));
  }
  Vector3d(double _x, double _y, double _z)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        z_(flatbuffers::EndianScalar(_z)) {
  }
  double x() const {
    return flatbuffers::EndianScalar(x_);
  }
  double y() const {
    return flatbuffers::EndianScalar(y_);
  }
  double z() const {
    return flatbuffers::EndianScalar(z_);
  }
};
FLATBUFFERS_STRUCT_END(Vector3d, 24);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Matrix3x3f FLATBUFFERS_FINAL_CLASS {
 private:
  Vector3f _0_;
  Vector3f _1_;
  Vector3f _2_;

 public:
  Matrix3x3f() {
    memset(static_cast<void *>(this), 0, sizeof(Matrix3x3f));
  }
  Matrix3x3f(const Vector3f &__0, const Vector3f &__1, const Vector3f &__2)
      : _0_(__0),
        _1_(__1),
        _2_(__2) {
  }
  const Vector3f &_0() const {
    return _0_;
  }
  const Vector3f &_1() const {
    return _1_;
  }
  const Vector3f &_2() const {
    return _2_;
  }
};
FLATBUFFERS_STRUCT_END(Matrix3x3f, 36);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(8) Matrix3x3d FLATBUFFERS_FINAL_CLASS {
 private:
  Vector3d _0_;
  Vector3d _1_;
  Vector3d _2_;

 public:
  Matrix3x3d() {
    memset(static_cast<void *>(this), 0, sizeof(Matrix3x3d));
  }
  Matrix3x3d(const Vector3d &__0, const Vector3d &__1, const Vector3d &__2)
      : _0_(__0),
        _1_(__1),
        _2_(__2) {
  }
  const Vector3d &_0() const {
    return _0_;
  }
  const Vector3d &_1() const {
    return _1_;
  }
  const Vector3d &_2() const {
    return _2_;
  }
};
FLATBUFFERS_STRUCT_END(Matrix3x3d, 72);

}  // namespace webgl
}  // namespace chrono

#endif  // FLATBUFFERS_GENERATED_MATH_CHRONO_WEBGL_H_
