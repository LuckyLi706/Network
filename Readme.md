# 网络学习（C/C++）

## Clion远程连接linux服务器 开发调试

```shell
# 首先卸载预装Cmake 2
yum remove cmake

# 首先安装必要的环境
yum install -y gcc g++ gcc-c++ make automake texinfo wget

# Cmake安装
wget https://cmake.org/files/v3.14/cmake-3.14.3.tar.gz
tar -xf cmake-3.14.3.tar.gz
cd cmake-3.14.3
./configure
make
make install

# 安装好后的Cmake位于 /usr/local/share/cmake-3.13/，可执行程序位于/usr/local/bin/cmake，为了让CLion能够自动识别，构建软连接。
ln -s /usr/local/bin/cmake /usr/bin/cmake

# 在安装GDB 前请先安装termcap库，否则在安装gdb时会报该库找不到的错误。
wget https://ftp.gnu.org/gnu/termcap/termcap-1.3.1.tar.gz
tar -xf termcap-1.3.1.tar.gz
cd termcap-1.3.1
./configure
make install

# GDB安装
yum remove -y gdb  # 卸载预装的gdb
wget http://mirrors.ustc.edu.cn/gnu/gdb/gdb-7.9.tar.xz
tar -xf gdb-7.9.tar.xz
cd gdb-7.9
./configure
# gdb将被安装到/usr/local/share/gdb目录，可执行程序位于/usr/local/bin/gdb，为了让CLion能够自动识别，构建软连接
ln -s /usr/local/bin/gdb /usr/bin/gdb 
```

