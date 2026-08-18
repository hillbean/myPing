# myPing

自己编写的 ICMP ping 程序。当前只支持 IPv4，用法为 `./ping <destination>`（暂不支持 `-v` 等选项）。

程序通过 **RAW socket** 发送 `ICMP Echo Request`，并打印回复的序号、TTL 和 RTT。依赖 POSIX / Linux 网络接口（`SOCK_RAW`、`recvmsg`、`netinet/ip_icmp.h` 等），**不能用 MSVC 在原生 Windows 上直接编译运行**。

## 依赖

- GCC（或兼容的 `gcc`）
- 能创建 ICMP RAW socket 的权限（Linux 上通常需要 `root` / `sudo`）

## Linux

### 编译

在项目根目录执行：

```bash
gcc -O0 -g3 -Wall -o ping src/*.c
```

也可以使用 Eclipse CDT 生成的 Makefile：

```bash
cd Debug
make
```

产物为 `Debug/ping`。若 Eclipse 的链接步骤失败，改用上面的一条 `gcc` 命令即可。

### 运行

RAW ICMP socket 需要特权，请用 `sudo` 运行。最后一个参数为目标主机名或 IPv4 地址：

```bash
sudo ./ping 8.8.8.8
sudo ./ping www.example.com
```

示例输出：

```text
ping www.example.com (93.184.216.34): 56 data bytes.
64 bytes from 93.184.216.34: seq=0, ttl=54, rtt=12.345 ms
```

程序会每秒发送一次探测并进入无限循环，用 `Ctrl+C` 结束。

未传目标时会打印 usage 并退出。程序每秒发送一帧，收到 Echo Reply 后打印序号、TTL 和 RTT；用 `Ctrl+C` 结束。

## Windows

本仓库源码使用 Linux 头文件与 POSIX API，**不能**用 Visual Studio / MSVC 或普通 MinGW 直接编译成原生 `.exe`。

在 Windows 上请使用 **WSL（Windows Subsystem for Linux）**：

1. 安装 WSL 及某个 Linux 发行版（如 Ubuntu），并安装编译器：

   ```bash
   sudo apt update
   sudo apt install -y build-essential
   ```

2. 在 WSL 中进入本仓库目录，按上面的 **Linux** 步骤编译、运行：

   ```bash
   gcc -O0 -g3 -Wall -o ping src/*.c
   sudo ./ping 8.8.8.8
   ```

Cygwin 理论上也可提供 POSIX 环境，但仍需 RAW socket 权限，且 ICMP 行为与 Linux 不完全一致，**推荐优先使用 WSL**。

## 用法说明

```text
./ping <destination>
```

- `<destination>`：主机名或 IPv4 地址（取命令行最后一个参数）
- ICMP 数据长度为 56 字节（与常见 `ping` 默认一致）
- 暂不支持 `-v` 等选项；传入多余参数时仍取最后一个作为目标

## 工程结构

```text
src/          C 源码与 ping.h
Debug/        Eclipse CDT 调试构建产物与 Makefile
.cproject     Eclipse CDT 工程（Cross GCC）
```

用 Eclipse 打开时：导入已有 CDT 工程，选择本目录，构建 Debug 配置后运行生成的 `ping` 可执行文件（同样需要 root 权限）。
