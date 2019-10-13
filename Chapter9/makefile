# makefile
.SUFFIXES : .c .o

OBJECTS = init_screen.o \
	  init_drm.o \
          draw_egl.o \
	  draw_opengl2 \
	  draw_drm.o \
	  draw_triangle.o \
	  draw_ortho.o \
	  draw_viewport.o \
	  draw_cube.o \
	  draw_rotate.o \
	  draw_color.o \
	  draw_cube_animation.o \
	  draw_opengl2_drm.o \
	  draw_opengl2_rotate.o \
	  draw_opengl2_animation.o

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux 
LDFLAGS = -L/opt/vc/lib/ -lbcm_host
#TARGET = $(OBJECTS:.o=)

all : init_screen draw_egl draw_opengl2 \
      init_drm draw_drm  \
      draw_triangle draw_ortho draw_viewport \
      draw_cube draw_rotate draw_color draw_cube_animation \
      draw_opengl2_drm draw_opengl2_rotate draw_opengl2_animation

$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c $(CFLAGS) $(LDFLAGS)

draw_egl : $(TARGET:=.c)
	$(CC) -o $@ $@.c $(CFLAGS) $(LDFLAGS) -lEGL -lGLESv1_CM

init_drm : $(TARGET:=.c)
	$(CC) -o $@ $@.c -I/usr/include/libdrm -ldrm -lgbm 

draw_drm : $(TARGET:=.c)
	$(CC) -o $@ $@.c -I/usr/include/libdrm -ldrm -lgbm -lEGL -lGLESv1_CM

draw_opengl2 : $(TARGET:=.c)
	$(CC) -o $@ $@.c $(CFLAGS) $(LDFLAGS) -lEGL -lGLESv2 -lGL

draw_triangle : $(TARGET:=.c)
	$(CC) -o $@ $@.c -I/usr/include/libdrm -ldrm -lgbm -lEGL -lGLESv1_CM

draw_ortho : $(TARGET:=.c)
	$(CC) -o $@ $@.c -I/usr/include/libdrm -ldrm -lgbm -lEGL -lGLESv1_CM -lGL

draw_viewport : $(TARGET:=.c)
	$(CC) -o $@ $@.c -I/usr/include/libdrm -ldrm -lgbm -lEGL -lGLESv1_CM -lGL

draw_cube : $(TARGET:=.c)
	$(CC) -o $@ $@.c -I/usr/include/libdrm -ldrm -lgbm -lEGL -lGLESv1_CM -lGL

draw_rotate : $(TARGET:=.c)
	$(CC) -o $@ $@.c -I/usr/include/libdrm -ldrm -lgbm -lEGL -lGLESv1_CM -lGL

draw_color : $(TARGET:=.c)
	$(CC) -o $@ $@.c -I/usr/include/libdrm -ldrm -lgbm -lEGL -lGLESv1_CM -lGL

draw_cube_animation : $(TARGET:=.c)
	$(CC) -o $@ $@.c -I/usr/include/libdrm -ldrm -lgbm -lEGL -lGLESv1_CM -lGL

draw_opengl2_drm : $(TARGET:=.c)
	$(CC) -o $@ $@.c -I/usr/include/libdrm -ldrm -lgbm -lEGL -lGLESv2 -lGL

draw_opengl2_rotate : $(TARGET:=.c)
	$(CC) -o $@ $@.c -I/usr/include/libdrm -ldrm -lgbm -lEGL -lGLESv2 -lGL

draw_opengl2_animation : $(TARGET:=.c)
	$(CC) -o $@ $@.c -I/usr/include/libdrm -ldrm -lgbm -lEGL -lGLESv2 -lGL

clean :
	$(RM) $(OBJECTS) $(OBJECTS:.o=) core 
