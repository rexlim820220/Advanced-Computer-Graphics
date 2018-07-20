#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include "render.h"
using namespace std;

inline int howlong(char);
Vector convert(string *,int);

struct Phong{
    double ka,kd,ks;
    int exp;
    double reflection,refraction;
    double Nr;
};

int main(int argc,char **argv)
{
    ifstream ifile("hw2_input.txt");
    int width,height,p=0;
    double angle,canvas_width;
    unsigned default_len;
    Vector src,des,ray;
    Sphere sphere;
    vector<Triangle>triangle;
    Color color[3];
    Phong phong[3];
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
        else if(h=='S'){
            double r=strtof(line[4].c_str(),0);
            sphere.assigns(convert(line,3),r);
        }
        else if(h=='T'){
            Triangle tri;
            tri.assignt(convert(line,3),convert(line,6),convert(line,9));
            triangle.push_back(tri);
        }
        else if(h=='L'){
            ray=convert(line,howlong(h));
        }
    }
    ifile.close();

    Color pixel[width][height];
    ofstream ofile("Bunny.ppm",std::ios::binary);
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
            for(size_t k=0;k<triangle.size();k+=1){
                if(triangle[k].Moller_Trumbore(light,vec)){
                    pixel[i][j]=color[1];
                    pixel[i][j].RED*=phong[1].ka;
                    pixel[i][j].GREEN*=phong[1].ka;
                    pixel[i][j].BLUE*=phong[1].ka;

                    Vector N=triangle[k].hitNormal();
                    Vector L=vec-N*dot_product(vec,N)*2;
                    double d=phong[1].reflection*dot_product(N,ray);
                    double s=phong[1].reflection*pow(dot_product(N,L),phong[1].exp);
                    Color diffuse(d,d,d),specular(s,s,s);
                    pixel[i][j]=pixel[i][j]+(diffuse*phong[1].kd+specular*phong[1].ks);
                }
            }
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
