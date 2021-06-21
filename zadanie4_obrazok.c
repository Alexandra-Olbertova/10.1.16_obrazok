#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#define PIX(im,x,y) (im->px[(im->height)*x+y])

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

GSI *gsi_create_with_geomtery(unsigned int m, unsigned int n){
	
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
	
	GSI *img;
	
	int x, y;
	char type[2];
	char *comment, *col;
	int m_n_px[5];
	
	int f = open(file_name, O_RDONLY);
	
	if(f < 0){
		printf("%s", strerror(errno));
		return NULL;
	}
	
	read(f, type, 2);
	
	if(type[0] != 'P' || type[1] != '5'){
		printf("%s", strerror(errno));
		return NULL;
	}
	
	if(comment[0] == '#')
		read(f, comment, sizeof(comment));
	
	read(f, m_n_px, 5);
	
	img->width = m_n_px[0];
	img->height = m_n_px[2];	
	
	PIX(img, x, y) = read(f, col, img->height*img->width);
	
	return img;
	
}

char save_as_pgm5(GSI *img, char *file_name, char *comment){
	
	int x,y;
	char type[2];
	unsigned int m_n_px[5];
	int max = 0;
	unsigned int *col;
	
	int f = open(file_name, O_WRONLY);
	
	write(f, "P5", 2);
	
	m_n_px[0] = img->width;
	m_n_px[3] = m_n_px[1] = ' ';
	m_n_px[2] = img->height;
	
	for(x = 0; x < img->height; x++){
		for(y = 0; y < img->width; y++){
			
			if(PIX(img, x, y) > max)
			max = PIX(img, x, y);
			
		}
	}
	m_n_px[5] = max;
	
	write(f, m_n_px, 5);
	
	if(comment[0] == '#')
		write(f, comment, sizeof(comment));

	for(x = 0; x < img->height; x++){
		for(y = 0; y < img->width; y++){
			
			write(f, col, PIX(img,x,y));
			
		}
	}
	
	if(close(f) == EOF){
		printf("Unable to close file\n");
		return;
	}
	
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
	
	GSI *img;
	
	img = gsi_create_empty();
	img = gsi_create_with_geometry_and_color;
	
	GSI *bin;
	
	gsi_binarize(img,bin);
	
	save_as_pgm5(bin, "file_name.pgm", 0);
	
	
	gsi_destroy(img);
	gsi_destroy(bin);
}
