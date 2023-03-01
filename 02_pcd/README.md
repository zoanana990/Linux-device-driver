# pseudo character device
This is an pseudo character device to simulate a memory buffer.
You can do the read, write, lseek, open and close operation

Compile the module by 
```shell
make
```

and insert the module by
```shell
sudo insmod main.ko
```

test the module by using administor rights
```
sudo su
echo "Hello world" > /dev/pcd
```

check the behavior by `dmesg`

Then use `cat` command to print it out
```
cat /dev/pcd
```

