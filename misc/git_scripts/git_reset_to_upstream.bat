git remote add upstream https://github.com/FOME-Tech/fome-fw.git

git fetch upstream
git checkout master
git reset --hard upstream/master
git push origin master -f
