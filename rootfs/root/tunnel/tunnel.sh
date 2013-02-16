stty -F /dev/ttyS1 115200 raw -echo

echo "1" > /proc/sys/net/ipv4/ip_forward
echo "1" > /proc/sys/net/ipv4/ip_dynaddr

tcc tunnel.c -o tunnel
./tunnel &

#ifconfig tun0 up



