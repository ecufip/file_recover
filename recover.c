#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }
 
    // remember filename
    char* infile = argv[1];
    
    // "r" to read the file
    FILE* inptr = fopen(infile, "r");
    
    // error if unable to open file
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    // output file
    FILE* img = NULL; 
    
    // counter for image number
    int image_number = 0;
    
    // create an array to store the 512 bytes of information - unsigned means a scale of 0 to 255 not -128 to 127
    unsigned char arr[512];
    
    // infinite loop
    while(1)
    {
        // store the information to the address of the array
        // no need to use &arr as an array is already a pointer
        fread(arr, 512, 1, inptr);
        
        //exits if the end of the file has been reached
        if(feof(inptr))
        {
           break; 
        }
        
        // check against the start sequence of a JPG
        if(arr[0] == 0xff && arr[1] == 0xd8 && arr[2] == 0xff && (arr[3] & 0xf0) == 0xe0)
        {
            image_number ++;
            
            // check to see if first image, if not then shut the previous one
            if(img != NULL)
            {
                fclose(img);
            }
            
            // define a variable for a new file 8 chars long: 3 x numbers, point, j, p, g, \0 to end
            char filename[8];
            
            // create the variable name based on the current image number
            sprintf(filename, "%03d.jpg", image_number - 1);
            
            // create a new file saved to 'img'
            img = fopen(filename, "w");
        }
        
        // if an image has been created, write the file
        if(image_number > 0)
        {
           fwrite(arr, 512, 1, img);         
        }
    }
    
    //closes open files
    fclose(img);
    fclose(inptr); 
    
    return 0;
}
