/*
˵����
1������ƴ�����뷨 
2��֧��ASCII��ͺ��ֻ����ʾ
3������ѡ����ʾ�ֿ����ʾģʽ 
4�������ļ�Ϊ "Hzk16.�ֿ�"��"mhzk16.�ֿ�"����"����ƴ����ʱ�.txt"�����ڵ�ǰ�ļ��У� 
���޸����ļ�·�������޸ĳ�����PATH1��PATH2�ĺ궨�� 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH1 "Hzk16.�ֿ�"
#define PATH2 "mhzk16.�ֿ�"
#define PATH3 "����ƴ����ʱ�.txt"
#define MAX_LINE 512
#define MAX_PINYIN 10

//Ϊ�˷����������ȫ�ֵ��� 
int q,w,adr;
int m,i,j,k,p;
char u[32];
short t[16];
char s[32];
FILE *fp;
char buf[MAX_LINE]; //������
char pinyin[MAX_PINYIN];
int len,flag,count,choice; //lenΪ���ַ�����
char *appear;

//ƴ������ 
void pinyinInput();
//GB�뼰Hzk�ֿ��ȡ���� 
void GBcode();
//GB��תZB�뼰mhzk�ֿ��ȡ����
void GBtoZB();
//��ʽ��ӡ������ 
void printmode1(); //������ʾ
void printmode2(); //�Ŵ�һ�� 
void printmode3(); //б����ʾ 
void printmode4(); //������ʾ 
void printmode5(); //������ʾ  

int main(){
	int ziku, mode, breakflag=0;
	
 	printf("��ѡ����ʾ�ֿ⣺\n1.Hzk16\n2.mhzk16\n");	
 	scanf("%d",&ziku);
	printf("��ѡ����ʾģʽ��\n1.������ʾ\n\
2.�Ŵ�һ��\n3.б����ʾ\n4.������ʾ\n5.������ʾ\n");
	scanf("%d",&mode);
	
	pinyinInput();
	GBcode();
	switch(ziku){
		case 1:
			break;
		case 2:
			GBtoZB();
			break;
	}
	
	switch(mode){
		case 1:
			printmode1();
			break;
		case 2:
			printmode2();
			break;
		case 3:
			printmode3();
			break;
		case 4:
			printmode4();
			break;
		case 5:
			printmode5();
			break;	
	} 
	
	return 0; 
}

//ƴ�����뷨
void pinyinInput(){	
	printf("������ƴ����\n");
	scanf("%s",pinyin);
	 
	fp=fopen(PATH3,"rb");
	flag=0;
	count=0;
	while(fgets(buf,MAX_LINE,fp) != NULL){
		len = strlen(buf);
		buf[len-1] = '\0';  //ȥ�����з�
		appear=strstr(buf, pinyin);
		if(appear!=NULL){
			flag=1;
		}
		if(flag==1){
			if(appear==NULL){
				flag=-1;
			}
			else{
				count++;
				buf[2] = '\0';
				printf("%d%s ",count,buf);
				if(count%10==0) printf("\n");
			}
		}
		if(flag==-1) break;
	}
	printf("\n");
	fclose(fp);	
	if(flag==0) {
		printf("����ƴ��������\n");
	}else{
		printf("��ѡ�����뺺��\n");
		scanf("%d",&choice);
		fp=fopen(PATH3,"rb");
		flag=0;
		count=0;
		while(fgets(buf,MAX_LINE,fp) != NULL){
			len = strlen(buf);
			buf[len-1] = '\0';  //ȥ�����з�
			appear=strstr(buf, pinyin);
			if(appear!=NULL){
				flag=1;
			}
			if(flag==1){
				if(appear!=NULL){
					count++;
					if(count==choice){
						s[0]=buf[0];
						s[1]=buf[1];
						s[2]='\0';
						flag=-1;
					}
				}
			}
			if(flag==-1) break;
		}
	}
	fclose(fp);	
}

//GB�뼰Hzk�ֿ��ȡ���� 
void GBcode(){ 
	q=(s[0]-0xA1)&0xFF;
	w=(s[1]-0xA1)&0xFF;
	adr=(q*94+w)*32;
	
	fp=fopen(PATH1,"rb");
	if(!fp){
		printf("�ļ��޷���\n");
		exit(1);
	}
	fseek(fp,adr,SEEK_SET);
	fread(u,1,32,fp); 
	fclose(fp);		
}
 
//����GB���ܱ�ZB��ת����mhzk�ֿ��ȡ
void GBtoZB(){
	if((s[0]&0x80)==0){	//ASCii
		t[0]=t[0]&0x0F;
		t[0]=t[0]&(s[0]);
	} else {				//HanZi
		q =(s[0]-0xA1)&0xFF; 	//Qu
		w =(s[1]-0xA1)&0xFF; 		//Wei
		m=0x2000+q*94+w; 			//ZB��
		t[0]=((short)m);
	}
	
	//�����һ���ַ������š����֡���ʾ�ַ��ȡ�
	if((t[0]&0xE000)==0){ //ASCii����������
		adr=adr; //�ֿ���λ��
	} else if((t[0]&0x8000)==0){ //�����ַ�
		adr=(t[0]-0x2000+256)*32;	//ǰ256���ַ���ascii��
 	}

	//mhzk�ֿ��ȡ
	fp=fopen(PATH2,"rb");
	if(!fp){
		printf("�ļ��޷���2\n");
		exit(1);
	}
	fseek(fp,adr,SEEK_SET);
	fread(u,1,32,fp);
	fclose(fp);	
}

//������ʾ
void printmode1(){
 	for(j=0;j<16;j++){
		m=0x80;
		for(i=0;i<8;i++){
			if(m&u[j*2]){
				printf("MM");
			}
			else{
				printf("  ");
			}
			m>>=1;
		}
		m=0x80;
		for(i=0;i<8;i++){
			if(m&u[j*2+1]){
				printf("MM");
			}
			else{
				printf("  ");
			}
			m>>=1;
		}
		printf("\n");
	}
		
	return;
}

//�Ŵ�һ��
void printmode2(){
 	for(j=0;j<16;j++){
		m=0x80;
		for(i=0;i<8;i++){
			if(m&u[j*2]){
				printf("MMMM");
			}
			else{
				printf("    ");
			}
			m>>=1;
		}
		m=0x80;
		for(i=0;i<8;i++){
			if(m&u[j*2+1]){
				printf("MMMM");
			}
			else{
				printf("    ");
			}
			m>>=1;
		}
		printf("\n");
		m=0x80;
		for(i=0;i<8;i++){
			if(m&u[j*2]){
				printf("MMMM");
			}
			else{
				printf("    ");
			}
			m>>=1;
		}
		m=0x80;
		for(i=0;i<8;i++){
			if(m&u[j*2+1]){
				printf("MMMM");
			}
			else{
				printf("    ");
			}
			m>>=1;
		}
		printf("\n");
	}
		
	return;
}

//б����ʾ
void printmode3(){
 	for(j=0;j<16;j++){
 		for(p=0;p<16-j;p++)
 			printf(" "); //������ǲ���б���trick�� 
		m=0x80;
		for(i=0;i<8;i++){
			if(m&u[j*2]){
				printf("MM");
			}
			else{
				printf("  ");
			}
			m>>=1;
		}
		m=0x80;
		for(i=0;i<8;i++){
			if(m&u[j*2+1]){
				printf("MM");
			}
			else{
				printf("  ");
			}
			m>>=1;
		}
		printf("\n");
	}	
	
	return;
}

//������ʾ
void printmode4(){
 	for(j=0;j<16;j++){
 		m=0x01;
		for(i=0;i<8;i++){
			if(m&u[j*2+1]){
				printf("MM");
			}
			else{
				printf("  ");
			}
			m<<=1;
		}
		m=0x01;
		for(i=0;i<8;i++){
			if(m&u[j*2]){
				printf("MM");
			}
			else{
				printf("  ");
			}
			m<<=1;
		}
		printf("\n");
	}	
	
	return;
}

//������ʾ 
void printmode5(){
 	for(j=16;j>0;j--){
		m=0x80;
		for(i=0;i<8;i++){
			if(m&u[j*2]){
				printf("MM");
			}
			else{
				printf("  ");
			}
			m>>=1;
		}
		m=0x80;
		for(i=0;i<8;i++){
			if(m&u[j*2+1]){
				printf("MM");
			}
			else{
				printf("  ");
			}
			m>>=1;
		}
		printf("\n");
	}	
	
	return;
}

