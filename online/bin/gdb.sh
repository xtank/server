
pid=`ps -ef|grep online-88|grep -v grep|awk '{print $2}'`
echo $pid
gdb -p $pid





