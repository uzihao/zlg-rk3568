## 基本操作

```bash
git init 
git remote add origin https://github.com/用户名/仓库名.git
git pull origin master 
git add . 
git commit -am "提交注释"
git push origin master 
```

## 子仓库

- 添加子仓库，`projectB.git`为子仓库路径：

  ```bash
  git submodule add projectB.git
  ```

  添加完成后，正常commit & push即可

- 切换分支后，子仓库需要手动checkout

- 子仓库某一个分支有更新时，需要手动更新：

  ```bash
  git submodule init
  ```

- 克隆一个带有子仓库的模块时，需要增加以下命令：

  ```bash
  git submodule init
  git submodule update
  ```

## add忽略错误

git add . --ignore-errors