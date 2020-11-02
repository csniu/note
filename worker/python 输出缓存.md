python 输出缓存
python输出缓冲区要满 4k 才写入文件，除非禁用缓存或者强制输出或者程序结束。中途 ctrl+c 中断会丢失一些输出。
python程序stdout会先输出到缓冲区，等缓冲区满或者脚本结束再输出，而stderr不会先输出到缓冲区。print 会调用 sys.stdout 的 write 方法。　　
解决的办法：　　
1、运行时加-u参数，如 # python -u test.py　　　
2、在不要缓冲的每一次print后面添加sys.stdout.flush()函数　　3、添加环境变量 PYTHONUNBUFFERED=1