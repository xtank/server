ps aux | grep dbser | grep -v grep | awk '{print $2}' | xargs -I^ kill -9  ^
sleep 2

case "$1" in
    "c" | "C" | "clean" | "Clean" ) rm -rf ../log/* ;;
    * ) ;;
esac

cd ../bin 
./dbser ../conf/bench.lua ./libdb.so
