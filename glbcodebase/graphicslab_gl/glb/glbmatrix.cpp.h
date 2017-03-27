//-----------------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016/06/10
// Brief: Define the matrix math tools
//-----------------------------------------------------------------------------------------
namespace glb {

Matrix::Matrix() {
    float m[4][4] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f
    };
    memcpy(&m_Matrix, m, sizeof(m));
}

Matrix::~Matrix() {
}

void Matrix::MakeScaleMatrix(float sx, float sy, float sz) {
    float m[4][4] = {
        sx, 0.0f, 0.0f, 0.0f,
        0.0f, sy, 0.0f, 0.0f,
        0.0f, 0.0f, sz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    memcpy(&m_Matrix, m, sizeof(m));
}

void Matrix::MakeRotateXMatrix(float rotate_degree) {
    float ran = rotate_degree / 180.0f * 3.1415927f;
    float m[4][4] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cos(ran), -sin(ran), 0.0f,
        0.0f, sin(ran), cos(ran), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    memcpy(&m_Matrix, m, sizeof(m));
}

void Matrix::MakeRotateYMatrix(float rotate_degree) {
    float ran = rotate_degree / 180.0f * 3.1415927f;
    float m[4][4] = {
        cos(ran), 0.0f, sin(ran), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sin(ran), 0.0f, cos(ran), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    memcpy(&m_Matrix, m, sizeof(m));
}

void Matrix::MakeRotateZMatrix(float rotate_degree) {
    float ran = rotate_degree / 180.0f * 3.1415927f;
    float m[4][4] = {
        cos(ran), -sin(ran), 0.0f, 0.0f,
        sin(ran), cos(ran), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    memcpy(&m_Matrix, m, sizeof(m));
}

void Matrix::MakeTranslateMatrix(float tx, float ty, float tz) {
    float m[4][4] = {
        1.0f, 0.0f, 0.0f, tx,
        0.0f, 1.0f, 0.0f, ty,
        0.0f, 0.0f, 1.0f, tz,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
     memcpy(&m_Matrix, m, sizeof(m));
}

void Matrix::MakeRotateZXYMatrix(float rx, float ry, float rz) {
    Matrix rotate_x;
    rotate_x.MakeRotateXMatrix(rx);

    Matrix rotate_y;
    rotate_y.MakeRotateYMatrix(ry);

    Matrix rotate_z;
    rotate_z.MakeRotateZMatrix(rz);

    this->MakeIdentityMatrix();
    this->Mul(rotate_y);
    this->Mul(rotate_x);
    this->Mul(rotate_z);
}

void Matrix::MakeIdentityMatrix() {
    float m[4][4] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    memcpy(&m_Matrix, m, sizeof(m));
}

void Matrix::MakeRotateAxisMatrix(Vector axis, float rotate_degree) {
    float rad = rotate_degree / 180.0f * 3.14159f;
    m_Matrix.m[0][0] = cos(rad) + (1 - cos(rad)) * axis.x * axis.x;
    m_Matrix.m[0][1] = (1 - cos(rad)) * axis.x * axis.y - axis.z * sin(rad);
    m_Matrix.m[0][2] = (1 - cos(rad)) * axis.x * axis.z + axis.y * sin(rad);
    m_Matrix.m[0][3] = 0.0f;

    m_Matrix.m[1][0] = (1 - cos(rad)) * axis.x * axis.y + axis.z * sin(rad);
    m_Matrix.m[1][1] = cos(rad) + (1 - cos(rad)) * axis.y * axis.y;
    m_Matrix.m[1][2] = (1 - cos(rad)) * axis.y * axis.z - axis.x * sin(rad);
    m_Matrix.m[1][3] = 0.0f;

    m_Matrix.m[2][0] = (1 - cos(rad)) * axis.x * axis.z - axis.y * sin(rad);
    m_Matrix.m[2][1] = (1 - cos(rad)) * axis.y * axis.z + axis.x * sin(rad);
    m_Matrix.m[2][2] = cos(rad) + (1 - cos(rad)) * axis.z * axis.z;
    m_Matrix.m[2][3] = 0.0f;

    m_Matrix.m[3][0] = 0.0f;
    m_Matrix.m[3][1] = 0.0f;
    m_Matrix.m[3][2] = 0.0f;
    m_Matrix.m[3][3] = 1.0f;
}

void Matrix::MakeProjectionMatrix(float aspect, float fov, float znear, float zfar) {
    fov = fov * 0.5f / 180.0f  * 3.14159f;
    float sina = sin(fov);
    float cosa = cos(fov);
    float cota = cosa / sina;

    this->m_Matrix.v[0 * 4 + 0] = cota /  aspect;
    this->m_Matrix.v[0 * 4 + 1] = 0.0f;
    this->m_Matrix.v[0 * 4 + 2] = 0.0f;
    this->m_Matrix.v[0 * 4 + 3] = 0.0f;

    this->m_Matrix.v[1 * 4 + 0] = 0.0f;
    this->m_Matrix.v[1 * 4 + 1] = cota;
    this->m_Matrix.v[1 * 4 + 2] = 0.0f;
    this->m_Matrix.v[1 * 4 + 3] = 0.0f;

    this->m_Matrix.v[2 * 4 + 0] = 0.0f;
    this->m_Matrix.v[2 * 4 + 1] = 0.0f;
    this->m_Matrix.v[2 * 4 + 2] = - (zfar + znear) / (zfar - znear);
    this->m_Matrix.v[2 * 4 + 3] = -2.0f * zfar * znear / (zfar - znear);

    this->m_Matrix.v[3 * 4 + 0] = 0.0f;
    this->m_Matrix.v[3 * 4 + 1] = 0.0f;
    this->m_Matrix.v[3 * 4 + 2] = -1.0f;
    this->m_Matrix.v[3 * 4 + 3] = 0.0f;
}

void Matrix::MakeProjectionMatrix(float left, float right, float bottom, float top, float znear, float zfar) {
    this->m_Matrix.m[0][0] = 2 * znear / (right - left);
    this->m_Matrix.m[1][0] = 0.0f;
    this->m_Matrix.m[2][0] = 0.0f;
    this->m_Matrix.m[3][0] = 0.0f;

    this->m_Matrix.m[0][1] = 0.0f;
    this->m_Matrix.m[1][1] = 2 * znear / (top - bottom);
    this->m_Matrix.m[2][1] = 0.0f;
    this->m_Matrix.m[3][1] = 0.0f;

    this->m_Matrix.m[0][2] = (right + left) / (right - left);
    this->m_Matrix.m[1][2] = (top + bottom) / (top - bottom);
    this->m_Matrix.m[2][2] = - (zfar + znear) / (zfar - znear);
    this->m_Matrix.m[3][2] = -1.0f;

    this->m_Matrix.m[0][3] = 0.0f;
    this->m_Matrix.m[1][3] = 0.0f;
    this->m_Matrix.m[2][3] = -2 * zfar * znear / (zfar - znear);
    this->m_Matrix.m[3][3] = 0.0f;
}

void Matrix::MakeOrthogonalMatrix(float left, float right, float bottom, float top, float znear, float zfar) {
    this->m_Matrix.m[0][0] = 2 / (right - left);
    this->m_Matrix.m[1][0] = 0.0f;
    this->m_Matrix.m[2][0] = 0.0f;
    this->m_Matrix.m[3][0] = 0.0f;

    this->m_Matrix.m[0][1] = 0.0f;
    this->m_Matrix.m[1][1] = 2 / (top - bottom);
    this->m_Matrix.m[2][1] = 0.0f;
    this->m_Matrix.m[3][1] = 0.0f;

    this->m_Matrix.m[0][2] = 0.0f;
    this->m_Matrix.m[1][2] = 0.0f;
    this->m_Matrix.m[2][2] = -2 / (zfar - znear);
    this->m_Matrix.m[3][2] = 0.0f;

    this->m_Matrix.m[0][3] = -(right + left) / (right - left);
    this->m_Matrix.m[1][3] = -(top + bottom) / (top - bottom);
    this->m_Matrix.m[2][3] = -(zfar + znear) / (zfar - znear);
    this->m_Matrix.m[3][3] = 1.0f;
}

void Matrix::MakeViewMatrix(Vector pos, Vector target) {
    Vector z_axis = pos - target;
    z_axis.Normalize();

    Vector ref_y_axis(0.0f, 1.0f, 0.0f);
    Vector x_axis = Cross(ref_y_axis, z_axis);
    Vector y_axis = Cross(z_axis, x_axis);
    x_axis.Normalize();
    y_axis.Normalize();

    this->m_Matrix.m[0][0] = x_axis.x;
    this->m_Matrix.m[1][0] = y_axis.x;
    this->m_Matrix.m[2][0] = z_axis.x;
    this->m_Matrix.m[3][0] = 0.0f;
    
    this->m_Matrix.m[0][1] = x_axis.y;
    this->m_Matrix.m[1][1] = y_axis.y;
    this->m_Matrix.m[2][1] = z_axis.y;
    this->m_Matrix.m[3][1] = 0.0f;

    this->m_Matrix.m[0][2] = x_axis.z;
    this->m_Matrix.m[1][2] = y_axis.z;
    this->m_Matrix.m[2][2] = z_axis.z;
    this->m_Matrix.m[3][2] = 0.0f;

    this->m_Matrix.m[0][3] = - Dot(pos, x_axis);
    this->m_Matrix.m[1][3] = - Dot(pos, y_axis);
    this->m_Matrix.m[2][3] = - Dot(pos, z_axis);
    this->m_Matrix.m[3][3] = 1.0f;
}

void Matrix::MakeViewMatrix(Vector pos, Vector x_axis, Vector y_axis, Vector z_axis) {
    this->m_Matrix.m[0][0] = x_axis.x;
    this->m_Matrix.m[1][0] = y_axis.x;
    this->m_Matrix.m[2][0] = z_axis.x;
    this->m_Matrix.m[3][0] = 0.0f;
    
    this->m_Matrix.m[0][1] = x_axis.y;
    this->m_Matrix.m[1][1] = y_axis.y;
    this->m_Matrix.m[2][1] = z_axis.y;
    this->m_Matrix.m[3][1] = 0.0f;

    this->m_Matrix.m[0][2] = x_axis.z;
    this->m_Matrix.m[1][2] = y_axis.z;
    this->m_Matrix.m[2][2] = z_axis.z;
    this->m_Matrix.m[3][2] = 0.0f;

    this->m_Matrix.m[0][3] = - Dot(pos, x_axis);
    this->m_Matrix.m[1][3] = - Dot(pos, y_axis);
    this->m_Matrix.m[2][3] = - Dot(pos, z_axis);
    this->m_Matrix.m[3][3] = 1.0f;
}

void Matrix::Scale(float sx, float sy, float sz) {
    Matrix scale;
    scale.MakeScaleMatrix(sx, sy, sz);
    scale.Mul(*this);
    *this = scale;
}

void Matrix::RotateX(float rotate_degree) {
    Matrix rotate_x;
    rotate_x.MakeRotateXMatrix(rotate_degree);
    rotate_x.Mul(*this);
    *this = rotate_x;
}

void Matrix::RotateY(float rotate_degree) {
    Matrix rotate_y;
    rotate_y.MakeRotateYMatrix(rotate_degree);
    rotate_y.Mul(*this);
    *this = rotate_y;
}

void Matrix::RotateZ(float rotate_degree) {
    Matrix rotate_z;
    rotate_z.MakeRotateZMatrix(rotate_degree);
    rotate_z.Mul(*this);
    *this = rotate_z;
}

void Matrix::RotateZXY(float rx, float ry, float rz) {
    Matrix rotate_zxy;
    rotate_zxy.MakeRotateZXYMatrix(rx, ry, rz);
    rotate_zxy.Mul(*this);
    *this = rotate_zxy;
}

void Matrix::Translate(float tx, float ty, float tz) {
    Matrix trans;
    trans.MakeTranslateMatrix(tx, ty, tz);
    trans.Mul(*this);
    *this = trans;
}

void Matrix::Mul(const Matrix& m) {
    float result[4][4];
    for (int32_t i = 0; i < 4; i++) {
        for (int32_t j = 0; j < 4; j++) {
            result[i][j] = m_Matrix.m[i][0] * m.m_Matrix.m[0][j]
                                + m_Matrix.m[i][1] * m.m_Matrix.m[1][j]
                                + m_Matrix.m[i][2] * m.m_Matrix.m[2][j]
                                + m_Matrix.m[i][3] * m.m_Matrix.m[3][j];
        }
    }

    memcpy(&m_Matrix, result, sizeof(m_Matrix));
}

void Matrix::Transpose() {
    float result[4][4];
    for (int32_t i = 0; i < 4; i++) {
        for (int32_t j = 0; j < 4; j++) {
            result[i][j] = m_Matrix.m[j][i];
        }
    }

    memcpy(&m_Matrix, result, sizeof(m_Matrix));
}

void Matrix::Inverse() {
    float m00 = m_Matrix.m[0][0],m01 = m_Matrix.m[0][1],m02 = m_Matrix.m[0][2],m03 = m_Matrix.m[0][3];
    float m10 = m_Matrix.m[1][0],m11 = m_Matrix.m[1][1],m12 = m_Matrix.m[1][2],m13 = m_Matrix.m[1][3];
    float m20 = m_Matrix.m[2][0],m21 = m_Matrix.m[2][1],m22 = m_Matrix.m[2][2],m23 = m_Matrix.m[2][3];
    float m30 = m_Matrix.m[3][0],m31 = m_Matrix.m[3][1],m32 = m_Matrix.m[3][2],m33 = m_Matrix.m[3][3];

    float det = 0.f;
    float det00,det01,det02,det03;
    det00 = m11*(m22*m33-m32*m23)-m12*(m21*m33-m31*m23)+m13*(m21*m32-m31*m22);
    det01 = m10*(m22*m33-m32*m23)-m12*(m20*m33-m30*m23)+m13*(m20*m32-m30*m22);
    det02 = m10*(m21*m33-m31*m23)-m11*(m20*m33-m30*m23)+m13*(m20*m31-m30*m21);
    det03 = m10*(m21*m32-m31*m22)-m11*(m20*m32-m30*m22)+m12*(m20*m31-m30*m21);

    det = m00*det00-m01*det01+m02*det02-m03*det03;
    float Invdet = 1.f/det;
    det01*=-1;det03*=-1;

    float det10,det11,det12,det13;
    det10 = m01*(m22*m33-m32*m23)-m02*(m21*m33-m31*m23)+m03*(m21*m32-m31*m22);
    det11 = m00*(m22*m33-m32*m23)-m02*(m20*m33-m30*m23)+m03*(m20*m32-m30*m22);
    det12 = m00*(m21*m33-m31*m23)-m01*(m20*m33-m30*m23)+m03*(m20*m31-m30*m21);
    det13 = m00*(m21*m32-m31*m22)-m01*(m20*m32-m30*m22)+m02*(m20*m31-m30*m21);
    det10*=-1;det12*=-1;

    float det20,det21,det22,det23;
    det20 = m01*(m12*m33-m32*m13)-m02*(m11*m33-m31*m13)+m03*(m11*m32-m31*m12);
    det21 = m00*(m12*m33-m32*m13)-m02*(m10*m33-m30*m13)+m03*(m10*m32-m30*m12);
    det22 = m00*(m11*m33-m31*m13)-m01*(m10*m33-m30*m13)+m03*(m10*m31-m30*m11);
    det23 = m00*(m11*m32-m31*m12)-m01*(m10*m32-m30*m12)+m02*(m10*m31-m30*m11);
    det21*=-1;det23*=-1;

    float det30,det31,det32,det33;
    det30 = m01*(m12*m23-m22*m13)-m02*(m11*m23-m21*m13)+m03*(m11*m22-m21*m12);
    det31 = m00*(m12*m23-m22*m13)-m02*(m10*m23-m20*m13)+m03*(m10*m22-m20*m12);
    det32 = m00*(m11*m23-m21*m13)-m01*(m10*m23-m20*m13)+m03*(m10*m21-m20*m11);
    det33 = m00*(m11*m22-m21*m12)-m01*(m10*m22-m20*m12)+m02*(m10*m21-m20*m11);
    det30*=-1;det32*=-1;

    m_Matrix.m[0][0] = det00*Invdet;m_Matrix.m[1][0] = det01*Invdet;m_Matrix.m[2][0] = det02*Invdet;m_Matrix.m[3][0] = det03*Invdet;
    m_Matrix.m[0][1] = det10*Invdet;m_Matrix.m[1][1] = det11*Invdet;m_Matrix.m[2][1] = det12*Invdet;m_Matrix.m[3][1] = det13*Invdet;
    m_Matrix.m[0][2] = det20*Invdet;m_Matrix.m[1][2] = det21*Invdet;m_Matrix.m[2][2] = det22*Invdet;m_Matrix.m[3][2] = det23*Invdet;
    m_Matrix.m[0][3] = det30*Invdet;m_Matrix.m[1][3] = det31*Invdet;m_Matrix.m[2][3] = det32*Invdet;m_Matrix.m[3][3] = det33*Invdet;
}

const Vector Matrix::operator*(const Vector& v) {
    Vector result(0.0f, 0.0f, 0.0f);
    result.x = m_Matrix.m[0][0] * v.x + m_Matrix.m[0][1] * v.y + m_Matrix.m[0][2] * v.z + m_Matrix.m[0][3] * v.w;
    result.y = m_Matrix.m[1][0] * v.x + m_Matrix.m[1][1] * v.y + m_Matrix.m[1][2] * v.z + m_Matrix.m[1][3] * v.w;
    result.z = m_Matrix.m[2][0] * v.x + m_Matrix.m[2][1] * v.y + m_Matrix.m[2][2] * v.z + m_Matrix.m[2][3] * v.w;
    result.w = m_Matrix.m[3][0] * v.x + m_Matrix.m[3][1] * v.y + m_Matrix.m[3][2] * v.z + m_Matrix.m[3][3] * v.w;

    return result;
}

const float* Matrix::GetData() {
    return m_Matrix.v;
}

float Matrix::GetElement(int32_t x, int32_t y) {
    if (0 <= x && x <= 3
        && 0 <= y && y <= 3) {
        return m_Matrix.m[x][y];
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return 0.0f;  // Can not happen
}

};  // namespace glb
