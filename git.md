创建GitHub仓库

新建仓库： 登录GitHub，点击右上角的“+”号，选择“New repository”。 输入仓库名称和描述（可选），设置仓库为Public或Private。 点击“Create repository”创建仓库^2^。

本地项目上传到GitHub

初始化本地仓库： 打开Git Bash，进入项目文件夹，执行以下命令初始化Git仓库： git init

配置Git用户信息： 设置用户名和邮箱： git config --global user.name "你的用户名" git config --global user.email "你的邮箱"

添加文件到仓库： 将项目文件添加到Git仓库： git add .

提交文件： 提交文件并添加提交信息： git commit -m "首次提交"

关联远程仓库： 将本地仓库与GitHub上的远程仓库关联： git remote add origin https://github.com/你的用户名/你的仓库名.git
git add .
git commit -m "Initial commit"
初始化
git branch xxx
推送代码到GitHub： 将本地代码推送到远程仓库： git push -u origin xxx

注意事项

避免冲突：如果在创建远程仓库时勾选了“Initialize this repository with a README”，在推送本地代码时可能会遇到冲突。可以先拉取远程仓库内容再推送： git pull --rebase origin master git push origin master

文件大小限制：单个文件不能超过100MB，上传文件过多可能会导致超时
2
。

通过以上步骤，你可以成功将本地项目上传到GitHub，并进行版本控制和项目管理。