/**********************************************************
** 三次样条插值函数.C
** 进行自然边界，夹持边界，非扭结边界条件下的插值
**
** 2012-05-25 将函数从原来的需要至少4个插值点才能计算
**            扩展成只需要2个插值点就可以完成计算
**            其他一些改变
** 2008-12-27 创建函数
**********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "cubic_spline/cubic_spline.h"

/*
** 资源不足时函数返回 -1
** 插值点数小于2时，函数返回 -2
** 计算正确完成函数返回插值点的数量 n
*/
int spline(point *point)
{
    double *x = (*point).xCoordinate, *y = (*point).yCoordinate;
    int n = (*point).num - 1; // 循环上限
    int i;                    // 循环辅助变量
    coefficient *coe = (*point).coe;
    condition con = (*point).con;
    double *h, *d;
    double *a, *b, *c, *f, *m;
    double temp;

    if (n < 1)
    {return -2;}

    h = (double *)malloc( (6 * n + 4) * sizeof(double) ); /* 0,       1,...,(n-1)          */
    if (h == NULL)
    {return -1;}
    d = h + n;                                            /* 0,       1,...,(n-1)          */
    a = d + n;                                            /* 特别使用,1,...,(n-1),       n */
    b = a + (n + 1);                                      /*        0,1,...,(n-1),       n */
    c = b + (n + 1);                                      /*        0,1,...,(n-1),特别使用 */
    f = c + (n + 1);                                      /*        0,1,...,(n-1),       n */
    m = b;


    /* 计算 h[] d[] */
    for (i = 0; i < n; i++)
    {
        h[i] = x[i + 1] - x[i];
        d[i] = (y[i + 1] - y[i]) / h[i];
        /* printf("%f\t%f\n", h[i], d[i]); */
    }
    /**********************
    ** 初始化系数增广矩阵
    **********************/
    a[0] = (*point).f0;
    c[n] = (*point).fn;
    /* 计算 a[] b[] c[] f[] */
    switch(con)
    {
    case NATURAL:
        f[0] = 0;
        f[n] = 0;
        a[0] = 0;
        c[n] = 0;
        c[0] = 0;
        a[n] = 0;
        b[0] = 1;
        b[n] = 1;
        break;

    case CLAMPED:
        f[0] = 6 * (d[0] - a[0]);
        f[n] = 6 * (c[n] - d[n - 1]);
        a[0] = 0;
        c[n] = 0;
        c[0] = h[0];
        a[n] = h[n - 1];
        b[0] = 2 * h[0];
        b[n] = 2 * h[n - 1];
        break;

    case NOTAKNOT:
        f[0] = 0;
        f[n] = 0;
        a[0] = h[0];
        c[n] = h[n - 1];
        c[0] = -(h[0] + h[1]);
        a[n] = -(h[n - 2] + h[n - 1]);
        b[0] = h[1];
        b[n] = h[n - 2];
        break;
    }

    for (i = 1; i < n; i++)
    {
        a[i] = h[i - 1];
        b[i] = 2 * (h[i - 1] + h[i]);
        c[i] = h[i];
        f[i] = 6 * (d[i] - d[i - 1]);
    }
    /* for (i = 0; i < n+1; i++)   printf("%f\n", c[i]); */

    /*************
    ** 高斯消元
    *************/
    if (n > 2)
    {
        /* 第0行到第(n-3)行的消元 */
        for(i = 0; i <= n - 3; i++)
        {
            /* 选主元 */
            if ( ABS(a[i + 1]) > ABS(b[i]) )
            {
                SWAP(a[i + 1], b[i], temp);
                SWAP(b[i + 1], c[i], temp);
                SWAP(c[i + 1], a[i], temp);
                SWAP(f[i + 1], f[i], temp);
            }
            temp = a[i + 1] / b[i];
            a[i + 1] = 0;
            b[i + 1] = b[i + 1] - temp * c[i];
            c[i + 1] = c[i + 1] - temp * a[i];
            f[i + 1] = f[i + 1] - temp * f[i];
        }
    }
    if (n >= 2)
    {
        /* 最后3行的消元 */
        /* 第(n-2)行的消元 */
        /* 选主元 */
        if ( ABS(a[n - 1]) > ABS(b[n - 2]) )
        {
            SWAP(a[n - 1], b[n - 2], temp);
            SWAP(b[n - 1], c[n - 2], temp);
            SWAP(c[n - 1], a[n - 2], temp);
            SWAP(f[n - 1], f[n - 2], temp);
        }
        /* 选主元 */
        if ( ABS(c[n]) > ABS(b[n - 2]) )
        {
            SWAP(c[n], b[n - 2], temp);
            SWAP(a[n], c[n - 2], temp);
            SWAP(b[n], a[n - 2], temp);
            SWAP(f[n], f[n - 2], temp);
        }
        /* 消元 */
        temp = a[n - 1] / b[n - 2];
        a[n - 1] = 0;
        b[n - 1] = b[n - 1] - temp * c[n - 2];
        c[n - 1] = c[n - 1] - temp * a[n - 2];
        f[n - 1] = f[n - 1] - temp * f[n - 2];
        /* 消元 */
        temp = c[n] / b[n - 2];
        c[n] = 0;
        a[n] = a[n] - temp * c[n - 2];
        b[n] = b[n] - temp * a[n - 2];
        f[n] = f[n] - temp * f[n - 2];
    }
    /* 最后2行的消元 */
    /* 第(n-1)行的消元 */
    /* 选主元 */
    if ( ABS(a[n]) > ABS(b[n - 1]) )
    {
        SWAP(a[n], b[n - 1], temp);
        SWAP(b[n], c[n - 1], temp);
        SWAP(f[n], f[n - 1], temp);
    }
    /* 消元 */
    temp = a[n] / b[n-1];
    a[n] = 0;
    b[n] = b[n] - temp * c[n - 1];
    f[n] = f[n] - temp * f[n - 1];

    /******************
    ** 回代求解 m[]
    ******************/
    m[n] = f[n] / b[n];
    m[n - 1] = (f[n - 1] - c[n - 1] * m[n]) / b[n-1];
    for (i = n - 2; i >= 0; i--)
    {
        m[i] = ( f[i] - (m[i + 2] * a[i] + m[i + 1] * c[i]) ) / b[i];
    }
    /* for (i = 0; i < n+1; i++)   printf("%f\n", m[i]); */

    /***********************
    ** 计算插值函数所有参数
    ***********************/
    for (i = 0; i < n; i++)
    {
        coe[i].a3 = (m[i + 1] - m[i]) / (6 * h[i]);
        coe[i].b2 = m[i] / 2;
        coe[i].c1 = d[i] - (h[i] / 6) * (2 * m[i] + m[i + 1]);
        coe[i].d0 = y[i];
    }
    free(h);
    // 计算正确完成返回插值点的数量
    return n + 1;
}

