#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

struct Color{
    Color(){}
    Color(double,double,double);
    const Color operator + (const Color &)const;
    const Color operator * (const double &)const;
    double RED,GREEN,BLUE;
};

struct Vector{
    Vector(){}
    Vector(double,double,double);
    const Vector operator + (const Vector &)const;
    const Vector operator - (const Vector &)const;
    const Vector operator / (const double &)const;
    const Vector operator * (const double &)const;
    double x,y,z;
};

struct Light{
    Light(Vector,Vector);
    Vector ori;
    Vector des;
};

class Sphere{
public:
    Sphere(){}
    void assigns(Vector,double);
    bool intersect(Light,Vector);
    Vector hitNormal(Vector);
private:
    Vector centre;
    double radius;
};

class Triangle{
public:
    void assignt(Vector,Vector,Vector);
    bool Moller_Trumbore(Light,Vector);
    Vector hitNormal();
private:
    Vector v1,v2,v3;
};

float dot_product(Vector,Vector);

Vector cross_product(Vector,Vector);

#endif // RENDER_H_INCLUDED
