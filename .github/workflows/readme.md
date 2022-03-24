Here is a diagram of which configure scripts are used for which workflows.
This is not a complete diagram of all workflows, nor does it show everything that these jobs do.

```
                                         Jenkins
                                           +
                                           v
                                         run.bat
      +------------+-----------------------+
      |            |                       v
      |            |                compile_and_upload.bat
      |            |     +--------------+  +------------------------------+
      |            |     |              v  v                              v
      |            |     |              gen_config                      compile.sh
      |            |     |        +--------+-------------------+            +
      v            v     |        v        v                   v            v
     Sim        Console  | ConfigKinetis   ConfigDefault    ConfigBoard   Firmware    Package
      ^            ^     |       ^             ^ ^            ^    ^      ^  ^           ^
      |            |     |       |             | |            |    |      |  |           |
      |            |     |       |             | |            |    |      |  |           |
      |            |     |       |             | |            |    +------+  |           |
      |            |     |       |             | |            |    |         |           |
      +---+--------+-------------+-------------+--------------+--------------+-----------+
          |              |                       |                 |
          +              +                       +                 +
Firmware Push            Gen Config      Unit Tests       Firmware PR
```

https://github.com/rusefi/rusefi/wiki/Build-Server-and-Automation