import launch
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    """
    通过 IncludeLaunchDescription 启动其他 launch 文件。

    场景：当一个项目有多个子 launch 文件时，可以写一个"总 launch 文件"一键启动所有。
    也可以向被包含的 launch 文件传递参数（launch_arguments），覆盖其默认值。

    运行：
      ros2 launch cpp_topic actions.launch.py
    """

    # ======== 第一步：获取功能包的 share 目录 ========
    # get_package_share_directory 返回功能包的安装/构建后的 share 路径
    # launch 文件位于该目录下的 launch/ 子目录
    # 使用列表拼接路径是 ROS 2 的惯例写法，跨平台兼容
    pkg_share = get_package_share_directory('cpp_topic')

    # ======== 第二步：包含 demo.launch.py ========
    # IncludeLaunchDescription：把另一个 launch 文件当作一个 Action 嵌入当前描述
    # PythonLaunchDescriptionSource：指定要包含的 .py 格式 launch 文件路径
    # 这里没传 launch_arguments，所以被包含的 launch 文件全部使用默认值
    include_demo = launch.actions.IncludeLaunchDescription(
        PythonLaunchDescriptionSource([pkg_share, '/launch/demo.launch.py']),
    )

    # ======== 第三步：包含 talker_listener_param.launch.py，并覆盖参数 ========
    # launch_arguments 是一个 dict，写法和命令行 := 语法等价：
    #   命令行写法：ros2 launch xxx.launch.py start_count:=200
    #   launch_arguments 写法：{'start_count': '200'}
    # 注意：值必须是字符串类型
    include_talker_listener_param = launch.actions.IncludeLaunchDescription(
        PythonLaunchDescriptionSource([pkg_share, '/launch/talker_listener_param.launch.py']),
        launch_arguments={
            'start_count': '200',           # 覆盖默认值 0
            'publish_interval_ms': '500',   # 覆盖默认值 1000，改为 500ms 发一条
            # topic_name 不传，使用被包含 launch 文件中的默认值 '/my_topic'
        }.items(),                          # .items() 转成 [(key, value), ...] 格式
    )

    # ======== 第四步：一次性返回所有被包含的 launch 描述 ========
    # 每个 IncludeLaunchDescription 都会独立启动自己的节点集合
    # 这里会同时启动：
    #   - demo.launch.py 中的 talker + listener
    #   - talker_listener_param.launch.py 中的 talker_param + listener_param（带覆盖参数）
    return launch.LaunchDescription([
        include_demo,
        include_talker_listener_param,
    ])
