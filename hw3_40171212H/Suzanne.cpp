#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

struct Color{
    Color(){}
    Color(double,double,double);
    const Color operator + (const Color &)const;
    const Color operator + (const double &)const;
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

class Triangle{
public:
    bool getbool(){return this->material;}
    void setbool(bool _m){this->material=_m;}
    void assignt(Vector,Vector,Vector,Vector);
    bool Moller_Trumbore(Light,Vector,double&);
    Vector hitNormal();
private:
    bool material;
    Vector v1,v2,v3,N;
};

struct Phong{
    double ka,kd,ks;
    int exp;
    double reflection,refraction;
    double Nr;
};

float dot_product(Vector,Vector);
Vector cross_product(Vector,Vector);

inline int howlong(char);
Vector convert(string *,int);

int main(int argc,char **argv)
{
    ifstream ifile("Input_Suzanne.txt");
    int width,height,p=0;
    double angle,canvas_width;
    unsigned default_len;
    Vector src,des,ray;
    vector<Triangle>triangle;
    Color color[2];
    Phong phong[2];
    string line[default_len=14];
    while(!ifile.eof())
    {
        string input;
        getline(ifile,input);
        cout<<input<<'\n';
        stringstream ssin(input);
        int i=0;
        while(ssin.good()&&i<=howlong(input[0])){
            ssin>>line[i];
            ++i;
        }
        char h=*line[0].c_str();
        if(h=='E'){
            src=convert(line,howlong(h));
        }
        else if(h=='V'){
            des=convert(line,howlong(h));
        }
        else if(h=='F'){
            angle=strtof(line[1].c_str(),0);
            angle/=180;
            angle*=3.14159;
            canvas_width=tan(angle/2);
        }
        else if(h=='R'){
            width=strtof(line[1].c_str(),0);
            height=strtof(line[2].c_str(),0);
        }
        else if(h=='M'){
            color[p].RED=strtof(line[1].c_str(),0)*255;
            color[p].GREEN=strtof(line[2].c_str(),0)*255;
            color[p].BLUE=strtof(line[3].c_str(),0)*255;

            phong[p].ka=strtof(line[4].c_str(),0);
            phong[p].kd=strtof(line[5].c_str(),0);
            phong[p].ks=strtof(line[6].c_str(),0);

            phong[p].exp=strtof(line[7].c_str(),0);
            phong[p].reflection=strtof(line[8].c_str(),0);
            phong[p].refraction=strtof(line[9].c_str(),0);
            phong[p].Nr=strtof(line[10].c_str(),0);

            ++p;
        }
        else if(h=='T'){
            Triangle tri;
            tri.assignt(convert(line,3),convert(line,6),convert(line,9),convert(line,12));
            tri.setbool(p-1);
            triangle.push_back(tri);
        }
        else if(h=='L'){
            ray=convert(line,howlong(h));
        }
    }
    ifile.close();

    Color pixel[width][height];
    ofstream ofile("Output_Suzanne.ppm",std::ios::binary);
    ofile<<"P6"<<'\n'<<width<<' '<<height<<' '<<"\n255\n";
    double cw=canvas_width/width,ch=canvas_width/height;
    for(int i=0;i<width;i+=1){
        for(int j=0;j<height;j+=1){
            pixel[i][j]={0,0,0};
        }
    }
    for(int j=0;j<height;j+=1){
        for(int i=0;i<width;i+=1){
            Light light(src,{src.x+des.x-canvas_width+cw*i*3,
                        src.y+des.y+canvas_width-ch*j*3,
                        src.z+des.z});
            Vector vec=light.des-light.ori;
            double length=sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
            vec=vec/length;
            double ambient,diffuse,specular,t,temp=length;
            for(size_t k=triangle.size()-1;k>0;k-=1){
                Color c=(!triangle[k].getbool())?color[0]:color[1];
                Phong P=(!triangle[k].getbool())?phong[0]:phong[1];
                if(triangle[k].Moller_Trumbore(light,vec,t)){
                    if(temp>t){
                        pixel[i][j]=c;
                        Vector N=triangle[k].hitNormal();
                        Vector L=vec-N*dot_product(vec,N)*2;
                        ambient=P.ka;
                        diffuse=P.kd*dot_product(N,ray);
                        specular=P.ks*pow(dot_product(N,L),P.exp);
                        temp=t;
                    }
                }
            }
            pixel[i][j]=pixel[i][j]*ambient;
            pixel[i][j]=pixel[i][j]+(diffuse+specular);
            //pixel[i][j].RED*=(ambient+diffuse+specular);
            //pixel[i][j].GREEN*=(ambient+diffuse+specular);
            //pixel[i][j].BLUE*=(ambient+diffuse+specular);
            ofile<<(char)pixel[i][j].RED;
            ofile<<(char)pixel[i][j].GREEN;
            ofile<<(char)pixel[i][j].BLUE;
        }
    }
    ofile.close();
    return 0;
}

inline int howlong(char c)
{
    int range;
    if(c=='E'||c=='V'){range=3;}
    else if(c=='F'){range=1;}
    else if(c=='R'){range=2;}
    else if(c=='L'){range=3;}
    else if(c=='S'){range=4;}
    else if(c=='T'){range=12;}
    else if(c=='M'){range=10;}
    return range;
}

Vector convert(std::string *s,int a)
{
    Vector _vector;
    _vector.x=strtof(s[a-2].c_str(),0);
    _vector.y=strtof(s[a-1].c_str(),0);
    _vector.z=strtof(s[a].c_str(),0);
    return _vector;
}

Color::Color(double red,double green,double blue)
{
    RED=red;
    GREEN=green;
    BLUE=blue;
}

const Color Color::operator+(const Color &rhs)const
{
    Color self=*this;
    self.RED+=rhs.RED;
    self.GREEN+=rhs.GREEN;
    self.BLUE+=rhs.BLUE;
    return self;
}

const Color Color::operator+(const double &constant)const
{
    Color self=*this;
    self.RED+=constant;
    self.GREEN+=constant;
    self.BLUE+=constant;
    return self;
}

const Color Color::operator*(const double &constant)const
{
    Color self=*this;
    self.RED*=constant;
    self.GREEN*=constant;
    self.BLUE*=constant;
    return self;
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

const Vector Vector::operator * (const double &length)const
{
    Vector vec=*this;
    vec.x*=length;
    vec.y*=length;
    vec.z*=length;
    return vec;
}

Light::Light(Vector o,Vector d)
{
    ori=o;
    des=d;
}

void Triangle::assignt(Vector fv,Vector mv,Vector tv,Vector normal)
{
    this->v1=fv;
    this->v2=mv;
    this->v3=tv;
    this->N=normal;
}

bool Triangle::Moller_Trumbore(Light light,Vector vec,double &t)
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
    t=dot_product(e2,q);
    if(t<=EPI){
        light.des.x*=t;
        light.des.y*=t;
        light.des.z*=t;

        return false;
    }
    return true;
}

Vector Triangle::hitNormal()
{
    Vector e1=this->v2-this->v1,e2=this->v3-this->v1;
    return this->N;
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
