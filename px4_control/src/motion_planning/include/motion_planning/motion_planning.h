#ifndef __MOTION_PLANNING_H__
#define __MOTION_PLANNING_H__

#define AHEAD_DIS 150 //50 //前视距离
#define K 0.1 //0.1 //寻找合适目标点时，飞机速度的权重系数
#define UNIT 300 //规划出的路径中每两个点之间细分的数量

typedef struct 
{
    double x;
    double y;
    double yaw;
}target_path_t;

enum MissionState
{
    TakeOff,
    Patrol_Go,
    Patrol_Return,
    TrackingCam,
    Landing
};

enum patrol_state
{
    GO,
    RETURN
};

extern int generate_target_course(double *x, double *y, target_path_t* target_path, int n, patrol_state state);
extern int get_target_index(geometry_msgs::PoseStamped curr_pose, geometry_msgs::TwistStamped curr_velocity,
                             target_path_t* target_path, int n, patrol_state state); 

#endif