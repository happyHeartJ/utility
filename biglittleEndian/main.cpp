bool checkCPU()     // ����Ǵ��ģʽ��������
{
    short int test = 0x1234;
    
    if( *((char *)&test) == 0x12)     // �͵�ַ��Ÿ��ֽ�����
        return true;
    else
        return false;
}
/*
int main(void)
{
    if( !checkCPU())
        cout<<"Little endian"<<endl;
    else
        cout<<"Big endian"<<endl;

    return 0;
}
*/

int main(void)
{
    short int a = 0x1234;
    char x0 , x1;

    x0 = ((char *)&a)[0];
    x1 = ((char *)&a)[1];

    if( x0 == 0x34)
        cout<<"Little endian"<<endl;
    else
        cout<<"Big endian"<<endl;

    return 0;
}
