#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define PIX(im,x,y) (im->px[(im->height)*x+y])
#define FAIL 0

typedef struct{
	unsigned int width;
	unsigned int height;
	unsigned char *px;
}GSI;

GSI *gsi_create_empty(void){
	
	GSI *img;
	
	img = malloc(sizeof(GSI));
	
	if(img == NULL){
		free(img);
		return NULL;
	}
	
	return img;
}

GSI *gsi_create_with_geometry(unsigned int m, unsigned int n){
	
	GSI *img;
	
	img = (GSI*)malloc(sizeof(unsigned int)*m*n);
	
	if(img == NULL){
		free(img);
		return NULL;
	}
	
	img->height = m;
	img->width = n;
	
	return img;	
}

GSI *gsi_create_with_geometry_and_color(unsigned int m, unsigned int n, unsigned char color){
	
	GSI *img;
	int i,j;
	
	img = (GSI*)malloc(sizeof(unsigned int)*m*n);
	
	if(img == NULL){
		free(img);
		return NULL;
	}
	
	img->height = m;
	img->width = n;	
	img->px = (unsigned char*)malloc(sizeof(unsigned int)*m*n);
	
	for(i = 0; i < m; i++){
		for(j = 0; j < n; j++){
			PIX(img,m,n) = color;
		}
	}
	
	return img;	
}

GSI *gsi_create_by_pgm5(char *file_name){
	
	GSI *imgNew;
	
	int x, y;
	char type[2];
	char *comment;
	int m_n_px[5];
	
	int f = open(file_name, O_RDONLY);
	
	if(f < 0)
		return NULL;
	
	if(read(f, type, 2) < 0){
		close(f);
		return FAIL;
	}
	
	if(type[0] != 'P' || type[1] != '5'){
		close(f);
		return FAIL;
	}
	
	if(comment[0] == '#')
		read(f, comment, sizeof(comment));
	
	if(read(f, m_n_px, 5) < 0){
		close(f);
		return FAIL;
	}
	
	if((imgNew = gsi_create_with_geometry_and_color(m_n_px[0],m_n_px[2],m_n_px[4])) == NULL){
		close(f);
		return FAIL;
	}
		
	if(read(f, imgNew->px, sizeof(unsigned char)*imgNew->width*imgNew->height) < 0){
		close(f);
		return FAIL;
	}
	
	if(close(f) == EOF){
		return FAIL;
	}
	
	return imgNew;
	
}

char save_as_pgm5(GSI *img, char *file_name, char *comment){
	
	char type[2];
	unsigned int m_n_px[5];
	int max = 0;
	unsigned int *col;
	int x,y;
	
	int f = open(file_name, O_WRONLY);
	
	if(f < 0){
		return;
	}
	
	if(write(f, "P5\n", 2) < 0){
		close(f);
		return FAIL;
	}
	
	m_n_px[0] = img->width;
	m_n_px[3] = m_n_px[1] = ' ';
	m_n_px[2] = img->height;
	
	for(x = 0; x < img->height; x++){
		for(y = 0; y < img->width; y++){
			
			if(PIX(img, x, y) > max)
			max = PIX(img, x, y);
			
		}
	}
	m_n_px[4] = max;
	
	if(write(f, m_n_px, 5) < 0){
		close(f);
		return FAIL;
	}
	
	if(comment[0] == '#')
		write(f, comment, sizeof(comment));

	if(write(f,img->px,sizeof(unsigned char)*img->width*img->height) < 0){
		close(f);
		return FAIL;
	}
	
	if(close(f) == EOF){
		return FAIL;
	}
	
	return f;
	
}	
	
void gsi_destroy(GSI *img){
	free(img);
}

char gsi_binarize(GSI *img, GSI *bin){
	
	int x,y;
	
	for(x = 0; x < img->height; x++){
		for(y = 0; y < img->width; y++){
			
			if(PIX(img,x,y) < 126)
				PIX(bin,x,y) = 0;
			else
				PIX(bin,x,y) = 255;			
		}
	}
}

main(){
	
	GSI *img, *bin;
	
	img = gsi_create_by_pgm5("baboon.pgm");
	bin = gsi_create_empty();
	
	gsi_binarize(img, bin);
	
	save_as_pgm5(bin, "baboon_bin.pgm",0);
	
	gsi_destroy(img);
	gsi_destroy(bin);
	
}
