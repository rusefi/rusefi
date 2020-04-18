


git fetch                                >>    log.txt 2>>   err.txt
git reset --hard origin/master           >>    log.txt 2>>   err.txt
cd firmware                   
call gen_config.bat                      >> ../log.txt 2>>../err.txt
call gen_live_documentation.bat          >> ../log.txt 2>>../err.txt
git commit -am "Fresh generated - auto"   >> ../log.txt 2>>../err.txt
git push
