
gcc -o sdp-example sdp-example.c `pkg-config --cflags --libs nice`
gcc -o simple-example simple-example.c `pkg-config --cflags --libs nice`
gcc -o threaded-example threaded-example.c `pkg-config --cflags --libs nice`	