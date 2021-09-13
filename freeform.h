#pragma once

void    ptr(void);
void  store(void);
void  loadX(void);
void  loadY(void);
void    add(void);
void    mul(void);
void   done(void);

void interp(void(*program[])(void), void* args[], int i);
