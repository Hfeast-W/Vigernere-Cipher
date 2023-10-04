//Vigenere Cipher Encrypt
# include<stdio.h>
# include<string.h>
# include "head.h"
# define N 10000
# define _CRT_SECURE_NO_WARNINGS

char text[N] = { '\0' };
char result[N] = { '\0' };
char key[N] = { '\0' };
char sub[N] = { '\0' };
char ch[2] = { '\0' };
int position[N] = { 0 };
int count_ch[26] = { 0 };
float prob_ch[26] = { 0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.020, 0.061, 0.070, 0.002,
					 0.008, 0.040, 0.024, 0.067, 0.075, 0.019, 0.001, 0.060, 0.063, 0.091,
					 0.028, 0.010, 0.023, 0.001, 0.020, 0.001 };

int ct_t, ct_k, ct_r, mode;
ct_k = 0;
ct_r = 0;
int sp_arr = 0;

// ��ȡԭ��
int readfile()
{
	/*char a[100];*/
	int index;
	FILE* fp = fopen("plaintext.txt", "r");
	if (fp == NULL)
	{
		printf("�ļ���ȡ��Ч.\n");
		return -1;
	}
	for (index = 0; !feof(fp); index++)
		fscanf(fp, "%c", &text[index]);

	fclose(fp);
	fp = NULL;

	/*for (index = 0; text[index] != '\0'; index++)
		printf("%c", text[index]);
	printf("\n");*/

	return 0;
}

//��txt�ļ�����Ŀ¼��û�и��ļ����򴴽�
int writein()
{
	FILE* fpWrite;
	fpWrite = fopen("cipher.txt", "w");
	if (fpWrite == NULL)
	{
		return 1;
	}

//д����a�����ݵ�data_.txt�ļ���
	for (int index = 0; result[index] != '\0'; index++)
	{
		fprintf(fpWrite, "%c", result[index]);
	}

//�ر���д�ļ�

	fclose(fpWrite);
}


//�����ַ������Ӵ����ֵĴ��� 
int subString(char* str, char* sub, int* position)//str��ĸ��
{
	int count = 0, i, j, k;
	k = 0;
	for (i = 0; i < strlen(str); i++)
	{
		for (j = 0; j < strlen(sub); j++)
		{
			if (str[i + j] != sub[j])
				break; // �����˲�ͬ�ַ����˳�ѭ��
		}
		if (j == strlen(sub))
		{
			position[k++] = i;
			printf(" %d ", i);
			count++; // �˳�ѭ������j��ֵ�����Ӵ��ĳ��ȣ�������Ӵ�
		}
	}
	return count;
}


//int main(void)
//{
//	char str[100], sub[50];
//	printf("������ĸ����");
//	gets_s(str);
//	printf("�������Ӵ���");
//	gets_s(sub);
//	printf("%d\n", subString(str, sub));
//	return 0;
//}
//Encrypt Cipher
int Pim(char* str, char ch)
{
	char* p_str = str; 
	int count = 0;
	while (*p_str)
	{	
		if (*p_str++ == ch)
		{
			count++;
		}
	}
	return count;
}

void Pim_arr(char* str, char ch) // �����ַ����м���ch�����������洢��������
{
	char* p_str = str;
	int count = 0;
	while (*p_str)
	{
		if (*p_str++ == ch)
		{
			count++;
		}
	}
	count_ch[ch - 'A'] = count;
}

void di_sub(char* result, int m) // m��ʾҪ�����Ĵ����m��
{
		int k = 0;
		int j = 0;
		while (result[sp_arr + k * m] != '\0')
		{
			sub[j] = result[sp_arr + k * m];
			j++;
			k++;
		}
		sp_arr++;
}

int encrypt (char* text, char* result, char* key)
{
	int len_t = strlen(text);
	int len_k = strlen(key);
	for ( ct_t = 0; ct_t < len_t; ct_t++)
	{
			if (text[ct_t] >= 'A' && text[ct_t] <= 'Z')
			{
				if (ct_k == len_k)
				{
					ct_k = 0;
				}

				result[ct_t] = 'A' + ((text[ct_t] - 'A' + key[ct_k] - 'A') % 26);
				ct_k++;
			}
			else if (text[ct_t] >= 'a' && text[ct_t] <= 'z')
			{
				if (ct_k == len_k)
				{
					ct_k = 0;
				}

				result[ct_t] = 'a' + ((text[ct_t] - 'a' + key[ct_k] - 'a') % 26);
				ct_k++;
			}
			else
			{
				result[ct_t] = text[ct_t];
			}
			
	}
	return 0;
}

int decrypt()
{
	printf("��������Ҫ���ı��в�ѯ���Ӵ�:");
	scanf("%s", sub);
	
	int count = subString(result, sub, position);
	printf("�Ӵ� %s ���ֵĴ���Ϊ: %d\n", sub, count);//CUT = 3	
	
	//��Kasiski���Է��²�
	int d1 = gcd(position[1] - position[0], position[2] - position[1]);
	int d2 = gcd(position[1] - position[0], position[2] - position[0]);
	int d3 = gcd(position[2] - position[1], position[2] - position[0]);
	printf("ǰ����λ������֮���gcdΪ: %d %d %d\n", d1, d2, d3);
	//printf("���������Լ���ʹ��gcd()������λ��֮������Լ��\n");
	int key_len;
	printf("����������ȡ����Կ����:");
	scanf("%d", &key_len);

	//���غ�ָ������һ��ȷ��
	
	float numer = 0;
	for ( int i = 1; i <= key_len; i++)
	{	
		printf("\n��i=%dʱ���غ�ָ��Ϊ:(Խ�ӽ�0.065Խ����ȷ��) ", i);
		memset(sub, 0, sizeof(sub));// sub�ַ�����������
		sp_arr = 0;
		for ( int m = 0; m < i; m++)
		{
			di_sub(result, i);// �з��ַ�,�з��������ַ�������sub��
			numer = 0; // ���ӹ���
			for ( int j = 0; j < 26; j++) // ch[0]���� a~z
			{
				char ch = 'A' + j;
				// Pim(sub, ch); ����sub����ĸ��Ƶ��
				int pim_ch = Pim(sub, ch);
				numer += pim_ch * (pim_ch - 1);
			}
			float len_sub = strlen(sub);
			printf(" %.3f ", numer / (len_sub * (len_sub - 1)));
			memset(sub, 0, sizeof(sub));// sub�ַ�����������
		}  
	}	
	// ��ʼȷ���������ԿK=()
	int sub_key_len = strlen(result) / key_len;
	float sum_Mg = 0;
	memset(sub, 0, sizeof(sub));// sub�ַ�����������
	sp_arr = 0; // sp_arr����,Ϊ���ܹ�5��result
	for ( int sub_i = 0; sub_i < key_len; sub_i++)
	{
		di_sub(result, key_len);
		printf("\nsub_i=%d, M_g(sub)��ֵ: ",sub_i + 1); // M_g = sigma(i=0~25) p_i*f_(i+g)/sub_key_len
		for (int g = 0; g < 26; g++)
		{
			for (int index = 0; index < 26; index++)
			{
				float f_ch = Pim(sub, 'A' + (index + g) % 26);
				sum_Mg += prob_ch[index] * f_ch / sub_key_len;
				
			}
			printf(" %.3f ", sum_Mg);
			sum_Mg = 0;
		}
		
		memset(sub, 0, sizeof(sub));// sub�ַ�����������
	}
	
	


	
}

int main()
{
	readfile();

	printf("��ӭ����ά�������������/����ϵͳ\n");
	/*printf("��������Ҫ������ı�:");
	scanf_s("%[^\n]", text, sizeof(text));*/
	printf("��������Ҫѡ���ģʽ(1:����,2:����):");
	//scanf_s("%d", &mode, sizeof(mode));
	scanf("%d", &mode);

	if (mode == 1)
	{
		printf("��������Կ:");
		scanf_s("%s", key, sizeof(key));

		encrypt(text, result, key);
		writein();
		printf("���ܺ�Ľ����:%s",result);
	}

	else if (mode == 2)
	{
		int index;
		FILE* fp = fopen("cipher.txt", "r");
		if (fp == NULL)
		{
			printf("�ļ���ȡ��Ч.\n");
			return -1;
		}
		for (index = 0; !feof(fp); index++)
			fscanf(fp, "%c", &result[index]);

		fclose(fp);
		fp = NULL;
		decrypt();
	}
	return 0;
}