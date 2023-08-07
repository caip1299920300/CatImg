# 一、命令行显示图片

> 设置环境
>
> （如果只用于显示图片，./user/Opencv/otherlib目录可以删去）

```
# 配置lib的opencv环境
export LD_LIBRARY_PATH=:./user/Opencv/lib:$LD_LIBRARY_PATH
sudo ldconfig
```

> 测试

```
./user/CatImg ./img_movie/cai.jpg
```

![image-20230806235646744](.\img_movie\image-20230806235646744.png)

# 二、命令行显示视频(暂不支持)

> 设置环境

```
# 配置lib的opencv环境
export LD_LIBRARY_PATH=:./user/Opencv/lib:$LD_LIBRARY_PATH:./user/Opencv/otherlib
sudo ldconfig
```

> 测试

```
# CatMovie 视频路径 显示一帧的时长ms为单位
./user/CatMovie ./img_movie/hongmao.mp4 2000
```

![image-20230807004041422](.\img_movie\image-20230807004041422.png)