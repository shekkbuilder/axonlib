#ifndef axSort_included
#define axSort_included
//----------------------------------------------------------------------
// far not finished, just copy/paste, needs fixup

void axRadixSort (long *source, long *dest, long N, int byte)
{
  int i;
  long count[256];
  long index[256];
  memset(count,0,sizeof (count));
  for (i=0; i<N; i++) count[((source[i])>>(byte*8))&0xff]++;
  index[0]=0;
  for ( i=1; i<256; i++ ) index[i]=index[i-1]+count[i-1];
  for ( i=0; i<N; i++ ) dest[index[((source[i])>>(byte*8))&0xff]++] = source[i];
}

//----------

// bubble sort?
void axBubbleSort(int*a,int n)
{
  int*b,*c,t;
  for (b=a+n; --b>a; )
    for (c=b; --c>=a; )
      if (*c>*b) t=*b,*b=*c,*c=t;
}

//----------

// quick sort
void axQuickSort(int*a, int*b)
{
  int *h = a, *q = b;
  int p = *a;
  if (a >= b) return;
  while (h < q)
  {
    while (*q>=p && h< q) q--;
    if (h<q) *h = *q;
    while (*h<=p && h<q) h++;
    if (h<q) *q = *h;
  }
  *q = p;
  axQuickSort(a, h-1);
  axQuickSort(q+1, b);
}

//----------

// insert sort
// Written by Sanchit Karve A.K.A born2c0de
// born2c0de AT hotmail DOT com
void axInsertSort(int x[], int length)
{
  int key,i;
  for (int j=1; j<length; j++)
  {
    key = x[j];
    i = j-1;
    while (x[i]>key && i>=0)
    {
      x[i+1] = x[i];
      i--;
    }
    x[i+1] = key;
  }
}
//#include <iostream>
//#define ELEMENTS 6
//int main()
//{
//  int A[ELEMENTS]={5,2,4,6,1,3};
//  int x;
//  cout<<"NON SORTED LIST:"<<endl;
//  for(x=0;x<ELEMENTS;x++)
//  {
//    cout<<A[x]<<endl;
//  }
//  insertion_sort(A,ELEMENTS);
//  cout<<endl<<"SORTED LIST"<<endl;
//  for(x=0;x<ELEMENTS;x++)
//  {
//    cout<<A[x]<<endl;
//  }
//  return 0;
//}

//----------------------------------------------------------------------
#endif
