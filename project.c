
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// MACROS
#define INT_MIN -32000 
#define BOXSIZE 50 // change to change plot size



// Structs
struct Equation{
    float a,b,c;
};

struct Point{
    float x,y;
};

int pointCompare(struct Point p1, struct Point p2){
    if (abs(p1.x - p2.x) < 0.001 && abs(p1.y - p2.y) > -0.001) return 1;
    return 0;
}

int EquationCompare(struct Equation e1, struct Equation e2){
    if ((e1.a*e2.b - e1.b*e2.a) <= 0.001 && (e1.a*e2.b - e1.b*e2.a) >= -0.001){
        if (abs(e1.c / e2.c) - (e1.a / e2.a) < 0.001 && abs(e1.c / e2.c) - (e1.a / e2.a) > -0.001){
            return 0;
        }
        else{
            return -1;
        }
    }
    return 1;
}

struct Point* substitutionMethod (struct Equation e1,struct Equation e2){
    if (EquationCompare(e1,e2) == 0){
        printf("Infinite Solutions\n");
        return NULL;
    }
    else if (EquationCompare(e1,e2) == -1){
        printf("No Solutions\n");
        return NULL;
    }
    float xIntercept, yIntercept;
    yIntercept = (e2.c*e1.a - e2.a*e1.c)/(e2.b*e1.a - e2.a*e1.b);
    if (e1.a == 0){
        xIntercept = (e2.c - e2.b*yIntercept)/e2.a;
    }
    else {
        xIntercept = (e1.c - e1.b*yIntercept)/e1.a;
    }
    struct Point *result = (struct Point*)malloc(sizeof(struct Point));
    result->x = xIntercept;
    result->y = yIntercept;
    return result;
}

struct Point* matrixMethod (struct Equation e1,struct Equation e2){
    float derterminent = e1.a*e2.b - e1.b*e2.a;
    printf("Determinant: %f\n",derterminent);
    if (EquationCompare(e1,e2) == 0){
        printf("Infinite Solutions\n");
        return NULL;
    }
    else if (EquationCompare(e1,e2) == -1){
        printf("No Solutions\n");
        return NULL;
    }
    struct Equation inverse1,inverse2;
    inverse1.a = e2.b/derterminent;
    inverse1.b = -e1.b/derterminent;
    inverse2.a = -e2.a/derterminent;
    inverse2.b = e1.a/derterminent;

    printf("Inverse: \n");
    printf("%f %f\n%f %f\n",inverse1.a,inverse1.b,inverse2.a,inverse2.b);

    struct Point *result = (struct Point*)malloc(sizeof(struct Point));
    result->x = inverse1.a*e1.c + inverse1.b*e2.c;
    result->y = inverse2.a*e1.c + inverse2.b*e2.c;
    return result;
}

void plot(struct Equation e1,struct Equation e2,struct Point sol){
    int X1[BOXSIZE];
    int X2[BOXSIZE];
    int Y[BOXSIZE];
    int avg1=0,avg2=0;
    int slopeFlag = 0;
    if (e1.a == 0 && e2.b == 0){
        for (int i=0;i<BOXSIZE+2;++i){
            printf("*");
        }
        printf("\n");
        for (int i=0;i<BOXSIZE;++i){
            if (i == BOXSIZE/2){
                printf("*");
                for (int i=0;i<BOXSIZE;++i){
                    printf("1");
                }
                printf("*\n");
                continue;
            }
            printf("*");
            printf("%*c",BOXSIZE/2,'2');
            printf("%*c\n",BOXSIZE/2,'*');
        }
        for (int i=0;i<BOXSIZE+2;++i){
            printf("*");
        }   
        printf("\n"); 
        return;
    }
    else if (e2.a == 0 && e1.b == 0){
       for (int i=0;i<BOXSIZE+2;++i){
            printf("*");
        }
        printf("\n");
        for (int i=0;i<BOXSIZE;++i){
            if (i == BOXSIZE/2){
                printf("*");
                for (int i=0;i<BOXSIZE;++i){
                    printf("2");
                }
                printf("*\n");
                continue;
            }
            printf("*");
            printf("%*c",BOXSIZE/2,'1');
            printf("%*c\n",BOXSIZE/2,'*');
        }
        for (int i=0;i<BOXSIZE+2;++i){
            printf("*");
        }  
        printf("\n");  
        return;
    }
    else{
        for(int y=sol.y-BOXSIZE/2, i=0;y<sol.y+BOXSIZE/2;++y,++i){
            Y[i] = y;
            X1[i] = (e1.c - e1.b*y)/(e1.a + 0.001);
            X2[i] = (e2.c - e2.b*y)/(e2.a + 0.001);
            avg1 += X1[i];
            avg2 += X2[i];
        }
    }
    avg1 /= 50;
    avg2 /= 50;
    int avg = (avg1+avg2)/2;
    for (int i=0;i<BOXSIZE+2;++i){
        printf("*");
    }
    printf("\n");
    for (int i=0;i<BOXSIZE;++i){
        printf("*");
        X1[i] += avg+25;
        X2[i] += avg+25;
        if ((X1[i] < 0 || X1[i] > BOXSIZE) && (X2[i] < 0 || X2[i] > BOXSIZE)){
            printf("%*c\n",BOXSIZE,'*');
        }
        else if (X1[i] < 0 || X1[i] > BOXSIZE){
            printf("%*c",X2[i],'2');
            printf("%*c\n",BOXSIZE-X2[i],'*');
        }
        else if (X2[i] < 0 || X2[i] > BOXSIZE){
            printf("%*c",X1[i],'1');
            printf("%*c\n",BOXSIZE-X1[i],'*');
        }
        else{
            if (abs(X1[i] - X2[i]) < 0.001){
                printf("%*c",X1[i],'0');
                printf("%*c Y:%.3f\n",BOXSIZE-X1[i],'*',sol.y);
            }
            else if (X1[i] < X2[i]){
                printf("%*c",X1[i],'1');
                printf("%*c",X2[i]-X1[i],'2');
                printf("%*c\n",BOXSIZE-X2[i],'*');
            }
            else if (X1[i] > X2[i]){
                printf("%*c",X2[i],'2');
                printf("%*c",X1[i]-X2[i],'1');
                printf("%*c\n",BOXSIZE-X1[i],'*');
            }
        }
    }
    for (int i=0;i<BOXSIZE+2;++i){
        printf("*");
    }
}

int main()
{
    struct Equation e1,e2;
    printf("Eq1: ");
    scanf("%f %f %f",&e1.a,&e1.b,&e1.c);
    printf("Eq2: ");
    scanf("%f %f %f",&e2.a,&e2.b,&e2.c);
    struct Point *res1 = matrixMethod(e1,e2);
    if (res1 != NULL){ 
        printf("Matrix method: X: %f Y: %f\n",res1->x,res1->y);
    }
    struct Point *res2 = substitutionMethod(e1,e2);
    if (res2 != NULL){
        printf("Substitution method: X: %f Y: %f\n",res2->x,res2->y);
    }
    if (res1 != NULL && res2 != NULL){
        printf(pointCompare(*res1,*res2) ? "Consistant\n" : "Inconsistant\n");
        if (res1 != NULL){
            plot(e1,e2,*res1);
        }
    }
}
