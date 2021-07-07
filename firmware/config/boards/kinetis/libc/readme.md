# Kinetis libC

Here is the magic here
1) AndreiKA has downloaded libtinyc from somewhere
2) AndreiKA has compiled libtinyc locally
3) AndreiKA has taken some of the .o object files and pasted those here
4) AndreiKA has added his custom libc_stub.c

libtinyc.a is the gcc library produced from all the above mentioned object files

todo: open question where is the official home of libtinyc

This is about wasting as little RAM as possible. Even complete libtinyc wastes some RAM. 
