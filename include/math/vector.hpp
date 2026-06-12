#pragma once
#include <cmath>
#include <iostream>
#include <numbers>
#define PI 3.14159265358979323846
        struct Vector2{
            public:
                double x;
                double y;
                //Constructors
                Vector2(const double& x, const double& y): x{x},y{y}{}
                explicit Vector2(const double& x): Vector2{x,x}{}
                Vector2():Vector2{0,0}{}

                //Functions
                Vector2 rotate(double degree)const{
                    const auto radians = degree * (PI / 180.0);
                    const auto res_x = this->x * std::cos(radians) - this->y * std::sin(radians);
                    const auto res_y = this->x * std::sin(radians) + this->y * std::cos(radians);
                    return Vector2{res_x, res_y};
                }
                double magnitude() const{
                    return std::sqrt((this->x*this->x )+(this->y * this->y));
                }
                //Operators
                Vector2 operator+(const Vector2& other) const{
                    return Vector2{this->x+ other.x , this->y + other.y};
                }
                Vector2 operator-(const Vector2& other) const {
                    return Vector2{this->x - other.x , this->y - other.y};
                }
                Vector2 operator*(const double& scalar) const {
                    return Vector2{this->x * scalar, this->y * scalar};
                }
                
        };

        //Freie Funktionen
        inline Vector2 operator*(const double& scalar, const Vector2& vec){
            return Vector2{scalar * vec.x, scalar* vec.y };
        }

        inline std::ostream& operator<<(std::ostream& os, Vector2 vec){
            return os <<  "{" << vec.x << ", " << vec.y << "}";
        }
        
