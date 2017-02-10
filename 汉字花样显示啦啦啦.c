/*
说明：
1、采用拼音输入法 
2、支持ASCII码和汉字混合显示
3、可以选择显示字库和显示模式 
4、依赖文件为 "Hzk16.字库"，"mhzk16.字库"，和"汉字拼音五笔表.txt"放置于当前文件夹， 
若修改了文件路径，请修改程序中PATH1，PATH2的宏定义 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH1 "Hzk16.字库"
#define PATH2 "mhzk16.字库"
#define PATH3 "汉字拼音五笔表.txt"
#define MAX_LINE 512
#define MAX_PINYIN 10

//为了方便变量都用全局的了 
int q,w,adr;
int m,i,j,k,p;
char u[32];
short t[16];
char s[32];
FILE *fp;
char buf[MAX_LINE]; //缓冲区
char pinyin[MAX_PINYIN];
int len,flag,count,choice; //len为行字符个数
char *appear;

//拼音输入 
void pinyinInput();
//GB码及Hzk字库读取点阵 
void GBcode();
//GB码转ZB码及mhzk字库读取点阵
void GBtoZB();
//花式打印点阵函数 
void printmode1(); //正常显示
void printmode2(); //放大一倍 
void printmode3(); //斜体显示 
void printmode4(); //镜像显示 
void printmode5(); //倒立显示  

int main(){
	int ziku, mode, breakflag=0;
	
 	printf("请选择显示字库：\n1.Hzk16\n2.mhzk16\n");	
 	scanf("%d",&ziku);
	printf("请选择显示模式：\n1.正常显示\n\
2.放大一倍\n3.斜体显示\n4.镜像显示\n5.倒立显示\n");
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

//拼音输入法
void pinyinInput(){	
	printf("请输入拼音：\n");
	scanf("%s",pinyin);
	 
	fp=fopen(PATH3,"rb");
	flag=0;
	count=0;
	while(fgets(buf,MAX_LINE,fp) != NULL){
		len = strlen(buf);
		buf[len-1] = '\0';  //去掉换行符
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
		printf("输入拼音不存在\n");
	}else{
		printf("请选择输入汉字\n");
		scanf("%d",&choice);
		fp=fopen(PATH3,"rb");
		flag=0;
		count=0;
		while(fgets(buf,MAX_LINE,fp) != NULL){
			len = strlen(buf);
			buf[len-1] = '\0';  //去掉换行符
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

//GB码及Hzk字库读取点阵 
void GBcode(){ 
	q=(s[0]-0xA1)&0xFF;
	w=(s[1]-0xA1)&0xFF;
	adr=(q*94+w)*32;
	
	fp=fopen(PATH1,"rb");
	if(!fp){
		printf("文件无法打开\n");
		exit(1);
	}
	fseek(fp,adr,SEEK_SET);
	fread(u,1,32,fp); 
	fclose(fp);		
}
 
//国标GB到哲标ZB的转换及mhzk字库读取
void GBtoZB(){
	if((s[0]&0x80)==0){	//ASCii
		t[0]=t[0]&0x0F;
		t[0]=t[0]&(s[0]);
	} else {				//HanZi
		q =(s[0]-0xA1)&0xFF; 	//Qu
		w =(s[1]-0xA1)&0xFF; 		//Wei
		m=0x2000+q*94+w; 			//ZB码
		t[0]=((short)m);
	}
	
	//检测哪一类字符：符号、汉字、显示字符等。
	if((t[0]&0xE000)==0){ //ASCii、其它符号
		adr=adr; //字库中位置
	} else if((t[0]&0x8000)==0){ //汉字字符
		adr=(t[0]-0x2000+256)*32;	//前256个字符是ascii码
 	}

	//mhzk字库读取
	fp=fopen(PATH2,"rb");
	if(!fp){
		printf("文件无法打开2\n");
		exit(1);
	}
	fseek(fp,adr,SEEK_SET);
	fread(u,1,32,fp);
	fclose(fp);	
}

//正常显示
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

//放大一倍
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

//斜体显示
void printmode3(){
 	for(j=0;j<16;j++){
 		for(p=0;p<16-j;p++)
 			printf(" "); //这里就是产生斜体的trick啦 
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

//镜像显示
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

//倒立显示 
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

