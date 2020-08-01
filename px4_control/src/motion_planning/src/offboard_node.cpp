#include <ros/ros.h>

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>

#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/PositionTarget.h>

#include <tf/tf.h>

#include "cubic_spline/cubic_spline.h"
#include "motion_planning/motion_planning.h"
#include "motion_planning/camera_msg.h"

mavros_msgs::State current_state;//飞控当前状态
void state_cb(const mavros_msgs::State::ConstPtr& msg)
{
    current_state = *msg;
}

geometry_msgs::PoseStamped current_pose;//飞机位姿
void local_pos_cb(const geometry_msgs::PoseStamped::ConstPtr& curr_p)
{
    current_pose = *curr_p;
}

geometry_msgs::TwistStamped current_velocity;//飞机当前速度
void local_vel_cb(const geometry_msgs::TwistStamped::ConstPtr& curr_v)
{
    current_velocity = *curr_v;
}

motion_planning::camera_msg camera_data;//摄像头节点传来的消息
void cam_subCallback(const motion_planning::camera_msg::ConstPtr& cam_data)
{ 
	camera_data = *cam_data;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "motion_plannning_node");
    ros::NodeHandle nh;

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>
            ("mavros/state", 10, state_cb);
    ros::Subscriber local_velocity_sub = nh.subscribe
            ("/mavros/local_position/velocity_body", 1, local_vel_cb);
    ros::Subscriber local_position_sub = nh.subscribe
            ("/mavros/local_position/pose", 10, local_pos_cb);
    ros::Subscriber cam_sub = nh.subscribe
            ("/camera_topic", 5, cam_subCallback);//摄像头节点发布的话题

    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>
            ("mavros/setpoint_position/local", 10);//控制飞机运动

    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>
            ("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>
            ("mavros/set_mode");

   
    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

    // wait for FCU connection
    while(ros::ok() && !current_state.connected){
        ros::spinOnce();
        rate.sleep();
        ROS_INFO("connecting...");
    }
    ROS_INFO("connected");

    double x_go[] = {0.0, 50.0, 150.0, 200.0, 400.0, 500.0, 600.0, 750.0, 800.0};
    double y_go[] = {0.0, 50.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0};//飞机去时的路径点

    double x_return[] = {800.0, 750.0, 600.0, 500.0, 400.0, 200.0, 150.0, 100.0, 0.0};
    double y_return[] = {300.0, 300.0, 300.0, 300.0, 300.0, 300.0, 300.0, 250.0, 0.0};//飞机返回时的路径点

    int n_go = sizeof(x_go) / sizeof(double);
    int n_return = sizeof(x_return) / sizeof(double);
	int target_index = 0;
    int patrol_go_end_times = 0;
    int patrol_return_end_times = 0;

    bool enter_camtracking_from_patrol_go = 0;
    bool enter_camtracking_from_patrol_return = 0;
    bool landing_box1_success_flag = 0;
    bool landing_box2_success_flag = 0;//一系列标志

    double anti_x_return[n_return];
    double anti_y_return[n_return];

    for(int i = 0; i < n_return; ++i)
    {
        anti_x_return[i] = x_return[n_return - i - 1];
        anti_y_return[i] = y_return[n_return - i - 1];
        ROS_INFO("x = %lf", anti_x_return[i]);
        ROS_INFO("y = %lf", anti_y_return[i]);
    }//将返回时的路径倒序存放

    target_path_t target_path_go[n_go * UNIT];
    target_path_t target_path_return[n_return * UNIT];

    generate_target_course(x_go, y_go, target_path_go, n_go, GO);//生成规划后的路径
    generate_target_course(anti_x_return, anti_y_return, target_path_return, n_return, RETURN);

    ROS_INFO("n_go = %d",n_go);
    ROS_INFO("n_return = %d",n_return);

    geometry_msgs::PoseStamped target_pose;
    target_pose.pose.position.x = 0;
    target_pose.pose.position.y = 0;
    target_pose.pose.position.z = 14;

    //在进入在进入机外控制模式之前，就必须要开始发送设定值，否则模式切换会被拒绝
    for(int i = 100; ros::ok() && i > 0; --i){
        local_pos_pub.publish(target_pose);
        ros::spinOnce();
        rate.sleep();
    }

    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";

    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;

	MissionState PX4_current_mission_state = TakeOff;

    ros::Time last_request = ros::Time::now();
    

    while(ros::ok())
    {
        
        if( current_state.mode != "OFFBOARD" && (ros::Time::now() - last_request > ros::Duration(5.0)))
        {
            if( set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent)
            {
                ROS_INFO("Offboard enabled");
            }
            last_request = ros::Time::now();
        } 
        else 
        {
            if( !current_state.armed && (ros::Time::now() - last_request > ros::Duration(5.0)))
            {
                if( arming_client.call(arm_cmd) && arm_cmd.response.success)
                {
                    ROS_INFO("Vehicle armed");
                }
                last_request = ros::Time::now();
            }
        }

        if(!current_pose.pose.position.x && !current_pose.pose.position.y)
        {
            
        }
		else if(PX4_current_mission_state == TakeOff)//起飞模式
		{
			target_pose.pose.position.x = 0;
            target_pose.pose.position.y = 0;
            target_pose.pose.position.z = 14;

            ROS_INFO("State = TakeOff");

            if(14 - current_pose.pose.position.z < 0.2)//认为此时成功起飞
            {
                PX4_current_mission_state = Patrol_Go;
            }
		}
		else if(PX4_current_mission_state == Patrol_Go)
		{
            target_index = get_target_index(current_pose, current_velocity, target_path_go, n_go, GO);//获取合适的目标点
            ROS_INFO("index = %d",target_index);
            target_pose.pose.position.x = target_path_go[target_index].x;
            target_pose.pose.position.y = target_path_go[target_index].y;
            target_pose.pose.position.z = 14;
            target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(target_path_go[target_index].yaw);
                                                                    //在设置飞机位置的同时，也要对飞机的姿态进行设置
            
            ROS_INFO("State = Patrol_Go");

            if(target_index > (n_go - 1) * UNIT - 20)//此时证明已经到达去时的路径终点
            {
                patrol_go_end_times++;
                if(patrol_go_end_times > 3)//避免index的跳跃
                {
                    PX4_current_mission_state = Patrol_Return;
                }
            }
            
            if(camera_data.find_box_flag && !landing_box1_success_flag)//找到箱子就进入跟随摄像头模式，同时防止多次进入（因为摄像头看箱子没有最小距离）
            {
                PX4_current_mission_state = TrackingCam;
                enter_camtracking_from_patrol_go = 1;
            }

		}
        else if(PX4_current_mission_state == Patrol_Return)
		{
            target_index = get_target_index(current_pose, current_velocity, 
                                            target_path_return, n_return, RETURN);//获取合适的目标点
            ROS_INFO("index = %d",target_index);
            target_pose.pose.position.x = target_path_return[target_index].x;
            target_pose.pose.position.y = target_path_return[target_index].y;
            target_pose.pose.position.z = 14;
            target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(target_path_return[target_index].yaw);
                                                                    //在设置飞机位置的同时，也要对飞机的姿态进行设置

            ROS_INFO("State = Patrol_Return");

            if(current_pose.pose.position.x < 1 && current_pose.pose.position.y < 1)
            {
                PX4_current_mission_state = Landing;
            }
            if(camera_data.find_box_flag && !landing_box2_success_flag)//找到箱子就进入跟随摄像头模式，同时防止多次进入（因为摄像头看箱子没有最小距离）
            {
                PX4_current_mission_state = TrackingCam;
                enter_camtracking_from_patrol_return = 1;
            }

		}
		else if(PX4_current_mission_state == TrackingCam)
		{
            ROS_INFO("State = TrackingCam");
            if(fabs(camera_data.x_center_dis) < 1 && fabs(camera_data.y_center_dis) < 1 
                            && fabs(target_pose.pose.position.z - camera_data.box_hight) < 0.5)//此时认为飞机降落成功
            {
                if(enter_camtracking_from_patrol_go)    
                {
                    landing_box1_success_flag = 1;
                }
                else if(enter_camtracking_from_patrol_return)
                {
                    landing_box2_success_flag = 1;
                }
                for(int i = 0; i < 5; ++i)
                {
                    ROS_INFO("Landing Successful!");
                }
            }
            
            if(!camera_data.find_box_flag || (enter_camtracking_from_patrol_go & landing_box1_success_flag)
             || (enter_camtracking_from_patrol_return & landing_box2_success_flag))//中途看不到箱子或者成功降落
            {
                if(enter_camtracking_from_patrol_go)    
                {
                    PX4_current_mission_state = Patrol_Go;
                    enter_camtracking_from_patrol_go = 0;
                }
                else if(enter_camtracking_from_patrol_return)
                {
                    PX4_current_mission_state = Patrol_Return;
                    enter_camtracking_from_patrol_return = 0;
                }
            }
            else
            {
                target_pose.pose.position.x = camera_data.x_center_dis + current_pose.pose.position.x;
                target_pose.pose.position.y = camera_data.y_center_dis + current_pose.pose.position.y;
                if(fabs(camera_data.x_center_dis) < 5 && fabs(camera_data.y_center_dis) < 5)//接近箱子之后就可以慢慢降落了，在降落的过程中调整位置
                {
                    ROS_INFO("Landing to box...");
                    target_pose.pose.position.z -= (target_pose.pose.position.z - camera_data.box_hight) / 100;//缓慢降落
                }
            }
		}
        else if(PX4_current_mission_state == Landing)
        {
            target_pose.pose.position.x = 0;
            target_pose.pose.position.y = 0;
            target_pose.pose.position.z = 0;
            ROS_INFO("State = Landing");
            
            switch(landing_box1_success_flag + landing_box2_success_flag)//任务完成度报告
            {
            case 2:
                ROS_INFO("Complete ALL Missions!");
                break;
            case 1:
                ROS_INFO("Only Complete Half Of Missions!");
                break;
            case 0:
                ROS_INFO("Missions Fail!");
                break;
            }


            return 0;

        }

		//std::cout << current_state.mode << std::endl;
        std::cout << current_pose.pose.position.x << " " << current_pose.pose.position.y << " " << current_pose.pose.position.z << std::endl;

        local_pos_pub.publish(target_pose);
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
