# HPM6E00 RT-Thread Out-of-tree BSP 开发指南

本工程已完成“树外（Out-of-tree）”重构，实现了 RT-Thread 内核与 HPM SDK 的解耦。您可以直接使用标准 SCons 流程进行开发。

## 1. 核心修改点（环境打通的关键）

为了让工程跑起来，我们做了以下关键调整：

- **构建路径解耦 (`rtconfig.py`)**：
    - 重新定义了 `RTT_ROOT` 和 `HPM_SDK_BASE`，指向外部独立路径。
    - 锁定了本地 GCC 工具链路径。
- **自动化构建策略 (`SConstruct`)**：
    - **SDK 扫描**：自动包含了 SoC 特定的头文件路径（如 `soc/HPM6E00/HPM6E80`）。
    - **驱动补全**：在 `SDK_SRC` 列表中手动添加了必要的 SDK 驱动源文件（Clock, Sysctl, GPIO, UART, DMAv2 等）。
    - **全局包含**：在编译选项中注入了 `-include hpm_soc.h`，确保底层中断号和寄存器宏定义在全工程可见。
- **链接冲突修复 (`flash_rtt.ld`)**：
    - 手动定义了 `.fast_ram.bss` 段并标记为 `(NOLOAD)`，解决了链接时常见的 `section overlaps` 段重叠报错。
- **启动逻辑修复 (`rtt_board.c`)**：
    - **早期堆初始化**：将 `rt_system_heap_init` 挪到了 `rt_hw_board_init` 的最顶端（使用了 `0x01240000` 附近的 AXI SRAM），防止 `dma_mgr_init` 等早期组件因申请不到内存而死循环。

## 2. 后续开发注意事项

### 如何添加您的代码？
- **普通文件**：直接放入 `applications/` 目录。工程会自动扫描该目录下所有的 `.c` 文件，**无需修改任何配置**。
- **子模块/文件夹**：如果您新建了文件夹（如 `services/`），请在 `SConstruct` 中仿照 `applications` 的写法添加扫描逻辑。

### 如何添加新的 SDK 驱动？
- 如果您后续需要使用 PWM、CAN、SPI 等外设，请在 `SConstruct` 文件的 `SDK_SRC` 列表中添加对应的 `.c` 路径，例如：
  `os.path.join(HPM_SDK_BASE, 'drivers', 'src', 'hpm_mcan_drv.c')`

### 如何添加大型第三方库？
- 建议将库放入 `libraries/` 目录。
- 在库文件夹内创建一个 `SConscript` 文件（参考 RT-Thread 标准）。
- 在主 `SConstruct` 中使用 `objs += SConscript('libraries/xxx/SConscript')` 引入。

### 编译与清理
- **编译**：`scons -j8`
- **清理**：`scons -c`
- **生成工程**：`scons --target=vs -s`（如果您想更新 VSCode 的智能感知）

## 3. 内存布局提示
- **堆空间**：目前手动固定在 `0x01240000` 到 `0x01260000` (128KB)。如需更大空间，请修改 `board/rtt_board.c`。
- **段映射**：如果以后遇到 `undefined reference to _ramfunc_start` 之类的错误，请检查 `flash_rtt.ld` 的段定义。

---
**Happy Coding! -- Antigravity AI Assistant**
