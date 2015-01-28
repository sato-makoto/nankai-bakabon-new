
/* ===================
     <<nanba160.c>>
   =================== */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <jstring.h>
#include <time.h>
#include <conio.h>

#define WORDS_MAX 2500
#define KUTOTEN_LEN 41
#define KUGIRI " !?:;.,^~_/<>()[]{}�����\'\"\t\n�@�I�H�F�G�D�C�B�A�E\
�c�d�O�P�Q�^�����i�j�m�n�u�v�k�l�o�p�q�r�s�t�w�x�y�z�g�h�e�f"

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


/*  ����l  */
int mn=1;		/*  �ꕶ�̒���(�ŏ�)  */
int mx=5;		/*  �ꕶ�̒���(�ő�)  */
int sn=6;		/*  ��i��������̕��̌�(1-32000)  */
int rc=2;		/*  ������̏d���`�F�b�N���ǂ̒��x���邩(0-10)  */
int sc=60;		/*  �����ȗ����銄��(0-100)  */
int sh=70;		/*  ��������ȗ����銄��(0-100)  */
int jt=50;		/*  �S�q��̂�����������̊���(0-100)  */
int sz=50;		/*  �ڑ���ŕ��ƕ���ڑ����銄��(0-100)  */
int bm=50;		/*  ������ŕ����I��点�銄��(0-100)  */
char kuten[KUTOTEN_LEN]="�B";	/*  ��_  */
char toten[KUTOTEN_LEN]="�A";	/*  �Ǔ_  */
int c_outs=0;	/* �o�͂���i����(0=����,or 1-32000) */

char *w[WORDS_MAX],buf[256],tmp[256];
char sz_b[5]; /* �ڑ������̏d���`�F�b�N�p */
char jt_b[3][5],bm_b[5]; /* ������̏q�����̏d���`�F�b�N�p */
char infile[81]="",outfile[81]="";
unsigned char count[WORDS_MAX];
int wlow[8],whigh[8],wnum[8],wcheck[8],wmax;

char dtype; /* ������o�͐�(0=console,1=disk,2=console&disk) */
char nf=0; /* ��ʕ\���p */
char chkfo=0; /* �t�@�C���o�͗p */
char chkinf=0,chkoutf=0; /* �I�v�V�����w��p */

FILE *fp;



main(int argc,char **argv)
{
	int a,slen,st;
	int chp=0; /* �i�������p */

	srand((unsigned)time(NULL));

	printf("\33[2J");
	dtype=0;
	title_a();

	if(argc>1){get_opt1(argc,argv);}

	open_infile();
	read_data();
	if(chkfo==0){
		open_outfile();
		dtype=1;
		title_b();
		prt("�����t�@�C����:",dtype); prt(infile,dtype); prt("\n\n",dtype);
	}

	if(nf==0 && chkfo==0) dtype=2;

mainloop:

	slen=abs(mn+rand1()*(mx-mn+1));  /* �ꕶ�̒��� */
	if(slen<1)slen=1;
	st=1;  /* '���̐擪'������ */

	for(a=1;a<=slen;a++){

	/*  �����傻�̂P  */
		if(rand100()>sh){
			sel(2);
			cprt(tmp,dtype);
		}

	/*  ���Ə���  */
		if(rand100()>sc || st==1){
			st=0;
			sel(0);
			cprt(tmp,dtype);
			sel(1);
			cprt(tmp,dtype);
		}

	/*  �����傻�̂Q  */
		if(rand100()>sh){
			sel(2);
			cprt(tmp,dtype);
		}
	
	/*  �q���  */
		if(rand100()<jt) sel(3); /* ������ */
		else{
			sel(0); /* �ړI�� */
			cprt(tmp,dtype);
			sel(4); /* ������ */
		}
	
	/*  ���̐ڑ��܂��͕�������  */
		if(a<slen){
			if(rand100()<sz){
				cprt(tmp,dtype);
				sel(5);  /* �ڑ��� */
				cprt(tmp,dtype);
			}
			else{
				inflect(); /* ��`�ω� */
				cprt(tmp,dtype);
			}
		}
		else {
			cprt(tmp,dtype);
			if(rand100()<bm){
				sel(6); /* ������ */
				cprt(tmp,dtype);
			}
			cprt(kuten,dtype); /* ��_ */
		}
	
	}

	/*  �i������  */
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



	if(kbhit()==0)goto mainloop;
	getch();

	end_of_main:
		prt("\n\n",dtype);
		fcloseall();
		exit(0);
}


void title_a(){
	int a;
	for(a=1;a<=23;a++) prt("�`",dtype);
	prt("\n",dtype);
	prt("�@�@�����앶�\�t�g�w����o�J�{���xVer.1.60    \n",dtype);
	for(a=1;a<=23;a++) prt("�`",dtype);
	prt("\n",dtype);
}

void title_b(){
	int a;
	time_t tm;
	time(&tm);
	for(a=1;a<=37;a++) prt("�`",dtype);
	prt("\n",dtype);
	prt("�@�@�����앶�\�t�g�w����o�J�{���xVer.1.60    ",dtype);
	prt(ctime(&tm),dtype);
	for(a=1;a<=37;a++) prt("�`",dtype);
	prt("\n",dtype);
}


void usage(){
	printf("command option:\n"
		" -i[filename]  �����t�@�C���̎w��     -n  ��ʕ\����}��\n"
		" -o[filename]  �o�̓t�@�C���̎w��     -v  �t�@�C���o�͂��Ȃ�\n"
		" -s[1-32000]   �o�͂���i����\n\n");
	printf("data option:\n"
		" -MN[1-100]    �ꕶ�̒���(�ŏ�)       -JT[0-100]   �������̊���\n"
		" -MX[1-100]    �ꕶ�̒���(�ő�)       -SZ[0-100]   �ڑ���̊���\n"
		" -SN[1-32000]  ��i���̕��̌�       -BM[0-100]   ������̊���\n"
		" -RC[0-10]     ���̏d���`�F�b�N     -KT\"������\"  ��_\n"
		" -SC[0-100]    �����ȗ����銄��     -TT\"������\"  �Ǔ_\n"
		" -SH[0-100]    ��������ȗ����銄��\n");
}


void stop(char *p){
	printf("%s\a\n",p);
	fcloseall();
	exit(1);
}


void prt(char *p,char _dtype){
	switch(_dtype){
	case 3:
		break;
	case 2:
		printf("%s",p);
	case 1:
		if(fputs(p,fp)!=0){
			stop("\nError:�t�@�C���o�͂����܂������܂���");
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
		cprintf("%s",p);
	case 1:
		if(fputs(p,fp)!=0){
			stop("\nError:�t�@�C���o�͂����܂������܂���");
		}
		break;
	case 0:
		cprintf("%s",p);
		break;
	}
}


double rand1(){
	return( (double)rand()/32768 );
}


int rand100(){
	return( (int) ( (double)rand()/327.68 ) );
}


int set_val(int a,int l,int h){
	if(a<l){return(l);}
	if(a>h){return(h);}
	return(a);
}


char *strtail(char *p,int n){
	char tmp[256];
	int a;

	if(strlen(p)>255)return(NULL);

	if(n>strlen(p)) n=strlen(p);
	for(a=0;a<=n-1;a++){
		tmp[a]=*(p+strlen(p)-n+a);
	}
	tmp[n]='\0';

	return(tmp);
}


char *get_strval(char *p){
	char tmp[41];
	int a=0,b=0;

	if(strlen(p)>40) return(NULL);

	for(a=0;a<=strlen(p);a++){
		if(a>=strlen(p)) return(NULL);
		if(*(p+a)=='\"') break;
	}
	a++;

	for(;a<=strlen(p);a++){
		tmp[b]=*(p+a);
		if(a>=strlen(p)) return(NULL);
		if(*(p+a)=='\"') break;
		b++;
	}

	tmp[b]='\0';
	return(tmp);
}


char *jstrrem(char *s,char *t){
	char *p,v[256]="";
	if(strlen(s)>255){return(NULL);}
	strcpy(v,p=jstrtok(s,t));
	while((p=jstrtok(NULL,t))!=NULL){
		strcat(v,p);
	}
	return(v);
}


void open_infile(){
	if(chkinf==0){
		printf("�����t�@�C������");
		gets(infile);
	}
	if(infile[0]=='\0')  strcpy(infile,"nanba.nb1");
	else if(strchr(infile,'.')==NULL)  strcat(infile,".nb1");
	if((fp=fopen(infile,"r"))==NULL){
		stop("\nError:�����t�@�C����������܂���");
	}
	printf("\n");
}


void open_outfile(){
	if(chkoutf==0){
		printf("�o�̓t�@�C������");
		gets(outfile);
	}
	if(outfile[0]=='\0') strcpy(outfile,"nanba.txt");
	if((fp=fopen(outfile,"a"))==NULL){
		stop("\nError:�o�̓t�@�C�������܂���");
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
		stop("Error:�����t�@�C���̍��ڐ�������Ȃ��݂����ł�");
		exit(1);
	default:
		if((w[wmax]=(char *)malloc(strlen(buf)+1))==NULL){
			stop("Error:������������Ȃ���, �f�[�^���傫������悤�ł�");
		}
		buf[strlen(buf)-1]='\0';
		strcpy(w[wmax],buf);
		wmax++;
		if(wmax>=WORDS_MAX){
			stop("Error:�f�[�^�̌����������܂�");
		}
		goto loop;
	}
	fclose(fp);
	printf("%d�̌���ǂݍ��݂܂���\n\n",wmax);

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
			case'n':	/* ��ʕ\����}�� */
			case'N':
				if(strlen(t)>2)goto mukou;
				if(chkfo!=0)goto mukou;
				nf=1;
				break;

			/*--------------------------------------*/
			case'v':
			case'V':	/* �t�@�C���o�͂����Ȃ� */
				if(strlen(t)>2)goto mukou;
				if(nf==1)goto mukou;
				if(strlen(outfile)!=0)goto mukou;
				chkfo=1;
				break;

			/*--------------------------------------*/
			case'i':	/* ���̓t�@�C�����̎w�� */
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
					if(_tmp[0]!='-' && _tmp[0]!='/'){
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
			case'o':	/* �o�̓t�@�C�����̎w�� */
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
					if(_tmp[0]!='-' && _tmp[0]!='/'){
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
			case's':	/* �o�͂���i���� */
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
			case'h':	/* �w���v�\�� */
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
			printf("Warning:���L�I�v�V�����͖����ł�\n");
			printf("%s\n\n",t);
		}

	}
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
		printf("Warning:���L�I�v�V�����͖����ł�(%s)\n",infile);
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
	/* ��O���� */
	if(strlen(w[a])>=4 && strcmp(strtail(buf,2),"��")==0){
		char _left[5];
		strncpy(_left,w[a],4);
		_left[4]='\0';

		/* (�`�e��+��ׂ���͕s��) */
		if(strcmp(_left,"�ׂ�")==0) goto loop;
		if(strcmp(_left,"�ׂ�")==0) goto loop;
		if(strcmp(_left,"�ׂ�")==0) goto loop;
		if(strcmp(_left,"�ׂ�")==0) goto loop;

		/* (�`�e��+�������͕s��) */
		if(strcmp(_left,"����")==0) goto loop;

		/* (�`�e��+����ߣ�͕s��) */
		if(strcmp(_left,"����")==0) goto loop;

		/* (�`�e��+��ɂͣ�͕s��) */
		if(strcmp(_left,"�ɂ�")==0) goto loop;

		/* (�`�e��+��܂ţ�͕s��) */
		if(strcmp(_left,"�܂�")==0) goto loop;

	}

	/*--------------------------------------*/
	/* �ڑ������̏d���`�F�b�N */
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
	/* ������̏q�����̏d���`�F�b�N */
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
	/* ���O�̌��Ƃ̔�r�ɂ��d���`�F�b�N */
	if(fm_count>0){
		if(jstrstr(buf,"�ł�")!=NULL && jstrstr(w[a],"�ł�")!=NULL){
			fm_count--; goto loop;}

		if(jstrstr(buf,"����")!=NULL && jstrstr(w[a],"����")!=NULL){
			fm_count--; goto loop;}

		if(jstrstr(buf,"�Ƃ�")!=NULL && jstrstr(w[a],"�Ƃ�")!=NULL){
			fm_count--; goto loop;}

		if(jstrstr(buf,"�ƌ�")!=NULL && jstrstr(w[a],"�ƌ�")!=NULL){
			fm_count--; goto loop;}

		if(jstrstr(buf,"�ł�")!=NULL && jstrstr(w[a],"�ł�")!=NULL){
			fm_count--; goto loop;}

		if(jstrstr(buf,"����")!=NULL && jstrstr(w[a],"����")!=NULL){
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

		if(strcmp(_right,"���Ȃ�")==0) strcat(_left,"����"),f=1;
		else if(strcmp(_right,"�͂Ȃ�")==0) strcat(_left,"�͂Ȃ�"),f=1;
		else if(strcmp(_right,"���Ȃ�")==0) strcat(_left,"���Ȃ�"),f=1;
		else if(strcmp(_right,"�ɂȂ�")==0) strcat(_left,"�ɂȂ�"),f=1;
		else if(strcmp(_right,"�Ă���")==0) strcat(_left,"�Ă���"),f=1;
		else if(strcmp(_right,"�ł���")==0) strcat(_left,"�ł���"),f=1;
	}

	if(f==0 && strlen(tmp)>4){
		strcpy(_left,tmp);
		_left[strlen(tmp)-4]='\0';
		strcpy(_right,strtail(tmp,4));

		if(strcmp(_right,"����")==0) strcat(_left,"��"),f=1;
		else if(strcmp(_right,"����")==0) strcat(_left,"��"),f=1;
		else if(strcmp(_right,"����")==0) strcat(_left,"����"),f=1;
		else if(strcmp(_right,"����")==0) strcat(_left,"��"),f=1;
		else if(strcmp(_right,"����")==0) strcat(_left,"��"),f=1;
		else if(strcmp(_right,"����")==0) strcat(_left,"��"),f=1;
		else if(strcmp(_right,"�߂�")==0) strcat(_left,"��"),f=1;
		else if(strcmp(_right,"���")==0) strcat(_left,"��"),f=1;
		else if(strcmp(_right,"����")==0) strcat(_left,"����"),f=1;
		else if(strcmp(_right,"����")==0) strcat(_left,"����"),f=1;
		else if(strcmp(_right,"����")==0) strcat(_left,"����"),f=1;
		else if(strcmp(_right,"����")==0) strcat(_left,"����"),f=1;
		else if(strcmp(_right,"�o��")==0) strcat(_left,"�o��"),f=1;
		else if(strcmp(_right,"����")==0) strcat(_left,"����"),f=1;
		else if(strcmp(_right,"����")==0) strcat(_left,"����"),f=1;
		else if(strcmp(_right,"����")==0) strcat(_left,"����"),f=1;
		else if(strcmp(_right,"����")==0) strcat(_left,"����"),f=1;
	}

	if(f==0 && strlen(tmp)>2){
		strcpy(_left,tmp);
		_left[strlen(tmp)-2]='\0';
		strcpy(_right,strtail(tmp,2));

		if(strcmp(_right,"��")==0) strcat(_left,"��"),f=1;
		else if(strcmp(_right,"��")==0) strcat(_left,"��"),f=1;
		else if(strcmp(_right,"��")==0) strcat(_left,"��"),f=1;
		else if(strcmp(_right,"��")==0) strcat(_left,"��"),f=1;
		else if(strcmp(_right,"��")==0) strcat(_left,"��"),f=1;
		else if(strcmp(_right,"��")==0) strcat(_left,"��"),f=1;
	}
	
	if(f==1) strcpy(tmp,_left);
	else strcat(tmp,"��");

	strcat(tmp,toten);
}


/* �ǂ݂ɂ����t�@�C���Ő\���󂠂�܂���(^^;)�B���Ȃ݂ɂ��̃\�t�g��
   ��҂͂��łɂb����̎g�������قƂ�ǖY��Ă��܂����̂ŁA�Z�p�I��
   �ʂɂ��Ă̂��ӌ����v�]�ɂ͂������ł��܂���(^^;)               */

