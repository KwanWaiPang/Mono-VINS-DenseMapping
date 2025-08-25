#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_types.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <iostream>
#include <image_geometry/pinhole_camera_model.h>

using namespace std;
 
ros::Publisher pub_point_cloud2;
std::string PointCloud_frameid;
bool colorful;
 
bool is_K_empty = 1;//只读一次
image_geometry::PinholeCameraModel model;//相机模型

cv_bridge::CvImageConstPtr rgb_ptr;//读入的rgb image

// double K[9];//相机内参矩阵
//     [fx  0 cx]
// K = [ 0 fy cy]
//     [ 0  0  1]

void rgbImage_callback(const sensor_msgs::ImageConstPtr &msg)
{
    try
    {
        rgb_ptr = cv_bridge::toCvShare(msg, msg->encoding);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
}
 
void depthImage_callback(const sensor_msgs::ImageConstPtr &raw_img_msg)
{
    if(is_K_empty){
        ROS_WARN("NO CAMERA INFO");
        return;
    }

    // 修改图像尺寸
    cv_bridge::CvImageConstPtr cv_ptr= cv_bridge::toCvShare(raw_img_msg, raw_img_msg->encoding);
    cv::Mat resized_image;
    cv::resize(cv_ptr->image, resized_image, cv::Size(model.fullResolution().width, model.fullResolution().height));
    // 将OpenCV图像转换回ROS图像消息
    sensor_msgs::ImageConstPtr img_msg = cv_bridge::CvImage(raw_img_msg->header, raw_img_msg->encoding, resized_image).toImageMsg();

    // Step1: 读取深度图
    //ROS_INFO("image format: %s %dx%d", img_msg->encoding.c_str(), img_msg->height, img_msg->width);
    int height = img_msg->height;
    int width = img_msg->width;
    
    cv_bridge::CvImageConstPtr depth_ptr= cv_bridge::toCvShare(img_msg, img_msg->encoding);
    cv::Mat depth = depth_ptr->image;//此处相当于不停的更新depth_mat


    // 通过指针强制转换，读取为16UC1数据，单位是mm
    unsigned short *depth_data = (unsigned short*)&img_msg->data[0];//所有的深度数据

    // Step2: 深度图转点云
    // 设置点云
    sensor_msgs::PointCloud2 point_cloud2;
    point_cloud2.header=img_msg->header;
    point_cloud2.height=img_msg->height;//以点数指定点云数据集的高度
    point_cloud2.width=img_msg->width;//以点数指定点云数据集的宽度
    point_cloud2.is_dense=true;//如果包含Inf / NaN值，则为false，否则为真
    point_cloud2.is_bigendian=false;// Is this data bigendian?
    point_cloud2.fields.clear();//（描述二进制数据blob中的通道及其布局。）此消息包含N维点的集合，其中可能包含法线，强度等附加信息。点数据以二进制Blob存储，其布局由“fields”数组的内容描述
    point_cloud2.fields.reserve(2);

    // //输出point_cloud2的信息
    // std::cout<<"point_cloud2.header="<<point_cloud2.header<<std::endl;
    // std::cout<<"point_cloud2.height="<<point_cloud2.height<<std::endl;
    // std::cout<<"point_cloud2.width="<<point_cloud2.width<<std::endl;

    //普通点云
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);//此处单纯是XYZ
    for(int uy=0; uy<height; uy++)
    {
        for(int ux=0; ux<width; ux++)
        {
            //遍历深度图
            float x, y, z;
            z = *(depth_data + uy*width + ux) / 1000.0;
            if(z!=0)//若不为0，就是存在这个点，然后添加这个点到点云
            {
                // 计算这个点的空间坐标
                // x = z * (ux - K[2]) / K[0];
                // y = z * (uy - K[5]) / K[4];
                x = (ux - model.cx()) * z / model.fx();
                y = (uy - model.cy()) * z / model.fy();
                pcl::PointXYZ p(x, y, z);
                cloud->push_back(p);
                
            }
        }
    }

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_RGBA(new pcl::PointCloud<pcl::PointXYZRGB>);  //创建XYZRGB点云
    if(colorful){
        cv::Mat rgb_img;//彩色图
        if (rgb_ptr != nullptr) {//当获取了rgb图像
            if (rgb_ptr->image.type()==CV_8UC1) {//若为单通道
                cv::Mat image_single = rgb_ptr->image;    
                cv::cvtColor(image_single, rgb_img, cv::COLOR_GRAY2RGB);//变换为三通道的       
            }
            else if(rgb_ptr->image.type()==CV_8UC3){//三通道的image
                rgb_img = rgb_ptr->image;
                //先变为单通道
                cv::cvtColor(rgb_img, rgb_img, cv::COLOR_BGR2GRAY);
                //再变为三通道
                cv::cvtColor(rgb_img, rgb_img, cv::COLOR_GRAY2RGB);
            }
        }
        else
            return;
        // rgb_img.resize(img_msg->data.size());//保证rgb的size跟深度图的一样
        cv::resize(rgb_img, rgb_img, cv::Size(width, height), 0, 0, cv::INTER_LINEAR);//保证rgb的size跟深度图的一样

        for (int m = 0; m < depth.rows; m++)
        {
            for (int n=0; n < depth.cols; n++)
            {
                ushort d = depth.ptr<ushort>(m)[n];   // 获取深度图中(m,n)处的值

                // d 可能没有值，若如此，跳过此点
                if (d == 0)
                    continue;
                // d 存在值，则向点云增加一个点

                pcl::PointXYZRGB p;

                // 计算这个点的空间坐标
                p.z = double(d) / 1000;
                p.x = (n - model.cx()) * p.z / model.fx();
                p.y = (m - model.cy()) * p.z / model.fy();

                // 从rgb图像中获取它的颜色
                // rgb是三通道的BGR格式图，所以按下面的顺序获取颜色
                p.b = rgb_img.ptr<uchar>(m)[n*3];
                p.g = rgb_img.ptr<uchar>(m)[n*3+1];
                p.r = rgb_img.ptr<uchar>(m)[n*3+2];
                 
                // 把p加入到点云中
                cloud_RGBA->points.push_back( p );
            }
        }

        // int length = rgb_img.rows * rgb_img.cols;
        // //动态创建数组存放rgb值
        // uchar*R_array = new uchar[length];
        // uchar*G_array = new uchar[length];
        // uchar*B_array = new uchar[length];

        // for (int r = 0; r < rgb_img.rows; r++) //遍历图片存放rgb值
        // {
        //     for (int c = 0; c < rgb_img.cols; c++)
        //     {
        //         B_array[r*rgb_img.cols+c] = rgb_img.at<cv::Vec3b>(r, c)[0];//注意opencvz中Scalar(B,G,R)不是R，G，B这个顺序
        //         G_array[r*rgb_img.cols+c] = rgb_img.at<cv::Vec3b>(r, c)[1];
        //         R_array[r*rgb_img.cols+c] = rgb_img.at<cv::Vec3b>(r, c)[2];
        //     }
        // }

        // //彩色点云
        // // pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_RGBA(new pcl::PointCloud<pcl::PointXYZRGB>);  //创建XYZRGB点云
        // cloud_RGBA->width = length;  //点云初始化，点云数量和图像像素数量一样
        // cloud_RGBA->height = 1;
        // cloud_RGBA->is_dense = true;
        // cloud_RGBA->resize(length);
        // for(int t = 0; t < length; t++) //给点云赋值（RGB值和XYZ坐标）
        // {
        //     cloud_RGBA->points[t].r = R_array[t];
        //     cloud_RGBA->points[t].g = G_array[t];
        //     cloud_RGBA->points[t].b = B_array[t];
        //     cloud_RGBA->points[t].x = cloud->points[t].x;
        //     cloud_RGBA->points[t].y = cloud->points[t].y;
        //     cloud_RGBA->points[t].z = cloud->points[t].z;
        // }
    }

    // Step3: 发布点云
    if(colorful){
        // cloud_RGBA->height = 1;
        // cloud_RGBA->width = cloud->points.size();
        pcl::toROSMsg(*cloud_RGBA, point_cloud2);
    }
    else{
        pcl::toROSMsg(*cloud, point_cloud2);
    }


    // point_cloud2.header.frame_id = "world";
    point_cloud2.header.frame_id = PointCloud_frameid;
    // point_cloud2.header.stamp=ros::Time::now();
    // std::cout<<"time="<<point_cloud2.header.stamp.toSec()<<std::endl;
    // std::cout<<"pointcloud frame_id="<<point_cloud2.header.frame_id<<std::endl;
    pub_point_cloud2.publish(point_cloud2);
}


void camera_info_callback(const sensor_msgs::CameraInfoConstPtr &camera_info_msg)
{
    // 读取相机参数
    if(is_K_empty)
    {
        // for(int i=0; i<9; i++)
        // {
        //     K[i] = camera_info_msg->K[i];
        // }
        model.fromCameraInfo(camera_info_msg);
        is_K_empty = 0;
        std::cout<<"info of the camera:"<<std::endl
             <<model.fullResolution().width<<" X "<<model.fullResolution().height<<std::endl
             <<model.fx()<<std::endl
             <<model.fy()<<std::endl
             <<model.cx()<<std::endl
             <<model.cy()<<std::endl;
    }
}
 
int main(int argc, char **argv)
{
    ros::init(argc, argv, "DepthtoPointcloud_node");
    ROS_INFO("Runing the node of obtaining pointcloud from depth map");
    ros::NodeHandle n;

    //参数调取
    n.getParam("PointCloud_frameid", PointCloud_frameid);
    n.getParam("colorful", colorful);
    // n.param<std::string>("PointCloud_frameid", PointCloud_frameid, "depth_camera");
    // n.param<bool>("colorful", colorful, false);
    // std::cout<<"colorful="<<colorful<<std::endl;
    std::cout<<"PointCloud_frameid="<<PointCloud_frameid<<std::endl;
    ros::Subscriber  sub_img;
    if (colorful){//是否彩色点云
        sub_img = n.subscribe("/image_rgb", 1000, rgbImage_callback); //获取彩色相机
    }
    
     // 获取深度相机参数
    ros::Subscriber sub_cmara_info = n.subscribe("camera_info", 1, camera_info_callback);
    // 深度图，在其回调函数中把深度图转化为点云，并发布出来
    ros::Subscriber sub_depth_img = n.subscribe("/image_depth", 1000, depthImage_callback);

    //要发布的点云
    pub_point_cloud2 = n.advertise<sensor_msgs::PointCloud2>("/PointCloud_from_DepthImage", 1000);
    
    ros::spin();
    return 0;
}


// 参考
// https://blog.csdn.net/Jeff_zjf/article/details/124255916
// https://blog.csdn.net/weixin_43464623/article/details/117856642