
/* ===================
     <<nanba160.c>>
  
     Originally, written by BXA00031 at Nifty Serve
       for MS-DOS(LSI-C) in 1994/4/3
     for UNIX(gcc)  modified by T.AIBA 1996/02/15
     for UTF-8      modified by Sato Makoto 2012/07/01
         just nkf -w , and some comments.
   =================== */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <math.h>

#include "jstring.c"
#include <time.h>

#define WORDS_MAX 2500
#define KUTOTEN_LEN 41
#define KUGIRI " !?:;.,^~_/<>()[]{}\'\"\t\n　！？：；．，。、・\
…‥＾‾＿／＜＞（）［］「」〔〕｛｝〈〉《》『』【】“”‘’"

/* max file name size + 2 */
#define FNAME_SIZE_MAX 257

void port_cre();
void title_a();
void title_b();
void usage();
void stop(char *p);
void prt(char *p,char _dtype);
void cprt(char *p,char _dtype);
double rand1();
int rand100();
int set_val(int a,int l,int h);
char *strtail(char *p,int n);
char *get_strval(char *p);
char *jstrrem(char *s,char *t);
void open_infile();
void open_outfile();
void read_data();
int get_opt1(int n,char **p);
void get_opt2();
void sel(int wtype);
void inflect();


/*  内定値  */
int mn=1;		/*  一文の長さ(最小)  */
int mx=5;		/*  一文の長さ(最大)  */
int sn=6;		/*  一段落あたりの文の個数(1-32000)  */
int rc=2;		/*  同一語句の重複チェックをどの程度するか(0-10)  */
int sc=60;		/*  主語を省略する割合(0-100)  */
int sh=70;		/*  副詞句を省略する割合(0-100)  */
int jt=50;		/*  全述語のうち他動詞句の割合(0-100)  */
int sz=50;		/*  接続句で文と文を接続する割合(0-100)  */
int bm=50;		/*  文末句で文を終わらせる割合(0-100)  */
char kuten[KUTOTEN_LEN]="。";	/*  句点  */
char toten[KUTOTEN_LEN]="、";	/*  読点  */
int c_outs=0;	/* 出力する段落数(0=無限,or 1-32000) */

char *w[WORDS_MAX],buf[256],tmp[256];
char sz_b[5]; /* 接続助詞の重複チェック用 */
char jt_b[3][5],bm_b[5]; /* 文末句の述語語尾の重複チェック用 */
char infile[FNAME_SIZE_MAX]="",outfile[FNAME_SIZE_MAX]="";
unsigned char count[WORDS_MAX];
int wlow[8],whigh[8],wnum[8],wcheck[8],wmax;

char dtype; /* 文字列出力先(0=console,1=disk,2=console&disk) */
char nf=0; /* 画面表示用 */
char chkfo=0; /* ファイル出力用 */
char chkinf=0,chkoutf=0; /* オプション指定用 */

FILE *fp;



int main(int argc,char **argv)
{
	int a,slen,st;
	int chp=0; /* 段落分け用 */

	srand((unsigned)time(NULL));
	/*
	printf("\33[2J");
	*/
	dtype=0;
	title_a();

	if(argc>1){get_opt1(argc,argv);}

	open_infile();
	read_data();
	if(chkfo==0){
		open_outfile();
		dtype=1;
		title_b();
		prt("辞書ファイル名:",dtype); prt(infile,dtype); prt("\n\n",dtype);
	}

	if(nf==0 && chkfo==0) dtype=2;

mainloop:

	slen=fabs(mn+rand1()*(mx-mn+1));  /* 一文の長さ */
	if(slen<1)slen=1;
	st=1;  /* '文の先頭'を示す */

	for(a=1;a<=slen;a++){

	/*  副詞句その１  */
		if(rand100()>sh){
			sel(2);
			cprt(tmp,dtype);
		}

	/*  主語と助詞  */
		if(rand100()>sc || st==1){
			st=0;
			sel(0);
			cprt(tmp,dtype);
			sel(1);
			cprt(tmp,dtype);
		}

	/*  副詞句その２  */
		if(rand100()>sh){
			sel(2);
			cprt(tmp,dtype);
		}
	
	/*  述語節  */
		if(rand100()<jt) sel(3); /* 自動詞 */
		else{
			sel(0); /* 目的語 */
			cprt(tmp,dtype);
			sel(4); /* 他動詞 */
		}
	
	/*  文の接続または文末処理  */
		if(a<slen){
			if(rand100()<sz){
				cprt(tmp,dtype);
				sel(5);  /* 接続句 */
				cprt(tmp,dtype);
			}
			else{
				inflect(); /* 語形変化 */
				cprt(tmp,dtype);
			}
		}
		else {
			cprt(tmp,dtype);
			if(rand100()<bm){
				sel(6); /* 文末句 */
				cprt(tmp,dtype);
			}
			cprt(kuten,dtype); /* 句点 */
		}
	
	}

	/*  段落分け  */
	chp++;
	if(chp==sn){
		prt("\n\n",dtype);
		chp=0;
		if(nf==1) printf(".");

		if(c_outs!=0){
			c_outs--;
			if(c_outs<=0){goto end_of_main;}
		}
	}

	if(rc!=0){
		for(a=0;a<=wmax;a++){
			if(count[a]>0) count[a]--;
		}
	}

	/*
	if(kbhit()==0)
	*/
	  goto mainloop;

	/*
	getch();
	*/
	end_of_main:
		prt("\n\n",dtype);
	        /*
		fcloseall();
		*/
		exit(0);
}

void port_cre(){
        printf("\n"
	" Originally, written by BXA00031 at Nifty Serve\n"
	"   for MS-DOS(LSI-C) in 1994/4/3\n"
	"   for UNIX(gcc)  modified by T.AIBA 1996/02/15\n"
	"   for UTF-8      modified by Sato Makoto 2012/07/01\n"
	"     just nkf -w , and some comments.\n");
}

void title_a(){
	int a;
	for(a=1;a<=28;a++) prt("〜",dtype);
	prt("\n",dtype);
	prt("　　自動作文ソフト『難解バカボン』Ver.1.60    \n",dtype);
	port_cre();
	for(a=1;a<=28;a++) prt("〜",dtype);
	prt("\n",dtype);
}

void title_b(){
	int a;
	time_t tm;
	time(&tm);
	for(a=1;a<=37;a++) prt("〜",dtype);
	prt("\n",dtype);
	prt("　　自動作文ソフト『難解バカボン』Ver.1.60    ",dtype);
	prt(ctime(&tm),dtype);
	for(a=1;a<=37;a++) prt("〜",dtype);
	prt("\n",dtype);
}


void usage(){
	printf("command option:\n"
		" -i[filename]  辞書ファイルの指定     -n  画面表示を抑制\n"
		" -o[filename]  出力ファイルの指定     -v  ファイル出力しない\n"
		" -s[1-32000]   出力する段落数\n\n");
	printf("data option:\n"
		" -MN[1-100]    一文の長さ(最小)       -JT[0-100]   他動詞の割合\n"
		" -MX[1-100]    一文の長さ(最大)       -SZ[0-100]   接続句の割合\n"
		" -SN[1-32000]  一段落の文の個数       -BM[0-100]   文末句の割合\n"
		" -RC[0-10]     語句の重複チェック     -KT\"文字列\"  句点\n"
		" -SC[0-100]    主語を省略する割合     -TT\"文字列\"  読点\n"
		" -SH[0-100]    副詞句を省略する割合\n");
}


void stop(char *p){
	printf("%s\a\n",p);
	/*
	fcloseall();
	*/
	exit(1);
}


void prt(char *p,char _dtype){
	switch(_dtype){
	case 3:
		break;
	case 2:
		printf("%s",p);
	case 1:
		if(EOF == fputs(p,fp)){
			stop("\nError:ファイル出力がうまくいきませんf");
		}
		break;
	case 0:
		printf("%s",p);
		break;
	}
}


void cprt(char *p,char _dtype){
	switch(_dtype){
	case 3:
		break;
	case 2:
		printf("%s",p);
	case 1:
		if(EOF == fputs(p,fp)){
			stop("\nError:ファイル出力がうまくいきませんc");
		}
		break;
	case 0:
		printf("%s",p);
		break;
	}
}


double rand1(){
	return( (double)(rand()%0x10000)/0x10000 );
}


int rand100(){
	return((rand()%0x10000)%100);
}


int set_val(int a,int l,int h){
	if(a<l){return(l);}
	if(a>h){return(h);}
	return(a);
}


char *strtail(char *p,int n){
	static char ltmp[256];
	int a;

	if(strlen(p)>255)return(NULL);

	if(n>strlen(p)) n=strlen(p);
	for(a=0;a<=n-1;a++){
		ltmp[a]=*(p+strlen(p)-n+a);
	}
	ltmp[n]='\0';

	return(ltmp);
}


char *get_strval(char *p){
	static char ltmp[41];
	int a=0,b=0;

	if(strlen(p)>40) return(NULL);

	for(a=0;a<=strlen(p);a++){
		if(a>=strlen(p)) return(NULL);
		if(*(p+a)=='\"') break;
	}
	a++;

	for(;a<=strlen(p);a++){
		ltmp[b]=*(p+a);
		if(a>=strlen(p)) return(NULL);
		if(*(p+a)=='\"') break;
		b++;
	}

	ltmp[b]='\0';
	return(ltmp);
}


char *jstrrem(char *s,char *t){
	char *p;
	static char v[256]="";
	if(strlen(s)>255){return(NULL);}
	strcpy(v,p=jstrtok(s,t));
	while((p=jstrtok(NULL,t))!=NULL){
		strcat(v,p);
	}
	return(v);
}


void open_infile(){
	if(chkinf==0){
		printf("dict filename: ");
		fgets(infile, FNAME_SIZE_MAX, stdin);
		infile[strlen(infile)-1] = '\0';
	}
	if(infile[0]=='\0')  strcpy(infile,"nanba.nb1");
	else if(strchr(infile,'.')==NULL)  strcat(infile,".nb1");
	if((fp=fopen(infile,"r"))==NULL){
		stop("\nError:Cannot Open Dict File.");
	}
	printf("\n");
}


void open_outfile(){
	if(chkoutf==0){
		printf("output filename: ");
		fgets(outfile, FNAME_SIZE_MAX, stdin);
		outfile[strlen(outfile)-1] = '\0';
	}
	if(outfile[0]=='\0') strcpy(outfile,"nanba.txt");
	if((fp=fopen(outfile,"w"))==NULL){
		stop("\nError:Cannot Create Output File.");
	}
}


void read_data(){
	int a=0;

	wlow[0]=0;
	wmax=0;

	loop:
	fgets(buf,240,fp);
	if(strlen(buf)>200){buf[200]='\0';}

	switch(buf[0]){
	case'\n':
	case'\'':
		 goto loop;
	case'-':
	case'/':
		get_opt2();
		goto loop;
	case'@':
		whigh[a]=wmax-1;
		a++;
		if(a<=6){
			wlow[a]=wmax;
			goto loop;
		}
		else break;
	case'\0':
		stop("Error:辞書ファイルの項目数が足らないみたいです");
		exit(1);
	default:
		if((w[wmax]=(char *)malloc(strlen(buf)+1))==NULL){
			stop("Error:メモリが足らないか, データが大きすぎるようです");
		}
		buf[strlen(buf)-1]='\0';
		strcpy(w[wmax],buf);
		wmax++;
		if(wmax>=WORDS_MAX){
			stop("Error:データの個数が多すぎます");
		}
		goto loop;
	}
	fclose(fp);
	/*
	printf("%d個の語句を読み込みました\n\n",wmax);
	*/

	for(a=0;a<=6;a++){
		wnum[a]=whigh[a]-wlow[a]+1;
		wcheck[a]=wnum[a]*rc/10;
		if(wcheck[a]>255)wcheck[a]=255;
	}
}


int get_opt1(int n,char **p){
	int a;
	char t[81];

	for(a=1;a<=n-1;a++){
		strncpy(t,*(p+a),80);
		t[80]='\0';
		if(t[0]=='-' || t[0]=='/'){

			switch(t[1]){
			/*--------------------------------------*/
			case'n':	/* 画面表示を抑制 */
			case'N':
				if(strlen(t)>2)goto mukou;
				if(chkfo!=0)goto mukou;
				nf=1;
				break;

			/*--------------------------------------*/
			case'v':
			case'V':	/* ファイル出力をしない */
				if(strlen(t)>2)goto mukou;
				if(nf==1)goto mukou;
				if(strlen(outfile)!=0)goto mukou;
				chkfo=1;
				break;

			/*--------------------------------------*/
			case'i':	/* 入力ファイル名の指定 */
			case'I':
				if(strlen(infile)>0)goto mukou;
				if(strlen(t)>2){
					strcpy(infile,strtail(t,strlen(t)-2));
					chkinf=1;
					break;
				}
				else if(a<n-1){
					char _tmp[2];
					strncpy(_tmp,*(p+a+1),1);
					if(_tmp[0]!='-'){
						strncpy(infile,*(p+a+1),80);
						chkinf=1;
						a++;
						break;
					}
					else goto mukou;
				}
				else goto mukou;
				break;

			/*--------------------------------------*/
			case'o':	/* 出力ファイル名の指定 */
			case'O':
				if(strlen(outfile)>0)goto mukou;
				if(chkfo!=0)goto mukou;
				if(strlen(t)>2){
					strcpy(outfile,strtail(t,strlen(t)-2));
					chkoutf=1;
					break;
				}
				else if(a<n-1){
					char _tmp[2];
					strncpy(_tmp,*(p+a+1),1);
					if(_tmp[0]!='-'){
						strncpy(outfile,*(p+a+1),80);
						chkoutf=1;
						a++;
						break;
					}
					else goto mukou;
				}
				else goto mukou;
				break;

			/*--------------------------------------*/
			case's':	/* 出力する段落数 */
			case'S':
				if(c_outs!=0)goto mukou;
				if(strlen(t)>2){
					char _tmp[80];
					strcpy(_tmp,strtail(t,strlen(t)-2));
					c_outs=set_val(atoi(_tmp),1,32000);
					break;
				}
				else if(a<n-1){
					char _tmp[80];
					strncpy(_tmp,*(p+a+1),1);
					if(_tmp[0]!='-' && _tmp[0]!='/'){
						strncpy(_tmp,*(p+a+1),80);
						c_outs=set_val(atoi(_tmp),1,32000);
						a++;
						break;
					}
					else goto mukou;
				}
				else goto mukou;
				break;

			/*--------------------------------------*/
			case'h':	/* ヘルプ表示 */
			case'H':
			case'?':
				usage();
				exit(0);

			/*--------------------------------------*/
			default:
				goto mukou;
			}/*switch*/

		}/*if*/
		else{
			mukou:
			printf("Warning:下記オプションは無効です\n");
			printf("%s\n\n",t);
		}

	}
	return 0; 
}


void get_opt2(){
	int c;
	char _tmp[3];

	if(strlen(buf)<3){goto mukou;}

	_tmp[0]=tolower(buf[1]),_tmp[1]=tolower(buf[2]),_tmp[2]='\0';
	c=atoi(&buf[3]);

	if(strcmp(_tmp,"mn")==0 && c>=0){mn=set_val(c,1,100);return;}
	else if(strcmp(_tmp,"mx")==0 && c>=0){mx=set_val(c,1,100);return;}
	else if(strcmp(_tmp,"sn")==0 && c>=0){sn=set_val(c,1,32000);return;}
	else if(strcmp(_tmp,"rc")==0 && c>=0){rc=set_val(c,0,10);return;}
	else if(strcmp(_tmp,"bm")==0 && c>=0){bm=set_val(c,0,100);return;}
	else if(strcmp(_tmp,"sc")==0 && c>=0){sc=set_val(c,0,100);return;}
	else if(strcmp(_tmp,"sh")==0 && c>=0){sh=set_val(c,0,100);return;}
	else if(strcmp(_tmp,"sz")==0 && c>=0){sz=set_val(c,0,100);return;}
	else if(strcmp(_tmp,"jt")==0 && c>=0){jt=set_val(c,0,100);return;}
	else if(strcmp(_tmp,"kt")==0){
		char *s;
		s=get_strval(buf);
		if(s!=NULL){strcpy(kuten,s);return;}
	}
	else if(strcmp(_tmp,"tt")==0){
		char *s;
		s=get_strval(buf);
		if(s!=NULL){strcpy(toten,s);return;}
	}

	mukou:
		printf("Warning:下記オプションは無効です(%s)\n",infile);
		printf("%s\n",buf);
}


void sel(int wtype){
	int a;
	char sz_count=15,jt_count=15,fm_count=15;

	strcpy(buf,tmp);

	loop:
	a=wlow[wtype]+wnum[wtype]*rand1();
	if(count[a]>0) {
		count[a]--;
		goto loop;
	}

	/*--------------------------------------*/
	/* 例外処理 */
	if(strlen(w[a])>=4 && strcmp(strtail(buf,2),"い")==0){
		char _left[5];
		strncpy(_left,w[a],4);
		_left[4]='\0';

		/* (形容詞+「べし」は不可) */
		if(strcmp(_left,"べか")==0) goto loop;
		if(strcmp(_left,"べき")==0) goto loop;
		if(strcmp(_left,"べく")==0) goto loop;
		if(strcmp(_left,"べし")==0) goto loop;

		/* (形容詞+「しか」は不可) */
		if(strcmp(_left,"しか")==0) goto loop;

		/* (形容詞+「ため」は不可) */
		if(strcmp(_left,"ため")==0) goto loop;

		/* (形容詞+「には」は不可) */
		if(strcmp(_left,"には")==0) goto loop;

		/* (形容詞+「まで」は不可) */
		if(strcmp(_left,"まで")==0) goto loop;

	}

	/*--------------------------------------*/
	/* 接続助詞の重複チェック */
	if(sz_count>0 && wtype==5){
		char _tmp[256];
		strcpy(_tmp,w[a]);
		strcpy(_tmp,jstrrem(_tmp,toten));
		strcpy(_tmp,jstrrem(_tmp,KUGIRI));
		strcpy(_tmp,strtail(_tmp,4));
		if(strcmp(_tmp,sz_b)==0){
			sz_count--;
			goto loop;
		}
		strcpy(sz_b,_tmp);
	}

	/*--------------------------------------*/
	/* 文末句の述語語尾の重複チェック */
	if(jt_count>0 && wtype==6){
		int c;
		char _tmp[5];
		strcpy(_tmp,strtail(w[a],4));
		if(strcmp(_tmp,bm_b)==0){
			jt_count--;
			goto loop;
		}
		for(c=0;c<=2;c++){
			if(strcmp(_tmp,jt_b[c])==0){
				jt_count--;
				goto loop;
			}
		}
		strcpy(bm_b,_tmp);
	}

	if(wtype==3 || wtype==4){
		int c;
		for(c=0;c<=1;c++){strcpy(jt_b[c],jt_b[c+1]);}
		strcpy(jt_b[2],strtail(w[a],4));
	}

	/*--------------------------------------*/
	/* 直前の語句との比較による重複チェック */
	if(fm_count>0){
		if(jstrstr(buf,"であ")!=NULL && jstrstr(w[a],"であ")!=NULL){
			fm_count--; goto loop;}

		if(jstrstr(buf,"され")!=NULL && jstrstr(w[a],"され")!=NULL){
			fm_count--; goto loop;}

		if(jstrstr(buf,"とい")!=NULL && jstrstr(w[a],"とい")!=NULL){
			fm_count--; goto loop;}

		if(jstrstr(buf,"と言")!=NULL && jstrstr(w[a],"と言")!=NULL){
			fm_count--; goto loop;}

		if(jstrstr(buf,"でき")!=NULL && jstrstr(w[a],"でき")!=NULL){
			fm_count--; goto loop;}

		if(jstrstr(buf,"もの")!=NULL && jstrstr(w[a],"もの")!=NULL){
			fm_count--; goto loop;}
	}

	/*--------------------------------------*/

	strcpy(tmp,w[a]);
	count[a]=wcheck[wtype];

}


void inflect(){
	int f=0;
	char _left[256],_right[21];

	if(strlen(tmp)>6){
		strcpy(_left,tmp);
		_left[strlen(tmp)-6]='\0';
		strcpy(_right,strtail(tmp,6));

		if(strcmp(_right,"しない")==0) strcat(_left,"せず"),f=1;
		else if(strcmp(_right,"はない")==0) strcat(_left,"はなく"),f=1;
		else if(strcmp(_right,"がない")==0) strcat(_left,"がなく"),f=1;
		else if(strcmp(_right,"にない")==0) strcat(_left,"になく"),f=1;
		else if(strcmp(_right,"ている")==0) strcat(_left,"ており"),f=1;
		else if(strcmp(_right,"でいる")==0) strcat(_left,"でおり"),f=1;
	}

	if(f==0 && strlen(tmp)>4){
		strcpy(_left,tmp);
		_left[strlen(tmp)-4]='\0';
		strcpy(_right,strtail(tmp,4));

		if(strcmp(_right,"する")==0) strcat(_left,"し"),f=1;
		else if(strcmp(_right,"ずる")==0) strcat(_left,"じ"),f=1;
		else if(strcmp(_right,"ある")==0) strcat(_left,"あり"),f=1;
		else if(strcmp(_right,"える")==0) strcat(_left,"え"),f=1;
		else if(strcmp(_right,"げる")==0) strcat(_left,"げ"),f=1;
		else if(strcmp(_right,"ぜる")==0) strcat(_left,"ぜ"),f=1;
		else if(strcmp(_right,"める")==0) strcat(_left,"め"),f=1;
		else if(strcmp(_right,"れる")==0) strcat(_left,"れ"),f=1;
		else if(strcmp(_right,"言う")==0) strcat(_left,"言い"),f=1;
		else if(strcmp(_right,"いう")==0) strcat(_left,"いい"),f=1;
		else if(strcmp(_right,"入る")==0) strcat(_left,"入り"),f=1;
		else if(strcmp(_right,"見る")==0) strcat(_left,"見て"),f=1;
		else if(strcmp(_right,"出る")==0) strcat(_left,"出て"),f=1;
		else if(strcmp(_right,"得る")==0) strcat(_left,"得て"),f=1;
		else if(strcmp(_right,"しい")==0) strcat(_left,"しく"),f=1;
		else if(strcmp(_right,"あう")==0) strcat(_left,"あい"),f=1;
		else if(strcmp(_right,"合う")==0) strcat(_left,"合い"),f=1;
	}

	if(f==0 && strlen(tmp)>2){
		strcpy(_left,tmp);
		_left[strlen(tmp)-2]='\0';
		strcpy(_right,strtail(tmp,2));

		if(strcmp(_right,"だ")==0) strcat(_left,"で"),f=1;
		else if(strcmp(_right,"く")==0) strcat(_left,"き"),f=1;
		else if(strcmp(_right,"つ")==0) strcat(_left,"ち"),f=1;
		else if(strcmp(_right,"む")==0) strcat(_left,"み"),f=1;
		else if(strcmp(_right,"ぐ")==0) strcat(_left,"ぎ"),f=1;
		else if(strcmp(_right,"ぶ")==0) strcat(_left,"び"),f=1;
	}
	
	if(f==1) strcpy(tmp,_left);
	else strcat(tmp,"し");

	strcat(tmp,toten);
}


/* 読みにくいファイルで申し訳ありません(^^;)。ちなみにこのソフトの
   作者はすでにＣ言語の使い方をほとんど忘れてしまったので、技術的な
   面についてのご意見ご要望にはお答えできません(^^;)               */

