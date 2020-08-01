#ifndef __CUBICSPLINE_H__
#define __CUBICSPLINE_H__

#define ABS(p) ( ((p) > 0) ? (p) : -(p) )
#define SWAP(x, y, temp) (temp) = (y); (y) = (x); (x) = (temp);

typedef enum _condition
{
    NATURAL, // 自然边界
    CLAMPED, // 夹持边界
    NOTAKNOT // 非扭结边界
}condition;

typedef struct _coefficient
{
    double a3;
    double b2;
    double c1;
    double d0;
}coefficient;

typedef struct _point
{
    coefficient *coe;    // 插值结果系数矩阵
    double *xCoordinate; // 插值点横坐标
    double *yCoordinate; // 插值点纵坐标
    double f0;           // 在夹持条件下的最左边点的导数值
    double fn;           // 在夹持条件下的最右边点的导数值
    int num;             // 插值点数
    condition con;       // 边界条件
}point;

extern int spline(point *point);

#endif