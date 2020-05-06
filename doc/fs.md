# File System

Os 2020 handles the FAT 32 file system, here is how to retrieve / modify files from the os.

## Create / Modify files of Os 2020

To modify the file system, just modify the file fs/fs and build the os.
This file is created at the building of this os and you can also sync Os 2020's files (see next section for details).

Here are steps to modify files from your os (after a first build) :

1. Go to fs and create the modification directory :

```sh
cd fs && mkdir vfs
```

2. Mount the file system image :

```sh
sudo mount fs vfs
```

3. Modify / create some files with root privileges :

```sh
sudo sh -c 'cd vfs && mkdir dir && echo "File content" > dir/file'
```

4. You can now unmount the file system :

```sh
cd .. && sudo umount vfs
```

5. Rebuild the os and run it :

```sh
cd .. && make run
```

## Retrieve files from Os 2020

There is a script **scripts/getfs.sh** that can be runned from the root of this repo which copies the file system from bin/os to fs/fs.

## Notes

The file system is 64MiB wide but this size is the minimal size, you can change the size in the makefile or just create another image of different size in fs/fs.
Be carefull, set clusters of size 512.

## Specs of the driver

The Fat32 driver handles only sectors of size 512 and clusters of 1 sector.
