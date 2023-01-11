﻿# 依赖

- Reflection
- Serializer

# 功能

- 高级的引用类型
    - 安全指针
    - 支持主动销毁
- 序列化和持久化
- 集中对象管理：如查找、遍历
- 生命周期
    - 原生状态：单纯的Object指针物体，仅支持有限的功能，不安全
    - 实例状态：触发生命周期并获得所有功能权限，会自动实例关联物体