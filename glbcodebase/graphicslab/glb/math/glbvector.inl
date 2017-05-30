//-------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 10
// Brief: Define the vector tools
//-------------------------------------------------------------------------
Vector::Vector()
: x(0.0f)
, y(0.0f)
, z(0.0f) {
}

Vector::Vector(float x, float y, float z, float w)
: x(x)
, y(y)
, z(z)
, w(w) {
}

Vector::~Vector() {
}

void Vector::Set(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector Vector::operator-() {
    return Vector(-x, -y, -z, w);
}

Vector Vector::operator+(const Vector& v) {
    Vector result(0.0f, 0.0f, 0.0f);
    result.x = this->x + v.x;
    result.y = this->y + v.y;
    result.z = this->z + v.z;

    return result;
}

Vector Vector::operator-(const Vector& v) {
    Vector result(0.0f, 0.0f, 0.0f);
    result.x = this->x - v.x;
    result.y = this->y - v.y;
    result.z = this->z - v.z;

    return result;
}

Vector Vector::operator*(float v) {
    Vector result(0.0f, 0.0f, 0.0f);
    result.x = this->x * v;
    result.y = this->y * v;
    result.z = this->z * v;

    return result;
}

const Vector& Vector::operator+=(const Vector& v) {
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    return *this;
}

const Vector& Vector::operator/=(float v) {
    this->x /= v;
    this->y /= v;
    this->z /= v;
    return *this;
}

const Vector& Vector::operator*=(float v) {
    this->x *= v;
    this->y *= v;
    this->z *= v;
    return *this;
}

float Vector::Length() {
    return sqrtf(x * x + y * y + z * z);
}

float Vector::SqureLength() {
    return x * x + y * y + z * z;
}

void Vector::Normalize() {
    float len = this->Length();
    if (len <= 0.00001f) {
        len = 1.0f;
    }

    this->x /= len;
    this->y /= len;
    this->z /= len;
}

const Vector Cross(const Vector& v1, const Vector& v2) {
    Vector result(0.0f, 0.0f, 0.0f);
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;

    return result;
}

float Dot(const Vector& v1, const Vector& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector CalcRotateByLookVecZXY(const Vector& v) {
    // Calculate current pose
    Vector up(0.0f, 1.0f, 0.0f);
    Vector right = Cross(up, v);
    right.Normalize();
    up = Cross(v, right);
    up.Normalize();

    // Calculate rotate
    float rx = 0.0f, ry = 0.0f, rz = 0.0f;
    if (right.x != -1.0f && right.x != 1.0f) {
        float theta = -asin(right.z);
        float psi = atan2(up.z / cos(theta), v.z / cos(theta));
        float phi = atan2(right.y / cos(theta), right.x / cos(theta));
        rx = psi * 180.0f / 3.14159f;
        ry = theta * 180.0f / 3.14159f;
        rz = phi * 180.0f / 3.14159f;
    } else {
        float phi = 0.0f, theta = 0.0f, psi = 0.0f;
        if (right.x == -1.0f) {
            theta = 0.5f * 3.14159f;
            psi = phi + atan2(up.x, v.x);
        }
        else {
            theta = -0.5f * 3.14159f;
            psi = -phi + atan2(-up.x, -v.x);
        }
        rx = psi * 180.0f / 3.14159f;
        ry = theta * 180.0f / 3.14159f;
        rz = phi * 180.0f / 3.14159f;
    }

    return Vector(rx, ry, rz);
}
