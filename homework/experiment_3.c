#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define OK 1
#define ERROR 0
#define status int

typedef struct book {
	char num[20];
	char name[30];
	float price;
	struct book* next;
} book;

typedef struct {
	book* head;
	book* tail;
	int len;
} Sqlist;

status init(Sqlist* List) { 			//初始化
	List->head = (book*)malloc(sizeof(book));
	List->head->next = NULL;
	List->tail = List->head;
	List->len = 0;
	return OK;
}

status creatlist(Sqlist* List) { 			//创建
	book* q;
	printf("请逐行输入图书信息（书号 书名 价格），输入 0 0 0 结束：\n");
	while (1) {
		q = (book*)malloc(sizeof(book));
		scanf("%s%s%f", q->num, q->name, &q->price);
		if (q->price == 0) {
			printf("创建完成，共录入 %d 本图书。\n", List->len);
			break;
		}
		List->tail->next = q;
		List->tail = q;
		List->len++;
	}
	List->tail->next = NULL;
	return OK;
}


status printlist(Sqlist* List) {				//输出
	book* q;
	if (List->len == 0) {
		printf("库中没有图书。\n");
		return ERROR;
	}
	q = List->head->next;
	printf("库中现有 %d 本图书信息如下：\n", List->len);
	printf("书号\t\t书名\t\t价格\n");
	while (q) {
		printf("%s\t%s\t\t%.2f\n", q->num, q->name, q->price);
		q = q->next;
	}
	return OK;
}



//定义这个Sqlist的时候，应当是通过malloc返回一个指针，因此传参和返回类型均为指针类型
//任务3，完成价格更改
Sqlist* changpric(Sqlist* List) {
	//遍历求解这个均值
	book* q = List->head;
	float sum = 0;
	while (q->next!=NULL) {
		sum = sum + (q->next)->price;
		q = q->next;
	}
	float equalprice = sum / (List->len);
	printf("当前图书平均价格为：%.2f\n", equalprice);

	//价格的重新修改
	q = List->head;
	while (q->next != NULL) {
		if ((q->next->price) >= equalprice) {
			q->next->price = (q->next->price) * 1.1;
		} else {
			q->next->price = (q->next->price) * 1.2;
		}
		q = q->next;
	}
	return List;

}
//任务四：通过头插法完成这个逆序存储
Sqlist* nixu(Sqlist* List) {
    book* p = List->head->next;   // 原第一个数据节点
    book* temp = NULL;
    List->head->next = NULL;      // 头结点后暂时为空

    while (p != NULL) {
        temp = p->next;           // 保存下一个节点
        // 头插 p
        p->next = List->head->next;
        List->head->next = p;
        p = temp;                 // 继续处理下一个原节点
    }

    // 更新尾指针
    List->tail = List->head;
    while (List->tail->next != NULL) {
        List->tail = List->tail->next;
    }
    return List;
}



//任务二：按价格降序排序（冒泡排序，交换节点数据域）
Sqlist* sortlist(Sqlist* List) {
	if (List->len <= 1) return List;
	int i, j;
	book* p;
	//冒泡排序，每轮将最小值沉到末尾
	for (i = 0; i < List->len - 1; i++) {
		p = List->head->next;
		for (j = 0; j < List->len - 1 - i; j++) {
			//价格小的往后换，实现降序
			if (p->price < p->next->price) {
				char tmpnum[20], tmpname[30];
				float tmpprice;
				strcpy(tmpnum, p->num);
				strcpy(tmpname, p->name);
				tmpprice = p->price;
				strcpy(p->num, p->next->num);
				strcpy(p->name, p->next->name);
				p->price = p->next->price;
				strcpy(p->next->num, tmpnum);
				strcpy(p->next->name, tmpname);
				p->next->price = tmpprice;
			}
			p = p->next;
		}
	}
	return List;
}

//任务五：查找价格最高的图书（可能有多本同价）
status findmax(Sqlist* List) {
	printf("正在查找最贵图书...\n");
	if (List->len == 0) {
		printf("库中没有图书。\n");
		return ERROR;
	}
	//第一轮遍历找最高价格
	book* q = List->head->next;
	float maxprice = q->price;
	while (q) {
		if (q->price > maxprice) {
			maxprice = q->price;
		}
		q = q->next;
	}
	//第二轮遍历统计数量
	int count = 0;
	q = List->head->next;
	while (q) {
		if (q->price == maxprice) count++;
		q = q->next;
	}
	printf("找到 %d 本最贵图书：\n", count);
	//第三轮遍历输出最贵图书
	q = List->head->next;
	while (q) {
		if (q->price == maxprice) {
			printf("%s %s %.2f\n", q->num, q->name, q->price);
		}
		q = q->next;
	}
	return OK;
}

//任务六：按书名查找最爱图书
status findfav(Sqlist* List) {
	int m, i;
	printf("请输入要查找的次数：");
	scanf("%d", &m);
	for (i = 0; i < m; i++) {
		char target[30];
		printf("请输入书名：");
		scanf("%s", target);
		//遍历统计同名图书数量
		int count = 0;
		book* q = List->head->next;
		while (q) {
			if (strcmp(q->name, target) == 0) count++;
			q = q->next;
		}
		if (count == 0) {
			printf("抱歉，没有你的最爱！\n");
		} else {
			printf("找到 %d 本同名图书：\n", count);
			q = List->head->next;
			while (q) {
				if (strcmp(q->name, target) == 0) {
					printf("%s %s %.2f\n", q->num, q->name, q->price);
				}
				q = q->next;
			}
		}
	}
	return OK;
}

//任务七：按位置查找图书
status findpos(Sqlist* List) {
	int m, i;
	printf("请输入要查找的次数：");
	scanf("%d", &m);
	for (i = 0; i < m; i++) {
		int pos;
		printf("请输入位置序号：");
		scanf("%d", &pos);
		if (pos < 1 || pos > List->len) {
			printf("抱歉，最佳位置上的图书不存在！\n");
		} else {
			//从头结点走 pos 步到达目标节点
			book* q = List->head;
			int j;
			for (j = 0; j < pos; j++) {
				q = q->next;
			}
			printf("%s %s %.2f\n", q->num, q->name, q->price);
		}
	}
	return OK;
}

//任务八：在指定位置插入新图书
status insertbook(Sqlist* List) {
	int pos;
	printf("请输入入库位置：");
	scanf("%d", &pos);
	if (pos < 1 || pos > List->len + 1) {
		printf("抱歉，入库位置非法！\n");
		return ERROR;
	}
	book* newbook = (book*)malloc(sizeof(book));
	printf("请输入新图书信息（书号 书名 价格）：");
	scanf("%s%s%f", newbook->num, newbook->name, &newbook->price);

	//找到第 pos-1 个节点（插入位置的前驱）
	book* q = List->head;
	int i;
	for (i = 0; i < pos - 1; i++) {
		q = q->next;
	}
	newbook->next = q->next;
	q->next = newbook;
	List->len++;

	//如果插在末尾，更新尾指针
	if (newbook->next == NULL) {
		List->tail = newbook;
	}

	printf("入库成功！当前图书信息：\n");
	q = List->head->next;
	while (q) {
		printf("%s %s %.2f\n", q->num, q->name, q->price);
		q = q->next;
	}
	return OK;
}

//任务九：删除指定位置的图书
status deletebook(Sqlist* List) {
	int pos;
	printf("请输入出库位置：");
	scanf("%d", &pos);
	if (pos < 1 || pos > List->len) {
		printf("抱歉，出库位置非法！\n");
		return ERROR;
	}
	//找到第 pos-1 个节点（删除位置的前驱）
	book* q = List->head;
	int i;
	for (i = 0; i < pos - 1; i++) {
		q = q->next;
	}
	book* del = q->next;
	q->next = del->next;
	List->len--;

	//如果删除的是尾节点，更新尾指针
	if (del == List->tail) {
		List->tail = q;
	}
	free(del);

	printf("出库成功！当前图书信息：\n");
	q = List->head->next;
	while (q) {
		printf("%s %s %.2f\n", q->num, q->name, q->price);
		q = q->next;
	}
	return OK;
}

//任务十：图书去重（书号相同则删除后出现的）
status dedupbook(Sqlist* List) {
	book* p = List->head->next;
	//对每本书，检查其后是否有书号相同的，有则删除
	while (p) {
		book* prev = p;
		book* cur = p->next;
		while (cur) {
			if (strcmp(p->num, cur->num) == 0) {
				//删除 cur 节点
				prev->next = cur->next;
				if (cur == List->tail) {
					List->tail = prev;
				}
				free(cur);
				cur = prev->next;
				List->len--;
			} else {
				prev = cur;
				cur = cur->next;
			}
		}
		p = p->next;
	}
	printf("去重完成！剩余 %d 本图书：\n", List->len);
	book* q = List->head->next;
	while (q) {
		printf("%s %s %.2f\n", q->num, q->name, q->price);
		q = q->next;
	}
	return OK;
}


int main() {
	int x;
	Sqlist* L;
	L = (Sqlist*)malloc(sizeof(Sqlist));
	init(L);
	do {
		printf("\n**************菜单***************\n");
		printf("*******请选择您想进行的操作*****：\n");
		printf("*********1.创建图书信息***********\n");
		printf("*********2.输出图书信息***********\n");
		printf("*********3.价格调整***************\n");
		printf("*********4.逆序存储***************\n");
		printf("*********5.价格降序排序***********\n");
		printf("*********6.最贵图书查找***********\n");
		printf("*********7.最爱图书查找***********\n");
		printf("*********8.按位置查找*************\n");
		printf("*********9.新图书入库*************\n");
		printf("********10.旧图书出库*************\n");
		printf("********11.图书去重***************\n");
		printf("*********0.退出系统***************\n");
		if (scanf("%d", &x) != 1) {
			while (getchar() != '\n');  //清空输入缓冲区
			x = -1;
		}
		switch (x) {
		case 1:
			creatlist(L);
			break;
		case 2:
			printlist(L);
			break;
		case 3:
			changpric(L);
			printf("价格调整完成！\n");
			break;
		case 4:
			nixu(L);
			printf("逆序存储完成！\n");
			break;
		case 5:
			sortlist(L);
			printf("排序完成！\n");
			break;
		case 6:
			findmax(L);
			break;
		case 7:
			findfav(L);
			break;
		case 8:
			findpos(L);
			break;
		case 9:
			insertbook(L);
			break;
		case 10:
			deletebook(L);
			break;
		case 11:
			dedupbook(L);
			break;
		case 0:
			printf("退出系统\n");
			return 0;
		default:
			printf("输入错误，请重新选择！\n");
			break;
		}
	} while (1);
}