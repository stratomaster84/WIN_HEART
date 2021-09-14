//---------------------------------------------------------------------------

#ifndef HeartModH
#define HeartModH

#define _PEQUAL 0.00000001

#define HEART_INITED            0
#define HEART_ALPHA             1
#define HEART_SCALE             2
#define HEART_DEPTH             3
#define HEART_HEIGHT            4
#define HEART_WIDTH             5
#define HEART_RADIUS            6
#define HEART_SECTORS           7
#define HEART_ROUNDS            8
#define HEART_POINTS_NUMBER     9
#define HEART_TRIANGLES_NUMBER  10


typedef float point3[3];
typedef unsigned int triangle_index3[3];
typedef float texel2[2];

class _Heart_Model{
protected:
        int _list;

                // начало переменных, которые можно запросить
        bool _inited;

        double _alpha;
        double _scale;

        double _depth;  //y
        double _height; // x
        double _width; //z

        double _radius;

        unsigned int sectors; // не меньше 8
        unsigned int rounds;  // не меньше 4

        unsigned int _point_num;
        unsigned int _triangle_num;
                // конец переменных, которые можно запросить

        float _max_x;
        float _max_z;

        point3                  *_Vertice;
        point3                  *_Normal;
        texel2                  *_Texel;
        triangle_index3         *_Triangle;

        unsigned int Get_VNum(bool IsMain,unsigned int sector,unsigned int round);
        unsigned int Get_TNum(unsigned int sector,unsigned int round,bool IsFirst);

        void MadeHeartMap();
        void DeleteHeartMap();

        void SetNormals();
        void SetTexels();
        void NormCoords();
        void ScaleCoords();

        void Func(bool IsMain,unsigned int sector,unsigned int round);

        void Draw_Half_1();
        void Draw_Half_2();

public:
        _Heart_Model();
        ~_Heart_Model();

        void MadeGLList(double scale,double alpha,unsigned int _sectors, unsigned int _rounds, double width,double height,double depth);

        void Draw_Heart();
        double Get_Parameter(unsigned int parameter);

};

//---------------------------------------------------------------------------
#endif
 