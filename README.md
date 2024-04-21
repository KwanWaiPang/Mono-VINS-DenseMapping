 [comment]: <> (# Mono-VINS-DenseMapping)

 <h1 align="center"> Mono-VINS-DenseMapping (VINS-MONO+quadtree_mapping,仅供个人学习记录用)
  </h1>


[comment]: <> (  <h2 align="center">PAPER</h2>)
  <h3 align="center">
  <a href="https://github.com/HKUST-Aerial-Robotics/VINS-Mono">Original Github of VINS-MONO</a>
  | <a href="https://github.com/HKUST-Aerial-Robotics/open_quadtree_mapping">Original Github of open_quadtree_mapping</a>
  | <a href="https://github.com/arclab-hku/Event_based_VO-VIO-SLAM?tab=readme-ov-file#5-evi-sam">Testing Dataset</a>
  </h3>
  <div align="center"></div>

<br>

### Developer: [Guan Weipeng](https://kwanwaipang.github.io/), [Chen Peiyu](https://github.com/cpymaple)

<br>

  * 为了方便本人使用，对代码做了修改(适配ubuntu20.04)，源码请参见原github仓库~
  * 创建工作空间，然后catkin_make即可（运行前记得source一下）

~~~
  roslaunch open_quadtree_mapping example.launch
~~~

# 测试demo（可实现单目实时的稠密重建）
* [Mono-VINS-DenseMapping (单目稠密重建) 分辨率752*480](https://www.bilibili.com/video/BV1Yp421X7xt/?spm_id_from=333.788&vd_source=a88e426798937812a8ffc1a9be5a3cb7)
* [Mono-VINS-DenseMapping (单目VIO+单目稠密重建) 分辨率640*480](https://www.bilibili.com/video/BV15H4y1N7un/?vd_source=a88e426798937812a8ffc1a9be5a3cb7)
* [Mono-VINS-DenseMapping (单目VIO+单目稠密重建) 分辨率346*260](https://www.bilibili.com/video/BV1Ji42127Vi/?vd_source=a88e426798937812a8ffc1a9be5a3cb7)
<<<<<<< HEAD


# 添加了TSDF-based Map Fusion
* 效果很差，远不如EVI-SAM的效果～

~~~
  roslaunch open_quadtree_mapping example_hku_globalmapping.launch
~~~

<p align="center">
  <a href="">
    <img src="pic/2024-04-21 16-31-47 的屏幕截图.png" alt="teaser" width="80%">
  </a>

</p>
=======
>>>>>>> 621e79236d011a842407570c9f780f7c7160f5a4
