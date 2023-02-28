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

### qml中调用C++

c++ main 函数中注册方式如下：

```c++
qmlRegisterType<CnlMgr>("AT502.CnlMgr", 1, 0, "CnlMgr");
```

`AT502.CnlMgr`是单引入名，1为大版本，0为小版本，如下

```html
import AT502.CnlMgr 1.0
```

CnlMgr是生成为单例的名字

```html
CnlMgr{
	id: cnlMgr
}
```

调用时类似于qml组件，其中switchChannel为调用类的方法

```html
onClicked: {
	cnlMgr.switchChannel(index, swBtn.isChecked)
}
```

这种方法适合于小组件，生成多个单例



如果声明想要注册为全局单例，即qml直接调用对象，不要qml手动生成

```c++
qmlRegisterSingletonType <CnlMgr>("AT502.CnlMgr", 1, 0, "CnlMgr", &CnlMgr::singleton);
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

此时在qml中不需要手动生成单例，直接调用即可。可见，类名CnlMgr可直接使用

```html
onClicked: {
	CnlMgr.switchChannel(index, swBtn.isChecked)
}
```

