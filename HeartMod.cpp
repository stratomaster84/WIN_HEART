//---------------------------------------------------------------------------

#pragma hdrstop

#include "HeartMod.h"
#include <math.h>
#include <windows.h>
#include <gl\gl.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

unsigned int _Heart_Model::Get_VNum(bool IsMain,unsigned int sector,unsigned int round){
        if(IsMain)
                return 0;
        else
                return sectors*round + sector+1;
}
unsigned int _Heart_Model::Get_TNum(unsigned int sector,unsigned int round,bool IsFirst){
        if(round == 0)
                return sector;
        else if(IsFirst)
                return (2*round-1)*sectors +sector*2;
        else
                return (2*round-1)*sectors +sector*2 + 1;
}

void _Heart_Model::Func(bool IsMain,unsigned int sector,unsigned int round){
        unsigned int _p = Get_VNum(IsMain,sector,round);
        point3 _tmp;
        _tmp[0] = _Vertice[_p][0];
        _tmp[1] = _Vertice[_p][1];
        _tmp[2] = _Vertice[_p][2];

        float _rad = _tmp[0]*_tmp[0]+_tmp[1]*_tmp[1];
        if(_rad >= 1.0f)
                _tmp[2] = 0.0f;
        else
                _tmp[2] = sqrt(1.0f-_rad);

        double _x2,_z2,_new_alpha;
        _new_alpha = atan(_scale*tan(_alpha));
        _x2 = (_tmp[0]+1.0f)*cos(_alpha) - _tmp[2]*sin(_alpha);
        _z2 = _scale*((_tmp[0]+1.0f)*sin(_alpha) + _tmp[2]*cos(_alpha));
        _tmp[0] = _x2*cos(_new_alpha) + _z2*sin(_new_alpha);
        if(round == rounds-1)
                _tmp[2] = 0.0f;
        else
                _tmp[2] = -_x2*sin(_new_alpha) + _z2*cos(_new_alpha);
        _Vertice[_p][0] = _tmp[0];
        _Vertice[_p][1] = _tmp[1];
        _Vertice[_p][2] = _tmp[2];

        if(_tmp[0]>_max_x)
                _max_x = _tmp[0];
        if(_tmp[2]>_max_z)
                _max_z = _tmp[2];

}


void _Heart_Model::MadeHeartMap(){

        _point_num = 1 + sectors*rounds;
        _Vertice = new point3 [_point_num];
        _Normal = new point3 [_point_num];
        _Texel = new texel2 [_point_num];
        _triangle_num = sectors*(2*rounds -1);
        _Triangle = new triangle_index3[_triangle_num];

        _Vertice[Get_VNum(1,0,0)][0] = 0;
        _Vertice[Get_VNum(1,0,0)][1] = 0;
        Func(1,0,0);

        unsigned int i,j;
        double _angle,_h;
        _angle = 2*M_PI/(double)sectors;
        _h = 1.0f/(double)rounds;

        for(j=0;j<rounds;j++){
                for(i=0;i<sectors;i++){
                        _Vertice[Get_VNum(0,i,j)][0] = _h*(double)(j+1)*cos(_angle*(double)i);
                        _Vertice[Get_VNum(0,i,j)][1] = _h*(double)(j+1)*sin(_angle*(double)i);
                        Func(0,i,j);
                }
        }
        for(i=0;i<sectors;i++){
                _Triangle[Get_TNum(i,0,0)][0] = Get_VNum(1,0,0);
                _Triangle[Get_TNum(i,0,0)][1] = Get_VNum(0,i,0);
                _Triangle[Get_TNum(i,0,0)][2] = Get_VNum(0,((i+1)==sectors)?0:(i+1),0);

                for(j=1;j<rounds;j++){   // начало с первого круга
                        _Triangle[Get_TNum(i,j,1)][0] = Get_VNum(0,i,j-1);
                        _Triangle[Get_TNum(i,j,1)][1] = Get_VNum(0,i,j);
                        _Triangle[Get_TNum(i,j,1)][2] = Get_VNum(0,((i+1)==sectors)?0:(i+1),j);

                        _Triangle[Get_TNum(i,j,0)][0] = Get_VNum(0,i,j-1);
                        _Triangle[Get_TNum(i,j,0)][1] = Get_VNum(0,((i+1)==sectors)?0:(i+1),j);
                        _Triangle[Get_TNum(i,j,0)][2] = Get_VNum(0,((i+1)==sectors)?0:(i+1),j-1);
                }
        }
}


_Heart_Model::_Heart_Model(){
        _inited = false;
}
_Heart_Model::~_Heart_Model(){
        glDeleteLists (_list,1);
        _inited = false;
}

void _Heart_Model::Draw_Half_1(){
        unsigned int i;
        glBegin(GL_TRIANGLES);
                for(i=0;i<_triangle_num;i++){
                        glTexCoord2fv(_Texel  [_Triangle[i][0]]);
                        glNormal3fv  (_Normal [_Triangle[i][0]]);
                        glVertex3fv  (_Vertice[_Triangle[i][0]]);

                        glTexCoord2fv(_Texel  [_Triangle[i][1]]);
                        glNormal3fv  (_Normal [_Triangle[i][1]]);
                        glVertex3fv  (_Vertice[_Triangle[i][1]]);

                        glTexCoord2fv(_Texel  [_Triangle[i][2]]);
                        glNormal3fv  (_Normal [_Triangle[i][2]]);
                        glVertex3fv  (_Vertice[_Triangle[i][2]]);
                }
        glEnd();
}
void _Heart_Model::Draw_Half_2(){
        unsigned int i;

        glBegin(GL_TRIANGLES);
                for(i=0;i<_triangle_num;i++){
                        glTexCoord2f(1.0f -_Texel  [_Triangle[i][1]][0], _Texel  [_Triangle[i][1]][1]);
                        glNormal3f(_Normal [_Triangle[i][1]][0],_Normal [_Triangle[i][1]][1],-_Normal [_Triangle[i][1]][2]);
                        glVertex3f(_Vertice[_Triangle[i][1]][0],_Vertice[_Triangle[i][1]][1],-_Vertice[_Triangle[i][1]][2]);

                        glTexCoord2f(1.0f - _Texel  [_Triangle[i][0]][0],_Texel  [_Triangle[i][0]][1]);
                        glNormal3f(_Normal [_Triangle[i][0]][0],_Normal [_Triangle[i][0]][1],-_Normal [_Triangle[i][0]][2]);
                        glVertex3f(_Vertice[_Triangle[i][0]][0],_Vertice[_Triangle[i][0]][1],-_Vertice[_Triangle[i][0]][2]);

                        glTexCoord2f(1.0f - _Texel  [_Triangle[i][2]][0], _Texel  [_Triangle[i][2]][1]);
                        glNormal3f(_Normal [_Triangle[i][2]][0],_Normal [_Triangle[i][2]][1],-_Normal [_Triangle[i][2]][2]);
                        glVertex3f(_Vertice[_Triangle[i][2]][0],_Vertice[_Triangle[i][2]][1],-_Vertice[_Triangle[i][2]][2]);

                }
        glEnd();
}
void _Heart_Model::Draw_Heart(){
        if(_inited)
                glCallList(_list);
}

void _Heart_Model::MadeGLList(double scale,double alpha,unsigned int _sectors, unsigned int _rounds,  double width,double height,double depth){

        if(_inited)
                return;
        sectors = _sectors; // не меньше 8
        rounds = _rounds;  // не меньше 4
        _alpha = alpha*M_PI/180;
        _scale = scale;
        _width = width;
        _height = height;
        _depth = depth;

        _max_x=-1000.0f;
        _max_z=-1000.0f;
        _radius = 0.0f;

        MadeHeartMap();
        NormCoords();
        SetTexels();
        ScaleCoords();
        SetNormals();

        _list = glGenLists(1);
        glNewList(_list, GL_COMPILE);
                glRotatef(-90, 1.0f, 0.0f, 0.0f);
                glRotatef(-90, 0.0f, 1.0f, 0.0f);

                Draw_Half_1();
                Draw_Half_2();
        glEndList();

        DeleteHeartMap();
        _inited = true;

}
void _Heart_Model::DeleteHeartMap(){
        if(!_inited)
                return;
        if(_Vertice!=NULL)
                delete [] _Vertice;
        if(_Triangle!=NULL)
                 delete [] _Triangle;
        if(_Texel!=NULL)
                delete [] _Texel;
        if(_Normal!=NULL)
                 delete [] _Normal;
}
void _Heart_Model::SetNormals(){
        double _rad;
        for(unsigned int i=0;i<_point_num;i++){
                _rad = sqrt(_Vertice[i][0]*_Vertice[i][0] +
                        _Vertice[i][1]*_Vertice[i][1] +
                        _Vertice[i][2]*_Vertice[i][2]);
                _Normal[i][0] = _Vertice[i][0]/_rad;
                _Normal[i][1] = _Vertice[i][1]/_rad;
                _Normal[i][2] = _Vertice[i][2]/_rad;
                if(_rad > _radius)
                        _radius = _rad;

        }
}

void _Heart_Model::SetTexels(){
        float _z_tex;
        for(unsigned int i=0;i<_point_num;i++){
                _Texel[i][1] = _Vertice[i][0];

                if(_Vertice[i][1] < 0.0f)
                        _Texel[i][0] = 0.5f-_Vertice[i][2];
                else
                        _Texel[i][0] = 0.5f+_Vertice[i][2];
        }

}
void _Heart_Model::NormCoords(){
        unsigned int i;
        for(i=0;i<_point_num;i++){
                _Vertice[i][0] /= _max_x;
                _Vertice[i][2] /= _max_z*2;
        }
}

void _Heart_Model::ScaleCoords(){
        unsigned int i;
        for(i=0;i<_point_num;i++){
                _Vertice[i][0]  = (_Vertice[i][0]-0.5f)*_height;
                _Vertice[i][1]  *= _depth*0.5f;
                _Vertice[i][2]  *= _width;
        }

}
double _Heart_Model::Get_Parameter(unsigned int parameter){
        if(!_inited)
                return 0.0f;
        switch(parameter){
                case HEART_INITED:
                        if(_inited)
                                return 1.0f;
                        else
                                return 0.0f;
                       // break;
                case HEART_ALPHA:
                        return 180.0f*_alpha/M_PI;
                       // break;
                case HEART_SCALE:
                        return _scale;
                     //   break;
                case HEART_DEPTH:
                        return _depth;
                     //   break;
                case HEART_HEIGHT:
                        return _height;
                     //   break;
                case HEART_WIDTH:
                        return _width;
                     //   break;
                case HEART_RADIUS:
                        return _radius;
                      //  break;
                case HEART_SECTORS:
                        return (double)sectors;
                      //  break;
                case HEART_ROUNDS:
                        return (double)rounds;
                     //   break;
                case HEART_POINTS_NUMBER:
                        return (double)_point_num;
                      //  break;
                case HEART_TRIANGLES_NUMBER:
                        return (double)_triangle_num;
                      //  break;
                default:
                        return 0.0f;
        }
       // return 0.0f;
}
