Title: Clangd with gcc
Author: am00ma
Created: Wed, 06 May 2026 08:17:46 +0200
State: open

```bash
gcc-12 -E -v -xc++ /dev/null
# /usr/lib/gcc/x86_64-pc-linux-gnu/16.1.1/../../../../include/c++/16.1.1
# /usr/lib/gcc/x86_64-pc-linux-gnu/16.1.1/../../../../include/c++/16.1.1/x86_64-pc-linux-gnu
# /usr/lib/gcc/x86_64-pc-linux-gnu/16.1.1/../../../../include/c++/16.1.1/backward
# /usr/lib/gcc/x86_64-pc-linux-gnu/16.1.1/include
# /usr/local/include
# /usr/include
```

`~/.config/clangd/config.yaml`

```yaml
If:
  PathMatch: .*\.(c|h)$
CompileFlags:
  Compiler: /usr/bin/gcc

---
If:
  PathMatch: .*\.(cpp|hpp)$
CompileFlags:
  Compiler: /usr/bin/g++
  Add:
    - -I/usr/lib/gcc/x86_64-pc-linux-gnu/16.1.1/../../../../include/c++/16.1.1
    - -I/usr/lib/gcc/x86_64-pc-linux-gnu/16.1.1/../../../../include/c++/16.1.1/x86_64-pc-linux-gnu
    - -I/usr/lib/gcc/x86_64-pc-linux-gnu/16.1.1/../../../../include/c++/16.1.1/backward
    - -I/usr/lib/gcc/x86_64-pc-linux-gnu/16.1.1/include
    - -I/usr/local/include
    - -I/usr/include
    - -Wall
    - -Wextra
    - -Wconversion
    - -std=c++26
```
