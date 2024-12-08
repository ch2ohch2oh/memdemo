# C++ memory (external) memory fragmentation demo

## Memory leak vs memory fragmentation

Memory leak generally means `new` without `delete`. It is easy to catch with 
various tools. Memory fragmentation means the memory space of the process is fragmented,
thus using more memory than needed. 


## Tools

There are a number of tools to check the memory usage of a process.

### `ps`
    
    ps v [PID]

### `strace`

Monitor all `mmap/munmap/brk` calls

    strace -e trace=memory -p [PID]

You can tell the `brk` pointer is growing

```txt
strace: Process 106455 attached
brk(0x564eaa0f2000)                     = 0x564eaa0f2000
brk(0x564eaa113000)                     = 0x564eaa113000
brk(0x564eaa134000)                     = 0x564eaa134000
brk(0x564eaa155000)                     = 0x564eaa155000
brk(0x564eaa176000)                     = 0x564eaa176000
brk(0x564eaa197000)                     = 0x564eaa197000
brk(0x564eaa1b8000)                     = 0x564eaa1b8000
brk(0x564eaa1d9000)                     = 0x564eaa1d9000
brk(0x564eaa1fa000)                     = 0x564eaa1fa000
brk(0x564eaa21b000)                     = 0x564eaa21b000
```

### `pmap`

Print the address space 
    
    pmap -x [PID]

You can tell where the 56MB of heap is.

```txt
92239:   /home/dazhi/mem/out/build/GCC 11.4.0 x86_64-linux-gnu/memdemo
Address           Kbytes     RSS   Dirty Mode  Mapping
000055c606dec000       8       8       0 r---- memdemo
000055c606dee000      24      24       0 r-x-- memdemo
000055c606df4000      16      16       0 r---- memdemo
000055c606df8000       4       4       4 r---- memdemo
000055c606df9000       4       4       4 rw--- memdemo
000055c62bf3c000   56172   56052   56052 rw---   [ anon ]
00007f49b942a000      16      16      16 rw---   [ anon ]
00007f49b942e000      56      56       0 r---- libm.so.6
00007f49b943c000     496     248       0 r-x-- libm.so.6
00007f49b94b8000     364       0       0 r---- libm.so.6
00007f49b9513000       4       4       4 r---- libm.so.6
00007f49b9514000       4       4       4 rw--- libm.so.6
00007f49b9515000     160     160       0 r---- libc.so.6
00007f49b953d000    1620    1020       0 r-x-- libc.so.6
00007f49b96d2000     352     184       0 r---- libc.so.6
00007f49b972a000       4       0       0 ----- libc.so.6
00007f49b972b000      16      16      16 r---- libc.so.6
00007f49b972f000       8       8       8 rw--- libc.so.6
00007f49b9731000      52      20      20 rw---   [ anon ]
00007f49b973e000      12      12       0 r---- libgcc_s.so.1
00007f49b9741000      92      64       0 r-x-- libgcc_s.so.1
00007f49b9758000      16      16       0 r---- libgcc_s.so.1
00007f49b975c000       4       4       4 r---- libgcc_s.so.1
00007f49b975d000       4       4       4 rw--- libgcc_s.so.1
00007f49b975e000     616     616       0 r---- libstdc++.so.6.0.30
00007f49b97f8000    1092     728       0 r-x-- libstdc++.so.6.0.30
00007f49b9909000     444     112       0 r---- libstdc++.so.6.0.30
00007f49b9978000       4       0       0 ----- libstdc++.so.6.0.30
00007f49b9979000      44      44      44 r---- libstdc++.so.6.0.30
00007f49b9984000      12      12      12 rw--- libstdc++.so.6.0.30
00007f49b9987000      12      12      12 rw---   [ anon ]
00007f49b9993000       8       8       8 rw---   [ anon ]
00007f49b9995000       8       8       0 r---- ld-linux-x86-64.so.2
00007f49b9997000     168     168       0 r-x-- ld-linux-x86-64.so.2
00007f49b99c1000      44      44       0 r---- ld-linux-x86-64.so.2
00007f49b99cd000       8       8       8 r---- ld-linux-x86-64.so.2
00007f49b99cf000       8       8       8 rw--- ld-linux-x86-64.so.2
00007ffec1523000     136      20      20 rw---   [ stack ]
00007ffec15ec000      16       0       0 r----   [ anon ]
00007ffec15f0000       8       4       0 r-x--   [ anon ]
---------------- ------- ------- -------
total kB           62136   59736   56248
```

### `valgrind`

`--tool=memcheck`

```txt
==107816== 
==107816== HEAP SUMMARY:
==107816==     in use at exit: 0 bytes in 0 blocks
==107816==   total heap usage: 7,500,083 allocs, 7,500,083 frees, 557,714,632 bytes allocated
==107816== 
==107816== All heap blocks were freed -- no leaks are possible
==107816== 
==107816== For lists of detected and suppressed errors, rerun with: -s
==107816== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
dazhi@DESKTOP-B4KMF2O:~/mem/out/build/GCC 11.4.0 x86_64-linux-gnu$ 
```

`--tool=hhat`

```txt
==109659== 
==109659== Total:     557,714,632 bytes in 7,500,083 blocks
==109659== At t-gmax: 51,058,616 bytes in 600,009 blocks
==109659== At t-end:  0 bytes in 0 blocks
==109659== Reads:     907,511,780 bytes
==109659== Writes:    808,700,736 bytes
==109659== 
==109659== To view the resulting profile, open
==109659==   file:///usr/libexec/valgrind/dh_view.html
==109659== in a web browser, click on "Load...", and then select the file
==109659==   /home/dazhi/mem/out/build/GCC 11.4.0 x86_64-linux-gnu/dhat.out.109659
==109659== The text at the bottom explains the abbreviations used in the output.
```

`valgrind --tool=massif` will output a `massif.out.*` file, which could be viewed using
`ms_print [filename]` or `massif-visualizer [file]`. 
Either way the reported memory usage is *not* the `rss` reported in `ps`. 

```txt
--------------------------------------------------------------------------------
Command:            ./memdemo
Massif arguments:   (none)
ms_print arguments: massif.out.117426
--------------------------------------------------------------------------------


    MB
56.16^             #                                                          
     |             #             :              @                           ::
     |            @#             :             :@            :              ::
     |            @#             :             :@            ::            @::
     |            @#           :::            ::@            ::            @::
     |          @@@#           : :            ::@          ::::           :@::
     |          @ @#          :: :          ::::@          : ::          ::@::
     |        @@@ @#         ::: :          : ::@         :: ::         :::@::
     |        @ @ @#         ::: :         @: ::@        @:: ::         :::@::
     |       :@ @ @#        :::: :         @: ::@        @:: ::        ::::@::
     |       :@ @ @#        :::: :       ::@: ::@       :@:: ::       :::::@::
     |      @:@ @ @#      :::::: :       : @: ::@      ::@:: ::      ::::::@::
     |      @:@ @ @#      : :::: ::      : @: ::@      ::@:: ::      ::::::@::
     |    @@@:@ @ @#     :: :::: ::     :: @: ::@     :::@:: ::     @::::::@::
     |    @ @:@ @ @#     :: :::: ::   :::: @: ::@:    :::@:: ::     @::::::@::
     |   @@ @:@ @ @#    ::: :::: ::   : :: @: ::@:   ::::@:: ::    :@::::::@::
     |   @@ @:@ @ @#:   ::: :::: ::  :: :: @: ::@:   ::::@:: ::    :@::::::@::
     |   @@ @:@ @ @#: @@::: :::: ::  :: :: @: ::@: ::::::@:: ::   ::@::::::@::
     | ::@@ @:@ @ @#: @ ::: :::: :: @:: :: @: ::@::: ::::@:: ::  :::@::::::@::
     |@: @@ @:@ @ @#::@ ::: :::: :: @:: :: @: ::@::: ::::@:: :: ::::@::::::@::
   0 +----------------------------------------------------------------------->Gi
     0                                                                   11.94

Number of snapshots: 68
 Detailed snapshots: [1, 3, 4, 5, 7, 8, 9, 10 (peak), 13, 24, 29, 33, 42, 54, 64]
```

## Learning material

- [This wiki][1] covers the malloc internal, which will help you understand why 
`free` really does not free the memory.
- [What’s Eating my RAM? - C++ Memory Management - Jianfei Pan - CppCon 2024][2] covers 
memory leak vs memory fragmentation and how using allocators could help to mitigate
memory fragmentation.
- [What Programmers Should Know About Memory Allocation - S. Al Bahra, H. Sowa, P. Khuong - CppCon 2019][3]

[1]: https://sourceware.org/glibc/wiki/MallocInternals
[2]: https://www.youtube.com/watch?v=y6AN0ks2q0A
[3]: https://www.youtube.com/watch?v=gYfd25Bdmws
