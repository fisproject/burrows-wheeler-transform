burrows-wheeler-transform
====

## Overview
・burrows-wheeler-transform in c

・move-to-front in c

## Description

## Usage
burrows-wheeler-transform

```
$ ./bwt -h

Options:
  -t :  Burrows-Wheeler Transform. Input File.
  -i :  Inverse Burrows-Wheeler Transform. Input File.
  -s :  Burrows-Wheeler Transform. Input Stream.
  -r :  Inverse Burrows-Wheeler Transform. Input Stream.
  -v :  Show Version.
  -h :  Show Help.
```

move-to-front
```
$ ./mtf  -h

Options:
  -t :  Move_to_front. Input File.
  -i :  Inverse Move_to_front. Input File.
  -s :  Move_to_front. Input Stream.
  -r :  Inverse Move_to_front. Input Stream.
  -v :  Show Version.
  -h :  Show Help.
```

## Example
```
$ ./bwt -t ../examples/linuxkernel.txt
```

```
$ ./bwt -i ../examples/linuxkernel_sa.txt
```

```
$ ./mtf -t ../examples/mtf.txt
1 17 15 0 0 5
```

```
$ ./mtf -s broood
1 17 15 0 0 5
```

## Author
[fisproject](https://github.com/fisproject)
