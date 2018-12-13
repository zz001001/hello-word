# hello-word
git 上传文档的步骤

第一步
cd到你的本地项目根目录下，执行git命令，此命令会在当前目录下创建一个.git文件夹。
git init

第二步：将项目的所有文件添加到仓库中
git add .
这个命令会把当前路径下的所有文件，添加到待上传的文件列表中。
如果想添加某个特定的文件，只需把.换成特定的文件名即可

第三步：将add的文件commit到仓库
git commit -m "注释语句"

第四步：将本地的仓库关联到github上
git remote add origin https://自己的仓库的地址

第五步，上传代码到github远程仓库
git push -u origin master

第六步 如果报错
也就是：error: failed to push some refs to 可能是本地仓库中README.md文件缺失了，解决的办法是： 
执行 git pull –rebase origin master  //先合并
再执行 git push -u origin master
