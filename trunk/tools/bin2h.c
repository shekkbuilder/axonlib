#include <stdio.h>

#define NUM 16

int main (int argc, char **argv)
{
  FILE *in, *out;
  unsigned char a[NUM];
  int i,c,total;

  if (argc!=4)
  {
    fprintf(stderr,"usage: bin2h input.png output.h name[]\n");
    return 1;
  }
  if (!(in=fopen(argv[1],"rb")))
  {
    fprintf(stderr,"error: can't open %s.\n",argv[1]);
    return 1;
  }
  if (!(out=fopen(argv[2],"wt")))
  {
    fclose(in);
    fprintf(stderr,"error: can't create %s.\n",argv[2]);
    return 1;
  }

  total = 0;

  fprintf (out,"unsigned char %s[] = \n{\n",argv[3]);
  while ((c=fread(a,1,NUM,in))!=0)
  {
    fprintf(out,"  ");
    if (c==NUM)
    {
      for (i=0; i<c; i++) fprintf(out,"0x%02x, ",a[i]);
      fprintf(out,"\n");
      total += NUM;
    } else
    {
      for (i=0; i<c-1; i++) fprintf(out,"0x%02x, ",a[i]);
      fprintf(out,"0x%02x\n",a[c-1]);
      total += c;
    }
  }
  fprintf(out,"};\n");  
  fprintf(out,"// '%s' : %i\n",argv[1],total);
  fclose (out);
  fclose (in);
  return 0;
}
