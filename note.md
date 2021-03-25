a++  和 ++a  返回和实现
疑惑的是 a++  ： iterator operator ++(int)   ??
为什么 要int 

friend 声明时加上 declaration 

避免循环包含头文件 
```
#ifndef HEAD
#define HEAD

//code

#endif

```

iterator 应该维护什么  
hashtable ，iterator对于使用者 是黑盒，对于使用者 可以把p当成对容器对象的指针(虽然并非这样)
所以
- *p 得到容器对象，所以返回引用
- -> 得到指向容器对象的指针 
然而实现中 需要维护 node* cur,hashmap* hp. 实现* -> ++ 等函数来表现成对容器对象的指针

iterator 本质上是类，而不是指针什么的。
可以用iterator类 来遍历/操作 stl容器。
思路/表现上是有点像指针，实现上大不同。

