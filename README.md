# ICPC / CCPC 算法模板

面向 GNU C++17 的算法模板、中文手册与验证程序，涵盖基础技巧、数据结构、图论与树、字符串、数学、动态规划及计算几何。

- [使用说明](ICPC_CCPC算法模板/README.md)
- [完整算法模板手册](ICPC_CCPC算法模板/算法模板手册.md)
- [分章节阅读](ICPC_CCPC算法模板/章节/)
- [算法源码](ICPC_CCPC算法模板/代码/)
- [测试程序](ICPC_CCPC算法模板/测试/)
- [验证报告](ICPC_CCPC算法模板/验证报告.md)

## 维护与验证

`handbook_data.py` 保存条目内容，`appendix.md` 保存附录。在项目根目录运行以下命令重新生成手册和算法文件：

```sh
python build_handbook.py
```

安装 Python 和 GNU C++17 编译器后，可验证独立程序：

```sh
python verify_headers.py
```

详细用法、下标约定及测试命令见上述使用说明。
