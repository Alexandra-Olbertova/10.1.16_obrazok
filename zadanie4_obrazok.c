#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

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
	
	img = (GSI*)malloc(sizeof(unsigned int)*m*n);
	
	if(img == NULL){
		free(img);
		return NULL;
	}
	
	img->height = m;
	img->width = n;	
	img->px = (unsigned char*)malloc(sizeof(unsigned int)*m*n);
//	img->px = color;
	
	return img;	
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
	
	GSI *bin;
	
	gsi_binarize(img,bin);
	
	gsi_destroy(img);
	gsi_destroy(bin);
}
