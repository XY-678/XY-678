#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// ���峣��
#define MAX_CUSTOMERS 254//���쿨�ͻ�����
#define MAX_RECORDS 254//����¼
#define TOP_ADMIN "admin"
#define TOP_PASSWORD "123456"

// �ͻ��ṹ��
typedef struct
{
	char name[50];          // ����
	char sex[10];           // �Ա�
	char birth[20];         // ����
	char phone[20];         // �ֻ���
	char cardnumber[20];    // ���ţ�ǰ׺$Ϊ��ʱ
	char card_effective[20];// ����Чʱ�䣬��Ϊ��λ
	char card_date[20];     // �쿨����[YY/DD]
	double money;           // ��ֵ���
} customer;


// ������¼�ṹ��
typedef struct
{
	char cardnumber[20];    // ����
	char machine_location[50];  // ����λ��
	char start_date[20];    // �������� [YY/DD]
	char start_time[20];    // ����ʱ�� [h] 24Сʱ������Сʱ���㣬1-24
	char end_date[20];      // �������� [YY/DD]
	char end_time[20];      // ����ʱ�� [h] 24Сʱ������Сʱ���㣬1-24
	double total_cost;      // ���û�Ӫҵ��
} internetRecord;

// ȫ�ֱ���
customer customers[MAX_CUSTOMERS];
int num_customers = 0;
internetRecord records[MAX_RECORDS];
int num_records = 0;
COORD coord; // ������λ��

// ��������
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

// ������
int main()
{
	char user[20];
	char word[20];
	system("title ���ɹ���ϵͳ");
	system("color 79");
	load_customers();
	load_records();
	int choice;
	do
		{
			system("cls");
			set_cursor_position(25, 3);
			printf("������ʹ����[1(����)/2(�û�)/0(�˳�)]:");
			scanf("%d", &choice);
			switch (choice)
				{
					case 1:
						system("cls");
						set_cursor_position(25, 3);
						printf("�û�����");
						scanf("%s", user);
						set_cursor_position(25, 4);
						printf("���룺");
						scanf("%s", word);
						if (strcmp(user, TOP_ADMIN) == 0 && strcmp(word, TOP_PASSWORD) == 0)
							{
								admin();
							}
						else
							{
								set_cursor_position(25, 5);
								printf("�û������������");


								break;
							case 2:
								customer1();
								break;
							case 0:
								set_cursor_position(25, 24);
								printf("��лʹ�ã�");
								break;
							default:
								printf("��Ч��ѡ�����������롣");
							}
				}
			if (choice != 0)
				{
					printf("\n");
					system("pause"); // ��ͣ�Ա��û��鿴��Ϣ
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
			printf("===�û�ϵͳ===");
			set_cursor_position(25, 4);
			printf("------------------------------------------------------------");
			set_cursor_position(30, 5);
			printf("1.��ѯָ����������\n");
			set_cursor_position(65, 5);
			printf("2.��ѯָ���·�����\n");
			set_cursor_position(30, 6);
			printf("3.��ѯ������Ϣ\n");
			set_cursor_position(65, 6);
			printf("0.�˳�\n");
			set_cursor_position(25, 7);
			printf("------------------------------------------------------------");
			set_cursor_position(25, 8);
			printf("���������:");
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
						printf("��лʹ�ã�");
						break;
					default:
						set_cursor_position(25, 9);
						printf("��Ч��ѡ�����������롣");
				}
			if (choice1 != 0)
				{
					printf("\n");
					system("pause"); // ��ͣ�Ա��û��鿴��Ϣ
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
						add_customer();  // �ͻ��쿨����
						break;
					case 2:
						up_customer();   // �ͻ������Ǽ�
						break;
					case 3:
						down_customer(); // �ͻ���������
						break;
					case 4:
						calculate_revenue();  // ͳ������Ӫҵ��
						break;
					case 5:
						monthly_consumption(); // ����ͳ�ư쿨�ͻ�����
						break;
					case 6:
						recharge_customer(); // �ͻ����书��
						break;
					case 7:
						query_customer();//��ѯ�ͻ���Ϣ
						break;
					case 8:
						modifyCustomerInfo();//�޸Ŀͻ���Ϣ
						break;
					case 9:
						deleteCustomer();//ɾ���ͻ�
						break;
					case 10:
						base_id_date_caculate();//���ݿ��ź����ڲ�ѯ����
						break;
					case 11:
						base_id_month_caculate();//���ݿ��ź��·ݲ�ѯ����
						break;
					case 0:
						set_cursor_position(25, 13);
						printf("��лʹ�ã�\n");  // �˳�
						break;
					default:
						set_cursor_position(25, 13);
						printf("��Ч��ѡ�����������롣\n");
				}
			if (choice != 0)
				{
					printf("\n");
					system("pause"); // ��ͣ�Ա��û��鿴��Ϣ
				}
		}
	while (choice != 0);

	save_customers();
	save_records();

	return ;
}

//չʾ�˵�
void display_menu()
{
	system("cls");
	set_cursor_position(25, 3);
	printf("���ɿͻ�����ϵͳ");
	set_cursor_position(25, 4);
	printf("------------------------------------------------------------");
	set_cursor_position(30, 5);
	printf("1.�ͻ��쿨����");
	set_cursor_position(65, 5);
	printf("2.�ͻ������Ǽ�");
	set_cursor_position(30, 6);
	printf("3.�ͻ���������");
	set_cursor_position(65, 6);
	printf("4.ͳ������Ӫҵ��");
	set_cursor_position(30, 7);
	printf("5.����ͳ�ư쿨�ͻ�����");
	set_cursor_position(65, 7);
	printf("6.�ͻ����书��");
	set_cursor_position(30, 8);
	printf("7.�ͻ���Ϣ��ѯ");
	set_cursor_position(65, 8);
	printf("8.�ͻ���Ϣ�޸�");
	set_cursor_position(30, 9);
	printf("9.�ͻ���Ϣɾ��");
	set_cursor_position(65, 9);
	printf("10.���ָ��������ĳ�յ�Ӫҵ��");
	set_cursor_position(30, 10);
	printf("11.���ָ��������ĳ�µ����Ѷ�");
	set_cursor_position(65, 10);
	printf("0.�˳�ϵͳ");
	set_cursor_position(25, 11);
	printf("------------------------------------------------------------");
	set_cursor_position(25, 12);
	printf("��������������룺");
}

// ���ù��λ��
void set_cursor_position(short x, short y)
{
	COORD coord;
	HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(ConsoleHandle, coord);
}

// �ͻ��쿨����
void add_customer()
{
	char card_number[20];
	set_cursor_position(25, 13);
	printf("=== �ͻ��쿨���� ===\n");
	set_cursor_position(25, 14);
	printf("������ͻ�������");
	scanf("%s", customers[num_customers].name);
	set_cursor_position(25, 15);
	printf("������ͻ��Ա�");
	scanf("%s", customers[num_customers].sex);
	set_cursor_position(25, 16);
	printf("������ͻ���������[YY/DD]��");
	scanf("%s", customers[num_customers].birth);
	set_cursor_position(25, 17);
	printf("������ͻ��ֻ��ţ�");
	scanf("%s", customers[num_customers].phone);
	set_cursor_position(25, 18);
	printf("������ͻ�����[XXXX]��");
	do
		{
			scanf("%s", card_number);
			if (comparecard(card_number))
				{
					set_cursor_position(55, 18);
					printf("�ÿ����Ѿ����ڣ�����������\n");
				}
			else
				{
					strcpy(customers[num_customers].cardnumber, card_number);
					break;
				}
		}
	while (1);
	set_cursor_position(25, 19);
	printf("�����뿨��Чʱ�䣺");
	scanf("%s", customers[num_customers].card_effective);
	set_cursor_position(25, 20);
	printf("������쿨����[YY/DD]��");
	scanf("%s", customers[num_customers].card_date);
	set_cursor_position(25, 21);
	printf("�������ֵ��");
	scanf("%lf", &customers[num_customers].money);

	num_customers++;   // �û�+1
	set_cursor_position(25, 22);
	printf("�ͻ��쿨�ɹ���\n");

}

// �ȽϿ���
int comparecard(const char *card_number)
{
	for (int i = 0; i < num_customers; i++)
		{
			if (strcmp(card_number, customers[i].cardnumber) == 0)
				{
					return 1; // ��ͬ����
				}
		}
	return 0; // ����ͬ����
}

// �޸Ŀͻ���Ϣ����
void modifyCustomerInfo()
{
	char cardNumber[20];
	set_cursor_position(25, 13);
	printf("������Ҫ�޸���Ϣ�Ŀͻ����ţ�");
	scanf("%s", cardNumber);

	// �������в��ҿͻ���Ϣ
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
			printf("δ�ҵ��ÿͻ���Ϣ��\n");
		}
	else
		{

			// ��ʾ�޸���Ϣ
			set_cursor_position(25, 14);
			printf("������Ҫ�޸ĵ���Ϣ������/�Ա�/����/�ֻ���/����Чʱ��ȣ���");
			set_cursor_position(25, 15);
			printf("������");
			scanf("%s", customers[foundIndex].name);
			set_cursor_position(25, 16);
			printf("�Ա�");
			scanf("%s", customers[foundIndex].sex);
			set_cursor_position(25, 17);
			printf("���գ�");
			scanf("%s", customers[foundIndex].birth);
			set_cursor_position(25, 18);
			printf("�ֻ��ţ�");
			scanf("%s", customers[foundIndex].phone);
			set_cursor_position(25, 19);
			printf("����Чʱ�䣺");
			scanf("%s", customers[foundIndex].card_effective);

			set_cursor_position(25, 20);
			printf("�ͻ���Ϣ�ѳɹ��޸ġ�\n");
		}
}


// ɾ���ͻ���Ϣ����
void deleteCustomer()
{
	char cardNumber[20];
	set_cursor_position(25, 13);
	printf("������Ҫɾ���Ŀͻ����ţ�");
	scanf("%s", cardNumber);

	// �������в��ҿͻ���Ϣ
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
			printf("δ�ҵ��ÿͻ���Ϣ��");
		}
	else
		{
			// �ƶ�����Ԫ����ɾ��ָ���ͻ�
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
			num_customers--; // ���¿ͻ�����
			set_cursor_position(25, 14);
			printf("�ͻ���Ϣ�ѳɹ�ɾ����\n");

		}
}

// �ͻ������Ǽ�
void up_customer()
{
	char card_number[20];
	char machine_location[50];
	char start_date[20];
	char start_time[20];
	set_cursor_position(25, 13);
	printf("=== �ͻ������Ǽ� ===\n");
	set_cursor_position(25, 14);
	printf("������ͻ�����[XXXX](ǰ׺$Ϊ��ʱ)��");
	scanf("%s", card_number);

	int is_temporary = (card_number[0] == '$') ? 1 : 0;
	set_cursor_position(25, 15);
	printf("������ʹ�û���λ�ã�");
	scanf("%s", machine_location);
	set_cursor_position(25, 16);
	printf("��������������[YY/DD]��");
	scanf("%s", start_date);
	set_cursor_position(25, 17);
	printf("����������ʱ��[h/min]��");
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
	printf("�Ǽǳɹ���\n");
}

// �ͻ���������
void down_customer()
{
	char card_number[20];
	char end_date[20];
	char end_time[20];
	int index;
	set_cursor_position(25, 13);
	printf("=== �ͻ��������� ===\n");
	set_cursor_position(25, 14);
	printf("������ͻ�����[XXXX]��");
	scanf("%s", card_number);

	int found = 0;
	for (int i = 0; i < num_records; i++)
		{
			if (strcmp(card_number, records[i].cardnumber) == 0 &&
			        strcmp(records[i].end_date, "N/A") == 0 &&
			        strcmp(records[i].end_time, "N/A") == 0)
				{
					set_cursor_position(25, 15);
					printf("��������������[YY/DD]��");
					scanf("%s", end_date);
					set_cursor_position(25, 16);
					printf("����������ʱ��[h/min]��");
					scanf("%s", end_time);

					strcpy(records[i].end_date, end_date);
					strcpy(records[i].end_time, end_time);

					index = i; // ��¼��ǰ�����Լ���ʱ��

					found = 1;
					break;
				}
		}

	if (!found)
		{
			set_cursor_position(25, 15);
			printf("δ�ҵ��ÿͻ����������ļ�¼��");
			return;
		}

	counthour(index); // ����ʱ��
}

// ����ʱ���ͷ���
void counthour(int i)
{
	int s_month, s_day, s_hour, s_min;
	int e_month, e_day, e_hour, e_min;
	double total_hours;
	double cost_per_hour = 2.0;
	double discount_rate = 1.0; // Ĭ��û���ۿ�

	// ������ʼʱ��
	sscanf(records[i].start_date, "%2d%2d", &s_month, &s_day);
	sscanf(records[i].start_time, "%2d:%2d", &s_hour, &s_min);

	// ��������ʱ��
	sscanf(records[i].end_date, "%2d%2d", &e_month, &e_day);
	sscanf(records[i].end_time, "%2d:%2d", &e_hour, &e_min);



	// ���㿪ʼ�ͽ������ڵ���Сʱ��
	double start_total_hours = (s_day - 1) * 24 + s_hour + s_min / 60.0;
	double end_total_hours = (e_day - 1) * 24 + e_hour + e_min / 60.0;

	// ����������ڵ��·ݴ��ڿ�ʼ���ڵ��·ݣ���Ҫ���´���
	if (e_month > s_month)
		{
			// ���㿪ʼ�·�ʣ��Сʱ��
			int day1 = 0;
			while (s_month > 1)
				{
					s_month--;
					day1 += days_in_month(s_month);

				}
			start_total_hours += day1 * 24;

			// ��������·�Сʱ��
			int day2 = 0;
			while (e_month > 1)
				{
					e_month--;
					day2 += days_in_month(e_month);

				}
			end_total_hours += day2 * 24;
		}
// ������ʱ��
	total_hours = end_total_hours - start_total_hours;

	if (records[i].cardnumber[0] == '$')
		{
			// �ж��Ƿ񳬹�8Сʱ
			if (total_hours > 8)
				{
					discount_rate = 0.9; // ����8Сʱ��9��
					records[i].total_cost = total_hours * cost_per_hour * discount_rate;
					set_cursor_position(25, 17);
					printf("����ɹ�������Ϊ��%.2lf\n", records[i].total_cost);
					return;
				}
		}

	// �ж��Ƿ�Ϊ�쿨�ͻ�������ǰ׺��Ϊ'$'��
	if (records[i].cardnumber[0] != '$')
		{
			discount_rate = 0.8; // �쿨�ͻ�����8���Ż�
		}
	// �������
	double total_cost = total_hours * cost_per_hour * discount_rate;
	double recharge_amount;
	// ��鿨������Ƿ��㹻֧������
	for (int j = 0; j < num_customers; j++)
		{
			if (strcmp(customers[j].cardnumber, records[i].cardnumber) == 0)
				{
					if (customers[j].money >= total_cost)
						{
							customers[j].money -= total_cost; // �۳����
							set_cursor_position(25, 17);
							printf("����ɹ�������Ϊ��%.2f���������Ϊ��%.2f\n", total_cost, customers[j].money);
						}
					else
						{
							do
								{
									set_cursor_position(25, 17);
									printf("�������㣬����ʧ�ܣ����������ֵ\n");


									set_cursor_position(25, 18);
									printf("�����������");
									scanf("%lf", &recharge_amount);
									customers[i].money += recharge_amount; // ���ӳ�ֵ���
									set_cursor_position(25, 19);
									printf("����ɹ�����ǰ�������Ϊ��%.2f\n", customers[i].money);
									if (customers[i].money >= total_cost)
										{
											set_cursor_position(25, 20);
											customers[i].money -= total_cost;
											printf("�۳����Ѻ�ʣ��%.2f\n", customers[i].money );
											break;
										}


								}
							while (customers[j].money < total_cost);
						}
					break;
				}
		}

	records[i].total_cost = total_cost; // ���¼�¼�е��ܷ���


}

// ����ָ���·ݵ�����
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

// ������ͳ������Ӫҵ�����ǰʮ���ͻ�����
void calculate_revenue()
{

	char target_date[20];
	double total_revenue = 0.0;
	set_cursor_position(25, 13);
	printf("=== ͳ������Ӫҵ�� ===");
	set_cursor_position(25, 14);
	printf("������Ҫͳ�Ƶ�����[YY/DD]��");
	scanf("%s", target_date);

	// �������м�¼��ɸѡָ�������ڵļ�¼��������Ӫҵ��
	for (int i = 0; i < num_records; i++)
		{
			if (strcmp(records[i].end_date, "N/A") != 0 && strcmp(records[i].start_date, target_date) == 0)
				{
					total_revenue += records[i].total_cost;
				}
		}
	set_cursor_position(25, 15);
	printf("ָ������ %s ����Ӫҵ��Ϊ��%.2f\n", target_date, total_revenue);

	// ͳ������ǰʮ���ͻ�����
	// ����һ���ṹ���������ڴ洢�ͻ������Ͷ�Ӧ�������Ѷ�
	typedef struct
	{
		char name[50];
		double total_cost;
	} CustomerCost;
	//printf("%s", customers[3].name);
	CustomerCost top_customers[MAX_CUSTOMERS];
	int num_top_customers = 0;

// �������м�¼���ۼ�ÿλ�ͻ��������Ѷ�
	for (int i = 0; i < num_records; i++)
		{
			if (strcmp(records[i].end_date, "N/A") != 0 && strcmp(records[i].start_date, target_date) == 0
			        && records[i].cardnumber[0] != '$')
				{
					int found = 0;
					// ���ݿ����ҵ���Ӧ�Ŀͻ�����
					int customer_index = -1;
					for (int k = 0; k < num_customers; k++)
						{
							if (strcmp(customers[k].cardnumber, records[i].cardnumber) == 0)
								{
									customer_index = k;
									break;
								}
						}

					// ����ҵ��˶�Ӧ�Ŀͻ�
					if (customer_index != -1)
						{
							// ����Ƿ��Ѿ���¼�ÿͻ������Ѷ���Ѽ�¼��������Ѷ����������¼
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
											printf("�Ѵﵽ���ͻ��������ƣ��޷���Ӹ���ͻ���\n");
											break;
										}
								}
						}
				}
		}
	//printf("%s", top_customers[3].name);

	// �Կͻ��������Ѷ����򣨽���
	for (int i = 0; i < num_top_customers - 1; i++)
		{
			for (int j = 0; j < num_top_customers - 1 - i; j++)
				{
					if (top_customers[j].total_cost < top_customers[j + 1].total_cost)
						{
							// ����˳��
							CustomerCost temp = top_customers[j];
							top_customers[j] = top_customers[j + 1];
							top_customers[j + 1] = temp;
						}
				}
		}



	// �������ǰʮ���ͻ�������
	set_cursor_position(25, 16);
	printf("����ǰʮ���ͻ������������Ѷ");
	for (int i = 0; i < 10 && i < num_top_customers; i++)
		{
			set_cursor_position(25, i + 17);

			printf("%d. %s - %.2f", i + 1, top_customers[i].name, top_customers[i].total_cost);
		}

}


// ����ͳ�ư쿨�ͻ����Ѳ��������
void monthly_consumption()
{
	int target_month;
	set_cursor_position(25, 13);
	printf("=== ����ͳ�ư쿨�ͻ����� ===");
	set_cursor_position(25, 14);
	printf("������Ҫͳ�Ƶ��·ݣ�");
	scanf("%d", &target_month);
	double total_revenue;
	// �������м�¼��ɸѡָ���¼�¼��������Ӫҵ��
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
	printf("ָ��%d�µ���Ӫҵ��Ϊ��%.2f\n", target_month, total_revenue);


	// ����һ���ṹ���������ڴ洢�쿨�ͻ������Ͷ�Ӧ�������Ѷ�
	typedef struct
	{
		char name[50];
		double total_cost;
	} CardCustomerCost;

	CardCustomerCost card_customers[MAX_CUSTOMERS];
	int num_card_customers = 0;

	// �������м�¼���ۼ�ÿλ�쿨�ͻ���ָ���·ݵ������Ѷ�
	for (int i = 0; i < num_records; i++)
		{
			int start_month, start_day;
			sscanf(records[i].start_date, "%2d%2d", &start_month, &start_day);

			if (start_month == target_month && records[i].cardnumber[0] != '$')
				{
					int found = 0;
					// ���ݿ����ҵ���Ӧ�Ŀͻ�����
					int customer_index = -1;
					for (int k = 0; k < num_customers; k++)
						{
							if (strcmp(customers[k].cardnumber, records[i].cardnumber) == 0)
								{
									customer_index = k;
									break;
								}
						}
					// ����ҵ��˶�Ӧ�Ŀͻ�
					if (customer_index != -1)
						{
							// ����Ƿ��Ѿ���¼�ÿͻ������Ѷ���Ѽ�¼��������Ѷ����������¼
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
											printf("�Ѵﵽ���ͻ��������ƣ��޷���Ӹ���ͻ���\n");
											break;
										}
								}
						}


				}
		}

	// �԰쿨�ͻ��������Ѷ����򣨽���
	for (int i = 0; i < num_card_customers - 1; i++)
		{
			for (int j = 0; j < num_card_customers - 1 - i; j++)
				{
					if (card_customers[j].total_cost < card_customers[j + 1].total_cost)
						{
							// ����˳��
							CardCustomerCost temp = card_customers[j];
							card_customers[j] = card_customers[j + 1];
							card_customers[j + 1] = temp;
						}
				}
		}

	// ��������Ѷ������İ쿨�ͻ����������Ѷ�
	set_cursor_position(25, 16);
	printf("�����Ѷ������İ쿨�ͻ������������Ѷ\n");
	for (int i = 0; i < num_card_customers; i++)
		{
			set_cursor_position(25, i + 17);
			printf("%d. %s - %.2f", i + 1, card_customers[i].name, card_customers[i].total_cost);
		}
}



// �ͻ����书��
void recharge_customer()
{
	char card_number[20];
	double recharge_amount;
	set_cursor_position(25, 13);
	printf("=== �ͻ����书�� ===\n");
	set_cursor_position(25, 14);
	printf("������ͻ�����[XXXX]��");
	scanf("%s", card_number);

	int found = 0;
	for (int i = 0; i < num_customers; i++)
		{
			if (strcmp(card_number, customers[i].cardnumber) == 0)
				{
					set_cursor_position(25, 15);
					printf("�����������");
					scanf("%lf", &recharge_amount);

					customers[i].money += recharge_amount; // ���ӳ�ֵ���
					set_cursor_position(25, 16);
					printf("����ɹ�����ǰ�������Ϊ��%.2f\n", customers[i].money);
					found = 1;
					break;
				}
		}

	if (!found)
		{
			set_cursor_position(25, 15);
			printf("δ�ҵ��ÿͻ��Ŀ��š�\n");

		}

}

void query_customer()
{
	char card_number[20];
	set_cursor_position(25, 13);
	printf("=== �ͻ���Ϣ��ѯ ===\n");
	set_cursor_position(25, 14);
	printf("������ͻ�����[XXXX](ǰ׺$Ϊ��ʱ)��");
	scanf("%s", card_number);

	int found = 0;
	for (int i = 0; i < num_customers; i++)
		{
			if (strcmp(card_number, customers[i].cardnumber) == 0)
				{
					set_cursor_position(25, 15);
					printf("����: %s\n", customers[i].name);
					set_cursor_position(25, 16);
					printf("�Ա�: %s\n", customers[i].sex);
					set_cursor_position(25, 17);
					printf("����: %s\n", customers[i].birth);
					set_cursor_position(25, 18);
					printf("�ֻ���: %s\n", customers[i].phone);
					set_cursor_position(25, 19);
					printf("����: %s\n", customers[i].cardnumber);
					set_cursor_position(25, 20);
					printf("����Чʱ��: %s\n", customers[i].card_effective);
					set_cursor_position(25, 21);
					printf("�쿨����: %s\n", customers[i].card_date);
					set_cursor_position(25, 22);
					printf("���: %.2f\n", customers[i].money);
					found = 1;
					break;
				}
		}

	if (!found)
		{
			set_cursor_position(25, 15);
			printf("δ�ҵ��ÿͻ���Ϣ��\n");
		}
}

void base_id_date_caculate()
{
	char cardnum[20];
	char s_date[20];
	double fee = 0;
	set_cursor_position(25, 13);
	printf("=== ���ݿ��ź����ڲ�ѯ���� ===");
	set_cursor_position(25, 14);
	printf("�����뿨��[XXXX]:");
	scanf("%s", cardnum);
	set_cursor_position(25, 15);
	printf("�������ѯ����[MM/DD]��");
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
	printf("����%s�� %s �����Ѷ�Ϊ%.2f", cardnum, s_date, fee);

}

void base_id_month_caculate()
{
	char cardnum[20];
	int s_month;
	double fee = 0;
	set_cursor_position(25, 13);
	printf("=== ���ݿ��ź��·ݲ�ѯ���� ===");
	set_cursor_position(25, 14);
	printf("�����뿨��[XXXX]:");
	scanf("%s", cardnum);
	set_cursor_position(25, 15);
	printf("�������ѯ�·�[XX]��");
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
	printf("����%s�� %d �µ����Ѷ�Ϊ%.2f", cardnum, s_month, fee);
}


// ����ͻ���Ϣ���ı��ļ�
void save_customers()
{
	FILE *fp = fopen("customers.txt", "w");
	if (fp == NULL)
		{
			printf("�޷��򿪿ͻ������ļ���\n");
			return;
		}

	fprintf(fp, "%d\n", num_customers); // д��ͻ�����

	for (int i = 0; i < num_customers; i++)
		{
			fprintf(fp, "%s %s %s %s %s %s %s %lf\n", customers[i].name, customers[i].sex, customers[i].birth,
			        customers[i].phone, customers[i].cardnumber, customers[i].card_effective, customers[i].card_date, customers[i].money);
		}

	fclose(fp);
	set_cursor_position(25, 25);
	printf("�ͻ������ѱ��档\n");
}

// ���ؿͻ���Ϣ�ı��ļ�
void load_customers()
{
	FILE *fp = fopen("customers.txt", "r");
	if (fp == NULL)
		{
			printf("δ�ҵ��ͻ������ļ�������ͷ��ʼ��\n");
			return;
		}

	fscanf(fp, "%d", &num_customers); // ��ȡ�ͻ�����

	for (int i = 0; i < num_customers; i++)
		{
			fscanf(fp, "%s %s %s %s %s %s %s %lf\n", customers[i].name, customers[i].sex, customers[i].birth,
			       customers[i].phone, customers[i].cardnumber, customers[i].card_effective, customers[i].card_date, &customers[i].money);
		}

	fclose(fp);
	printf("�ͻ������Ѽ��ء�\n");
}


// ����������¼���ı��ļ�
void save_records()
{
	FILE *fp = fopen("records.txt", "w");
	if (fp == NULL)
		{
			printf("�޷���������¼�����ļ���\n");
			return;
		}

	fprintf(fp, "%d\n", num_records); // д���¼����

	for (int i = 0; i < num_records; i++)
		{
			fprintf(fp, "%s %s %s %s %s %s %lf\n", records[i].cardnumber, records[i].machine_location,
			        records[i].start_date, records[i].start_time, records[i].end_date, records[i].end_time, records[i].total_cost);
		}

	fclose(fp);
	set_cursor_position(25, 26);
	printf("������¼�����ѱ��档\n");
}

// ����������¼�ı��ļ�
void load_records()
{
	FILE *fp = fopen("records.txt", "r");
	if (fp == NULL)
		{
			printf("δ�ҵ�������¼�����ļ�������ͷ��ʼ��\n");
			return;
		}

	fscanf(fp, "%d", &num_records); // ��ȡ��¼����

	for (int i = 0; i < num_records; i++)
		{
			fscanf(fp, "%s %s %s %s %s %s %lf\n", records[i].cardnumber, records[i].machine_location,
			       records[i].start_date, records[i].start_time, records[i].end_date, records[i].end_time, &records[i].total_cost);
		}

	fclose(fp);
	printf("������¼�����Ѽ��ء�\n");
}



