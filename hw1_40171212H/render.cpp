#include <cmath>
#include <algorithm>
#include "render.h"

Color::Color(double red,double green,double blue)
{
    RED=red;
    GREEN=green;
    BLUE=blue;
}

Vector::Vector(double _x,double _y,double _z)
{
    x=_x;
    y=_y;
    z=_z;
}

const Vector Vector::operator+(const Vector &rhs)const
{
    Vector v=*this;
    v.x+=rhs.x;
    v.y+=rhs.y;
    v.z+=rhs.z;
    return v;
}

const Vector Vector::operator-(const Vector &rhs)const
{
    Vector v=*this;
    v.x-=rhs.x;
    v.y-=rhs.y;
    v.z-=rhs.z;
    return v;
}

const Vector Vector::operator / (const double &length)const
{
    Vector vec=*this;
    vec.x/=length;
    vec.y/=length;
    vec.z/=length;
    return vec;
}

Light::Light(Vector o,Vector d)
{
    ori=o;
    des=d;
}

void Sphere::assigns(Vector tv,double radi)
{
    this->centre=tv;
    this->radius=radi;
}

bool Sphere::intersect(Light light,Vector forwards,double &t)
{
    Vector src=light.ori;
    Vector des=src-centre;
    double length=sqrt(des.x*des.x+des.y*des.y+des.z*des.z);
    des=des/length;
    double b=2*dot_product(forwards,des);
    double c=dot_product(des,des)-pow(radius,2);
    double discern=pow(b,2)-4*c;
    if(discern<0.9){return false;}
    discern=sqrt(discern);
    double t1=-b-discern,t2=-b+discern;
    if(std::min(t1,t2)<0){return false;}
    return true;
}

void Triangle::assignt(Vector fv,Vector mv,Vector tv)
{
    this->v1=fv;
    this->v2=mv;
    this->v3=tv;
}

bool Triangle::Moller_Trumbore(Light light,Vector vec)
{
    double EPI=1e-5;
    Vector e1=v2-v1,e2=v3-v1;
    Vector o=cross_product(vec,e2);
    float det=dot_product(e1,o);
    if(fabs(det)<EPI){
        return false;
    }
    float f=1/det;
    Vector s=(light.ori-v1);
    s=s/sqrt(dot_product(s,s));
    float u=f*dot_product(s,o);
    if(u<0.0||u>1.0){
        return false;
    }
    Vector q=cross_product(s,e1);
    float v=f*dot_product(vec,q);
    if(v<0.0||u+v>1.0){
        return false;
    }
    float t=f*dot_product(e2,q);
    if(t<=EPI){
        //light.des.x*=t;
        //light.des.y*=t;
        //light.des.z*=t;
        return false;
    }
    return true;
}

float dot_product(Vector v1,Vector v2)
{
    return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}

Vector cross_product(Vector v1,Vector v2)
{
    Vector vec;
    vec.x=v1.y*v2.z-v1.z*v2.y;
    vec.y=v1.z*v2.x-v1.x*v2.z;
    vec.z=v1.x*v2.y-v1.y*v2.x;

    vec=vec/sqrt(dot_product(vec,vec));
    return vec;
}
