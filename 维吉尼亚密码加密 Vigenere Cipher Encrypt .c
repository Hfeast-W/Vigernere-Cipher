//Vigenere Cipher Decrypt & Encrypt
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

// 读取原文
int readfile()
{
	/*char a[100];*/
	int index;
	FILE* fp = fopen("plaintext.txt", "r");
	if (fp == NULL)
	{
		printf("文件读取无效.\n");
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

//打开txt文件，若目录下没有该文件，则创建
int writein()
{
	FILE* fpWrite;
	fpWrite = fopen("cipher.txt", "w");
	if (fpWrite == NULL)
	{
		return 1;
	}

//写数组a的数据到data_.txt文件中
	for (int index = 0; result[index] != '\0'; index++)
	{
		fprintf(fpWrite, "%c", result[index]);
	}

//关闭所写文件

	fclose(fpWrite);
}


//计算字符串中子串出现的次数 
int subString(char* str, char* sub, int* position)//str是母串
{
	int count = 0, i, j, k;
	k = 0;
	for (i = 0; i < strlen(str); i++)
	{
		for (j = 0; j < strlen(sub); j++)
		{
			if (str[i + j] != sub[j])
				break; // 出现了不同字符就退出循环
		}
		if (j == strlen(sub))
		{
			position[k++] = i;
			printf(" %d ", i);
			count++; // 退出循环后若j的值等于子串的长度，则存在子串
		}
	}
	return count;
}


//int main(void)
//{
//	char str[100], sub[50];
//	printf("请输入母串：");
//	gets_s(str);
//	printf("请输入子串：");
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

void Pim_arr(char* str, char ch) // 在子字符串中计算ch的数量，并存储在数组中
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

void di_sub(char* result, int m) // m表示要将密文串拆成m行
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
	printf("请输入您要在文本中查询的子串:");
	scanf("%s", sub);
	
	int count = subString(result, sub, position);
	printf("子串 %s 出现的次数为: %d\n", sub, count);//CUT = 3	
	
	//用Kasiski测试法猜测
	int d1 = gcd(position[1] - position[0], position[2] - position[1]);
	int d2 = gcd(position[1] - position[0], position[2] - position[0]);
	int d3 = gcd(position[2] - position[1], position[2] - position[0]);
	printf("前三个位置两两之差的gcd为: %d %d %d\n", d1, d2, d3);
	//printf("现在您可以继续使用gcd()函数求位置之差的最大公约数\n");
	int key_len;
	printf("请输入您猜取的秘钥长度:");
	scanf("%d", &key_len);

	//用重合指数法进一步确认
	
	float numer = 0;
	for ( int i = 1; i <= key_len; i++)
	{	
		printf("\n当i=%d时，重合指数为:(越接近0.065越是正确答案) ", i);
		memset(sub, 0, sizeof(sub));// sub字符型数组清零
		sp_arr = 0;
		for ( int m = 0; m < i; m++)
		{
			di_sub(result, i);// 切分字符,切分完后的子字符串存在sub里
			numer = 0; // 分子归零
			for ( int j = 0; j < 26; j++) // ch[0]遍历 a~z
			{
				char ch = 'A' + j;
				// Pim(sub, ch); 计算sub中字母的频数
				int pim_ch = Pim(sub, ch);
				numer += pim_ch * (pim_ch - 1);
			}
			float len_sub = strlen(sub);
			printf(" %.3f ", numer / (len_sub * (len_sub - 1)));
			memset(sub, 0, sizeof(sub));// sub字符型数组清零
		}  
	}	
	// 开始确定具体的秘钥K=()
	int sub_key_len = strlen(result) / key_len;
	float sum_Mg = 0;
	memset(sub, 0, sizeof(sub));// sub字符型数组清零
	sp_arr = 0; // sp_arr清零,为了能够5分result
	for ( int sub_i = 0; sub_i < key_len; sub_i++)
	{
		di_sub(result, key_len);
		printf("\nsub_i=%d, M_g(sub)的值: ",sub_i + 1); // M_g = sigma(i=0~25) p_i*f_(i+g)/sub_key_len
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
		
		memset(sub, 0, sizeof(sub));// sub字符型数组清零
	}
	
	


	
}

int main()
{
	readfile();

	printf("欢迎来到维吉尼亚密码加密/解密系统\n");
	/*printf("请输入您要处理的文本:");
	scanf_s("%[^\n]", text, sizeof(text));*/
	printf("请输入您要选择的模式(1:加密,2:解密):");
	//scanf_s("%d", &mode, sizeof(mode));
	scanf("%d", &mode);

	if (mode == 1)
	{
		printf("请输入秘钥:");
		scanf_s("%s", key, sizeof(key));

		encrypt(text, result, key);
		writein();
		printf("加密后的结果是:%s",result);
	}

	else if (mode == 2)
	{
		int index;
		FILE* fp = fopen("cipher.txt", "r");
		if (fp == NULL)
		{
			printf("文件读取无效.\n");
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
