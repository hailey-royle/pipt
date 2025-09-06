
# pipt (pīpt (piped))

Generate a fancy terminal graph from a subset of markdown  

## syntax

### input

```
# box  
  
> what is a box  
> i dont know  
> its for cats or smt  
 
- cat  

# cat  

>   /\_/\
>  ( o.o )
>   > ^ <

```

### output  

```
.........................  
.+-box-----------------+.  
.| what is a box       |.  
.| i dont know         |.  
.| its for cats or smt |.  
.+---------------------+.  
......|..................  
......v..................  
.+---------+.............
.|  /\_/\  |.............
.| ( o.o ) |.............
.|  > ^ <  |.............
.+---------+.............
.........................
```

(arrow not yet implemented)

---

the title is marked with a single #  
the body is marked with >  
connections are marked with -  
connections are the title of another box, and an arrow will be drawn between them (not yet implemented)  
