# 新仓库上传文件步骤

精简版

1 git init

2 git add . (所有文件)

3 git commit -m "Initial commit"（提交信息）

4 git remote add origin git@github.com:Elecrow-RD/CrowPanel-Advance-HMI-Courses.git（将本地仓库与远程 GitHub 仓库关联起来。使用以下命令将远程仓库添加为 origin：）

5$ git push -u origin master --force（**如果远程仓库没有重要的变更**，你可以强制推送本地代码，覆盖远程分支的内容。但这种做法有风险，可能会丢失远程仓库上的更改。可以使用 --force 参数：）





# 本地仓库与原创仓库未同步，需要先拉取完整的仓库再进行删除新增操作。

------

# 🚀 第 1 步：换到干净工作目录（关键）

不要再用你之前那个坏 repo。

```
cd F:/wiki
mkdir git_safe
cd git_safe
```

# 第 2 步：初始化 Git

```
git init
```

------

# 🚀 第 3 步：连接远程仓库

```
git remote add origin https://github.com/Elecrow-RD/CrowPanel-Advance-5-HMI-ESP32-S3-AI-Powered-IPS-Touch-Screen-800x480.git

git remote add origin https://github.com/Elecrow-RD/CrowPanel-Advance-7-HMI-ESP32-S3-AI-Powered-IPS-Touch-Screen-800x480.git

git remote add origin https://github.com/Elecrow-RD/CrowPanel-Advance-4.3-HMI-ESP32-S3-AI-Powered-IPS-Touch-Screen-800x480.git

git remote add origin https://github.com/Elecrow-RD/CrowPanel-Advance-3.5-HMI-ESP32-S3-AI-Powered-IPS-Touch-Screen-480x320.git

git remote add origin https://github.com/Elecrow-RD/CrowPanel-Advance-2.8-HMI-ESP32-S3-AI-Powered-IPS-Touch-Screen-320x240.git

git remote add origin https://github.com/Elecrow-RD/CrowPanel-Advance-2.4-HMI-ESP32-S3-AI-Powered-IPS-Touch-Screen-320x240.git

git remote add origin https://github.com/Elecrow-RD/-CrowPanel-Advanced-5inch-ESP32-P4-HMI-AI-Display-800x480-IPS-Touch-Screen.git

git remote add origin https://github.com/Elecrow-RD/CrowPanel-Advanced-7inch-ESP32-P4-HMI-AI-Display-1024x600-IPS-Touch-Screen.git

git remote add origin https://github.com/Elecrow-RD/CrowPanel-Advanced-9inch-ESP32-P4-HMI-AI-Display-1024x600-IPS-Touch-Screen.git

git remote add origin https://github.com/Elecrow-RD/CrowPanel-Advanced-10.1inch-ESP32-P4-HMI-AI-Display-1024x600-IPS-Touch-Screen.git

git remote add origin https://github.com/Elecrow-RD/All-in-one-Starter-Kit-for-ESP32-P4-with-Common-Board-design.git

git remote add origin https://github.com/Elecrow-RD/CrowPanel-2.4-ESP32-HMI-320x240.git

git remote add origin https://github.com/Elecrow-RD/CrowPanel-2.8-ESP32-HMI-320x240.git

git remote add origin https://github.com/Elecrow-RD/CrowPanel-3.5-HMI-ESP32-Display-480x320.git

git remote add origin https://github.com/Elecrow-RD/CrowPanel-4.3-HMI-ESP32-Display-480x272.git

git remote add origin https://github.com/Elecrow-RD/CrowPanel-5.0-HMI-ESP32-Display-800x480.git

git remote add origin https://github.com/Elecrow-RD/CrowPanel-7.0-HMI-ESP32-Display-800x480.git

```

------

# 🚀 第 4 步：用“无 pack 模式”拉取（避免你现在的错误）

这是重点（绕过你之前失败的 pack）

```
git -c core.compression=0 fetch origin
```

------

# 🚀 第 5 步：切换到远程主分支

```
git checkout -B master origin/master
```

------

# 🚀 第 6 步：确认 example/V1.2 是否存在

```
ls example
```

如果看到：

- ✔ V1.2 → 直接下一步
- ❌ 没有 → 创建

```
mkdir -p example/V1.2

mkdir example/V1.0_and_V2.0
```

------

# 🚀 第 7 步：复制你的文件进去（追加，不破坏）

（增加文件先复制新增文件到本地仓库）

假设你的文件在：

```
F:/wiki/new_files/
```

执行：

```
cp -r /f/wiki/new_files/* example/V1.2/
```

👉 作用：

- 新文件 ✔ 添加
- 同名文件 ⚠️ 覆盖
- 其他远程文件 ✔ 不动

------

# 🚀 第 8 步：只提交这个目录

（删除或者新增操作）

```
git add example/V1.2/

git add example/V1.0/idf-code/Lesson13-Camera_Real-Time/

git add libraries

git add example/V1.2_and_V1.3_and_V1.4/Mimiclaw

git add example/V1.0/Arduino/libraries example/V1.2_and_V1.3_and_V1.4

修改文件名：
git mv Datasheet docs/Datasheet
git mv example samples
git mv "3D file" 3d_models

移动文件目录，先新建文件目录
$ git mv example/Esphome example/V1.0_and_V2.0


```

------

# 🚀 第 9 步：提交

```
git commit -m "sync example/V1.2"
```

------

# 🚀 第 10 步：推送

```
git push origin master
```







