copyIfDifferent() {
  custom_file=$1
  current_file=$2
# maybe change to `rsync --checksum` a good general way to "copy if changed"?
cmp --silent $custom_file $current_file
is_same_content=$?
echo "is_same_content=$is_same_content"
if [ "$is_same_content" == 1 ]; then
  echo "[$custom_file] files are different, copying!"
  cp $custom_file $current_file
else
  echo "files same, not touching to preserve incremental build"
fi
}
