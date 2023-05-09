## 动画

在QML中，实现动画最常用的是State(状态)&Transactions(过渡) 与 Animating Properyty Changes（属性改变动画）这两种方法。

[Qt终结者之QML动画 (bbsmax.com)](https://www.bbsmax.com/A/LPdol2Zwz3/)



## border

有时候只是想设置控件某一边的border，貌似Rectangle不支持，需要自己根据需求画线（用Rectangle）



## 窗口创建完成状态

```css
    Component.onCompleted: {
        console.log("good")
    }
```



## QML和C++交互

### 全局单例

全局单例适合于业务模块，数据库模块、驱动模块等全局模块

c++ main 函数中注册方式如下：

```c++
qmlRegisterSingletonType <CnlMgr>("AT502.CnlMgr", 1, 0, "CnlMgr", &CnlMgr::singleton);
```

`AT502.CnlMgr`是单引入名，1为大版本，0为小版本，如下

```html
import AT502.CnlMgr 1.0
```

singleton方法如下：

```c++
public:
	static QObject *singleton(QQmlEngine *engine, QJSEngine *scriptEngine) // 固定
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        CnlMgr *example = new CnlMgr(); // 在C++中创建对象
        return example;
    }
```

此时在qml中不需要手动生成单例，直接调用即可，所有Qml文件调用的都是同一模块

```c++
import AT502.CnlMgr 1.0
...
// CnlMgr已经注册为一个单例类了，所以无论是调用方法还是接收信号都不需要ID进行引用
    
onClicked: {
	CnlMgr.switchChannel(index, swBtn.isChecked) // 调用方法
}

Connections {
    target: CnlMgr // 在C++中注册的单例类
        function onSwitchChannelChanged(){ // 这里C++侧的信号是switchChannelChanged，s->S且前面加on
        console.log("sw sig back")
    }
}

...
```

### 多个单例情况

多单例适合于对话框等小组件

c++ main 函数中注册方式如下：

```c++
qmlRegisterType<CnlMgr>("AT502.CnlMgr", 1, 0, "CnlMgr");
```

`AT502.CnlMgr`是单引入名，1为大版本，0为小版本，如下

```html
import AT502.CnlMgr 1.0
```

在Qml中必须先手动生成单例，通过id实现调用

```html
import AT502.CnlMgr 1.0
...

CnlMgr{
	id: cnlMgr
	onSwitchChannelChanged: { // 这里C++侧的信号是switchChannelChanged，s->S且前面加on
		console.log("sw sig back")
	}
}
onClicked: {
	cnlMgr.switchChannel(index, swBtn.isChecked)
}

...
```





# 其他

- Qml ListView的各个节点是动态加载的，即滑动时如果节点数量多，会反复销毁创建节点，具体动态加载数据可能是十几个。因此如果节点多，存储的显示数据要放在C++中

- Qml ListView的数据一多，ListView enter的动画就不生效了，因此建议用常规动画，即Trans...

- QQ和QD是Qt中的宏定义，用于简化代码中的参数传递和函数调用。

  QQ宏定义是用于在QObject对象之间传递指针的。它的语法是QQ(T* pointer)，其中T是QObject的子类，pointer是指向该子类对象的指针。使用Q_Q宏定义可以避免在QObject对象之间传递裸指针时发生内存泄漏的风险，同时也可以提高代码的可读性和可维护性。

  QD宏定义是用于在私有函数中访问QObject对象的私有成员的。它的语法是QD(T)，其中T是QObject的子类。使用Q_D宏定义可以避免在私有函数中频繁使用this指针，同时也可以提高代码的可读性和可维护性。

  使用QQ和QD宏定义的优点包括：

  1.避免内存泄漏：使用Q_Q宏定义可以避免在QObject对象之间传递裸指针时发生内存泄漏的风险。

  2.提高代码的可读性和可维护性：使用QQ和QD宏定义可以使代码更易读、易懂，同时也方便后续的代码维护。

  3.提高代码的安全性：使用QQ和QD宏定义可以避免一些常见的错误，例如在私有函数中访问QObject对象的私有成员时使用this指针

