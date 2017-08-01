/*
 * Author	: JiangHongjun
 * Date		: 2016/9/30
 * Reference	: http://blog.csdn.net/sangyongjia/article/details/33397109
 * Hash Table
 * An implement about MPQ HASH, and you can use <prepareCryptTable>, <HashString>, <GetHashTablePos_MPQ> directly. 
 * You have better do changes to <insert_string> according to your project before use it.
 */

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct  
{  
    int nHashA;  
    int nHashB;  
    char bExists;  
} SOMESTRUCTRUE,MPQHASHTABLE; 


//crytTable[]���汣�����HashString�������潫���õ���һЩ���ݣ���prepareCryptTable
//���������ʼ��
unsigned long cryptTable[0x500];

//����prepareCryptTable���µĺ�������һ������Ϊ0x500����10��������1280����cryptTable[0x500]  
void prepareCryptTable()  
{   
    unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;  
  
    for( index1 = 0; index1 < 0x100; index1++ )  
    {   
        for( index2 = index1, i = 0; i < 5; i++, index2 += 0x100 )  
        {   
            unsigned long temp1, temp2;  
  
            seed = (seed * 125 + 3) % 0x2AAAAB;  
            temp1 = (seed & 0xFFFF) << 0x10;  
  
            seed = (seed * 125 + 3) % 0x2AAAAB;  
            temp2 = (seed & 0xFFFF);  
  
            cryptTable[index2] = ( temp1 | temp2 );   
        }   
    }   
}

//���º�������lpszFileName �ַ�����hashֵ������dwHashType Ϊhash�����ͣ�
//������GetHashTablePos����������ñ������������ȡ��ֵΪ0��1��2���ú���
//����lpszFileName �ַ�����hashֵ��  
unsigned long HashString(const char *lpszkeyName, unsigned long dwHashType )  
{  
    unsigned char *key  = (unsigned char *)lpszkeyName;  
    unsigned long seed1 = 0x7FED7FED;  
    unsigned long seed2 = 0xEEEEEEEE;  
    int ch;  
  
    while( *key != 0 )  
    {  ch = *key++;  
        seed1 = cryptTable[(dwHashType<<8) + ch] ^ (seed1 + seed2);  
        seed2 = ch + seed1 + seed2 + (seed2<<5) + 3;  
    }  
    return seed1;  
}

//����GetHashTablePos��������Ϊ��Hash���в����Ƿ����Ŀ���ַ��������򷵻�Ҫ�����ַ�����Hashֵ������return -1.  
//lpszStringҪ��Hash���в��ҵ��ַ�����lpTableΪ�洢�ַ���Hashֵ��Hash��
int GetHashTablePos(char *lpszString, SOMESTRUCTURE *lpTable )  
{   
    int nHash = HashString(lpszString);  //������������HashString������Ҫ�����ַ���lpszString��Hashֵ��  
    int nHashPos = nHash % nTableSize;  
   
    if ( lpTable[nHashPos].bExists  &&  !strcmp( lpTable[nHashPos].pString, lpszString ) )   
    {  
	//����ҵ���Hashֵ�ڱ��д��ڣ���Ҫ���ҵ��ַ�������ж�Ӧλ�õ��ַ�����ͬ��  
        return nHashPos;    //�����ҵ���Hashֵ  
    }   
    else  
    {  
        return -1;    
    }   
}

//����GetHashTablePos_MPQ(��ѩ��hash�㷨)�У�lpszString ΪҪ��hash���в��ҵ��ַ�����lpTable Ϊ�洢�ַ���hashֵ��hash��nTableSize Ϊhash��ĳ��ȣ�   
int GetHashTablePos_MPQ( char *lpszString, MPQHASHTABLE *lpTable, int nTableSize )  
{  
    const int  HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;  
   
    int  nHash = HashString( lpszString, HASH_OFFSET );  
    int  nHashA = HashString( lpszString, HASH_A );  
    int  nHashB = HashString( lpszString, HASH_B );  
    int  nHashStart = nHash % nTableSize;  
    int  nHashPos = nHashStart;  
   
    while ( lpTable[nHashPos].bExists )  
   {  
//     ����������ж��ڸñ���ʱ���������ַ������ͱȽ�������hashֵ�Ϳ����ˣ����öԽṹ���е��ַ������бȽϡ�  
//         ������ӿ����е��ٶȣ�����hash��ռ�õĿռ䣿���ַ���һ��Ӧ����ʲô���ϣ�  
        if ( �� lpTable[nHashPos].nHashA == nHashA  
        &&  lpTable[nHashPos].nHashB == nHashB )  
       {  
            return nHashPos;  
       }  
       else  
       {  
            nHashPos = (nHashPos + 1) % nTableSize;  
       }  
   
        if (nHashPos == nHashStart)  
              break;  
    }  
     return -1;  
}

//ֱ�ӵ��������hashstring��nHashPos���Ƕ�Ӧ��HASHֵ��
/*
 * �˷�����Ծ���Ҫ��������ʱʹ�ã�������Ҫ��Ӧ�����ݽṹ
 * Ҳ��Ҫ���������ͷ�ļ�������ʹ��ʱ�����
 * �˴�����ʹ�÷�������ǰ�ļ����ֱ�ӱ�����������
 * 
 */
typedef struct
{
    int	weight;
    char *pkey;
}KEYNODE;
MPQHASHTABLE TestHashTable[nTableSize];  
int TestHashCTable[nTableSize];  
int TestHashDTable[nTableSize];  
list<KEYNODE> test_data[nTableSize];  

int insert_string(const char *string_in)  
{  
    const int HASH_OFFSET = 0, HASH_C = 1, HASH_D = 2;  
    unsigned int nHash = HashString(string_in, HASH_OFFSET);  
    unsigned int nHashC = HashString(string_in, HASH_C);  
    unsigned int nHashD = HashString(string_in, HASH_D);  
    unsigned int nHashStart = nHash % nTableSize;  
    unsigned int nHashPos = nHashStart;  
    int ln, ires = 0;  
  
    while (TestHashTable[nHashPos].bExists)  
    {
	//�ж�hash�����Ƿ���λ�ÿ��Դ�ţ�ȷ�����λ��
	nHashPos = (nHashPos + 1) % nTableSize;  
  
        if (nHashPos == nHashStart)  
            break;  
    }  
  
    ln = strlen(string_in);  
    if (!TestHashTable[nHashPos].bExists && (ln < nMaxStrLen))//nMaxStrLen���ַ�����󳤶�����
    {
	;
	//��������
	//��У�����ݷֱ𱣴�������������
        /*
	 * TestHashCTable[nHashPos] = nHashC;  
         * TestHashDTable[nHashPos] = nHashD;  
	*/

  	/* �����ڴ棬������������ַ��������ݺͶ�Ӧ��hashֵ
        test_data[nHashPos] = (KEYNODE *) malloc (sizeof(KEYNODE) * 1);  
        if(test_data[nHashPos] == NULL)  
        {  
            printf("10000 EMS ERROR !!!!\n");  
            return 0;  
        }  
  
        test_data[nHashPos]->pkey = (char *)malloc(ln+1);  
        if(test_data[nHashPos]->pkey == NULL)  
        {  
            printf("10000 EMS ERROR !!!!\n");  
            return 0;  
        }  
  
        memset(test_data[nHashPos]->pkey, 0, ln+1);  
        strncpy(test_data[nHashPos]->pkey, string_in, ln);  
        *((test_data[nHashPos]->pkey)+ln) = 0;  
        test_data[nHashPos]->weight = nHashPos;  
  	*/
	/*
	 * �����λ������Ϊtrue���������λ���Ѿ��������
        TestHashTable[nHashPos].bExists = 1;  
	*/
    }  
    else  
    {
	//�޷�����
	;
	/*
	 * �������
        if(TestHashTable[nHashPos].bExists)  
            printf("30000 in the hash table %s !!!\n", string_in);  
        else  
            printf("90000 strkey error !!!\n"); 
	*/
    }  
    return nHashPos;  
} 


 //��main�в���argv[1]������hashֵ��
 //./hash  "arr/units.dat"
 //./hash  "unit\neutral/acritter.grp"
 int main( int argc, char **argv )
 {
     char *test1 = "arr/units.dat";
     char *test2 = "unit/neutral/acritter.grp";
     
     unsigned long ulHashValue;
     int i = 0;
     /*��ʼ�����飺crytTable[0x500]*/
     prepareCryptTable();

     ulHashValue = HashString(test1, 0 );
     printf("----%X ----\n", ulHashValue );

     ulHashValue = HashString(test2, 1 );
     printf("----%X ----\n", ulHashValue );

     system("PAUSE");
     return 0;
 }
