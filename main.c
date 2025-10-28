/*		TODO		*/
//Import Image into program	✅️
//Resize the image 2x1 in height ✅️
//Convert image from RGB to BW
//Print the image in BW

//Convert image from RGB to HSV
//Print the image in Color (using ASCII escape code)
//Highlight the edges using a sobel filter

//Fix git user ✅️



#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "stb/stb_image.h"
#include "stb/stb_image_resize2.h"


int main(int argc, char const *argv[]){

	char saturation[] = {'\0','.','-','=','+','*','x','#','$','&','X','@'};
	/*		import image in its original size		*/
	int width;
	int height;
	int channels;
	unsigned char* imagedata = NULL;
	const char* filename = "icon.png";
	
	imagedata = stbi_load(filename, &width,&height,&channels,3);

	if(imagedata == NULL){
		printf("ERROR\n");
		printf("%s\n",stbi_failure_reason());
		return 0;
	}
	printf("Successfully imported\n");
	printf("width: %d\n",width);
	printf("Height: %d\n",height);
	printf("Channels: %d\n",channels);

	/*		resize the image to half its height		*/

	int res_width = width;
	int res_height = height/2;
	int res_channels = 3;

	size_t new_image_size = (size_t)res_width*(size_t)res_height*res_channels;
	unsigned char* resized_image = (unsigned char*)malloc(new_image_size);

	stbir_pixel_layout layout = STBIR_RGB; 
	stbir_resize_uint8_linear(  imagedata, width, height, 0,
								resized_image, res_width, res_height, 0,
								layout);
	if(resized_image != NULL){
		stbi_image_free(imagedata);
		imagedata = resized_image;
		width = res_width;
		height = res_height;
	}else{
		printf("Error on resize");
		free(resized_image);
	}

	printf("New width: %d\n",width);
	printf("New Height: %d\n",height);
	printf("Channels: %d\n",res_channels);

	/*		print all pixels		*/

	for (int y = 0 ; y < height;y++){
		for (int x = 0; x < width; x++)
		{	
			size_t index = (y*width+x) * res_channels;
			printf("(%d|%d|%d)|",
				imagedata[index],
				imagedata[index+1],
				imagedata[index+2]);
		}
		printf("\n");
	}

	size_t image_BW_size = (size_t)res_width*(size_t)res_height;
	unsigned char* image_BW = (unsigned char*)malloc(image_BW_size);
	if(image_BW == NULL){
		printf("Error allocating memory");
		free(image_BW);
	}

	for (int y = 0 ; y < height;y++){
		for (int x = 0; x < width; x++)
		{
			size_t index = (y * width + x) * res_channels;
			size_t index_bw = (y * width + x);
			unsigned char gray = (unsigned char)(0.299 * imagedata[index]) 	+ 
							  					(0.587 * imagedata[index + 1]) +
							  					(0.114 * imagedata[index + 2]);

			image_BW[index] = gray;
			printf("(%d)",image_BW[index_bw]);
		}
		printf("\n");
	}


	printf("\n");
	stbi_image_free(imagedata);
	return 0;
}