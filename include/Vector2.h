struct Vector2 {
  int x;
  int y;

  // default constructor
  Vector2(int x, int y) : x(x), y(y) {}

  // + operator
  Vector2 operator+(const Vector2 &other) const {
    return Vector2(x + other.x, y + other.y);
  }

  // - operator
  Vector2 operator-(const Vector2 &other) const {
    return Vector2(x - other.x, y - other.y);
  }
};
