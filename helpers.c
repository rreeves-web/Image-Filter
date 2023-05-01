#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int i;
    for (i = 0; i < height; i++)
    {
    int j;
    for (j = 0; j < width; j++)
    {
        BYTE rgbtRed = image[i][j].rgbtRed;
        BYTE rgbtBlue = image[i][j].rgbtBlue;
        BYTE rgbtGreen = image[i][j].rgbtGreen;
        float rgbtAvgFloat = ((rgbtRed + rgbtBlue + rgbtGreen) / 3.0);
        int rgbtAvg = round(rgbtAvgFloat);
        image[i][j].rgbtRed = rgbtAvg;
        image[i][j].rgbtBlue = rgbtAvg;
        image[i][j].rgbtGreen = rgbtAvg;
    }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int i;
    for (i = 0; i < height; i++)
    {
        int j;
        for (j = 0; j < width; j++)
        {
            // Initialize these calculations as variables so that they can be checked and processed individually
            // Sepia Red floats to allow for decimal formula calclations
            float sepiaRedrgbtRed = .393 * image[i][j].rgbtRed;
            float sepiaRedrgbtGreen = .769 * image[i][j].rgbtGreen;
            float sepiaRedrgbtBlue = .189 * image[i][j].rgbtBlue;
            // Add floats
            float sepiaRedFloat = sepiaRedrgbtRed + sepiaRedrgbtBlue + sepiaRedrgbtGreen;
            // Round to int
            int sepiaRed = round(sepiaRedFloat);
            // Cap final value at 255
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            // Same as above but for sepiaGreen
            float sepiaGreenrgbtRed = .349 * image[i][j].rgbtRed;
            float sepiaGreenrgbtGreen = .686 * image[i][j].rgbtGreen;
            float sepiaGreenrgbtBlue = .168 * image[i][j].rgbtBlue;
            float sepiaGreenFloat = sepiaGreenrgbtRed + sepiaGreenrgbtBlue + sepiaGreenrgbtGreen;
            int sepiaGreen = round(sepiaGreenFloat);
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            // Same for sepiaBlue
            float sepiaBluergbtRed = .272 * image[i][j].rgbtRed;
            float sepiaBluergbtGreen = .534 * image[i][j].rgbtGreen;
            float sepiaBluergbtBlue = .131 * image[i][j].rgbtBlue;
            float sepiaBlueFloat = sepiaBluergbtRed + sepiaBluergbtGreen + sepiaBluergbtBlue;
            int sepiaBlue = round(sepiaBlueFloat);
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            // Swap values for newly formulated sepia RGB values
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Create copy of original array in tmp[][] variable array
    // Regardless of array length
    RGBTRIPLE tmp[height][width];
    // Copy array into tmp array
    int i;
    for (i = 0; i < height; i++)
    {
        int j;
        for (j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }
    // Instantiate integer defining total number of indeces/values in tmp
    int size = sizeof(tmp) / sizeof(RGBTRIPLE);
    // If width is even, reflect array
    if (width % 2 == 0)
    {
        for (i = 0; i < height; i++)
        {
            int j;
            for (j = 0; j < width; j++)
            {
                RGBTRIPLE rTriple;
                rTriple = tmp[i][j];
                image[i][(width - 1) - j] = rTriple;
            }
        }
    }
    // If width is odd, swap values to middle value, then skip middle value
    // and continue swapping until the end of the array

    else if (width % 2 == 1)
    {
        // Iterates to midpoint of array, copying values from the right side to the midpoint of the array from tmp (copy) to output
        for (i = 0; i < height; i++)
        {
            int j;
            for (j = 0; j < (width / 2); j++)
            {
                RGBTRIPLE rTriple;
                rTriple = tmp[i][j];
                // vv MOST RECENT CHANGE TO CODE BELOW THIS LINE
                // +( - 1)
                image[i][(width - 1) - j] = rTriple;
            }
        }
        for (i = 0; i < height; i++)
        {
            // Iterate from the midpoint of tmp array through end of tmp, copying data to reversed entries in image
            int j;
            for (j = (width / 2); j < width; j++)
            {
                RGBTRIPLE rTriple;
                rTriple = tmp[i][j];
                image[i][(width - 1) - j] = rTriple;
            }
        }
        for (i = 0; i < height; i++)
        {
            int j;
            for (j = 0; j < width; j++)
            {
                if (j == (width / 2))
                {
                    RGBTRIPLE rTriple;
                    rTriple = tmp[i][j];
                    image[i][j] = rTriple;
                }
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Instantiate copied array variable for reading original pixel values
    RGBTRIPLE copy[height][width];
    // Copy original image
    int i;
    for (i = 0; i < height; i++)
    {
        int j;
        for (j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    // With the array copied, now blur pixels into original array from copied array

    // Nested loop which blurs pixels by averaging surrounding pixels inclusively
    for (i = 0; i < height; i++)
    {
        int j;
        for (j = 0; j < width; j++)
        {
            // Conditionals to cover corner pixels
            // Top left corner
            if (i == 0 && j == 0)
            {
            float rgbtRedFloat = ((copy[i][j].rgbtRed + copy[i+1][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i+1][j+1].rgbtRed) / 4.0);
            float rgbtGreenFloat = ((copy[i][j].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i+1][j+1].rgbtGreen) / 4.0);
            float rgbtBlueFloat = ((copy[i][j].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i+1][j+1].rgbtBlue) / 4.0);
            int rgbtRedNew = round(rgbtRedFloat);
            int rgbtGreenNew = round(rgbtGreenFloat);
            int rgbtBlueNew = round(rgbtBlueFloat);
            image[i][j].rgbtRed = rgbtRedNew;
            image[i][j].rgbtGreen = rgbtGreenNew;
            image[i][j].rgbtBlue = rgbtBlueNew;
            }
            // Top Right corner
            else if (i == 0 && j == width - 1)
            {
            float rgbtRedFloat = ((copy[i][j].rgbtRed + copy[i+1][j].rgbtRed + copy[i][j-1].rgbtRed + copy[i+1][j-1].rgbtRed) / 4.0);
            float rgbtGreenFloat = ((copy[i][j].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i][j-1].rgbtGreen + copy[i+1][j-1].rgbtGreen) / 4.0);
            float rgbtBlueFloat = ((copy[i][j].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i][j-1].rgbtBlue + copy[i+1][j-1].rgbtBlue) / 4.0);
            int rgbtRedNew = round(rgbtRedFloat);
            int rgbtGreenNew = round(rgbtGreenFloat);
            int rgbtBlueNew = round(rgbtBlueFloat);
            image[i][j].rgbtRed = rgbtRedNew;
            image[i][j].rgbtGreen = rgbtGreenNew;
            image[i][j].rgbtBlue = rgbtBlueNew;
            }
            // Bottom Left Corner
            else if (i == height - 1&& j == 0)
            {
            float rgbtRedFloat = ((copy[i][j].rgbtRed + copy[i-1][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i-1][j+1].rgbtRed) / 4.0);
            float rgbtGreenFloat = ((copy[i][j].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i-1][j+1].rgbtGreen) / 4.0);
            float rgbtBlueFloat = ((copy[i][j].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i-1][j+1].rgbtBlue) / 4.0);
            int rgbtRedNew = round(rgbtRedFloat);
            int rgbtGreenNew = round(rgbtGreenFloat);
            int rgbtBlueNew = round(rgbtBlueFloat);
            image[i][j].rgbtRed = rgbtRedNew;
            image[i][j].rgbtGreen = rgbtGreenNew;
            image[i][j].rgbtBlue = rgbtBlueNew;
            }
            // Bottom right corner
            else if (i == height - 1 && j == width - 1)
            {
            float rgbtRedFloat = ((copy[i][j].rgbtRed + copy[i-1][j].rgbtRed + copy[i][j-1].rgbtRed + copy[i-1][j-1].rgbtRed) / 4.0);
            float rgbtGreenFloat = ((copy[i][j].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i][j-1].rgbtGreen + copy[i-1][j-1].rgbtGreen) / 4.0);
            float rgbtBlueFloat = ((copy[i][j].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i][j-1].rgbtBlue + copy[i-1][j-1].rgbtBlue) / 4.0);
            int rgbtRedNew = round(rgbtRedFloat);
            int rgbtGreenNew = round(rgbtGreenFloat);
            int rgbtBlueNew = round(rgbtBlueFloat);
            image[i][j].rgbtRed = rgbtRedNew;
            image[i][j].rgbtGreen = rgbtGreenNew;
            image[i][j].rgbtBlue = rgbtBlueNew;
            }
            // Conditional to cover pixels in top row that are not corner pixels
            else if (i == 0 && j > 0 && j < width - 1)
            {
            float rgbtRedFloat = ((copy[i][j].rgbtRed + copy[i+1][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i][j-1].rgbtRed + copy[i+1][j-1].rgbtRed + copy[i+1][j+1].rgbtRed) / 6.0);
            float rgbtGreenFloat = ((copy[i][j].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i][j-1].rgbtGreen + copy[i+1][j-1].rgbtGreen + copy[i+1][j+1].rgbtGreen) / 6.0);
            float rgbtBlueFloat = ((copy[i][j].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i][j-1].rgbtBlue + copy[i+1][j-1].rgbtBlue + copy[i+1][j+1].rgbtBlue) / 6.0);
            int rgbtRedNew = round(rgbtRedFloat);
            int rgbtGreenNew = round(rgbtGreenFloat);
            int rgbtBlueNew = round(rgbtBlueFloat);
            image[i][j].rgbtRed = rgbtRedNew;
            image[i][j].rgbtGreen = rgbtGreenNew;
            image[i][j].rgbtBlue = rgbtBlueNew;
            }
            // Conditional to cover pixels in bottom row that are not corner pixels
            else if (i == height - 1 && j > 0 && j < width)
            {
            float rgbtRedFloat = ((copy[i][j].rgbtRed + copy[i-1][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i][j-1].rgbtRed + copy[i-1][j-1].rgbtRed + copy[i-1][j+1].rgbtRed) / 6.0);
            float rgbtGreenFloat = ((copy[i][j].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i][j-1].rgbtGreen + copy[i-1][j-1].rgbtGreen + copy[i-1][j+1].rgbtGreen) / 6.0);
            float rgbtBlueFloat = ((copy[i][j].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i][j-1].rgbtBlue + copy[i-1][j-1].rgbtBlue + copy[i-1][j+1].rgbtBlue) / 6.0);
            int rgbtRedNew = round(rgbtRedFloat);
            int rgbtGreenNew = round(rgbtGreenFloat);
            int rgbtBlueNew = round(rgbtBlueFloat);
            image[i][j].rgbtRed = rgbtRedNew;
            image[i][j].rgbtGreen = rgbtGreenNew;
            image[i][j].rgbtBlue = rgbtBlueNew;
            }
            // Conditional to cover pixels in left collumn that are not corner pixels
            else if (i > 0 && i < height && j == 0)
            {
            float rgbtRedFloat = ((copy[i][j].rgbtRed + copy[i-1][j].rgbtRed + copy[i+1][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i-1][j+1].rgbtRed + copy[i+1][j+1].rgbtRed) / 6.0);
            float rgbtGreenFloat = ((copy[i][j].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i-1][j+1].rgbtGreen + copy[i+1][j+1].rgbtGreen) / 6.0);
            float rgbtBlueFloat = ((copy[i][j].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i-1][j+1].rgbtBlue + copy[i+1][j+1].rgbtBlue) / 6.0);
            int rgbtRedNew = round(rgbtRedFloat);
            int rgbtGreenNew = round(rgbtGreenFloat);
            int rgbtBlueNew = round(rgbtBlueFloat);
            image[i][j].rgbtRed = rgbtRedNew;
            image[i][j].rgbtGreen = rgbtGreenNew;
            image[i][j].rgbtBlue = rgbtBlueNew;
            }
            // Conditional to cover pixels in right collumn that are not corner pixels
            else if ((j == width - 1) && i > 0 && i < height)
            {
            float rgbtRedFloat = ((copy[i][j].rgbtRed + copy[i-1][j].rgbtRed + copy[i+1][j].rgbtRed + copy[i][j-1].rgbtRed + copy[i-1][j-1].rgbtRed + copy[i+1][j-1].rgbtRed) / 6.0);
            float rgbtGreenFloat = ((copy[i][j].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i][j-1].rgbtGreen + copy[i-1][j-1].rgbtGreen + copy[i+1][j-1].rgbtGreen) / 6.0);
            float rgbtBlueFloat = ((copy[i][j].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i][j-1].rgbtBlue + copy[i-1][j-1].rgbtBlue + copy[i+1][j-1].rgbtBlue) / 6.0);
            int rgbtRedNew = round(rgbtRedFloat);
            int rgbtGreenNew = round(rgbtGreenFloat);
            int rgbtBlueNew = round(rgbtBlueFloat);
            image[i][j].rgbtRed = rgbtRedNew;
            image[i][j].rgbtGreen = rgbtGreenNew;
            image[i][j].rgbtBlue = rgbtBlueNew;
            }
            // Conditional to cover "body" pixels, or pixels surrounded by other pixels
            else
            {
            float rgbtRedFloat = ((copy[i][j].rgbtRed + copy[i-1][j].rgbtRed + copy[i+1][j].rgbtRed + copy[i][j-1].rgbtRed + copy[i][j+1].rgbtRed + copy[i-1][j-1].rgbtRed + copy[i+1][j+1].rgbtRed + copy[i+1][j-1].rgbtRed + copy[i-1][j+1].rgbtRed) / 9.0);
            float rgbtGreenFloat = ((copy[i][j].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i][j-1].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i-1][j-1].rgbtGreen + copy[i+1][j+1].rgbtGreen + copy[i+1][j-1].rgbtGreen + copy[i-1][j+1].rgbtGreen) / 9.0);
            float rgbtBlueFloat = ((copy[i][j].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i][j-1].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i-1][j-1].rgbtBlue + copy[i+1][j+1].rgbtBlue + copy[i+1][j-1].rgbtBlue + copy[i-1][j+1].rgbtBlue) / 9.0);
            int rgbtRedNew = round(rgbtRedFloat);
            int rgbtGreenNew = round(rgbtGreenFloat);
            int rgbtBlueNew = round(rgbtBlueFloat);
            image[i][j].rgbtRed = rgbtRedNew;
            image[i][j].rgbtGreen = rgbtGreenNew;
            image[i][j].rgbtBlue = rgbtBlueNew;
            }
        }
    }
    return;
}
