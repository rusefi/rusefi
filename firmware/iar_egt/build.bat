cd ..
call update_version.bat
cd iar
iarbuild ch.ewp -clean Debug 
iarbuild ch.ewp -build Debug
