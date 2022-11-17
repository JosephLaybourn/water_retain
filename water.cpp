#include "water.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>

static struct chunk
{
  int height;
  int waterLevel;
};

static std::vector<std::vector<chunk>> getVector(const char *filename, int &maxNum)
{
  std::ifstream stream;
  std::vector<std::vector<chunk>> data;
  int row;
  int column;
  std::string tempCount;

  stream.open(filename, std::ifstream::in);

  stream >> column;
  stream >> row;

  data.resize(row + 2);

  for (std::vector<chunk> &colVec : data)
  {
    colVec.resize(column + 2);
    colVec[0].height = 0;
    colVec[0].waterLevel = 0;
    colVec[colVec.size() - 1].height = 0;
    colVec[colVec.size() - 1].waterLevel = 0;
  }

  for (int i = 0; i < column; ++i)
  {
    for (int j = 0; j < row; ++j)
    {
      stream >> data[j + 1][i + 1].height;
      data[j + 1][i + 1].waterLevel = 0;

      maxNum = std::max(data[j + 1][i + 1].height, maxNum);

      // debug code
      //std::cout << data[j + 1][i + 1].height << ' ';
    }
    //std::cout << std::endl;
  }

  return data;
}

static long int collect(const std::vector<std::vector<chunk>> &input)
{
  int column = input.size() - 2;
  int row = input[0].size() - 2;

  long int sum = 0;

  for (int i = 0; i < row; ++i)
  {
    for (int j = 0; j < column; ++j)
    {
      const int rowTemp = j + 1;
      const int colTemp = i + 1;

      sum += input[rowTemp][colTemp].waterLevel;

      // debug code
      //std::cout << '[' << input[rowTemp][colTemp].height << ' ' << input[rowTemp][colTemp].waterLevel << ']';
    }
    //std::cout << std::endl;
  }
  return sum;
}

static void fillWater(std::vector<std::vector<chunk>> &input, int maxNum)
{
  int column = input.size() - 2;
  int row = input[0].size() - 2;

  for (int i = 0; i < row; ++i)
  {
    for (int j = 0; j < column; ++j)
    {
      const int rowTemp = j + 1;
      const int colTemp = i + 1;

      /*int newMax = std::max({ input[rowTemp - 1][colTemp].height, 
                              input[rowTemp][colTemp - 1].height, 
                              input[rowTemp + 1][colTemp].height, 
                              input[rowTemp][colTemp + 1].height });*/

      input[rowTemp][colTemp].waterLevel = maxNum - input[rowTemp][colTemp].height;

      /*if (input[rowTemp][colTemp].waterLevel < 0)
      {
        input[rowTemp][colTemp].waterLevel = 0;
      }*/

      // debug code
      std::cout << "  [" << std::setw(2) << input[rowTemp][colTemp].height << ' ' << std::setw(2) << input[rowTemp][colTemp].waterLevel << ']';
    }
    std::cout << std::endl;
  }
  std::cout << "TOTAL: " << collect(input);
  std::cout << std::endl << std::endl;
}

static void flow(std::vector<std::vector<chunk>> &input)
{
  int column = input.size() - 2;
  int row = input[0].size() - 2;
  bool done = false;

  while (!done)
  {
    done = true;
    for (int i = 0; i < row; ++i)
    {
      for (int j = 0; j < column; ++j)
      {
        const int rowTemp = j + 1;
        const int colTemp = i + 1;

        int drainage = std::min({ input[rowTemp - 1][colTemp].height + input[rowTemp - 1][colTemp].waterLevel,
                                input[rowTemp][colTemp - 1].height + input[rowTemp][colTemp - 1].waterLevel,
                                input[rowTemp + 1][colTemp].height + input[rowTemp + 1][colTemp].waterLevel,
                                input[rowTemp][colTemp + 1].height + input[rowTemp][colTemp + 1].waterLevel });

        if (input[rowTemp][colTemp].waterLevel && drainage < input[rowTemp][colTemp].height + input[rowTemp][colTemp].waterLevel)
        {
          input[rowTemp][colTemp].waterLevel--;
          std::cout << " !";
          done = false;
        }
        else
        {
          std::cout << "  ";
        }

        // debug code
        std::cout << "[" << std::setw(2) << input[rowTemp][colTemp].height << ' ' << std::setw(2) << input[rowTemp][colTemp].waterLevel << ']';
      }
      std::cout << std::endl;
    }
    std::cout << "TOTAL: " << collect(input);
    std::cout << std::endl << std::endl;
  }
}

long int waterret(char const * filename)
{
  int maxNum = 0;

  std::vector<std::vector<chunk>> data = getVector(filename, maxNum);
  
  fillWater(data, maxNum);
  flow(data);

  return collect(data);
}

