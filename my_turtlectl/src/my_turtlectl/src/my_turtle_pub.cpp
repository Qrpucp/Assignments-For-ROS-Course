/*
如果直接用tf坐标变换得到角速度和线速度的话，小乌龟行进轨迹是一条曲线。
所以我决定先让小乌龟的头部对准目标点，再前进。
有时候小乌龟需要转大于PI/2的角度，还需测试。
*/

#include "ros/ros.h"
#include "my_turtlectl/Pose.h"
#include <geometry_msgs/Twist.h>

extern bool move2xy(double x, double y);

typedef struct
{
	float x;
	float y;
}point_t;

 my_turtlectl::Pose current_pose;
 void chatterCallback(const my_turtlectl::Pose::ConstPtr& curr_p)
{
	current_pose = *curr_p;//获取小乌龟的当前位置
}

geometry_msgs::Twist target_pose;

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "my_turtle_pub");
	ros::NodeHandle n;

	ros::Publisher chatter_pub = n.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 40);
	ros::Subscriber sub = n.subscribe("/turtle1/pose", 40, chatterCallback);

	ros::Rate loop_rate(40);//频率稍微高一点

	point_t point[6] = {{3,2},{5.5,9.69},{8,2},{1.46,6.75},{9.54,6.75},{3,2}};//设定点

	while (ros::ok())
	{
		//避免在调用move2xy函数之前，还没有获取到小乌龟的当前位姿
		while(!current_pose.x)
		{
			ros::spinOnce();
			loop_rate.sleep();
		}
		for(int i = 0; i < sizeof(point)/(2*sizeof(float)); ++i)//依次移动到设定点
		{
			while(!move2xy(point[i].x,point[i].y))
			{
				chatter_pub.publish(target_pose);
				ros::spinOnce();
				loop_rate.sleep();
			}
		}
		//避免小乌龟在程序退出时还有速度
		while(current_pose.linear_velocity || current_pose.angular_velocity)
		{
			target_pose.angular.z = 0;
			target_pose.linear.x = 0;
			chatter_pub.publish(target_pose);
			ros::spinOnce();
			loop_rate.sleep();
		}
		return 0;
	}

	return 0;
}

bool move2xy(double x, double y)
{
	static double last_x = 0, last_y = 0;//记录是否更换目标点
	static double target_theta;
	static bool flag = 0;//每一个目标点角度只计算一次

	if(last_x != x || last_y != y)
	{
		last_x = x;
		last_y = y;
		flag = 0;
	}

	if(!flag)
	{
		if(x - current_pose.x > 0)
		{
			target_theta = atan((y - current_pose.y) / (x - current_pose.x));
		}
		else
		{
			//arctan的范围是正负PI/2，所以这里要加半个周期
			target_theta = atan((y - current_pose.y) / (x - current_pose.x)) + M_PI;
		}
		if(fabs(target_theta) < 0.05)//如果角度小于0.05度，认为小乌龟走的是直线
		{
			target_theta = 2 * M_PI;//这里不能等于0，否则在接近0的时候小乌龟的线速度最大
		}
		flag = 1;
	}
	
	target_pose.angular.z = 0;
	target_pose.linear.x = 0;//初始值需要设定为0

	if(fabs(current_pose.theta - target_theta) > 0.0001)//角度不精确的话，最后的位置会偏很多
	{
		if(target_theta > 0)
		{
			//P控制器控制小乌龟的角速度，比固定值要好一点
			target_pose.angular.z = fabs((current_pose.theta - target_theta));
		}
		else
		{
			target_pose.angular.z = -fabs((current_pose.theta - target_theta));
		}
		return 0;
	}
	else if(fabs(current_pose.x - x) > 0.2 || fabs(current_pose.y - y) > 0.2)
	{
		//线速度使用固定值就可以了，误差只存在于角度方面
		target_pose.linear.x = 2;
		return 0;
	}
	else
	{
		return 1;//表示到达了设定点
	}
}