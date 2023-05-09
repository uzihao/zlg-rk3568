

### 注册Qml

|                            |                   |
| -------------------------- | ----------------- |
| qmlRegisterUncreatableType | 不可在Qml中实例化 |
|                            |                   |
|                            |                   |

## Qt.pro DEFILES

"DEFINES"是QT的.pro文件中的一个关键字，用于定义预处理器定义。预处理器定义可以在源代码中使用#ifdef或#ifndef等指令，实现条件编译。例如，如果在.pro文件中定义了DEFINES += ENABLE_DEBUG，则可以在C++源代码中使用以下方式来开启调试模式：

```cpp
#ifdef ENABLE_DEBUG
    // Debug code here
#else
    // Release code here
#endif
```

此外，DEFINES还可以用来定义宏，以便在源代码中使用。你可以在.pro文件中添加类似以下的语句：

```bash
DEFINES += VERSION=\\\"1.0\\\"
```

这将在源代码中定义一个名为VERSION的宏，并使用值"1.0"进行初始化。在源代码中可以使用以下方式调用：

```bash
qDebug() << "Version: " << VERSION;
```



    QQuickView viewer; 
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);// 自动调整大小

```c++
QQmlEngine *engine = viewer.engine();
engine->rootContext()->setContextProperty(QStringLiteral("debugMessageModel"), AppMessages::getModel()); // 后面这个debugMessageModel就绑定到getModel的返回值
```



