/*
 *      jstring.c            by T.Aiba  1996/02
 */
#include<stdio.h>
#include<stdlib.h>

/* Main Routine */
unsigned char *jstrtok(unsigned char *s, const unsigned char *t);
unsigned char *jstrstr(const unsigned char *s, const unsigned char *t);

/* Sub Routines */
int jstrtok_sub_01(unsigned char *s, const unsigned char *t);
int is2byte(const unsigned char *s);

#define SJIS 0
#define EUC  1

static unsigned char *hold_pnt;
int                   Jcode = EUC;

unsigned char *jstrtok(unsigned char *s, const unsigned char *t)
{
	unsigned char *pnt, *pnt1;
	
	if(s) {
		pnt = s;
	} else {
		pnt = hold_pnt;
	}
	
	while(0 < jstrtok_sub_01(pnt,t)) {
		if(is2byte(pnt))	pnt += 2;
		else			pnt++;
	}
	if(0 == *pnt) {
/*		jstrtok_label_01: */
		hold_pnt = pnt;
		return(NULL);
	}
	
	pnt1 = pnt;
	while(0 == jstrtok_sub_01(pnt,t)) {
		if(is2byte(pnt))	pnt += 2;
		else			pnt++;
		if(0 == *pnt) {
			hold_pnt = pnt;
			return(pnt1);
		}
	}
	if(is2byte(pnt)) {
		hold_pnt = pnt + 2;
	} else {
		hold_pnt = pnt + 1;
	}
	*pnt = 0;
	return(pnt1);
}

unsigned char *jstrstr(const unsigned char *s, const unsigned char *t)
{
	unsigned char *sa, *sb, *ta, *tb;

	sb = (unsigned char *)s;
	tb = (unsigned char *)t;
  
	while(*sb) {
		for(sa = sb, ta = tb; *sa && *ta; sa++, ta++) {
			if(*sa != *ta) {
				break;
			}
		}
		if(0 == *ta) {
			return(sb);
		}
		sb++;
	}
	return(NULL);
}

int jstrtok_sub_01(unsigned char *s, const unsigned char *t)
{
	int flag;
	
	if(*s == 0) {
		return(-1);
	}
	flag = is2byte(s);
	while(1) {
		if(0 == *t) {
			return(0);
		}
		if(is2byte(t)) {
			if(flag) {
				if((s[0] == t[0]) && (s[1] == t[1])) {
					return(1);
				}
			}
			t += 2;
		} else {
			if(!flag) {
				if(s[0] == t[0]) {
					return(1);
				}
			}
			t++;
		}
	}
}

int is2byte(const unsigned char *s)
{
	if(Jcode == EUC) {
		if(0x80 <= s[0] && 0x80 <= s[1]) {
			return(1);
		}
	} else {
		if((((0x81 <= *s) && (*s <= 0x9f)) ||
		    ((0xe0 <= *s) && (*s <= 0xfc))) &&
		   (((0x40 <= s[1]) && (s[1] <= 0x7e)) ||
		    ((0x80 <= s[1]) && (s[1] <= 0xfc)))) {
			return(1);
		}
	}
	return(0);
}

