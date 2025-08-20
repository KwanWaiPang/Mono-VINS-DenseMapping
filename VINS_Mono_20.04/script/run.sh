#! /bin/bash

gnome-terminal --tab -e 'bash -c "roscore;exec bash"'
sleep 3s


##################*********************在EVI-SAM下验证VINS********************************##############
gnome-terminal --tab -e 'bash -c "source ~/catkin_ws/devel/setup.bash; roslaunch vins_estimator grcbank.launch;exec bash"'
gnome-terminal --tab -e 'bash -c "source ~/catkin_ws/devel/setup.bash; roslaunch vins_estimator vins_rviz.launch;exec bash"'



sleep 1s

# gnome-terminal --window -e 'bash -c "rosparam set use_sim_time true;rosbag play --pause --clock -s 50.0 ~/dataset/room708_imu_calib_20250818.bag;exec bash"'

# gnome-terminal --window -e 'bash -c "rosparam set use_sim_time true;rosbag play --pause --clock ~/dataset/room708/room708_push_20250818_1.bag;exec bash"'
gnome-terminal --window -e 'bash -c "rosparam set use_sim_time true;rosbag play --pause --clock ~/dataset/room708/room708_push_20250818_2.bag;exec bash"'
# gnome-terminal --window -e 'bash -c "rosparam set use_sim_time true;rosbag play --pause --clock ~/dataset/room708/room708_robot_20250818_1.bag;exec bash"'
# gnome-terminal --window -e 'bash -c "rosparam set use_sim_time true;rosbag play --pause --clock ~/dataset/room708/room708_robot_20250818_2.bag;exec bash"'


