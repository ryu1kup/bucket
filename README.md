# Bucket Monte-Carlo

## Compile

```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . --target install
```

## Usage

```
$ mkdir work
$ cd work
$ ../bin/bucket [options]
```
where options are

- `-p` ... path to the preinit macro
- `-v` ... path to the visualization macro
- `-r` ... path to the run macro
- `-o` ... filename of the output csv

Refer to the following command:

```
$ ../bin/bucket -p ../macros/preinit.mac -v ../macros/visualization.mac -r ../macros/run.mac -o output.csv
```

## Output

A csv file is created.

| header | unit | description |
| ------ | ---- | ----------- |
| nref   |      | # of reflection |
| time   | ns   | photon detection time |
