Problem statement:

we want fast local processing in case of no changes, and we want working processing with GitHub Actions

GitHub Actions are executed with fresh clone and git does not carry timestamps this is creating challenges for us.

See https://git.wiki.kernel.org/index.php/Git_FAQ#Why_isn.27t_Git_preserving_modification_time_on_files.3F

See https://stackoverflow.com/questions/2179722/checking-out-old-file-with-original-create-modified-timestamps