#include <iostream>
#include "Complex.h"
#include <cmath>

Complex::Complex() = default;

Complex::Complex(double x, double y)
  : x(x)
  , y(y)
  {
  }

Complex::~Complex()
{
}

Complex::Complex(const Complex& c)
  : x(c.x)
  , y(c.y)
  {
  }

  void Complex::set_param(double x, double y)
  {
    this->x = x;
    this->y = y;
  }

  void Complex::set_polar(double r, double angle)
  {
    if (r < 0)
    {
      throw("Error: 'r' must be positive\n");
    }
    else
    {
      this->x = r*cos(angle);
      this->y = r*sin(angle);
    }
  }

  double Complex::get_x() const { return x; }

  double Complex::get_y() const { return y; }

  Complex Complex::conjugate() const { return {x, -y}; }

  double Complex::modul() const { return pow((x*x + y*y), 0.5); }

  double Complex::angle() const
  {
    if (y > 0 && x == 0)
      return 1.57;
    if (y < 0 && x == 0)
      return 1.57*3;
    if (x > 0 && y == 0)
      return 0;
    if (x < 0 && y == 0)
      return 3.14;
    return acos(x/modul());
  }

  Complex& Complex::operator+=(const Complex& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  Complex& Complex::operator-=(const Complex& other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Complex operator+(Complex a, const Complex& b) 
  {
    a += b;
    return a;
  }

  Complex operator-(Complex a, const Complex& b)
  {
    a -= b;
    return a;
  }

  Complex operator*(Complex a, const Complex& b)
  {
    return Complex{a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x};
  }

  Complex& Complex::operator*=(const Complex& other)
  {
    *this = *this * other;
    return *this;
  }

  Complex operator/(const Complex a, const Complex b)
  {
    if (b.x != 0 || b.y != 0)
    {
      return Complex{(a.x*b.x + a.y*b.y)/(a.x*a.x + b.y*b.y), 
      (a.y*b.x - a.x*b.y)/(a.x*a.x + b.y*b.y)};
    }
    throw("Error: Dividing on zero!\n");
  }

  Complex& Complex::operator/=(const Complex other)
  {
    if (other.x != 0 || other.y != 0)
    {
      *this = *this / other;
      return *this;
    }
    throw("Error: Dividing on zero!\n");
  }

  std::ostream& operator<<(std::ostream& os, const Complex& a)
  {
    os << '(' << a.x << ", " << a.y << ")";
    return os;
  }

  std::istream& operator>>(std::istream& is, Complex& a)
  {
    is >> a.x >> a.y;
    return is;
  }

  bool Complex::operator==(const Complex& other) const { return modul() == other.modul(); }
  bool Complex::operator!=(const Complex& other) const { return modul() != other.modul(); }
  bool Complex::operator>(const Complex& other) const { return modul() > other.modul(); }
  bool Complex::operator<(const Complex& other) const { return modul() < other.modul(); }
  bool Complex::operator<=(const Complex& other) const { return modul() <= other.modul(); }
  bool Complex::operator>=(const Complex& other)  const { return modul() >= other.modul(); }
