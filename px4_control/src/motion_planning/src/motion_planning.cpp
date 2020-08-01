#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>

#include "cubic_spline/cubic_spline.h"
#include "motion_planning/motion_planning.h"

int generate_target_course(double *x, double *y, target_path_t* target_path, int n, patrol_state state)//进行路径规划
{
    coefficient *coe;
    coe = (coefficient *)malloc((n - 1) * sizeof(coefficient));
    point p;
    p.xCoordinate = x;
    p.yCoordinate = y;
    p.num = n;
    p.con = NOTAKNOT;//NATURAL;//自然边界
    p.coe = coe;
    spline(&p);//路径规划

    double y_max = y[0];
    double y_min = y[0];

    for(int i = 0; i < n; ++i)
    {
        if(y[i] > y_max)
        {
            y_max = y[i];
        }
        if(y[i] < y_min)
        {
            y_min = y[i];
        }
    }
    
    for(int i = 0; i < n - 1; ++i)
    {
        double x_error = x[i+1] - x[i];
        double uintx = x_error / UNIT;
        double x_now = x[i];
        for(int k = 0; k < UNIT; ++k)//对连续的路径进行拆分
        {
            target_path[i*UNIT+k].x = x_now;
            target_path[i*UNIT+k].y = coe[i].a3 * pow(target_path[i*UNIT+k].x, 3) + coe[i].b2 * pow(target_path[i*UNIT+k].x, 2) +
                                    coe[i].c1 * target_path[i*UNIT+k].x + coe[i].d0;
            if(target_path[i*UNIT+k].y > y_max + 20)//路径修正
            {
                target_path[i*UNIT+k].y = y_max + 20;
            }
            else if(target_path[i*UNIT+k].y < y_min - 20)
            {
                target_path[i*UNIT+k].y = y_min - 20;
            }
            target_path[i*UNIT+k].yaw = atan(coe[i].a3 * 3 * pow(target_path[i*UNIT+k].x, 2) + 
                                    coe[i].b2 * 2 * target_path[i*UNIT+k].x + coe[i].c1);//通过斜率算出角度
            if(state == RETURN)
            {
                target_path[i*UNIT+k].yaw += M_PI;//如果是返回的话，yaw需要加上PI的角度
            }
            x_now += uintx;
        } 
    }
    free(coe);
    return 1;
}

int get_target_index(geometry_msgs::PoseStamped curr_pose, geometry_msgs::TwistStamped curr_velocity, 
                    target_path_t* target_path, int n, patrol_state state) //获取合适的目标点
{
    double dis[n * UNIT];
    double dis_min;
    int index = 0;
    //寻找出路径中距离飞机当前位置最近的点
    for(int i = 0; i < (n-1)*UNIT; ++i)
    {
        dis[i] = sqrt(pow((curr_pose.pose.position.x - target_path[i].x), 2) + 
                        pow((curr_pose.pose.position.x - target_path[i].x), 2));
    }
    dis_min = dis[0];
    for(int i = 0;i < (n-1)*UNIT; ++i)
    {
       if(dis[i] < dis_min)
        {
            dis_min = dis[i];
            index = i;
        }
    }

    double L = 0.0;
    double LF = K * curr_velocity.twist.linear.x + AHEAD_DIS;//前视距离

    while (LF > L && index + 1 < (n - 1)* UNIT && index > 0)//如果不加index>1，index可能为负数，导致数组越界
    {
        L += sqrt(pow(target_path[index + 1].x - target_path[index].x, 2) + 
                    pow(target_path[index + 1].y - target_path[index].y,2));
        if(state == GO)//出发时从前向后检索
        {
            index += 1;
        }
        else//返回时从后向前检索
        {
            index -= 1;
        }
    }
    if(!index)//避免index一开始就是0，不进入while循环
    {
        index++;
    }
    return index;
}