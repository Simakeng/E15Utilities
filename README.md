# GameEngine

俩年前（2018）开始造的游戏引擎，人手不足依旧进度缓慢，2019年夏因为个人原因放弃继续开发，2020年春重新开始代码整理，并着手进行文档化。

## 目前状态
积极活跃的开发者目前只有(贡献顺):
1. [Simakeng](https://github.com/Simakeng)
2. [iriszero](https://github.com/iriszero48)


依旧在底层基础库的开发，目前进度:

- [x] Utilities 类型定义
- [x] Utilities.Common 常用模板库
- [x] Utilities.GUID UUID实现
- [x] Utilities.Window 窗口对象抽象 
- [ ] Utilities.Encrypthion 加密库
- [ ] Utilities.Encoding 字符串编码库
- [ ] Utilities.Graphics 底层图像API抽象

---

- [ ] Core  
- [ ] Core.Entity ECS系统实体
- [ ] Core.Component ECS系统组件


## 文档

本工程使用Doxygen进行文档生成

[这里](http://docs.e15studio.cn/)有一份在线文档, 在线文档更新稍有滞后，请以代码生成的文档为准。

```bash
# 要生成文档，执行如下命令行即可
doxygen Doxyfile
```



<p align="right">
司马坑，于 2020/5/30 16:26
</p>