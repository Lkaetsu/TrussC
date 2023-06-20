#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.141592654

///Definicao de um ponto
typedef struct{
    int alias;
    float x;
    float y;
    //float fx;
    //float fy;
}node;
///Definicao de um elemento
typedef struct{
    int alias;
    node P1;
    node P2;
    float length;
    float fx,fy,F;
    float theta;
    char type;
}element;
///Definicao de um suporte
typedef struct{
    char type;
    node P;
    float fx;
    float fy;
    float M;
}support;
///Definicao de uma forca
typedef struct{
    node P;
    float fx;
    float fy;
    float theta;
}force;
///Funcao para criar nos
node create_node(int Nnode)
{
    node Node;
    printf("Coordenada x do no= ");
    scanf("%f",&Node.x);
    printf("Coordenada y do no= ");
    scanf("%f",&Node.y);
    Node.alias = Nnode;
    return Node;
}
///Funcao para criar elementos
element create_element(int Nelem, node nodevet[])
{
    element Elem;
    //printf("Quantos elementos?");
//    scanf("%d",n);
    Elem.alias=Nelem;
    printf("No inicial da barra= ");
    scanf("%d",&Elem.P1.alias);
    ///int *cont[n];
    ///for(i=0;i<n;i++)
    ///    if(Elem.P1.alias=i)
    ///    cont[i]++;
    printf("No final da barra= ");
    scanf("%d",&Elem.P2.alias);
    Elem.P1=nodevet[Elem.P1.alias];
    Elem.P2=nodevet[Elem.P2.alias];
    Elem.length= sqrt(Elem.P2.x - Elem.P2.x * Elem.P2.x - Elem.P2.x + Elem.P2.y - Elem.P1.y * Elem.P2.y - Elem.P1.y);
    if(Elem.P1.x!=Elem.P2.x || Elem.P1.y!=Elem.P2.y)
    {
        Elem.theta=atan((Elem.P2.y-Elem.P1.y)/(Elem.P2.x-Elem.P1.x));
        //Elem.theta=Elem.theta*(180/PI);
    }
    else
        Elem.theta=0;
    if (Elem.theta<=0.00001&&Elem.theta>=-0.00001)
        Elem.fy=0;
    else if (Elem.theta<=PI/2+0.00001&&Elem.theta>=PI/2-0.00001)
        Elem.fx=0;
    return Elem;
}
///Funcao para criar Forcas
force create_force()
{
    force Force;
    printf("Em qual no a forca sera aplicada: ");
    scanf("%d",&Force.P.alias);
    printf("Coordenada da forca em x= ");
    scanf("%f",&Force.fx);
    printf("Coordenada da forca em y= ");
    scanf("%f",&Force.fy);
    if(Force.fx>=0.00001||Force.fx<=-0.00001)
        Force.theta=atan(Force.fy/Force.fx);
    else
        Force.theta=PI/2;
    return Force;
}
///Funcao para criar Apoios
support create_support(node nodevet[])
{
    support Sprt;
    printf("Em qual no esta o apoio: ");
    scanf("%d",&Sprt.P.alias);
    getchar();
    do
    {
        printf("Qual o tipo de apoio(e para engastamento, f para fixo,m para movel): ");
        scanf("%c",&Sprt.type);
    }
    while(Sprt.type!='f'&&Sprt.type!='m'&&Sprt.type!='e');
    Sprt.P=nodevet[Sprt.P.alias];
    Sprt.fx=0;
    Sprt.fy=0;
    return Sprt;
}
///Funcao para transformar uma forca axial em forca
force axial_to_force(int fP,element Elem)
{
    force Axialforce;
    Axialforce.P.alias=fP;
    Axialforce.fx=Elem.F*cos(Elem.theta);
    Axialforce.fy=Elem.F*sin(Elem.theta);
    Axialforce.theta=Elem.theta;
//        printf("fx:%f,fy:%f",Axialforce[i].fx,Axialforce[i].fy);
    return Axialforce;
}
int main()
{
    int i,j,nn=3,ne=3,nf=1,ns=2,Nnode=0,Nelem=0,cont=0,fw=0,fP[nf];
    force axialvet[1];
    float SumX=0,SumY=0,F[ne];
    printf("Quantos nos: ");
    scanf("%d",&nn);
    node nodevet[nn];
    for(i=0;i<nn;i++)
        {
            nodevet[i]=create_node(Nnode);
            Nnode++;
        }
    printf("Quantos elementos: ");
    scanf("%d",&ne);
    element elemvet[ne];
        for(i=0;i<ne;i++)
        {
            elemvet[i]=create_element(Nelem,nodevet);
            Nelem++;
        }
    printf("Quantas forças: ");
    scanf("%d",&nf);
    force forcevet[nf];
    for(i=0;i<nf;i++)
        forcevet[i]=create_force();
    printf("Quantos apoios: ");
    scanf("%d",&ns);
    support sprtvet[ns];
    for(i=0;i<ns;i++)
        sprtvet[i]=create_support(nodevet);
///Funcao para Calcular as Forca dos Apoios
    while(cont<ns)
    {
        ///Calcula reacoes a partir do momento
        if(sprtvet[cont].type!='e')
        {
            for(i=0;i<nf;i++)
            {
                if(nodevet[forcevet[i].P.alias].y-nodevet[sprtvet[cont].P.alias].y>0)
                    SumY=SumY+forcevet[i].fx*(nodevet[forcevet[i].P.alias].y-nodevet[sprtvet[cont].P.alias].y);
                else
                    SumY=SumY-forcevet[i].fx*(nodevet[forcevet[i].P.alias].y-nodevet[sprtvet[cont].P.alias].y);
                if(nodevet[forcevet[i].P.alias].x-nodevet[sprtvet[cont].P.alias].x>0)
                    SumY=SumY+forcevet[i].fy*(nodevet[forcevet[i].P.alias].x-nodevet[sprtvet[cont].P.alias].x);
                else
                    SumY=SumY-forcevet[i].fy*(nodevet[forcevet[i].P.alias].x-nodevet[sprtvet[cont].P.alias].x);
            }
            for(j=0;j<ns;j++)
            {
                if(sprtvet[j].P.alias!=sprtvet[cont].P.alias)
                {
                    if((nodevet[sprtvet[j].P.alias].x-nodevet[sprtvet[cont].P.alias].x)<=0.00001||(nodevet[sprtvet[j].P.alias].x-nodevet[sprtvet[cont].P.alias].x)>=-0.00001)
                        sprtvet[j].fy=SumY/(nodevet[sprtvet[j].P.alias].x-nodevet[sprtvet[cont].P.alias].x);
                    else
                        sprtvet[j].fy=0;
                }
            }
        }
        SumY=0;
        ///Calcula reacao em Y
        for(j=0;j<nf;j++)
        {
            SumY=SumY+forcevet[j].fy;
        }
        if(cont>0)
        {
            for(i=0;i<ns;i++)
            {
                if(sprtvet[i].P.alias!=sprtvet[cont].P.alias)
                {
                    if(sprtvet[i].fy<=0.00001&&sprtvet[i].fy>=-0.00001)
                    sprtvet[i].fy=SumY-sprtvet[cont].fy;
                }
            }
        }
        else if(sprtvet[cont].type=='e')
        {
            sprtvet[cont].fy=SumY;
        }
        SumY=0;
        SumX=0;
        ///Calcula reacao em X
        if(sprtvet[cont].type!='m')
        {
            for(j=0;j<nf;j++)
            {
                SumX=SumX+forcevet[j].fx;
            }
            if(cont>0)
            {
                for(i=0;i<ns;i++)
                {
                    if(sprtvet[i].P.alias!=sprtvet[cont].P.alias)
                    {
                        sprtvet[i].fx=SumX-sprtvet[cont].fx;
                    }
                }
            }
            else if(sprtvet[cont].type=='e')
                sprtvet[cont].fx=SumX;
            else
                sprtvet[cont].fx=SumX;
            SumX=0;
        }
        ///Calcula Momento
        if(sprtvet[cont].type=='e')
        {
            for(i=0;i<nf;i++)
            {
                SumY=SumY+forcevet[i].fx*(nodevet[forcevet[i].P.alias].y-nodevet[sprtvet[cont].P.alias].y);
                SumY=SumY+forcevet[i].fy*(nodevet[forcevet[i].P.alias].x-nodevet[sprtvet[cont].P.alias].x);
            }
            for(j=0;j<ns;j++)
            {
                if(sprtvet[j].P.alias!=sprtvet[cont].P.alias)
                {
                    if(sprtvet[j].fx>=0.00001||sprtvet[j].fx<=-0.00001)
                        SumY=SumY+sprtvet[j].fx*(nodevet[sprtvet[j].P.alias].y-nodevet[sprtvet[cont].P.alias].y);
                    if(sprtvet[j].fy>=0.00001||sprtvet[j].fy<=-0.00001)
                        SumY=SumY+sprtvet[j].fy*(nodevet[sprtvet[j].P.alias].x-nodevet[sprtvet[cont].P.alias].x);
                }
                sprtvet[cont].M=SumY;
            }
        }
        SumY=0;
        cont++;
    }
    for(i=0;i<ns;i++)
    {
        printf("\nReacoes do Apoio %d: tipo:%c, fx:%f, fy:%f",i,sprtvet[i].type,sprtvet[i].fx,sprtvet[i].fy);
    }
    cont=0;
    while(cont!=ne)
    {
        for(i=0;i<nf;i++)
        {
            if((sprtvet[i].P.alias==elemvet[cont].P1.alias || sprtvet[i].P.alias==elemvet[cont].P2.alias))
            {
                SumX=SumX+sprtvet[i].fx;
                SumY=SumY+sprtvet[i].fy;
                if(elemvet[cont].theta>=PI/2+0.00001||elemvet[cont].theta<=PI/2-0.00001)
                {
                    SumX=SumX*cos(elemvet[cont].theta);
//                    printf("\nSumX= %f, %d, %f",SumX,cont,elemvet[cont].theta);
                }
                else
                    SumX=0;
                if(elemvet[cont].theta>=0.00001||elemvet[cont].theta<=-0.00001)
                {
                    SumY=SumY*sin(elemvet[cont].theta);
//                    printf("\nSumY= %f, %d, %f",SumY,cont,elemvet[cont].theta);
                }
                else
                    SumY=0;
            F[cont]=sqrt(SumX*SumX+SumY*SumY);
            }
            SumX=0;
            SumY=0;
//            printf("%f",F[cont]);
            if(sprtvet[i].P.alias==elemvet[cont].P1.alias)
                fP[i]=elemvet[cont].P2.alias;
            else if(sprtvet[i].P.alias==elemvet[cont].P2.alias)
                fP[i]=elemvet[cont].P1.alias;
        }
        elemvet[cont].F=F[cont];///sqrt(SumX*SumX+SumY*SumY);
        cont++;
    }
    cont=0;
    while(cont!=ne)
    {
        if(elemvet[cont].F>=0.00001||elemvet[cont].F<=-0.00001)
        {
            for(i=0;i<nf;i++)
            {
                axialvet[i]=axial_to_force(fP[i],elemvet[cont]);
//                axialvet[i]=forcevet[i];
            }
            if(fw==0)
            {
                cont=0;
                fw=1;
            }
        }
        for(i=0;i<nf;i++)
        {
            if(elemvet[cont].F<=0.00001&&elemvet[cont].F>=-0.00001)
                elemvet[cont].F=sqrt(pow(axialvet[i].fx,2)+pow(axialvet[i].fy,2))/cos(elemvet[cont].theta-axialvet[i].theta);
        }
        cont++;
    }
    for(i=0;i<ne;i++)
    {
        if(elemvet[i].F>=0)
            elemvet[i].type='T';
        else
            elemvet[i].type='C';
        printf("\nForca axial na barra %d: %f, %c",elemvet[i].alias,elemvet[i].F,elemvet[i].type);
    }
    return 0;
}
