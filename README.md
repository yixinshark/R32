### r32程序打包说明
确保安装了Inno Setup 软件，提供安装包：innosetup-6.2.2.exe,双击安装即可。打包文件r32setup.iss文件点击即可打开。
#### 1. 编译release版本r32.exe可执行二进制文件
#### 2. 将r32.exe放到exe目录下
#### 3. 运行r32setup.iss,点击编译即可在当前文件夹下生成R32_Setup_1.0.0.exe安装文件

### 修改安装包程序版本号：
#### 1. 修改r32setup.iss文件中的#define MyAppVersion "1.0.0"版本号

见示意图：setup.png