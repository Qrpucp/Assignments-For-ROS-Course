#include <ros/ros.h>
#include <vector>
#include <fstream>
#include <tf/tf.h>
#include <geometry_msgs/PoseStamped.h>

//#include "opencvtest/contours.h"
#include "off_board/camera.h"

geometry_msgs::PoseStamped box_1;
geometry_msgs::PoseStamped box_2;

geometry_msgs::PoseStamped current_pose;

void local_pos_cb(const geometry_msgs::PoseStamped::ConstPtr& curr_p)
{
    current_pose = *curr_p;
}

/*
geometry_msgs::Vector3 Quaternion2Euler(const geometry_msgs::Quaternion msg)
{
    tf::Quaternion quat;
    tf::quaternionMsgToTF(msg, quat);
 
    double roll_, pitch_, yaw_;
    tf::Matrix3x3(quat).getRPY(roll_, pitch_, yaw_);
 
    geometry_msgs::Vector3 rpy;
    rpy.x = roll_ ;
    rpy.y = pitch_ ;
    rpy.z = yaw_ ;

	return rpy;
}
*/

int main(int argc, char **argv)
{
    ros::init(argc, argv, "cam_simulate_node1");//单无人机，所以只有一个节点
    ros::NodeHandle nh;

    ros::Subscriber local_position_sub = nh.subscribe("/mavros/local_position/pose", 1, local_pos_cb);
    ros::Publisher contours_pub_ = nh.advertise<off_board::camera>("/contours_topic", 5);

    ros::Rate rate(20.0);
    box_1.pose.position.x = 500;
    box_1.pose.position.y = 114;
    box_1.pose.position.z = 14.3;
    box_1.pose.orientation = tf::createQuaternionMsgFromYaw(0.00);//根据yaw轴数据获得四元数

    box_2.pose.position.x = 500;
    box_2.pose.position.y = 314;
    box_2.pose.position.z = 14.3;
    box_2.pose.orientation = tf::createQuaternionMsgFromYaw(M_PI);

    double d_min = 8;//能看到框的最小距离
    double d_max = 80;//能看到框的最大距离
    //由框的尺寸和相机的内参决定

    off_board::camera sim_cam_info;

    double distance_from_plane_to_box1;//飞机到框1的距离
    double distance_from_plane_to_box2;
    double error_yaw_between_plane_and_box1;//飞机和框1的yaw轴偏差
    double error_yaw_between_plane_and_box2;


    std::default_random_engine e; //随机引擎
    std::normal_distribution<double> gauss_rand(0, 0.1);//生成随机噪声

    while(ros::ok())
    {
        sim_cam_info.find_obs_flag = false;//是否找到框
        sim_cam_info.dis = -1;//到框的距离
        sim_cam_info.pos_left = -1;//到框左边的距离
        sim_cam_info.pos_right = -1;
        sim_cam_info.x_pos = -1;//到框中心点的距离
        sim_cam_info.y_pos = -1;

        distance_from_plane_to_box1 = box_1.pose.position.x - current_pose.pose.position.x;
        distance_from_plane_to_box2 = current_pose.pose.position.x - box_2.pose.position.x;
        error_yaw_between_plane_and_box1 = current_pose.pose.orientation.z - box_1.pose.orientation.z;
        error_yaw_between_plane_and_box2 = current_pose.pose.orientation.w - box_2.pose.orientation.w;
        //error_yaw_between_plane_and_box2 = current_pose.pose.orientation.z - box_2.pose.orientation.z;

        ROS_INFO("orientation.z = %lf",current_pose.pose.orientation.z);
        ROS_INFO("error_yaw_between_plane_and_box1 = %lf",error_yaw_between_plane_and_box1);
        ROS_INFO("error_yaw_between_plane_and_box2 = %lf",error_yaw_between_plane_and_box2);
        
        if(distance_from_plane_to_box1 < d_max && distance_from_plane_to_box1 > d_min && error_yaw_between_plane_and_box1 < 0.2474 && error_yaw_between_plane_and_box1 > -0.2474)
        {//框1和飞机的指向角在-5到+5度之间
            ROS_INFO("find box1 successful!");
            sim_cam_info.find_obs_flag = true;//这时候认为可以看到框了
            sim_cam_info.dis = distance_from_plane_to_box1 + gauss_rand(e);//距离+噪声
            sim_cam_info.x_pos = (int)100*(current_pose.pose.position.y - box_1.pose.position.y);
            sim_cam_info.y_pos = (int)100*(box_1.pose.position.z - current_pose.pose.position.z);
        }

        if(distance_from_plane_to_box2 < d_max && distance_from_plane_to_box2 > d_min && error_yaw_between_plane_and_box2 < 0.2474 && error_yaw_between_plane_and_box2 > -0.2474)
        {
            ROS_INFO("find box2 successful!");
            sim_cam_info.find_obs_flag = true;
            sim_cam_info.dis = distance_from_plane_to_box2 + gauss_rand(e);
            sim_cam_info.x_pos = (int)100*(box_2.pose.position.y - current_pose.pose.position.y);
            sim_cam_info.y_pos = (int)100*(box_2.pose.position.z - current_pose.pose.position.z);
        }
        std::cout << "x = " << sim_cam_info.x_pos << "  y = " << sim_cam_info.y_pos << " dis1 = " << distance_from_plane_to_box1 << std::endl;
        //打印判断结果
        contours_pub_.publish(sim_cam_info);
        ros::spinOnce();
        rate.sleep();
    }
    sim_cam_info.find_obs_flag = false;
    sim_cam_info.dis = -1;
    sim_cam_info.pos_left = -1;
    sim_cam_info.pos_right = -1;
    sim_cam_info.x_pos = -1;
    sim_cam_info.y_pos = -1;//循环结束之后恢复默认值
    contours_pub_.publish(sim_cam_info);
    
    return 0;
}