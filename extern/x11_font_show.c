

  /* font_show.c    show several different size fonts in a window    */
  /*          compile  gcc -o font_show  font_show.c -lXm -lXt -lX11 */

  /* to select a font, the format is:                                */
  /* -*-NAME-BOLD-KIND-normal-*-*-SIZE-BITS-*                        */
  /*                                                                 */
  /* NAME = fixed  courier  times  helvetica  new century schoolbook */
  /* BOLD = medium  bold                                             */
  /* KIND =  r  o  i     for regular  oblique  italic                */
  /* SIZE =  3 digit number, point size times 10                     */
  /* BITS =  75  100    dots per inch                                */

  #include <Xm/Xm.h> 
  #include <Xm/MainW.h>
  #include <Xm/DrawingA.h>
  #include <stdio.h>
  #include <string.h>

  int main(int argc, char *argv[])
  {
    XtAppContext app_context;
    Display *display;
    int screen_num;
    XGCValues xgcvalues;
    XColor gray_c, white_c, black_c ;
    XColor exact_c;
    Colormap cmap;
    Status status;
    Widget toplevel;
    Widget main_w;
    Widget draw_a;
    GC gc_text;

    char msg1[27]="abcdefghijklmnopqrstuvwxyz";
    char msg2[27]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char msg3[39]="0123456789+-*/()[]{}<>,.;:?|~!@#$%^&_=";
    Font font12, font12t;
    Font font14, font14t;
    Font font18;
    Font font24;

    printf("font_show starting, be patient if over network \n");

    /* Initialize toolkit, open display and create application shell. */
    toplevel = XtVaAppInitialize(&app_context, "font_show",
                               NULL, 0, &argc, argv, NULL, NULL);


    /*      Create MainWindow. */
    main_w = XtVaCreateManagedWidget("main",
                  xmMainWindowWidgetClass,   toplevel,
                  XmNshadowThickness,        0,
                  NULL);


    draw_a = XtVaCreateManagedWidget ("canvas",
                                      xmDrawingAreaWidgetClass,
                                      main_w,
                                      XmNwidth, 750,
                                      XmNheight, 500,
                                      NULL);
    XtManageChild(draw_a);
    XtRealizeWidget(toplevel);
    display = XtDisplay(toplevel);

    screen_num = XDefaultScreen(display);
    cmap = XDefaultColormap(display, screen_num);
    status = XAllocNamedColor(display, cmap, "gray", &gray_c, &exact_c);
    if (!status) printf("no gray (for parent)\n"); 
    status = XAllocNamedColor(display, cmap, "white", &white_c, &exact_c);
    if (!status) printf("no white \n"); 
    status = XAllocNamedColor(display, cmap, "black", &black_c, &exact_c);
    if (!status) printf("no black \n"); 

    gc_text = XCreateGC(XtDisplay(draw_a), XtWindow(draw_a), 0, &xgcvalues);
    XSetBackground(display, gc_text, white_c.pixel);
    XSetForeground(display, gc_text, black_c.pixel);
    XtVaSetValues(draw_a,
                  XmNbackground, white_c.pixel, NULL);

    /* -*-NAME-BOLD-KIND-normal-*-*-SIZE-BITS-*              */
    font12 = XLoadFont(display,                /* 12, 14, 18, 24 */
        "-*-courier-medium-r-normal-*-*-120-75-*");
    XSetFont(display, gc_text, font12);

    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 15, "12 pt courier", 13);
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 30, msg1, strlen(msg1));
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 45, msg2, strlen(msg2));
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 60, msg3, strlen(msg3));

    font12t = XLoadFont(display,
        "-*-times-medium-r-normal-*-*-120-75-*");
    XSetFont(display, gc_text, font12t);

    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            400, 15, "12 pt times", 11);
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            400, 30, msg1, strlen(msg1));
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            400, 45, msg2, strlen(msg2));
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            400, 60, msg3, strlen(msg3));


    font14 = XLoadFont(display,
        "-*-courier-medium-r-normal-*-*-140-75-*");
    XSetFont(display, gc_text, font14);

    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 100, "14 pt courier", 13);
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 120, msg1, strlen(msg1));
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 140, msg2, strlen(msg2));
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 160, msg3, strlen(msg3));


    font14t = XLoadFont(display,
        "-*-times-medium-r-normal-*-*-140-75-*");
    XSetFont(display, gc_text, font14t);

    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            400, 100, "14 pt times", 11);
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            400, 120, msg1, strlen(msg1));
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            400, 140, msg2, strlen(msg2));
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            400, 160, msg3, strlen(msg3));


    font18 = XLoadFont(display,
        "-*-courier-medium-r-normal-*-*-180-75-*");
    XSetFont(display, gc_text, font18);

    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 200, "18 pt courier", 13);
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 225, msg1, strlen(msg1));
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 250, msg2, strlen(msg2));
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 275, msg3, strlen(msg3));


    font24 = XLoadFont(display,
        "-*-courier-medium-r-normal-*-*-240-75-*");
    XSetFont(display, gc_text, font24);

    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 320, "24 pt courier", 13);
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 350, msg1, strlen(msg1));
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 380, msg2, strlen(msg2));
    XDrawImageString(display, XtWindow ( draw_a ), gc_text,
            20, 410, msg3, strlen(msg3));


    printf("font_show waiting to be killed \n");
    XtAppMainLoop(app_context);

    return 0;
  } /* end main of font_show */

