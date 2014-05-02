--得到所在地类型

--程序名称
local program_name=get_program_name() ;
--内网ip
local ip=get_ip(1) ;
--公网ip
local public_ip=get_ip(2) ;
svc_type="db"
project_name="iseer20"
tlog_file_interval_sec = 86400 


local local_type="";
if ( string.match(ip ,"^192.168.8." )  ) then
 	local_type="dx";
elseif ( string.match(ip ,"^192.168.240." )  ) then
 	local_type="wt";
end

print ("------------------------------------------------")
print ( string.format("参数: 程序名： %s  公网ip: %s 内网ip: %s ", program_name, public_ip,ip  )) ;
print ( string.format("类型 : %s",local_type )) ;
print ("------------------------------------------------")

--------- 全局变量
log_dir = "../log"
log_level = 8
log_size = 32000000

pkg_timeout = 5
worker_num = 1
--send_sem_key = 18156
--recv_sem_key = 18158
max_log_files = 0

mysql_host = "localhost"
mysql_user = "root"
mysql_passwd = "1987922"
mysql_port = "3306"
mysql_charset = "utf8"

--#====日志===begin="
--#sql"
IS_LOG_DEBUG_SQL = 1
--#16进制"
LOG_HEX_DATA_FLAG = 1

-- 
--cfg_center_ip ="10.1.1.5"
--cfg_center_port =13330


--处理 bind.conf
-- item 格式 {   "ip", port, "tcp/udp" ,  0 ("idle timeout")  } 
server_bench_bind_map={ }
local port=15002
server_bench_bind_map[ #server_bench_bind_map+1]={ ip , port, "tcp",0 };
server_bench_bind_map[ #server_bench_bind_map+1]={ ip , port, "udp",0 };

