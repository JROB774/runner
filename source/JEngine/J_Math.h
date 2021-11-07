#ifndef INCLUDE_J_MATH
#define INCLUDE_J_MATH

class J_Vector
{
    public:

        float x, y;

        J_Vector() = default;
        J_Vector(const J_Vector&) = default;
        J_Vector(J_Vector&&) = default;
        J_Vector(const float, const float);

        J_Vector& operator=(const J_Vector&) = default;
        J_Vector& operator=(J_Vector&&) = default;

        J_Vector& operator+=(const J_Vector&);
        J_Vector& operator+=(const float);
        J_Vector& operator-=(const J_Vector&);
        J_Vector& operator-=(const float);
        J_Vector& operator/=(const J_Vector&);
        J_Vector& operator/=(const float);
        J_Vector& operator*=(const J_Vector&);
        J_Vector& operator*=(const float);

        void normalise();
        void rotate(const float);
        float length() const;
        float dot(const J_Vector&) const;

        ~J_Vector() = default;
};

inline J_Vector operator+(J_Vector, const J_Vector&);
inline J_Vector operator+(J_Vector, const float);
inline J_Vector operator-(J_Vector, const J_Vector&);
inline J_Vector operator-(J_Vector, const float);
inline J_Vector operator/(J_Vector, const J_Vector&);
inline J_Vector operator/(J_Vector, const float);
inline J_Vector operator*(J_Vector, const J_Vector&);
inline J_Vector operator*(J_Vector, const float);

inline bool operator==(const J_Vector&, const J_Vector&);
inline bool operator!=(const J_Vector&, const J_Vector&);
inline bool operator<(const J_Vector&, const J_Vector&);
inline bool operator>(const J_Vector&, const J_Vector&);
inline bool operator<=(const J_Vector&, const J_Vector&);
inline bool operator>=(const J_Vector&, const J_Vector&);



class J_Math
{
    public:

        static const float PI;

        static int random();
        static int random(const int, const int);

        static double toRadians(const double);
        static double toDegrees(const double);
};

#endif // INCLUDE_J_MATH
