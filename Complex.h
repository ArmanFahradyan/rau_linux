#pragma once

class Complex // Complex numbers are defined as ordered pairs of real numbers, so 'x' is the first 
{             // real number, 'y' is the second one
private:
  double x;
  double y;

public:
  Complex(); //default constructer

  Complex(double x, double y=0.0); // constructer that recieves one or two arguments corresponding 
                                  // x and y (in case of one argument y recieves value 0)
  ~Complex(); //destructer

  Complex(const Complex& c); // copy constructer that recieves another object of our type Complex 
                            // as an argument and creates Complex number that has the same 'x' and 'y' 
  void set_param(double x, double y); // a function for changing 'x' and 'y' of our Complex number
                                      // that recieves parametric coordinates as arguments

  void set_polar(double r, double angle); // a function for changing 'x' and 'y' of our Complex
					  // number that recieves polar coordinates as arguments
  double get_x() const; // returns the value of x

  double get_y() const; // returns the value of y

  Complex conjugate() const; // returns a Complex number, wich is conjugate to our Complex number
                            // in another way if our Complex number is (a, b), its conjugate is 
                            // (a, -b)
  double modul() const; // returns the modul of our Complex number that is square root of 
                        // (a^2 + b^2), if our Complex number is (a, b)

  double angle() const; // there is a bijective mapping from Complex numbers to points on the plane
                        // So this function returns the angle of OX axis and line OM, where M is
			 // the point that corresponds to our Complex number

  Complex& operator+=(const Complex& other); //overloaded operator +=, that adds the Complex
					  //number other to our Complex number and returns our
					  //Complex number that is already changed
  Complex& operator-=(const Complex& other); //overloaded operator -=, that subtracts the Complex
					  //number other from our Complex number and returns
					  //our Complex number that is already changed
  friend Complex operator+(Complex a, const Complex& b); //overloaded operator +, that adds two 
                                                        // Complex numbers and returns the result

  friend Complex operator-(Complex a, const Complex& b); //overloaded operator -, that subtracts 
                                                        // two Complex numbers and returns the 
                                                        // result

  friend Complex operator*(Complex a, const Complex& b); //overloaded operator *, that multiplies 
                                                        // two Complex numbers and returns the 
                                                        // result

  Complex& operator*=(const Complex& other); //overloaded operator *=, that multiplies our
                                            // Complex number with other and returns our Complex
                                            // number that is already changed

  friend Complex operator/(const Complex a, const Complex b); //overloaded operator /, that
							  //divides first number by the second one
                                                        //and returns the result
  Complex& operator/=(const Complex other); // overloaded operator /=, that divides our 
                                            // Complex number by other and returns our Complex
                                            // number that is already changed

  friend std::ostream& operator<<(std::ostream& os, const Complex& a); //overloaded operator <<
                                                                  // for displaying our Complex //number

  friend std::istream& operator>>(std::istream& is, Complex& a); //overloaded operator >>
                                                              // for evaluating our Complex 
                                                              // number
                                                              // When entering 'x' and 'y'
                                                              // put a space between them
                                                              // and forget to write
                                                              // neither 'x' nor 'y'
                                                              //even if one of them is zero

  bool operator==(const Complex& other) const; // overloaded operator ==, that returns true if
                                              // the modul of our number is equal to modul of other
                                              // and false otherwise
  bool operator!=(const Complex& other) const; // overloaded operator !=, that returns true if
                                              // the modul of our number is NOT equal to modul of
					      //other and false otherwise
  bool operator>(const Complex& other) const; // overloaded operator >, that returns true if
                                              // the modul of our number is bigger than the modul
					     // of other and false otherwise
  bool operator<(const Complex& other) const; // overloaded operator <, that returns true if
                                              // the modul of our number is less than the modul 
                                              // of other and false otherwise
  bool operator<=(const Complex& other) const; // overloaded operator <=, that returns true if
                                              // the modul of our number is less than or equal the
					     // modul of other and false otherwise
  bool operator>=(const Complex& other) const; // overloaded operator >=, that returns true if
                                              // the modul of our number is bigger than or equal 
                                              // the modul of other and false otherwise
};
