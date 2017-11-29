sudo apt-get install git vim python-dev python3-dev cmake build-essential curl -y &&\
mkdir -p ~/.vim/autoload ~/.vim/bundle && \
curl -LSso ~/.vim/autoload/pathogen.vim https://tpo.pe/pathogen.vim &&\
git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim && \
git clone https://github.com/tomasr/molokai && mv -f molokai/* ~/.vim/ && rm -rf molokai
