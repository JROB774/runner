/// J_VECTOR //////////////////////////////////////////////////////////////////

J_Vector::J_Vector (const float a_x, const float a_y)
{
    x = a_x, y = a_y;
}



J_Vector& J_Vector::operator+= (const J_Vector& a_vector) { x += a_vector.x, y += a_vector.y; return *this; }
J_Vector& J_Vector::operator+= (const float a_value) { x += a_value, y += a_value; return *this; }
J_Vector& J_Vector::operator-= (const J_Vector& a_vector) { x -= a_vector.x, y -= a_vector.y; return *this; }
J_Vector& J_Vector::operator-= (const float a_value) { x -= a_value, y -= a_value; return *this; }
J_Vector& J_Vector::operator/= (const J_Vector& a_vector) { x /= a_vector.x, y /= a_vector.y; return *this; }
J_Vector& J_Vector::operator/= (const float a_value) { x /= a_value, y /= a_value; return *this; }
J_Vector& J_Vector::operator*= (const J_Vector& a_vector) { x *= a_vector.x, y *= a_vector.y; return *this; }
J_Vector& J_Vector::operator*= (const float a_value) { x *= a_value, y *= a_value; return *this; }



void J_Vector::normalise ()
{
    x /= length(), y /= length();
}

void J_Vector::rotate (const float a_angle)
{
    x = (x * cos(a_angle)) - (y * sin(a_angle));
    y = (x * sin(a_angle)) + (y * cos(a_angle));
}

float J_Vector::length () const
{
    return static_cast <float> (sqrt((x * x) + (y * y)));
}

float J_Vector::dot (const J_Vector& a_vector) const
{
    return (x * a_vector.x) + (y * a_vector.y);
}



inline J_Vector operator+ (J_Vector a_lhs, const J_Vector& a_rhs) { a_lhs += a_rhs; return a_lhs; }
inline J_Vector operator+ (J_Vector a_lhs, const float a_rhs) { a_lhs += a_rhs; return a_lhs; }
inline J_Vector operator- (J_Vector a_lhs, const J_Vector& a_rhs) { a_lhs -= a_rhs; return a_lhs; }
inline J_Vector operator- (J_Vector a_lhs, const float a_rhs) { a_lhs -= a_rhs; return a_lhs; }
inline J_Vector operator/ (J_Vector a_lhs, const J_Vector& a_rhs) { a_lhs /= a_rhs; return a_lhs; }
inline J_Vector operator/ (J_Vector a_lhs, const float a_rhs) { a_lhs /= a_rhs; return a_lhs; }
inline J_Vector operator* (J_Vector a_lhs, const J_Vector& a_rhs) { a_lhs *= a_rhs; return a_lhs; }
inline J_Vector operator* (J_Vector a_lhs, const float a_rhs) { a_lhs *= a_rhs; return a_lhs; }



inline bool operator== (const J_Vector& a_lhs, const J_Vector& a_rhs) { return (a_lhs.length() == a_rhs.length()) ? true : false; }
inline bool operator!= (const J_Vector& a_lhs, const J_Vector& a_rhs) { return !operator==(a_lhs, a_rhs); }
inline bool operator< (const J_Vector& a_lhs, const J_Vector& a_rhs) { return (a_lhs.length() < a_rhs.length()) ? true : false; }
inline bool operator> (const J_Vector& a_lhs, const J_Vector& a_rhs) { return operator<(a_rhs, a_lhs); }
inline bool operator<= (const J_Vector& a_lhs, const J_Vector& a_rhs) { return !operator>(a_lhs, a_rhs); }
inline bool operator>= (const J_Vector& a_lhs, const J_Vector& a_rhs) { return !operator<(a_lhs, a_rhs); }

/// J_VECTOR //////////////////////////////////////////////////////////////////



/// J_MATH ////////////////////////////////////////////////////////////////////

const float J_Math::PI = 3.14159f;



int J_Math::random ()
{
    return rand();
}

int J_Math::random (const int a_min, const int a_max)
{
    return rand() % a_max + a_min;
}



float J_Math::toRadians (const float a_degrees)
{
    return (a_degrees * PI) / 180.0f;
}

float J_Math::toDegrees (const float a_radians)
{
    return (a_radians * 180.0f) / PI;
}



float J_Math::roundToMultiple (const float a_num, const float a_multiple)
{
    if (a_multiple == 0) { return a_num; }

    float rem = fmod(abs(a_num), a_multiple);
    if (rem == 0) { return a_num; }

    if (a_num < 0) { return -(abs(a_num) - rem); }
    else { return a_num + a_multiple - rem; }
}

/// J_MATH ////////////////////////////////////////////////////////////////////
