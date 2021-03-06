/**
 * @file offb_node.cpp
 * @brief Offboard control example node, written with MAVROS version 0.19.x, PX4 Pro Flight
 * Stack and tested in Gazebo SITL
 */

#include <ros/ros.h>
#include <vector>
#include <fstream>
#include <tf/tf.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/PositionTarget.h>
#include <sensor_msgs/NavSatFix.h>
#include <Eigen/Dense>

//#include "opencvtest/contours.h"
#include "mission_planner/CubicSplinePlanner.h"
#include "mission_planner/RangeImpl.h"
#include "off_board/camera.h"

#define M_DEG_TO_RAD 1/57.295
#define CONSTANTS_RADIUS_OF_EARTH 6371000
#define CAM_CENTER_X 0
#define CAM_CENTER_Z 0

using namespace CubicSplinePlanner;

double ahead_distance = 50;
double k = 0.1; 

enum MissionState
{
    TakeOff,
    Patrol,
    TrackingCam
};
MissionState PX4_current_mission_state = TakeOff;
tfScalar yaw,pitch,roll;
tf::Quaternion q;
mavros_msgs::State current_state;
void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}
geometry_msgs::PoseStamped current_pose;
void local_pos_cb(const geometry_msgs::PoseStamped::ConstPtr& curr_p)
{
    current_pose = *curr_p;
}
geometry_msgs::TwistStamped current_velocity;
void local_vel_cb(const geometry_msgs::TwistStamped::ConstPtr& curr_v)
{
    current_velocity = *curr_v;
}


off_board::camera camera_data;
void cam_subCallback(const off_board::camera::ConstPtr& cam_data){ // camera data callback
	camera_data = *cam_data;
}


geometry_msgs::Vector3 Quaternion2Euler(const geometry_msgs::Quaternion msg)
{
    // the incoming geometry_msgs::Quaternion is transformed to a tf::Quaterion
    tf::Quaternion quat;
    tf::quaternionMsgToTF(msg, quat);
 
    // the tf::Quaternion has a method to acess roll pitch and yaw
    double roll_, pitch_, yaw_;
    tf::Matrix3x3(quat).getRPY(roll_, pitch_, yaw_);
 
    // the found angles are written in a geometry_msgs::Vector3
    geometry_msgs::Vector3 rpy;
    rpy.x = roll_ ;
    rpy.y = pitch_ ;
    rpy.z = yaw_ ;

	return rpy;
}

Spline2D//在二维平面内，三次样条路径规划，输入点，输出路径
generate_target_course(vector<double> x, vector<double> y, vector<double> &rx, vector<double> &ry, vector<double> &ryaw,
                       vector<double> &rc) {
    Spline2D csp(x, y);
    vector<double> ixy;
    // Cosmos::RangeImpl<int> s = Cosmos::Range(0, int(csp.s.back()), 5);
    for (auto i_s : Cosmos::Range(0, int(csp.s.back()), 1)) {
        ixy = csp.calc_position(i_s);
        rx.push_back(ixy[0]);
        ry.push_back(ixy[1]);
        ryaw.push_back(csp.calc_yaw(i_s));
        rc.push_back(csp.calc_curvature(i_s));
    }
    return csp;
}

int calc_target_index(geometry_msgs::PoseStamped curr_pose, vector<double> tx, vector<double> ty)
{
    int ind;
    vector<double> dx;
    vector<double> dy;
    vector<double> d;
    for(auto& itx : tx)
    {
        dx.push_back(curr_pose.pose.position.x - itx);
    }
    for(auto& ity : ty)
    {
        dy.push_back(curr_pose.pose.position.y - ity);
    }
    for(int i = 0; i < tx.size(); i++)
    {
        d.push_back(abs(sqrt(dx[i] * dx[i] + dy[i] * dy[i])));
    }
    auto d_min = min_element(d.begin(), d.end());
    ind = distance(d.begin(), d_min);

    double L = 0.0;
    double LF = k * current_velocity.twist.linear.x + ahead_distance;
    while (LF > L && ind + 1 < tx.size())
    {
        L += sqrt(pow(tx[ind + 1] - tx[ind], 2) + pow(ty[ind + 1] - ty[ind], 2));
        ind += 1;
    }
    
    return ind;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node");
    ros::NodeHandle nh;

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("/mavros/state", 10, state_cb);

    ros::Publisher local_target_pub = nh.advertise<mavros_msgs::PositionTarget>("/mavros/setpoint_raw/local", 10);

    ros::Subscriber local_position_sub = nh.subscribe("/mavros/local_position/pose", 1, local_pos_cb);
    ros::Subscriber local_velocity_sub = nh.subscribe("/mavros/local_position/velocity_body", 1, local_vel_cb);
    ros::Subscriber cam_sub = nh.subscribe("/contours_topic", 1, cam_subCallback);
    
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");


    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 10);
            

    // the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

    //测试是否连接上
    while(ros::ok() && !current_state.connected){
        ros::spinOnce();
        rate.sleep();
        ROS_INFO("connecting...");
    }
    ROS_INFO("connected");

    geometry_msgs::PoseStamped target_pose;

    mavros_msgs::PositionTarget position_target_local;
    position_target_local.position.x = 0;
    position_target_local.position.y = 0;
    position_target_local.position.z = 14.3;

    mavros_msgs::PositionTarget position_target_new;
    geometry_msgs::Pose current_pose_new;
    Eigen::Matrix3d Matrix_from_local_to_new;
    Matrix_from_local_to_new << cos(position_target_local.yaw), -sin(position_target_local.yaw), 0,
                                sin(position_target_local.yaw), cos(position_target_local.yaw), 0,
                                0, 0, 1;

    vector<double> x = {0.0, 50.0, 150.0, 200.0, 400.0, 500.0, 600.0, 750.0, 800.0, 800.0, 750.0, 600.0, 500.0, 400.0, 200.0, 150.0, 100.0, 0.0};
    vector<double> y = {0.0, 50.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 300.0, 300.0, 300.0, 300.0, 300.0, 300.0, 300.0, 250.0, 0.0};

    vector<double> tx;  //x坐标
    vector<double> ty;  //y坐标
    vector<double> tyaw;//偏航
    vector<double> tc;  //曲率
    int target_index;    //目标索引

    Spline2D csp = generate_target_course(x, y, tx, ty, tyaw, tc);//引用

    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";

    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;

    ros::Time last_request = ros::Time::now();

    PX4_current_mission_state = TakeOff;

    while(ros::ok())
    {
        if( current_state.mode != "OFFBOARD" &&
            (ros::Time::now() - last_request > ros::Duration(5.0))){
            if( set_mode_client.call(offb_set_mode) &&
                offb_set_mode.response.mode_sent){
                ROS_INFO("Offboard enabled");
            }
            last_request = ros::Time::now();
        } else {
            if( !current_state.armed &&
                (ros::Time::now() - last_request > ros::Duration(5.0))){
                if( arming_client.call(arm_cmd) &&
                    arm_cmd.response.success){
                    ROS_INFO("Vehicle armed");
                }
                last_request = ros::Time::now();
            }
        }

        if(PX4_current_mission_state == TakeOff)
        {
            position_target_local.position.x = 0;
            position_target_local.position.y = 0;
            position_target_local.position.z = 14.3;

            ROS_INFO("TakeOff");

            if(14.3 - current_pose.pose.position.z < 0.2)
            {
                PX4_current_mission_state = Patrol;
            }
            
        }
        else if(PX4_current_mission_state == Patrol)
        {
            target_index = calc_target_index(current_pose, tx, ty);
            position_target_local.position.x = tx[target_index];
            position_target_local.position.y = ty[target_index];
            position_target_local.yaw = tyaw[target_index];
            ROS_INFO("Patrol");
            ROS_INFO("index = %d",target_index);
            
            if(camera_data.find_obs_flag)
            {
                PX4_current_mission_state = TrackingCam;
            }
            
            
        }
        
        else if(PX4_current_mission_state == TrackingCam)
        {
            ROS_INFO("TrackingCam");
            Matrix_from_local_to_new << cos(Quaternion2Euler(current_pose.pose.orientation).z), -sin(Quaternion2Euler(current_pose.pose.orientation).z), 0,
                                        sin(Quaternion2Euler(current_pose.pose.orientation).z), cos(Quaternion2Euler(current_pose.pose.orientation).z), 0,
                                        0, 0, 1;
            Eigen::Vector3d temp_pos(current_pose.pose.position.x, current_pose.pose.position.y, current_pose.pose.position.z);
            Eigen::Vector3d temp_pos_new;
            temp_pos_new = Matrix_from_local_to_new.transpose() * temp_pos;
            current_pose_new.position.x = temp_pos_new(0);
            current_pose_new.position.y = temp_pos_new(1);
            current_pose_new.position.z = temp_pos_new(2);
            
            position_target_new.velocity.x = 0;
            if (camera_data.find_obs_flag)
            {
                position_target_new.position.x = current_pose_new.position.x;
                position_target_new.velocity.x = 5;
                position_target_new.position.y = current_pose_new.position.y - 0.02 * (camera_data.x_pos - CAM_CENTER_X);
                position_target_new.position.z = current_pose_new.position.z + 0.01 * (camera_data.y_pos - CAM_CENTER_Z);
            }
            else
            {
                PX4_current_mission_state = Patrol;
            }
            temp_pos_new << position_target_new.position.x,
                            position_target_new.position.y,
                            position_target_new.position.z;
            Eigen::Vector3d temp_vel_new(position_target_new.velocity.x, position_target_new.velocity.y, position_target_new.velocity.z);
            Eigen::Vector3d temp_vel;
            temp_pos = Matrix_from_local_to_new * temp_pos_new;
            position_target_local.position.x = temp_pos(0);
            position_target_local.position.y = temp_pos(1);
            position_target_local.position.z = temp_pos(2);
            temp_vel = Matrix_from_local_to_new * temp_vel_new;
            position_target_local.velocity.x = temp_vel(0);
            position_target_local.velocity.y = temp_vel(1);
            position_target_local.yaw_rate = 0;    
        }
        
        target_pose.pose.position.x = position_target_local.position.x + position_target_local.velocity.x * 2;
        target_pose.pose.position.y = position_target_local.position.y + position_target_local.velocity.x * 2;
        target_pose.pose.position.z = position_target_local.position.z;

        std::cout << current_pose.pose.position.x << " " << current_pose.pose.position.y << " " << current_pose.pose.position.z << std::endl;
        //local_target_pub.publish(position_target_local);
        local_pos_pub.publish(target_pose);
        ros::spinOnce();
        rate.sleep();
    }
    
    return 0;
}