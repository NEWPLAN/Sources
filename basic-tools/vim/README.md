# 一个完整vim的安装与配置
## 0 :安装基本工具和vim:
sudo apt-get install vim python-dev python3-dev cmake build-essential

## 1: 安装pathogen：
https://github.com/tpope/vim-pathogen
#### 安装指令：
mkdir -p ~/.vim/autoload ~/.vim/bundle && \\

curl -LSso ~/.vim/autoload/pathogen.vim https://tpo.pe/pathogen.vim


## 2: 安装vundle插件：
https://github.com/VundleVim/Vundle.vim
#### 安装指令：
git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim

//可以直接跳过吧
## 3: 安装'molokai'配色方案
git clone https://github.com/tomasr/molokai && mv -f molokai/* ~/.vim/ && rm -rf molokai

line  629:

E185: Cannot find color scheme 'molokai'

https://github.com/tomasr/molokai

Press ENTER or type command to continue


## 4: 更新插件：
命令行输入vim,然后输入‘:’,接着输入BundleUpdate进行安装和更新。

## 5: youcompleteMe的问题
youcompleteme插件是一个代码自动补全的工具，比较好使用，使用了一个YCM的服务，需要用户自己安装，在安装youcompleteme时候，可能会安装失败，因此需要进行手动的安装，进入.vim/bundle/youcompleteMe文件夹下面，使用脚本安装即可。
