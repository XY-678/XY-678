#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// 定义常量
#define MAX_CUSTOMERS 254//最大办卡客户数量
#define MAX_RECORDS 254//最大记录
#define TOP_ADMIN "admin"
#define TOP_PASSWORD "123456"

// 客户结构体
typedef struct
{
	char name[50];          // 姓名
	char sex[10];           // 性别
	char birth[20];         // 生日
	char phone[20];         // 手机号
	char cardnumber[20];    // 卡号，前缀$为临时
	char card_effective[20];// 卡有效时间，日为单位
	char card_date[20];     // 办卡日期[YY/DD]
	double money;           // 充值金额
} customer;


// 上网记录结构体
typedef struct
{
	char cardnumber[20];    // 卡号
	char machine_location[50];  // 机器位置
	char start_date[20];    // 上网日期 [YY/DD]
	char start_time[20];    // 上网时间 [h] 24小时计数，小时计算，1-24
	char end_date[20];      // 下网日期 [YY/DD]
	char end_time[20];      // 下网时间 [h] 24小时计数，小时计算，1-24
	double total_cost;      // 单用户营业额
} internetRecord;

// 全局变量
customer customers[MAX_CUSTOMERS];
int num_customers = 0;
internetRecord records[MAX_RECORDS];
int num_records = 0;
COORD coord; // 定义光标位置

// 函数声明
void add_customer();
void up_customer();
void down_customer();
void calculate_revenue();
void monthly_consumption();
void save_customers();
void load_customers();
void save_records();
void load_records();
int comparecard(const char *card_number);
void counthour(int index);
int days_in_month(int month);
void recharge_customer();
void set_cursor_position(short x, short y);
void display_menu();
void query_customer();
void modifyCustomerInfo();
void deleteCustomer();
void base_id_date_caculate();
void base_id_month_caculate();
void admin();
void customer1();

// 主程序
int main()
{
	char user[20];
	char word[20];
	system("title 网吧管理系统");
	system("color 79");
	load_customers();
	load_records();
	int choice;
	do
		{
			system("cls");
			set_cursor_position(25, 3);
			printf("请输入使用人[1(网管)/2(用户)/0(退出)]:");
			scanf("%d", &choice);
			switch (choice)
				{
					case 1:
						system("cls");
						set_cursor_position(25, 3);
						printf("用户名：");
						scanf("%s", user);
						set_cursor_position(25, 4);
						printf("密码：");
						scanf("%s", word);
						if (strcmp(user, TOP_ADMIN) == 0 && strcmp(word, TOP_PASSWORD) == 0)
							{
								admin();
							}
						else
							{
								set_cursor_position(25, 5);
								printf("用户名或密码错误！");


								break;
							case 2:
								customer1();
								break;
							case 0:
								set_cursor_position(25, 24);
								printf("感谢使用！");
								break;
							default:
								printf("无效的选择，请重新输入。");
							}
				}
			if (choice != 0)
				{
					printf("\n");
					system("pause"); // 暂停以便用户查看消息
				}

		}
	while (choice != 0);
	save_customers();
	save_records();
	return 0;

}

void customer1()
{
	int choice1;
	do
		{
			system("cls");
			set_cursor_position(25, 3);
			printf("===用户系统===");
			set_cursor_position(25, 4);
			printf("------------------------------------------------------------");
			set_cursor_position(30, 5);
			printf("1.查询指定日期消费\n");
			set_cursor_position(65, 5);
			printf("2.查询指定月份消费\n");
			set_cursor_position(30, 6);
			printf("3.查询卡号信息\n");
			set_cursor_position(65, 6);
			printf("0.退出\n");
			set_cursor_position(25, 7);
			printf("------------------------------------------------------------");
			set_cursor_position(25, 8);
			printf("请输入序号:");
			scanf("%d", &choice1);
			switch (choice1)
				{
					case 1:
						base_id_date_caculate();
						break;
					case 2:
						base_id_month_caculate();
						break;
					case 3:
						query_customer();
						break;
					case 0:
						set_cursor_position(25, 9);
						printf("感谢使用！");
						break;
					default:
						set_cursor_position(25, 9);
						printf("无效的选择，请重新输入。");
				}
			if (choice1 != 0)
				{
					printf("\n");
					system("pause"); // 暂停以便用户查看消息
				}
		}
	while (choice1 != 0);
}










void admin()


{

	int choice;
	do
		{
			display_menu();
			scanf("%d", &choice);

			switch (choice)
				{
					case 1:
						add_customer();  // 客户办卡服务
						break;
					case 2:
						up_customer();   // 客户上网登记
						break;
					case 3:
						down_customer(); // 客户下网结算
						break;
					case 4:
						calculate_revenue();  // 统计网吧营业额
						break;
					case 5:
						monthly_consumption(); // 按月统计办卡客户消费
						break;
					case 6:
						recharge_customer(); // 客户续充功能
						break;
					case 7:
						query_customer();//查询客户信息
						break;
					case 8:
						modifyCustomerInfo();//修改客户信息
						break;
					case 9:
						deleteCustomer();//删除客户
						break;
					case 10:
						base_id_date_caculate();//根据卡号和日期查询消费
						break;
					case 11:
						base_id_month_caculate();//根据卡号和月份查询消费
						break;
					case 0:
						set_cursor_position(25, 13);
						printf("感谢使用！\n");  // 退出
						break;
					default:
						set_cursor_position(25, 13);
						printf("无效的选择，请重新输入。\n");
				}
			if (choice != 0)
				{
					printf("\n");
					system("pause"); // 暂停以便用户查看消息
				}
		}
	while (choice != 0);

	save_customers();
	save_records();

	return ;
}

//展示菜单
void display_menu()
{
	system("cls");
	set_cursor_position(25, 3);
	printf("网吧客户管理系统");
	set_cursor_position(25, 4);
	printf("------------------------------------------------------------");
	set_cursor_position(30, 5);
	printf("1.客户办卡服务");
	set_cursor_position(65, 5);
	printf("2.客户上网登记");
	set_cursor_position(30, 6);
	printf("3.客户下网结算");
	set_cursor_position(65, 6);
	printf("4.统计网吧营业额");
	set_cursor_position(30, 7);
	printf("5.按月统计办卡客户消费");
	set_cursor_position(65, 7);
	printf("6.客户续充功能");
	set_cursor_position(30, 8);
	printf("7.客户信息查询");
	set_cursor_position(65, 8);
	printf("8.客户信息修改");
	set_cursor_position(30, 9);
	printf("9.客户信息删除");
	set_cursor_position(65, 9);
	printf("10.输出指定卡号在某日的营业额");
	set_cursor_position(30, 10);
	printf("11.输出指定卡号在某月的消费额");
	set_cursor_position(65, 10);
	printf("0.退出系统");
	set_cursor_position(25, 11);
	printf("------------------------------------------------------------");
	set_cursor_position(25, 12);
	printf("请输入操作描述码：");
}

// 设置光标位置
void set_cursor_position(short x, short y)
{
	COORD coord;
	HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(ConsoleHandle, coord);
}

// 客户办卡服务
void add_customer()
{
	char card_number[20];
	set_cursor_position(25, 13);
	printf("=== 客户办卡服务 ===\n");
	set_cursor_position(25, 14);
	printf("请输入客户姓名：");
	scanf("%s", customers[num_customers].name);
	set_cursor_position(25, 15);
	printf("请输入客户性别：");
	scanf("%s", customers[num_customers].sex);
	set_cursor_position(25, 16);
	printf("请输入客户出生日期[YY/DD]：");
	scanf("%s", customers[num_customers].birth);
	set_cursor_position(25, 17);
	printf("请输入客户手机号：");
	scanf("%s", customers[num_customers].phone);
	set_cursor_position(25, 18);
	printf("请输入客户卡号[XXXX]：");
	do
		{
			scanf("%s", card_number);
			if (comparecard(card_number))
				{
					set_cursor_position(55, 18);
					printf("该卡号已经存在，请重新输入\n");
				}
			else
				{
					strcpy(customers[num_customers].cardnumber, card_number);
					break;
				}
		}
	while (1);
	set_cursor_position(25, 19);
	printf("请输入卡有效时间：");
	scanf("%s", customers[num_customers].card_effective);
	set_cursor_position(25, 20);
	printf("请输入办卡日期[YY/DD]：");
	scanf("%s", customers[num_customers].card_date);
	set_cursor_position(25, 21);
	printf("请输入充值金额：");
	scanf("%lf", &customers[num_customers].money);

	num_customers++;   // 用户+1
	set_cursor_position(25, 22);
	printf("客户办卡成功！\n");

}

// 比较卡号
int comparecard(const char *card_number)
{
	for (int i = 0; i < num_customers; i++)
		{
			if (strcmp(card_number, customers[i].cardnumber) == 0)
				{
					return 1; // 相同卡号
				}
		}
	return 0; // 无相同卡号
}

// 修改客户信息函数
void modifyCustomerInfo()
{
	char cardNumber[20];
	set_cursor_position(25, 13);
	printf("请输入要修改信息的客户卡号：");
	scanf("%s", cardNumber);

	// 在数组中查找客户信息
	int foundIndex = -1;
	for (int i = 0; i < num_customers; ++i)
		{
			if (strcmp(customers[i].cardnumber, cardNumber) == 0)
				{
					foundIndex = i;
					break;
				}
		}

	if (foundIndex == -1)
		{
			set_cursor_position(25, 14);
			printf("未找到该客户信息。\n");
		}
	else
		{

			// 提示修改信息
			set_cursor_position(25, 14);
			printf("请输入要修改的信息（姓名/性别/生日/手机号/卡有效时间等）：");
			set_cursor_position(25, 15);
			printf("姓名：");
			scanf("%s", customers[foundIndex].name);
			set_cursor_position(25, 16);
			printf("性别：");
			scanf("%s", customers[foundIndex].sex);
			set_cursor_position(25, 17);
			printf("生日：");
			scanf("%s", customers[foundIndex].birth);
			set_cursor_position(25, 18);
			printf("手机号：");
			scanf("%s", customers[foundIndex].phone);
			set_cursor_position(25, 19);
			printf("卡有效时间：");
			scanf("%s", customers[foundIndex].card_effective);

			set_cursor_position(25, 20);
			printf("客户信息已成功修改。\n");
		}
}


// 删除客户信息函数
void deleteCustomer()
{
	char cardNumber[20];
	set_cursor_position(25, 13);
	printf("请输入要删除的客户卡号：");
	scanf("%s", cardNumber);

	// 在数组中查找客户信息
	int foundIndex = -1;
	for (int i = 0; i < num_customers; ++i)
		{
			if (strcmp(customers[i].cardnumber, cardNumber) == 0)
				{
					foundIndex = i;
					break;
				}
		}

	if (foundIndex == -1)
		{
			set_cursor_position(25, 14);
			printf("未找到该客户信息。");
		}
	else
		{
			// 移动数组元素以删除指定客户
			for (int i = foundIndex; i < num_customers - 1; ++i)
				{
					strcpy(customers[i].name, customers[i + 1].name);
					strcpy(customers[i].sex, customers[i + 1].sex);
					strcpy(customers[i].birth, customers[i + 1].birth);
					strcpy(customers[i].phone, customers[i + 1].phone);
					strcpy(customers[i].cardnumber, customers[i + 1].cardnumber);
					strcpy(customers[i].card_effective, customers[i + 1].card_effective);
					strcpy(customers[i].card_date, customers[i + 1].card_date);
					customers[i].money = customers[i + 1].money;
				}
			num_customers--; // 更新客户总数
			set_cursor_position(25, 14);
			printf("客户信息已成功删除。\n");

		}
}

// 客户上网登记
void up_customer()
{
	char card_number[20];
	char machine_location[50];
	char start_date[20];
	char start_time[20];
	set_cursor_position(25, 13);
	printf("=== 客户上网登记 ===\n");
	set_cursor_position(25, 14);
	printf("请输入客户卡号[XXXX](前缀$为临时)：");
	scanf("%s", card_number);

	int is_temporary = (card_number[0] == '$') ? 1 : 0;
	set_cursor_position(25, 15);
	printf("请输入使用机器位置：");
	scanf("%s", machine_location);
	set_cursor_position(25, 16);
	printf("请输入上网日期[YY/DD]：");
	scanf("%s", start_date);
	set_cursor_position(25, 17);
	printf("请输入上网时间[h/min]：");
	scanf("%s", start_time);

	strcpy(records[num_records].cardnumber, card_number);
	strcpy(records[num_records].machine_location, machine_location);
	strcpy(records[num_records].start_date, start_date);
	strcpy(records[num_records].start_time, start_time);
	strcpy(records[num_records].end_date, "N/A");
	strcpy(records[num_records].end_time, "N/A");
	records[num_records].total_cost = 0;

	num_records++;
	set_cursor_position(25, 18);
	printf("登记成功！\n");
}

// 客户下网结算
void down_customer()
{
	char card_number[20];
	char end_date[20];
	char end_time[20];
	int index;
	set_cursor_position(25, 13);
	printf("=== 客户下网结算 ===\n");
	set_cursor_position(25, 14);
	printf("请输入客户卡号[XXXX]：");
	scanf("%s", card_number);

	int found = 0;
	for (int i = 0; i < num_records; i++)
		{
			if (strcmp(card_number, records[i].cardnumber) == 0 &&
			        strcmp(records[i].end_date, "N/A") == 0 &&
			        strcmp(records[i].end_time, "N/A") == 0)
				{
					set_cursor_position(25, 15);
					printf("请输入下网日期[YY/DD]：");
					scanf("%s", end_date);
					set_cursor_position(25, 16);
					printf("请输入下网时间[h/min]：");
					scanf("%s", end_time);

					strcpy(records[i].end_date, end_date);
					strcpy(records[i].end_time, end_time);

					index = i; // 记录当前索引以计算时长

					found = 1;
					break;
				}
		}

	if (!found)
		{
			set_cursor_position(25, 15);
			printf("未找到该客户正在上网的记录。");
			return;
		}

	counthour(index); // 计算时长
}

// 计算时长和费用
void counthour(int i)
{
	int s_month, s_day, s_hour, s_min;
	int e_month, e_day, e_hour, e_min;
	double total_hours;
	double cost_per_hour = 2.0;
	double discount_rate = 1.0; // 默认没有折扣

	// 解析开始时间
	sscanf(records[i].start_date, "%2d%2d", &s_month, &s_day);
	sscanf(records[i].start_time, "%2d:%2d", &s_hour, &s_min);

	// 解析结束时间
	sscanf(records[i].end_date, "%2d%2d", &e_month, &e_day);
	sscanf(records[i].end_time, "%2d:%2d", &e_hour, &e_min);



	// 计算开始和结束日期的总小时数
	double start_total_hours = (s_day - 1) * 24 + s_hour + s_min / 60.0;
	double end_total_hours = (e_day - 1) * 24 + e_hour + e_min / 60.0;

	// 如果结束日期的月份大于开始日期的月份，需要跨月处理
	if (e_month > s_month)
		{
			// 计算开始月份剩余小时数
			int day1 = 0;
			while (s_month > 1)
				{
					s_month--;
					day1 += days_in_month(s_month);

				}
			start_total_hours += day1 * 24;

			// 计算结束月份小时数
			int day2 = 0;
			while (e_month > 1)
				{
					e_month--;
					day2 += days_in_month(e_month);

				}
			end_total_hours += day2 * 24;
		}
// 计算总时长
	total_hours = end_total_hours - start_total_hours;

	if (records[i].cardnumber[0] == '$')
		{
			// 判断是否超过8小时
			if (total_hours > 8)
				{
					discount_rate = 0.9; // 超过8小时打9折
					records[i].total_cost = total_hours * cost_per_hour * discount_rate;
					set_cursor_position(25, 17);
					printf("结算成功！费用为：%.2lf\n", records[i].total_cost);
					return;
				}
		}

	// 判断是否为办卡客户（卡号前缀不为'$'）
	if (records[i].cardnumber[0] != '$')
		{
			discount_rate = 0.8; // 办卡客户享受8折优惠
		}
	// 计算费用
	double total_cost = total_hours * cost_per_hour * discount_rate;
	double recharge_amount;
	// 检查卡内余额是否足够支付费用
	for (int j = 0; j < num_customers; j++)
		{
			if (strcmp(customers[j].cardnumber, records[i].cardnumber) == 0)
				{
					if (customers[j].money >= total_cost)
						{
							customers[j].money -= total_cost; // 扣除余额
							set_cursor_position(25, 17);
							printf("结算成功！费用为：%.2f，卡内余额为：%.2f\n", total_cost, customers[j].money);
						}
					else
						{
							do
								{
									set_cursor_position(25, 17);
									printf("卡内余额不足，结算失败！请进行余额充值\n");


									set_cursor_position(25, 18);
									printf("请输入续充金额：");
									scanf("%lf", &recharge_amount);
									customers[i].money += recharge_amount; // 增加充值金额
									set_cursor_position(25, 19);
									printf("续充成功！当前卡内余额为：%.2f\n", customers[i].money);
									if (customers[i].money >= total_cost)
										{
											set_cursor_position(25, 20);
											customers[i].money -= total_cost;
											printf("扣除网费后还剩：%.2f\n", customers[i].money );
											break;
										}


								}
							while (customers[j].money < total_cost);
						}
					break;
				}
		}

	records[i].total_cost = total_cost; // 更新记录中的总费用


}

// 返回指定月份的天数
int days_in_month(int month)
{
	switch (month)
		{
			case 1:  // January
			case 3:  // March
			case 5:  // May
			case 7:  // July
			case 8:  // August
			case 10: // October
			case 12: // December
				return 31;
			case 4:  // April
			case 6:  // June
			case 9:  // September
			case 11: // November
				return 30;
			case 2:  // February
				return 28;
			default:
				return 0;
		}
}

// 按日期统计网吧营业额及消费前十名客户姓名
void calculate_revenue()
{

	char target_date[20];
	double total_revenue = 0.0;
	set_cursor_position(25, 13);
	printf("=== 统计网吧营业额 ===");
	set_cursor_position(25, 14);
	printf("请输入要统计的日期[YY/DD]：");
	scanf("%s", target_date);

	// 遍历所有记录，筛选指定日期内的记录并计算总营业额
	for (int i = 0; i < num_records; i++)
		{
			if (strcmp(records[i].end_date, "N/A") != 0 && strcmp(records[i].start_date, target_date) == 0)
				{
					total_revenue += records[i].total_cost;
				}
		}
	set_cursor_position(25, 15);
	printf("指定日期 %s 的总营业额为：%.2f\n", target_date, total_revenue);

	// 统计消费前十名客户姓名
	// 创建一个结构体数组用于存储客户姓名和对应的总消费额
	typedef struct
	{
		char name[50];
		double total_cost;
	} CustomerCost;
	//printf("%s", customers[3].name);
	CustomerCost top_customers[MAX_CUSTOMERS];
	int num_top_customers = 0;

// 遍历所有记录，累计每位客户的总消费额
	for (int i = 0; i < num_records; i++)
		{
			if (strcmp(records[i].end_date, "N/A") != 0 && strcmp(records[i].start_date, target_date) == 0
			        && records[i].cardnumber[0] != '$')
				{
					int found = 0;
					// 根据卡号找到对应的客户索引
					int customer_index = -1;
					for (int k = 0; k < num_customers; k++)
						{
							if (strcmp(customers[k].cardnumber, records[i].cardnumber) == 0)
								{
									customer_index = k;
									break;
								}
						}

					// 如果找到了对应的客户
					if (customer_index != -1)
						{
							// 检查是否已经记录该客户的消费额，若已记录则更新消费额，否则新增记录
							for (int j = 0; j < num_top_customers; j++)
								{
									if (strcmp(top_customers[j].name, customers[customer_index].name) == 0)
										{
											top_customers[j].total_cost += records[i].total_cost;
											found = 1;
											break;
										}
								}
							if (!found)
								{
									if (num_top_customers < MAX_CUSTOMERS)
										{
											strcpy(top_customers[num_top_customers].name, customers[customer_index].name);
											top_customers[num_top_customers].total_cost = records[i].total_cost;
											num_top_customers++;
										}
									else
										{
											printf("已达到最大客户数量限制，无法添加更多客户。\n");
											break;
										}
								}
						}
				}
		}
	//printf("%s", top_customers[3].name);

	// 对客户按总消费额排序（降序）
	for (int i = 0; i < num_top_customers - 1; i++)
		{
			for (int j = 0; j < num_top_customers - 1 - i; j++)
				{
					if (top_customers[j].total_cost < top_customers[j + 1].total_cost)
						{
							// 交换顺序
							CustomerCost temp = top_customers[j];
							top_customers[j] = top_customers[j + 1];
							top_customers[j + 1] = temp;
						}
				}
		}



	// 输出消费前十名客户的姓名
	set_cursor_position(25, 16);
	printf("消费前十名客户姓名及其消费额：");
	for (int i = 0; i < 10 && i < num_top_customers; i++)
		{
			set_cursor_position(25, i + 17);

			printf("%d. %s - %.2f", i + 1, top_customers[i].name, top_customers[i].total_cost);
		}

}


// 按月统计办卡客户消费并排序输出
void monthly_consumption()
{
	int target_month;
	set_cursor_position(25, 13);
	printf("=== 按月统计办卡客户消费 ===");
	set_cursor_position(25, 14);
	printf("请输入要统计的月份：");
	scanf("%d", &target_month);
	double total_revenue;
	// 遍历所有记录，筛选指定月记录并计算总营业额
	for (int i = 0; i < num_records; i++)
		{
			int start_month, start_day;
			sscanf(records[i].start_date, "%2d%2d", &start_month, &start_day);

			if (start_month == target_month)
				{
					total_revenue += records[i].total_cost;
				}
		}
	set_cursor_position(25, 15);
	printf("指定%d月的总营业额为：%.2f\n", target_month, total_revenue);


	// 创建一个结构体数组用于存储办卡客户姓名和对应的总消费额
	typedef struct
	{
		char name[50];
		double total_cost;
	} CardCustomerCost;

	CardCustomerCost card_customers[MAX_CUSTOMERS];
	int num_card_customers = 0;

	// 遍历所有记录，累计每位办卡客户在指定月份的总消费额
	for (int i = 0; i < num_records; i++)
		{
			int start_month, start_day;
			sscanf(records[i].start_date, "%2d%2d", &start_month, &start_day);

			if (start_month == target_month && records[i].cardnumber[0] != '$')
				{
					int found = 0;
					// 根据卡号找到对应的客户索引
					int customer_index = -1;
					for (int k = 0; k < num_customers; k++)
						{
							if (strcmp(customers[k].cardnumber, records[i].cardnumber) == 0)
								{
									customer_index = k;
									break;
								}
						}
					// 如果找到了对应的客户
					if (customer_index != -1)
						{
							// 检查是否已经记录该客户的消费额，若已记录则更新消费额，否则新增记录
							for (int j = 0; j < num_card_customers; j++)
								{
									if (strcmp(card_customers[j].name, customers[customer_index].name) == 0)
										{
											card_customers[j].total_cost += records[i].total_cost;
											found = 1;
											break;
										}
								}
							if (!found)
								{
									if (num_card_customers < MAX_CUSTOMERS)
										{
											strcpy(card_customers[num_card_customers].name, customers[customer_index].name);
											card_customers[num_card_customers].total_cost = records[i].total_cost;
											num_card_customers++;
										}
									else
										{
											printf("已达到最大客户数量限制，无法添加更多客户。\n");
											break;
										}
								}
						}


				}
		}

	// 对办卡客户按总消费额排序（降序）
	for (int i = 0; i < num_card_customers - 1; i++)
		{
			for (int j = 0; j < num_card_customers - 1 - i; j++)
				{
					if (card_customers[j].total_cost < card_customers[j + 1].total_cost)
						{
							// 交换顺序
							CardCustomerCost temp = card_customers[j];
							card_customers[j] = card_customers[j + 1];
							card_customers[j + 1] = temp;
						}
				}
		}

	// 输出按消费额排序后的办卡客户姓名和消费额
	set_cursor_position(25, 16);
	printf("按消费额排序后的办卡客户姓名及其消费额：\n");
	for (int i = 0; i < num_card_customers; i++)
		{
			set_cursor_position(25, i + 17);
			printf("%d. %s - %.2f", i + 1, card_customers[i].name, card_customers[i].total_cost);
		}
}



// 客户续充功能
void recharge_customer()
{
	char card_number[20];
	double recharge_amount;
	set_cursor_position(25, 13);
	printf("=== 客户续充功能 ===\n");
	set_cursor_position(25, 14);
	printf("请输入客户卡号[XXXX]：");
	scanf("%s", card_number);

	int found = 0;
	for (int i = 0; i < num_customers; i++)
		{
			if (strcmp(card_number, customers[i].cardnumber) == 0)
				{
					set_cursor_position(25, 15);
					printf("请输入续充金额：");
					scanf("%lf", &recharge_amount);

					customers[i].money += recharge_amount; // 增加充值金额
					set_cursor_position(25, 16);
					printf("续充成功！当前卡内余额为：%.2f\n", customers[i].money);
					found = 1;
					break;
				}
		}

	if (!found)
		{
			set_cursor_position(25, 15);
			printf("未找到该客户的卡号。\n");

		}

}

void query_customer()
{
	char card_number[20];
	set_cursor_position(25, 13);
	printf("=== 客户信息查询 ===\n");
	set_cursor_position(25, 14);
	printf("请输入客户卡号[XXXX](前缀$为临时)：");
	scanf("%s", card_number);

	int found = 0;
	for (int i = 0; i < num_customers; i++)
		{
			if (strcmp(card_number, customers[i].cardnumber) == 0)
				{
					set_cursor_position(25, 15);
					printf("姓名: %s\n", customers[i].name);
					set_cursor_position(25, 16);
					printf("性别: %s\n", customers[i].sex);
					set_cursor_position(25, 17);
					printf("生日: %s\n", customers[i].birth);
					set_cursor_position(25, 18);
					printf("手机号: %s\n", customers[i].phone);
					set_cursor_position(25, 19);
					printf("卡号: %s\n", customers[i].cardnumber);
					set_cursor_position(25, 20);
					printf("卡有效时间: %s\n", customers[i].card_effective);
					set_cursor_position(25, 21);
					printf("办卡日期: %s\n", customers[i].card_date);
					set_cursor_position(25, 22);
					printf("余额: %.2f\n", customers[i].money);
					found = 1;
					break;
				}
		}

	if (!found)
		{
			set_cursor_position(25, 15);
			printf("未找到该客户信息。\n");
		}
}

void base_id_date_caculate()
{
	char cardnum[20];
	char s_date[20];
	double fee = 0;
	set_cursor_position(25, 13);
	printf("=== 根据卡号和日期查询消费 ===");
	set_cursor_position(25, 14);
	printf("请输入卡号[XXXX]:");
	scanf("%s", cardnum);
	set_cursor_position(25, 15);
	printf("请输入查询日期[MM/DD]：");
	scanf("%s", s_date);
	for (int i = 0; i < num_records; i++)
		{
			if (strcmp(cardnum, records[i].cardnumber)
			        == 0 && strcmp(s_date, records[i].start_date) == 0)
				{
					fee += records[i]
					       .total_cost;
				}
		}
	set_cursor_position(25, 16);
	printf("卡号%s在 %s 的消费额为%.2f", cardnum, s_date, fee);

}

void base_id_month_caculate()
{
	char cardnum[20];
	int s_month;
	double fee = 0;
	set_cursor_position(25, 13);
	printf("=== 根据卡号和月份查询消费 ===");
	set_cursor_position(25, 14);
	printf("请输入卡号[XXXX]:");
	scanf("%s", cardnum);
	set_cursor_position(25, 15);
	printf("请输入查询月份[XX]：");
	scanf("%d", &s_month);
	int s_m, s_d;
	for (int i = 0; i < num_records; i++)
		{
			sscanf(records[i].start_date, "%2d%2d", &s_m, &s_d);

			if (strcmp(cardnum, records[i].cardnumber)
			        == 0 && s_month == s_m)
				{
					fee += records[i]
					       .total_cost;
				}
		}
	set_cursor_position(25, 16);
	printf("卡号%s在 %d 月的消费额为%.2f", cardnum, s_month, fee);
}


// 保存客户信息到文本文件
void save_customers()
{
	FILE *fp = fopen("customers.txt", "w");
	if (fp == NULL)
		{
			printf("无法打开客户数据文件。\n");
			return;
		}

	fprintf(fp, "%d\n", num_customers); // 写入客户数量

	for (int i = 0; i < num_customers; i++)
		{
			fprintf(fp, "%s %s %s %s %s %s %s %lf\n", customers[i].name, customers[i].sex, customers[i].birth,
			        customers[i].phone, customers[i].cardnumber, customers[i].card_effective, customers[i].card_date, customers[i].money);
		}

	fclose(fp);
	set_cursor_position(25, 25);
	printf("客户数据已保存。\n");
}

// 加载客户信息文本文件
void load_customers()
{
	FILE *fp = fopen("customers.txt", "r");
	if (fp == NULL)
		{
			printf("未找到客户数据文件，将从头开始。\n");
			return;
		}

	fscanf(fp, "%d", &num_customers); // 读取客户数量

	for (int i = 0; i < num_customers; i++)
		{
			fscanf(fp, "%s %s %s %s %s %s %s %lf\n", customers[i].name, customers[i].sex, customers[i].birth,
			       customers[i].phone, customers[i].cardnumber, customers[i].card_effective, customers[i].card_date, &customers[i].money);
		}

	fclose(fp);
	printf("客户数据已加载。\n");
}


// 保存上网记录到文本文件
void save_records()
{
	FILE *fp = fopen("records.txt", "w");
	if (fp == NULL)
		{
			printf("无法打开上网记录数据文件。\n");
			return;
		}

	fprintf(fp, "%d\n", num_records); // 写入记录数量

	for (int i = 0; i < num_records; i++)
		{
			fprintf(fp, "%s %s %s %s %s %s %lf\n", records[i].cardnumber, records[i].machine_location,
			        records[i].start_date, records[i].start_time, records[i].end_date, records[i].end_time, records[i].total_cost);
		}

	fclose(fp);
	set_cursor_position(25, 26);
	printf("上网记录数据已保存。\n");
}

// 加载上网记录文本文件
void load_records()
{
	FILE *fp = fopen("records.txt", "r");
	if (fp == NULL)
		{
			printf("未找到上网记录数据文件，将从头开始。\n");
			return;
		}

	fscanf(fp, "%d", &num_records); // 读取记录数量

	for (int i = 0; i < num_records; i++)
		{
			fscanf(fp, "%s %s %s %s %s %s %lf\n", records[i].cardnumber, records[i].machine_location,
			       records[i].start_date, records[i].start_time, records[i].end_date, records[i].end_time, &records[i].total_cost);
		}

	fclose(fp);
	printf("上网记录数据已加载。\n");
}



