namespace math {

    struct Vec2 {
        float x = 0.0f;
        float y = 0.0f;

        Vec2() = default;
        Vec2(float a, float b) : x(a), y(b) {}
    
        Vec2 operator+(const Vec2& other) const{
            return Vec2(x + other.x, y + other.y);
        }

        Vec2 operator-(const Vec2& other) const{
            return Vec2(x - other.x, y - other.y);
        }

        Vec2 operator*(float z) const{
            return Vec2(x * z, y *z);
        }

        Vec2 operator/(float z) const{
            return Vec2(x / z, y / z);
        }

        float dot(const Vec2& other) const{
            return x * other.x + y * other.y;
        }

        float cross(const Vec2& other) const{
            return x * other.y - y * other.x;
        }

        float length() const{
            return std::sqrt(x * x + y * y);
        }

        void normalize() {
            float len = length();
            if(len != 0.0f){
                x /= len;
                y /= len;
            }
        }
    };
}