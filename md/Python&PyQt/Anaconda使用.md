PyCharm新建python项目，采用Conda工具搭建，项目命名为pyqml-demo1，创建完成后Canda自动生成对应环境配置

但是默认没有PySide6环境，需要安装



打开Anaconda Prompt，安装pyside6

```bash
pip install pyside6 -i https://mirrors.aliyun.com/pypi/simple
```

安装之后，在Anaconda的root environments可以看到pyside6，但是在独立的环境中搜不到

因为还要在Anaconda Prompt进行下面的步骤：

```bash
conda activate pyqml-demo1 #其中pyqml-demo1是我的独立环境的名字
```

```bash
pip install pyside6 -i https://mirrors.aliyun.com/pypi/simple
```



在PyCharm中可以导入

```python
# This Python file uses the following encoding: utf-8
import sys
from pathlib import Path

from PySide6.QtGui import QGuiApplication
from PySide6.QtQml import QQmlApplicationEngine


if __name__ == "__main__":
    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()
    qml_file = Path(__file__).resolve().parent / "main.qml"
    engine.load(qml_file)
    if not engine.rootObjects():
        sys.exit(-1)
    sys.exit(app.exec())
```

可以直接跑

```bash
import QtQuick 2.15
import QtQuick.Window 2.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Rectangle {
        id: x1
        anchors.fill: parent
        color: "yellow"
    }
}
```

但是qml没有代码补全

所以新建.pyproject文件，在QtCreater打开

```bash
{
    "files": ["main.py", "main.qml"]
}
```

同时QtCreater-编辑-首选项-Python-Interpreters-添加，选择刚刚创建项目已经搭建好的Canda构建程序python.exe，并设置为默认

```bash
..Anaconda3\envs\pyqml-demo1\python.exe
```



现在可以在QtCreater编辑qml，PyCharm编辑py，两个IDE都可以直接运行脚本，也不会污染windows的环境