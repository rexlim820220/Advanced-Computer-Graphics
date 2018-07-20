#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include "render.h"

using namespace std;

int howlong(char);
Vector convert(string *,int);

int main(int argc,char **argv)
{
    ifstream ifile;
    ifile.open("hw1_input.txt");
    int width,height,t=0;
    double angle,canvas_width;
    unsigned default_len;
    Vector src,des;
    Sphere sphere;
    Triangle triangle[2];
    string line[default_len=10];
    while(!ifile.eof()){
        string input;
        getline(ifile,input);
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
        else if(h=='S'){
            double r=strtof(line[4].c_str(),0);
            sphere.assigns(convert(line,3),r);
        }
        else if(h=='T'){
            triangle[t].assignt(convert(line,3),convert(line,6),convert(line,9));
            ++t;
        }
    }
    ifile.close();
    Color pixel[width][height];
    Color white(255,255,255);
    ofstream ofile("hw1_output.ppm",std::ios::binary);
    ofile<<"P6"<<'\n'<<width<<' '<<height<<' '<<"\n255\n";
    double t0,cw=canvas_width/width,ch=canvas_width/height;
    for(int j=0;j<height;j+=1){
        for(int i=0;i<width;i+=1){
            pixel[i][j]={0,0,0};
        }
    }
    for(int j=0;j<height;j+=1){
        for(int i=0;i<width;i+=1){
            Light light(src,{src.x+des.x-canvas_width+cw*i*4,
                        src.y+des.y+canvas_width-ch*j*4,
                        src.z+des.z});
            Vector vec=light.des-light.ori;
            double length=sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
            vec=vec/length;
            if(sphere.intersect(light,vec,t0)){
                pixel[i][j]=white;
            }
            for(int k=0;k<t;k+=1){
                if(triangle[k].Moller_Trumbore(light,vec)){
                    pixel[i][j]=white;
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


int howlong(char c)
{
    int range;
    if(c=='E'||c=='V'){range=3;}
    else if(c=='F'){range=1;}
    else if(c=='R'){range=2;}
    else if(c=='S'){range=4;}
    else if(c=='T'){range=9;}
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
