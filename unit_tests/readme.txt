In this folder we have a naive test set. I am not sure what is the best approach to tests in C (someone can educate me), but my approach is:

1) with a symlink linking some (only some, not all) folders of the firmware implementation, we can compile some files of the firmware as win32 code.
Please execute the 'makelinks.bat' file to get the links. On XP you might need to execute 'junction.exe' before exuting the .bat file.

2) we then compile & run the .exe, which is expected to say SUCCESS and not fail :)