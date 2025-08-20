#! /bin/bash

gnome-terminal --tab -e 'bash -c "roscore;exec bash"'
sleep 3s

gnome-terminal --tab -e 'bash -c "rosparam set use_sim_time true;rosbag play --pause --clock ~/Datasets/113001.bag;exec bash"'
# gnome-terminal --tab -e 'bash -c "rosparam set use_sim_time true;rosbag play --pause --clock ~/Datasets/113002.bag;exec bash"'
sleep 1s

##################*********************在EVI-SAM下验证VINS********************************##############
gnome-terminal --tab -e 'bash -c "roslaunch vins_estimator d455.launch;exec bash"'
gnome-terminal --tab -e 'bash -c "roslaunch vins_estimator vins_rviz.launch;exec bash"'

##################*********************在ECMD下验证VINS********************************##############
# gnome-terminal --tab -e 'bash -c "roslaunch vins_estimator ecmd.launch;exec bash"'
# gnome-terminal --tab -e 'bash -c "roslaunch vins_estimator vins_rviz.launch;exec bash"'
# gnome-terminal --tab -e 'bash -c "rosrun image_transport republish compressed in:=/camera_left/image_color raw out:=/camera_left/image_color;exec bash"'


# sleep 1s
# gnome-terminal --window -e 'bash -c "rosbag play --pause -r 1.0 --clock /home/cpy/Datasets/hku_cyberport_0629_suburban_road_29.40_31.40.bag;exec bash"'
# gnome-terminal --window -e 'bash -c "rosbag play --pause -r 1.0 --clock /home/cpy/Datasets/qingma_night_0629_suburban_road_40912_41054.bag;exec bash"'
# gnome-terminal --window -e 'bash -c "rosbag play --pause -r 1.0 --clock /home/cpy/Datasets/small_circle_day_strong_0627_first_circle_new.synced.merged.bag;exec bash"'
# gnome-terminal --window -e 'bash -c "rosbag play --pause -r 1.0 --clock /home/cpy/Datasets/qingma_night_0629_urban_street_38913_39034.bag;exec bash"'
# gnome-terminal --window -e 'bash -c "rosbag play --pause -r 1.0 --clock /home/cpy/Datasets/highspeed_all_0627_highway_338_458.bag;exec bash"'
# gnome-terminal --window -e 'bash -c "rosbag play --pause -r 1.0 --clock /home/cpy/Datasets/qingma_night_0629_urban_road_39154_39224.bag;exec bash"'
# gnome-terminal --window -e 'bash -c "rosbag play --pause -r 1.0 --clock /home/cpy/Datasets/highspeed_all_0627_tunnel_593_618.bag;exec bash"'
# gnome-terminal --window -e 'bash -c "rosbag play --pause -r 1.0 --clock /home/cpy/Datasets/highspeed_all_0627_tunnel_303_338.bag;exec bash"'
# gnome-terminal --window -e 'bash -c "rosbag play --pause -r 1.0 --clock /home/cpy/Datasets/highspeed_all_0627_highway_483_593.bag;exec bash"'
# gnome-terminal --window -e 'bash -c "rosbag play --pause -r 1.0 --clock /home/cpy/Datasets/qingma_night_0629_highway_40614_40674.bag;exec bash"'
# gnome-terminal --window -e 'bash -c "rosbag play --pause -r 1.0 --clock /home/cpy/Datasets/qingma_night_0822_1_1370_1480.bag;exec bash"'
# gnome-terminal --window -e 'bash -c "rosbag play --pause -r 1.0 --clock /home/cpy/Datasets/highspeed_all_0627_bridge_858_968.bag;exec bash"'
# gnome-terminal --window -e 'bash -c "rosbag play --pause -r 1.0 --clock /home/cpy/Datasets/highspeed_all_0627_bridge_1423_1543.bag;exec bash"'




