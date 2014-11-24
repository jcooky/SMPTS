#ifndef __INNER_TIME_H_
#define __INNER_TIME_H_

typedef struct InnerTimer {


    void (*getTime)(struct InnerTimer *self, char *buffer);

} InnerTimer;

InnerTimer* newInnerTimer();
void getTime(struct InnerTimer *self, char *buffer);

#endif // __INNER_TIME_H_