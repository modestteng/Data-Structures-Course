## Data structures-Course notes

# 整体感知

| 重点                             | 你要避免的误区     | 正确做法                                                     |
| -------------------------------- | ------------------ | ------------------------------------------------------------ |
| **先学“结构选择”再学“代码细节”** | 一上来死记代码模板 | 每学一个结构都问自己：它适合什么场景？和别的结构相比 trade-off 是什么？ |
| **每章都做“小实现”**             | 只看概念，不写代码 | 每章至少写 1-2 个核心操作（如链表插删、栈队列、树遍历、图遍历、排序） |
| **把“复杂度分析”贯穿全程**       | 只关心能跑出来     | 每次实现后都写一句：时间复杂度、空间复杂度、为什么这样选     |

确实如此，学习这个数据结构，我们需要关注这个复杂度，因为我们已经从语法阶段进步到这个效率，结构阶段，所以这个复杂度是我们必须要注意的

# 绪论

![image-20260223162227909](../../AppData/Roaming/Typora/typora-user-images/image-20260223162227909.png)

有意思，竟然是每一条语句执行的次数总和

![image-20260223162534327](../../AppData/Roaming/Typora/typora-user-images/image-20260223162534327.png)

好一个数量级，阶的翻译Order，那么这个时间复杂度就很容易理解了

![image-20260223162707635](../../AppData/Roaming/Typora/typora-user-images/image-20260223162707635.png)

他这个无论是时间复杂度，还是空间复杂度，都是一个随着n增长而定义的变化的概念、

![image-20260223162918926](../../AppData/Roaming/Typora/typora-user-images/image-20260223162918926.png)

一些定义



![image-20260223164241439](../../AppData/Roaming/Typora/typora-user-images/image-20260223164241439.png)

确实，他这个时间复杂度和空间复杂度的计算，应当建立这个极限的概念去理解

![image-20260223164449011](../../AppData/Roaming/Typora/typora-user-images/image-20260223164449011.png)

随着他的这个n无限增大的时候，他的这个执行的次数就是这个复杂度

![image-20260223164903060](../../AppData/Roaming/Typora/typora-user-images/image-20260223164903060.png)

首先他确实是这个所有段的次数的总和，但是由于这个无穷的特性，以及这个正比的特性，所以我们可以只求解这个最多次执行的语句他的这个时间复杂度，即可得到结果

或者说

不是“只看执行次数最多的那一条语句”，而是看**总成本里增长最快的那一类项（主导项）**。

时间复杂度就是“输入变大时，程序会慢多少”的**增长规律说明书**。

可以把它想成“工作量随 n*n* 变大的曲线”：

- O(1)：东西再多，工作量差不多不变
- O(n)：东西翻倍，工作量大约也翻倍
- O(n^2)：东西翻倍，工作量大约变成 4 倍
- O(log n)：东西翻倍，只多做一点点

所以它不是算“具体几秒”，而是比较“谁增长更快、谁扩展性更好”。
一句话：**时间复杂度看的是趋势，不是当前秒表读数。**

空间复杂度也一样通俗理解：

空间复杂度就是“输入变大时，程序要多占多少内存”的增长规律。

- O(1)：额外内存基本不随 n 变（只用几个变量）
- O(n)：n 变 2 倍，额外内存也大致 2 倍（如开一个长度为 n 的数组）
- O(n^2)：n 变 2 倍，内存约 4 倍（如 n*n 矩阵）

关键点：

- 通常说的是**额外空间**（辅助空间），不一定把输入本身算进去
- 它和时间复杂度一样，关注“增长趋势”，不是精确字节数

一句话：**时间复杂度看“多花多久”，空间复杂度看“多占多大”。**

空间复杂度可以理解为这个占据空间的大小，比如对于这个int，就是占据几个int空间

#### 无论时间复杂度还是空间复杂度，他们描述的都是这个随着这个输入的增加，这个处理所带来的上升程度的描述

# 线性表

![image-20260223170410458](../../AppData/Roaming/Typora/typora-user-images/image-20260223170410458.png)

这个线性表的位序是从这个1开始的，很奇特

![image-20260223171714269](../../AppData/Roaming/Typora/typora-user-images/image-20260223171714269.png)

计算这个，要跳出这个数组的影响，将其完全当做一个新的数据类型，但是有意思的是，你需要通过这个数组来构造这个数据类型

![image-20260223172613258](../../AppData/Roaming/Typora/typora-user-images/image-20260223172613258.png)

头结点本身为空或指向这个空，这就是有没有头节点的区别

![image-20260223173143702](../../AppData/Roaming/Typora/typora-user-images/image-20260223173143702.png)

原来如此，他是不知道这个总长度是多少的，它只能通过设计一个计数器，不断的往后走，然后加一

![image-20260223194021564](../../AppData/Roaming/Typora/typora-user-images/image-20260223194021564.png)

先找到这个节点，然后再将这个data=e的节点给插入到这个位置

![image-20260223194232248](../../AppData/Roaming/Typora/typora-user-images/image-20260223194232248.png)

始终在判断这个节点是否存在，即便找到了节点，在之后依然要判断下一个节点是否存在

![image-20260223195036218](../../AppData/Roaming/Typora/typora-user-images/image-20260223195036218.png)

时间复杂度，默认按照这个最坏时间复杂度来计算

![image-20260223203658124](../../AppData/Roaming/Typora/typora-user-images/image-20260223203658124.png)

顺序和链式两种存储方式，还有注意顺序存储结构和这个链式存储结构的适用的场景，对于这个前者是随机访问效率高，对于这个后者插入和删除的场景应用最好

![image-20260223205610325](../../AppData/Roaming/Typora/typora-user-images/image-20260223205610325.png)

![image-20260223205620482](../../AppData/Roaming/Typora/typora-user-images/image-20260223205620482.png)





# 二.栈和队列

![image-20260223211911928](../../AppData/Roaming/Typora/typora-user-images/image-20260223211911928.png)

这个队列直接模仿日常排队进行记忆即可

![image-20260223212056734](../../AppData/Roaming/Typora/typora-user-images/image-20260223212056734.png)

这个栈顶指向的位置还真是奇特

![image-20260223213619555](../../AppData/Roaming/Typora/typora-user-images/image-20260223213619555.png)

栈低到栈顶，由低地址到这个高地址

![image-20260223214602498](../../AppData/Roaming/Typora/typora-user-images/image-20260223214602498.png)

好一个重新规定原来的指针，因为这个地址发生了这个更新

![image-20260223214954681](../../AppData/Roaming/Typora/typora-user-images/image-20260223214954681.png)

这个栈本质是一个结构体，这个空间大小和这个栈在结构体中设计的大小是不同的，或者说不是直接关联的，而是手动创建，需要你自己进行这个更新的

![image-20260223215227226](../../AppData/Roaming/Typora/typora-user-images/image-20260223215227226.png)

想起来了，这不是解引用吗，真是忘光了，哈哈哈

![image-20260223215432479](../../AppData/Roaming/Typora/typora-user-images/image-20260223215432479.png)

真是精彩，方便入栈和出栈

![image-20260223215726800](../../AppData/Roaming/Typora/typora-user-images/image-20260223215726800.png)

好一个数据栈顶的下一个位置，始终比这个数据空间高一个位置，因此，我们要先减一，然后再弹出

![image-20260223221905780](../../AppData/Roaming/Typora/typora-user-images/image-20260223221905780.png)

front and rear，先这个指向，后移动

![image-20260223222041465](../../AppData/Roaming/Typora/typora-user-images/image-20260223222041465.png)

真是精彩

![image-20260224210728097](../../AppData/Roaming/Typora/typora-user-images/image-20260224210728097.png)

果然是一种后进先出的数据结构

![image-20260224210909293](../../AppData/Roaming/Typora/typora-user-images/image-20260224210909293.png)

尾部插入，头部删除，确实是一种后进先出的数据结构

队列就是排队，牢牢记住这一点即可

![image-20260224211755019](../../AppData/Roaming/Typora/typora-user-images/image-20260224211755019.png)

rear指针始终指向这个实际数据的后一个位置，完全包裹起来了

![image-20260224212003085](../../AppData/Roaming/Typora/typora-user-images/image-20260224212003085.png)

普通队列的问题在于它的这个不能复用的特点，因此我们引入这个循环指针，反复利用这个空间，更加节省和高效

![image-20260224212435902](../../AppData/Roaming/Typora/typora-user-images/image-20260224212435902.png)

有意思，这个地址可能为负值，他还给其加上一个模

![image-20260224213855701](../../AppData/Roaming/Typora/typora-user-images/image-20260224213855701.png)

这个取余操作是实现这个循环，重头再来的关键，真是精彩

下标和这个数组长度一样的时候是溢出，取余回到第一个位置

所以这个队满之后不能够再插入元素了，只能删除，然后再判断，插入

![image-20260224215347147](../../AppData/Roaming/Typora/typora-user-images/image-20260224215347147.png)

这个是有他自己的特点，就是如果出栈为n，那么其余比他小的必须在下面，否则矛盾

![image-20260224220409123](../../AppData/Roaming/Typora/typora-user-images/image-20260224220409123.png)

他的这个每一个节点依旧是这个结构体只是少了这个指针域

但是很有意思的是

![image-20260224220937312](../../AppData/Roaming/Typora/typora-user-images/image-20260224220937312.png)

q指向是用来访问这个结构体成员的，好久没见都忘记了

![image-20260225000322057](../../AppData/Roaming/Typora/typora-user-images/image-20260225000322057.png)

这个操作其实很有意思，或者说规范化，指针移动必须取模，为了可以达到这个循环的目的

![image-20260225101842592](../../AppData/Roaming/Typora/typora-user-images/image-20260225101842592.png)

为什么要加n取余，真是精彩至极，加这个n是为了这个rear可能绕回来，取余是为了防止这个值错误过大，或者说这个取余就规定了这个范围在0-max之间、

![image-20260225103514785](../../AppData/Roaming/Typora/typora-user-images/image-20260225103514785.png)

空一格或使用这个链式队列可以实现这个解决假溢出，或者说链表天然比较灵活

# 三.串，数组，广义表![image-20260225110240898](../../AppData/Roaming/Typora/typora-user-images/image-20260225110240898.png)

这个快链存储不就是这个data为这个字符串，还有这个数据结构里面的字符串都是需要我们自己去定义的，而不是传统意义上的字符串

他这里面传统的字符串都是由数组进行存储的，但是的是现在我们数据结构的字符串，将这个传统字符串进行封装到这个结构体里面，并加上一些其他的东西，构成了这个串

![image-20260225111512956](../../AppData/Roaming/Typora/typora-user-images/image-20260225111512956.png)

这样就能更加精确的理解这个数组的存储了，这样将这个最底部设置为这个每一个格子的地址其实非常精彩，主要是这个从最底部开始才能逐渐存储这个数据

![image-20260225113903932](../../AppData/Roaming/Typora/typora-user-images/image-20260225113903932.png)

还能这样进行递归，真是精彩

![image-20260225114627826](../../AppData/Roaming/Typora/typora-user-images/image-20260225114627826.png)

完全可以这样取理解，不是没有的意思，而是这个空表的意识

