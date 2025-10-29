/*		TODO		*/
// Import Image into program	✅️
// Resize the image 2x1 in height ✅️
// Convert image from RGB to BW ✅️
// Print the image in BW ✅️

// Convert image from RGB to HSV ✅️
// Print the image in Color (using ASCII escape code) ✅️
// Highlight the edges using a sobel filter ✅️

// Fix git user ✅️

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stb/stb_image.h"
#include "stb/stb_image_resize2.h"

int calc_saturation(int r, int g, int b);
int calc_Hue(int red, int green, int blue);
int calc_value(int r, int g, int b);
char* get_color(int red, int green, int blue);

int main(int argc, char const* argv[]) {
  /*		import image in its original size		*/
  int width;
  int height;
  int channels;
  unsigned char* imagedata = NULL;
  unsigned char* edge_magnitude = NULL;
  const char* filename = "Elephant.jpg";

  int sobel_x[3][3] =
  { {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1} };

  int sobel_y[3][3] =
  { {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1} };

  imagedata = stbi_load(filename, &width, &height, &channels, 3);

  if (imagedata == NULL) {
    printf("ERROR\n");
    printf("%s\n", stbi_failure_reason());
    return 0;
  }

  /*		resize the image to half its height		*/
  int scale = 10;
  int res_width = width / scale;
  int res_height = height / 2 / scale;
  int res_channels = 3;

  size_t new_image_size = (size_t)res_width * (size_t)res_height * res_channels;
  unsigned char* resized_image = (unsigned char*)malloc(new_image_size);

  stbir_pixel_layout layout = STBIR_RGB;
  stbir_resize_uint8_linear(imagedata, width, height, 0, resized_image,
                            res_width, res_height, 0, layout);
  if (resized_image != NULL) {
    stbi_image_free(imagedata);
    imagedata = resized_image;
    width = res_width;
    height = res_height;
  } else {
    printf("Error on resize");
    free(resized_image);
  }

  size_t image_BW_size = (size_t)res_width * (size_t)res_height;
  unsigned char* image_BW = (unsigned char*)malloc(image_BW_size);
  unsigned char* ASCII_Img = (unsigned char*)malloc(image_BW_size);
  if (image_BW == NULL) {
    printf("Error allocating memory");
    free(image_BW);
  }

  for (int y = 1; y < height-1; y++) {
    for (int x = 1; x < width-1; x++) {
      size_t index = (y * width + x) * res_channels;
      unsigned char r = (unsigned char)(imagedata[index]);
      unsigned char g = (unsigned char)(imagedata[index + 1]);
      unsigned char b = (unsigned char)(imagedata[index + 2]);

      long Gx = 0;
      long Gy = 0;

      for (int ky = -1; ky <= 1; ky++) {
        for (int kx = -1; kx <= 1; kx++) {
        	size_t px_index = ((y+ky)*width+(x+kx))*res_channels;
        	unsigned char nbr_r = imagedata[px_index];
        	unsigned char nbr_g = imagedata[px_index+1];
        	unsigned char nbr_b = imagedata[px_index+2];
      		int gray_pixel = (int)((float)calc_value(nbr_r,nbr_g,nbr_b) * 2.55);

        	Gx += gray_pixel * sobel_x[ky+1][kx+1];
        	Gy += gray_pixel * sobel_y[ky+1][kx+1];
        }
      }

      long magnitude = labs(Gx)+labs(Gy);
      int normalized = fmin(255,magnitude/4.0);
      if(normalized < 60){
	      char* dot = get_color(r, g, b);
	      if (dot != NULL) {
	        printf("%s", dot);
	        free(dot);
	        dot = NULL;
	      }	
      }else{
      	printf("%c", ' ');
      }
    }
    printf("\n");
  }

  stbi_image_free(imagedata);
  return 0;
}
int calc_Hue(int red, int green, int blue) {
  // Selects the color of the pixel
  double r = red / 255.0;
  double g = green / 255.0;
  double b = blue / 255.0;

  double max = fmax(r, fmax(g, b));
  double min = fmin(r, fmin(g, b));
  double hue = 0;

  if (min == max) {
    hue = 0;
  } else if (max == r) {
    hue = (g - b) / (max - min);
  } else if (max == g) {
    hue = 2.0 + (b - r) / (max - min);
  } else if (max == b) {
    hue = 4.0 + (r - g) / (max - min);
  }
  hue *= 60;
  if (hue < 0) {
    hue += 360;
  }

  return (int)hue;
}

int calc_value(int r, int g, int b) {
  // Selects the "size" of the char printed
  int max = fmax(r, fmax(g, b));

  return (int)((max / 255.0) * 100);
}

int calc_saturation(int r, int g, int b) {
  // Selects if the char is colored of white
  // Need to define: if > 0.3: color else: white
  int min = fmin(r, fmin(g, b));
  int max = fmax(r, fmax(g, b));
  if (max == 0) {
    return 0.0;
  } else {
    int sat = ((double)(max - min) / (double)max) * 100;
    return sat;
  }
}

char* get_color(int red, int green, int blue) {
  char possible_chars[] = " .-=+*x#$&X@";
  int hue = calc_Hue(red, green, blue);
  int saturation = calc_saturation(red, green, blue);
  int value = calc_value(red, green, blue);

  int selection = (value / 100.0) * (sizeof(possible_chars) - 1);
  char character = possible_chars[selection];

  char color_code[3];

  if (saturation > 30) {
    if (hue >= 330 || hue < 30) {
      // red
      strcpy(color_code, "91");
    } else if (hue >= 30 && hue < 90) {
      // yellow
      strcpy(color_code, "93");
    } else if (hue >= 90 && hue < 150) {
      // green
      strcpy(color_code, "92");

    } else if (hue >= 150 && hue < 210) {
      // cyan
      strcpy(color_code, "96");

    } else if (hue >= 210 && hue < 270) {
      // blue
      strcpy(color_code, "94");

    } else if (hue >= 270 && hue < 330) {
      // magenta
      strcpy(color_code, "95");
    }
  } else {
    if (value < 33) {
      // black
      strcpy(color_code, "30");

    } else if (value >= 33 && value < 66) {
      // gray
      strcpy(color_code, "90");

    } else if (value >= 66) {
      // white
      strcpy(color_code, "97");
    }
  }

  char* ascii_escape = malloc(sizeof(char) * 16);
  if (ascii_escape == NULL) return NULL;

  snprintf(ascii_escape, 16, "\x1B[%sm%c\x1B[0m", color_code, character);

  return ascii_escape;
}
