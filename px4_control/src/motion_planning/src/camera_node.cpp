#include <ros/ros.h>
#include <tf/tf.h>
#include <geometry_msgs/PoseStamped.h>

#include "motion_planning/camera_msg.h"

#define DIS_MAX 100 //能看见箱子的最大距离
#define DIS_MIN 0 //不存在看不见箱子的最小距离(和穿框不一样)
#define K_ERROR 10  //摄像头返回距离的误差系数
					//对于一个5*5*5米的箱子，飞机在14米高飞行，误差最大值在正负8m左右，误差中位数在正负3m左右（目测）

typedef struct
{
	double length, width, hight;
	double x_center, y_center;
	geometry_msgs::Quaternion quaternion;
}box_t;//箱子的尺寸和位姿

geometry_msgs::PoseStamped current_pose;//飞机当前位姿
void local_pos_cb(const geometry_msgs::PoseStamped::ConstPtr& curr_p)
{
	current_pose = *curr_p;
}

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "camera_node");//单无人机，所以只有一个节点
    ros::NodeHandle nh;

    ros::Subscriber local_position_sub = nh.subscribe("/mavros/local_position/pose", 10, local_pos_cb);

    ros::Publisher camera_pub = nh.advertise<motion_planning::camera_msg>("/camera_topic", 5);

	ros::Rate rate(20.0);

	box_t box_1 = {5,5,5,500,120};
	box_t box_2 = {5,5,5,200,300};//设定两个箱子的尺寸和空间位置
	box_1.quaternion = tf::createQuaternionMsgFromYaw(0.00);
	box_2.quaternion = tf::createQuaternionMsgFromYaw(M_PI);//获取两个箱子的四元数

	motion_planning::camera_msg camera_info;
	double box1_dis, box2_dis;//飞机和箱子之间的距离
	double error_yaw_between_plane_and_box1;
	double error_yaw_between_plane_and_box2;//飞机和箱子之间的yaw轴偏差

	std::default_random_engine e; //随机引擎
	std::normal_distribution<double> gauss_rand(0, 0.1);//生成随机噪声

	while(ros::ok())
	{
		camera_info.find_box_flag = 0;//默认值
		camera_info.y_center_dis = -1;
		camera_info.x_center_dis = -1;
		camera_info.box_hight = -1;

		box1_dis = sqrt(pow(box_1.x_center - current_pose.pose.position.x,2) + 
						pow(box_1.y_center - current_pose.pose.position.y,2));
		box2_dis = sqrt(pow(box_2.x_center - current_pose.pose.position.x,2) + 
						pow(box_2.y_center - current_pose.pose.position.y,2));

		error_yaw_between_plane_and_box1 = fabs(current_pose.pose.orientation.z) - box_1.quaternion.z;
        error_yaw_between_plane_and_box2 = fabs(current_pose.pose.orientation.z) - box_2.quaternion.z;
		
		
		ROS_INFO("current_pose.pose.orientation.z = %lf", current_pose.pose.orientation.z);
		ROS_INFO("error_yaw_between_plane_and_box1 = %lf", error_yaw_between_plane_and_box1);
		ROS_INFO("error_yaw_between_plane_and_box2 = %lf", error_yaw_between_plane_and_box2);
		

		if(box1_dis > DIS_MIN && box1_dis < DIS_MAX && error_yaw_between_plane_and_box1 < 0.2474 
													&& error_yaw_between_plane_and_box1 > -0.2474)//箱子在可视范围内，且箱子与飞机yaw轴的夹角在正负5度之间
		{
			camera_info.find_box_flag = 1;
			camera_info.box_hight = box_1.hight;
			camera_info.x_center_dis = (box_1.x_center - current_pose.pose.position.x) + 
								gauss_rand(e) * pow((current_pose.pose.position.z - box_1.hight), 2) / 
								(box_1.length * box_1.width) * K_ERROR;//摄像头测距的误差与箱子的尺寸以及飞机与箱子间的距离有关
			camera_info.y_center_dis = (box_1.y_center - current_pose.pose.position.y) + 
								gauss_rand(e) * pow((current_pose.pose.position.z - box_1.hight), 2) / 
								(box_1.length * box_1.width) * K_ERROR;
			ROS_INFO("rand = %lf",gauss_rand(e) * pow((current_pose.pose.position.z - box_2.hight), 2) / 
								(box_2.length * box_2.width) * K_ERROR);
		}
		if(box2_dis > DIS_MIN && box2_dis < DIS_MAX && error_yaw_between_plane_and_box2 < 0.2474 
													&& error_yaw_between_plane_and_box2 > -0.2474)//二号箱子
		{
			camera_info.find_box_flag = 1;
			camera_info.box_hight = box_2.hight;
			camera_info.x_center_dis = (box_2.x_center - current_pose.pose.position.x) + 
								gauss_rand(e) * pow((current_pose.pose.position.z - box_2.hight), 2) / 
								(box_2.length * box_2.width) * K_ERROR;
			camera_info.y_center_dis = (box_2.y_center - current_pose.pose.position.y) + 
								gauss_rand(e) * pow((current_pose.pose.position.z - box_2.hight), 2) / 
								(box_2.length * box_2.width) * K_ERROR;
			ROS_INFO("rand = %lf",gauss_rand(e) * pow((current_pose.pose.position.z - box_2.hight), 2) / 
								(box_2.length * box_2.width) * K_ERROR);
		}
		
		//ROS_INFO("x_center_dis = %lf",camera_info.x_center_dis);
		//ROS_INFO("y_center_dis = %lf",camera_info.y_center_dis);
		//ROS_INFO("current_pose.x = %lf", current_pose.pose.position.x);
		//ROS_INFO("current_pose.y = %lf", current_pose.pose.position.y);
		//ROS_INFO("current_pose.z = %lf", current_pose.pose.position.z);
		ROS_INFO("find_box_flag = %d",camera_info.find_box_flag);
		
		camera_pub.publish(camera_info);
        ros::spinOnce();
        rate.sleep();
	}

	camera_info.find_box_flag = 0;//默认值
	camera_info.y_center_dis = -1;
	camera_info.x_center_dis = -1;
	camera_info.box_hight = -1;

	camera_pub.publish(camera_info);//发布话题

	return 0;
}