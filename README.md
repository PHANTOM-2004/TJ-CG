# 说明

为了防止助教哥哥/姐姐无法编译我的程序(因为我的环境是archlinux)，
我通过CMake对linux(`gcc`,`clang`)和windows(`MSVC`)平台进行了适配，要求环境中具有opengl包（这个一般都有），

具体的package毕竟不同的linux发行版不一样，我就直接使用源码构建。

PS: 不要使用古老的编译器编译我的项。如果因此给我扣分，那我也没办法。对于编译器版本我没有做检查，我想现在也不太可能还有人用
`centos7`或者`ubuntu1604`这种老东西来作为日常系统吧。

PS: 及时更新的你的`MSVC`, 如果你发现了编译错误，旧版本的MSVC的预处理器存在`bug`，部分情况下无法正确处理`include`， 在最新版本已经修复。

PS: 如果你用的Mac，那我也没办法，用Mac的话就自己解决吧。

```shell
cmake -S ./ -B ./build -GNinja
cmake --build ./build --target xxx
```

- 第一次作业

```shell
cmake -S ./ -B ./build -GNinja
cmake --build ./build --target neon_cube
./target/neon_cube
```

- 第二次作业


```shell
cmake -S ./ -B ./build -GNinja
cmake --build ./build --target solar_system
./target/solar_system
```
