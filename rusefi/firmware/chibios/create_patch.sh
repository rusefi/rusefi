echo Cleaning destination folder...
rm -rf os_untouched
mkdir os_untouched


# first we need to create the directory structure
echo Creating folders...
find boards -type d -exec mkdir -p os_untouched\\'{}' \;
find os     -type d -exec mkdir -p os_untouched\\'{}' \;
find ext    -type d -exec mkdir -p os_untouched\\'{}' \;

echo Copying files...
# now copy all the files we use from the full ChibiOS bundle into our folder where we only keep the needed files
find boards -type f -exec cp ../../../ChibiOS_2.6.3/'{}' os_untouched/'{}' \;
find os     -type f -exec cp ../../../ChibiOS_2.6.3/'{}' os_untouched/'{}' \;
find ext    -type f -exec cp ../../../ChibiOS_2.6.3/'{}' os_untouched/'{}' \;
