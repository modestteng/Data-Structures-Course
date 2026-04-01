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

![image-20260225150232200](../../AppData/Roaming/Typora/typora-user-images/image-20260225150232200.png)

以行序进行存储，就是理解矩阵的特点就行每一行都是一个数组，好吧，你还是按照他说的这个先放完这个第一行，然后第二行

![image-20260225152322711](../../AppData/Roaming/Typora/typora-user-images/image-20260225152322711.png)

真是中间有几个数据，我们要完全按照这个公式来计算，这个公式还真是精彩

![image-20260225152958692](../../AppData/Roaming/Typora/typora-user-images/image-20260225152958692.png)

前面有多少个空格就加几，不要忘记这个初始存储地址

![image-20260225160142819](../../AppData/Roaming/Typora/typora-user-images/image-20260225160142819.png)

它这个维度的写法，三维

# 四.树

![image-20260225161430438](../../AppData/Roaming/Typora/typora-user-images/image-20260225161430438.png)

好一个贡献，通过贡献的角度可以求出这个叶子节点和2个子节点的节点之间的关系

![image-20260225162538849](../../AppData/Roaming/Typora/typora-user-images/image-20260225162538849.png)

这个章节很多操作都是向下取整

这个后面三条性质都是建立在完全二叉树的基础之上的，难怪这么特殊

![image-20260225162843403](../../AppData/Roaming/Typora/typora-user-images/image-20260225162843403.png)

确实即便你没有给这个数据，它依旧要进行这个空间的开辟，毕竟只有这样才能逆推出来一个二叉树，因此出现了很多问题

![image-20260225163935572](../../AppData/Roaming/Typora/typora-user-images/image-20260225163935572.png)

真是精彩的设计，一个左节点，一个右节点
![image-20260225171856886](../../AppData/Roaming/Typora/typora-user-images/image-20260225171856886.png)

原来如此，这个左右指的是这个左子树和右子树

![image-20260225172027036](../../AppData/Roaming/Typora/typora-user-images/image-20260225172027036.png)

所以这个遍历方式是在一定历史条件下的选择，具有三个优先级，而对于无法选择，那就往上返回，然后继续执行策略

原来如此，一格一格的返回，一格一格的检查，这样才有规律性

![image-20260225172537399](../../AppData/Roaming/Typora/typora-user-images/image-20260225172537399.png)

# 我站在这个节点上，我放不访问，先序排序是我站在即我访问，第一次经过即访问

# 我站在这个节点上，我不访问，第二次经过才访问，这是，中序遍历

# 第三次才访问，后序遍历

![image-20260225172854295](../../AppData/Roaming/Typora/typora-user-images/image-20260225172854295.png)

好一个三套规则，真是精彩

![image-20260225173157715](../../AppData/Roaming/Typora/typora-user-images/image-20260225173157715.png)

从后往前数吗，真是精彩的后序排序

![image-20260225173935235](../../AppData/Roaming/Typora/typora-user-images/image-20260225173935235.png)

确实应该从更加本质的角度去理解，访问两次

![image-20260225174220979](../../AppData/Roaming/Typora/typora-user-images/image-20260225174220979.png)

从这个角度来看，它好像是一个二维子树二维子树的释放

![image-20260225175016106](../../AppData/Roaming/Typora/typora-user-images/image-20260225175016106.png)

你能看到他的这个三次走路的循环往复

![image-20260225175854914](../../AppData/Roaming/Typora/typora-user-images/image-20260225175854914.png)

果然理解了本质之后一下子就写出来了

![image-20260225180329039](../../AppData/Roaming/Typora/typora-user-images/image-20260225180329039.png)

这是他们的思想要求，根据这个思想的要求，我们再总结出处理的基本规律



![image-20260225180742982](../../AppData/Roaming/Typora/typora-user-images/image-20260225180742982.png)

从先序遍历中找根，从中序遍历中找左右子树

![image-20260225182143234](../../AppData/Roaming/Typora/typora-user-images/image-20260225182143234.png)

从一条路径到另一条路径其结果是唯一的

![image-20260225183337658](../../AppData/Roaming/Typora/typora-user-images/image-20260225183337658.png)

好一个二叉树形态

![image-20260225183529881](../../AppData/Roaming/Typora/typora-user-images/image-20260225183529881.png)

好一个0和1的排除法，真是精彩

# 五.图

![image-20260225203256090](../../AppData/Roaming/Typora/typora-user-images/image-20260225203256090.png)

前者是起点，也就是这个行，后者是指向的终点，也就是这个列

![image-20260225203518540](../../AppData/Roaming/Typora/typora-user-images/image-20260225203518540.png)

好一个挂着一张链表，这个链表指的就是这个关系的指向与集合

![image-20260225205609707](../../AppData/Roaming/Typora/typora-user-images/image-20260225205609707.png)

每一个顶点都存储着他的这个顶点信息

![image-20260225205945090](../../AppData/Roaming/Typora/typora-user-images/image-20260225205945090.png)

有意思，竟然通过统计这个度的大小来统计这个边的条数



![image-20260225210749086](../../AppData/Roaming/Typora/typora-user-images/image-20260225210749086.png)

他是这个一步回退，然后继续搜索，这个体现的思想就是，尽可能的深入

![image-20260225211951703](../../AppData/Roaming/Typora/typora-user-images/image-20260225211951703.png)

真的是，和以前的东西彻底联系起来了

![image-20260225215038660](../../AppData/Roaming/Typora/typora-user-images/image-20260225215038660.png)

所以，是先发现的先处理，先发现的后处理，这其实很精彩，这个栈类型的函数递归处理，其实对应着的是return释放空间

![image-20260226103350635](../../AppData/Roaming/Typora/typora-user-images/image-20260226103350635.png)

好一个给节点打类型标签

![image-20260226105134378](../../AppData/Roaming/Typora/typora-user-images/image-20260226105134378.png)

如果你将这个联合起来进行理解，确实是合理的

新建立一个（a，b），然后创建sublist节点将其连接起来，这个就是子表的思想

![image-20260226105410766](../../AppData/Roaming/Typora/typora-user-images/image-20260226105410766.png)

子表节点本质就是这个，指向

![image-20260226105539059](../../AppData/Roaming/Typora/typora-user-images/image-20260226105539059.png)

递归思想体现的是真的很浓厚

![image-20260226113657184](../../AppData/Roaming/Typora/typora-user-images/image-20260226113657184.png)

这个创建新节点返回的都是这个指针类型

![image-20260226114640410](../../AppData/Roaming/Typora/typora-user-images/image-20260226114640410.png)

好一个递归，采用这个代码可以一直往下走，然后释放掉这个最下面的代码块

真是精彩的递归代码

![image-20260330224207234](../../AppData/Roaming/Typora/typora-user-images/image-20260330224207234.png)

比这个数小的后面的元素必须是降序的，必须是后进入的在前面，这种降序操作，而且这个操作是充分必要条件

![image-20260330225037941](../../AppData/Roaming/Typora/typora-user-images/image-20260330225037941.png)

原来如此，这是一个读取的过程，就是这个第一个读取的左括号必然是最后被匹配

![image-20260401144348178](../../AppData/Roaming/Typora/typora-user-images/image-20260401144348178.png)

如果你要对这个数据进行这个操作，那么你传入这个一级指针就可以了，如果你要对这个指针进行操作，你必须要传入二级指针，这是函数传参的特点所决定的，函数传参是拷贝这个副本

比如你要修改这个指针指向的这个内容的值，你就需要通过这个传入这个指针，然后解引用进行这个修改，那么如果你要修改这个一级指针的值，那你就需要传入这个二级指针，通过这个解引用进行这个修改

![image-20260401145107640](../../AppData/Roaming/Typora/typora-user-images/image-20260401145107640.png)

原来如此，这个二级指针，根本不需要去定义这个二级指针，我们需要修改这个指针的内容，传入这个一级指针的地址即可，即&一级指针

传入的更高的这个维度，我们只需要修改这个低一级的维度即可





