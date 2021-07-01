#include <stdio.h>
#include <stdlib.h>

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
	
	img->height = 0;
	img->width = 0;

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
	img->px = (unsigned char*)malloc(sizeof(unsigned int)*m*n);	
	
	if(img->px == NULL){
		free(img->px);
		return NULL;
	}
	
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
	
	if(img->px == NULL){
		free(img->px);
		return NULL;
	}
	
	for(i = 0; i < m; i++){
		for(j = 0; j < n; j++){
			PIX(img,m,n) = color;
		}
	}
	
	return img;	
}

GSI *gsi_create_by_pgm5(char *file_name){
	
	FILE* f;
	GSI *img;
	char line[80];
	unsigned int w,h;
	int x, y, col;
	
	f = fopen(file_name, "rb");
	if(f == NULL)
		return NULL;
		
	fgets(line, 10, f);
	
	if(line[0] != 'P' || line[1] != '5'){
		fclose(f);
		return FAIL;
	}
	
	do{
		fgets(line, 80, f);
	}while(line[0] == '#');
	
	sscanf(line, "%u%u", &h, &w);
	
	if((img = gsi_create_with_geometry(h, w)) == NULL){
		fclose(f);
		return FAIL;
	}
	
	for(x = 0; x < img->height; x++){
		for(y = 0; y < img->width; y++){
			fscanf(f, "%c ", &col);
			PIX(img, x, y) = col;
		}
	}
	
	if(fclose(f) == EOF){
		return FAIL;
	}
	
	return img;
	
}

char save_as_pgm5(GSI *img, char *file_name, char *comment){
	
	FILE *f;
	int max = 0;
	int x, y;
	
	f = fopen(file_name, "wb");
	
	if(f == NULL){
		return 0;
	}

	fprintf(f,"P5\n");
	
	if(comment != NULL){
		fprintf(f, "#");
		fprintf(f, comment);
		fprintf(f, "\n");
	}
	
	for(x = 0; x < img->height; x++){
		for(y = 0; y < img->width; y++){
			
			if(PIX(img, x, y) > max)
			max = PIX(img, x, y);
			
		}
	}
	
	fprintf(f,"%u %u %d\n", img->width, img->height, max);
	
	for(x = 0; x < img->height; x++){
		for(y = 0; y < img->width; y++){
			fprintf(f,"%c", PIX(img, x, y));
		}
	}	
	
	if(fclose(f) == EOF){
		return 0;
	}
	
	return 1;
	
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
	bin = gsi_create_with_geometry(img->width,img->height);
	
	gsi_binarize(img, bin);
	
	save_as_pgm5(bin,"baboon_bin.pgm", "pgm5");
	
	gsi_destroy(img);
	gsi_destroy(bin);
	
}
