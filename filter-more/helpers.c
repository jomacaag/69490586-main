#include "helpers.h"
#include "stdio.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float grey = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            grey = image[i][j].rgbtRed +  image[i][j].rgbtBlue + image[i][j].rgbtGreen;
            image[i][j].rgbtRed = round(grey / 3);
            image[i][j].rgbtBlue = round(grey / 3);
            image[i][j].rgbtGreen = round(grey / 3);
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][width - 1 - j] = image[i][j];

        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float pixelcount = 0;
            int avgR = 0, avgB = 0, avgG = 0;
            {
                //3x3 grid

                for (int k = -1; k <= 1; k++)
                {
                    for (int m = -1; m <= 1 ; m++)
                    {
                        if (i + k >= 0 && i + k < height && j + m >= 0 && j + m < width)
                        {
                            avgR += image[i + k][j + m].rgbtRed;
                            avgB += image[i + k][j + m].rgbtBlue;
                            avgG += image[i + k][j + m].rgbtGreen;
                            pixelcount++;
                        }
                    }
                }
            }
            temp[i][j].rgbtRed = round(avgR / pixelcount);
            temp[i][j].rgbtBlue = round(avgB / pixelcount);
            temp[i][j].rgbtGreen = round(avgG / pixelcount);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }


}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    // initialize kernels
    int GxKernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int GyKernel[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //reset values to 0
            int avgRx = 0, avgBx = 0, avgGx = 0;
            int avgRy = 0, avgBy = 0, avgGy = 0;
            int GR = 0, GB = 0, GG = 0;
            //3x3 grid
            for (int k = -1; k <= 1; k++)
            {
                for (int m = -1; m <= 1 ; m++)
                {
                    //this if statemet makes the calculation apply only when not edge case, else it will just do nothing,
                    // because we are treating the border as a black border, eg. RGB=00 00 00 then we dont have to do anything, and just divide do the calc
                    if (i + k >= 0 && i + k < height && j + m >= 0 && j + m < width)
                    {
                        //grabs and multiplies values for each RGB pixel in origial image
                        //First for X
                        avgRx += image[i + k][j + m].rgbtRed * GxKernel[k + 1][m + 1];
                        avgBx += image[i + k][j + m].rgbtBlue * GxKernel[k + 1][m + 1];
                        avgGx += image[i + k][j + m].rgbtGreen * GxKernel[k + 1][m + 1];

                        avgRy += image[i + k][j + m].rgbtRed * GyKernel[k + 1][m + 1];
                        avgBy += image[i + k][j + m].rgbtBlue * GyKernel[k + 1][m + 1];
                        avgGy += image[i + k][j + m].rgbtGreen * GyKernel[k + 1][m + 1];
                    }

                }
            }
            //calculates using the formula provided
            GR = round(sqrt((avgRx * avgRx) + (avgRy * avgRy)));
            GG = round(sqrt((avgGx * avgGx) + (avgGy * avgGy)));
            GB = round(sqrt((avgBx * avgBx) + (avgBy * avgBy)));
            // cant have greater than 255 in struct for RGBTRIPLE
            if (GB > 255)
            {
                GB = 255;
            }

            if (GR > 255)
            {
                GR = 255;
            }

            if (GG > 255)
            {
                GG = 255;
            }



            //writes each value to a temp array
            temp[i][j].rgbtRed = GR;
            temp[i][j].rgbtBlue = GB;
            temp[i][j].rgbtGreen = GG;



        }
    }
    //copies temp array to out image.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}