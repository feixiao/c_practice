####编译

  + gcc -Wall -std=c99 server.c -o server 
  
####运行

#####Server
  + ./server localhost 6666 1   // 使用et模式
  + ./server localhost 6666 0   // 使用lt模式
    
#####Client

 + telnet  localhost 6666
