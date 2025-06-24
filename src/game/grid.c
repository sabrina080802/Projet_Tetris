#include "grid.h"

void addGhostLine(char **grid, int rows, int cols)
{
    for (int r = 0; r < rows - 1; r++)
    {
        for (int c = 0; c < cols; c++)
            grid[r][c] = grid[r + 1][c];
    }

    int fallsCount = (rand() % 2) + 1;
    for (int c = 0; c < cols; c++)
    {
        if (fallsCount > 0 && (rand() % 10) > 4)
        {
            grid[rows - 1][c] = 0;
            fallsCount--;
        }
        else
        {
            grid[rows - 1][c] = 8;
        }
    }
}
char **createGrid(int rows, int cols)
{
    char **grid = malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++)
    {
        grid[i] = malloc(cols * sizeof(char));
        for (int j = 0; j < cols; j++)
            grid[i][j] = 0;
    }

    return grid;
}
void resetGrid(char **grid, int rows, int cols)
{
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            grid[r][c] = 0;
}
void showGhost(char **grid, char **renderingGrid, char **shape, int shapeRow, int shapeCol, int gridRows, int gridCols)
{
    int ghostRow = getLowestNoCollidesRow(shape, shapeRow, shapeCol, grid, gridRows, gridCols);

    if (ghostRow != -1)
        applyShape(shape, ghostRow, shapeCol, renderingGrid, gridRows, gridCols);
}
int getLowestNoCollidesRow(char **shape, int startRow, int startCol, char **grid, int rows, int cols)
{
    for (int r = startRow - 1; r < rows - 1; r++)
    {
        if (collides(shape, r + 1, startCol, grid, rows, cols))
            return r;
    }

    return -1;
}
bool collides(char **shape, int shapeRow, int shapeCol, char **grid, int gridRows, int gridCols)
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (shape[r][c] == 0)
                continue;

            int _x = shapeCol + c;
            int _y = shapeRow + r;
            if (_x < 0 || _x >= gridCols || _y < 0 || _y >= gridRows || grid[_y][_x] != 0)
            {
                return true;
            }
        }
    }

    return false;
}
void applyGrid(char **grid, char **newGrid, int rows, int cols)
{
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            grid[r][c] = newGrid[r][c];
}
char **copyGrid(char **grid, int rows, int cols)
{
    char **gridCopy = malloc(rows * sizeof(char *));
    for (int r = 0; r < rows; r++)
    {
        gridCopy[r] = malloc(sizeof(char) * cols);
        for (int c = 0; c < cols; c++)
            gridCopy[r][c] = grid[r][c];
    }

    return gridCopy;
}
void freeGrid(char **grid, int rows, int cols)
{
    for (int r = 0; r < rows; r++)
        free(grid[r]);

    free(grid);
}
void applyShape(char **shape, int shapeRow, int shapeCol, char **grid, int gridRows, int gridCols)
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (shape[r][c] > 0 && r + shapeRow < gridRows && c + shapeCol < gridCols)
            {
                grid[r + shapeRow][c + shapeCol] = shape[r][c];
            }
        }
    }
}

void rotateShape(char **shape)
{
    char temp[4][4] = {0};

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
            temp[c][3 - r] = shape[r][c];
    }

    int minRow = 4, minCol = 4;
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (temp[r][c])
            {
                if (r < minRow)
                    minRow = r;
                if (c < minCol)
                    minCol = c;
            }
        }
    }

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
            shape[r][c] = (r + minRow < 4 && c + minCol < 4) ? temp[r + minRow][c + minCol] : 0;
    }
}

int getCompletedLinesCount(char **grid, int rows, int cols)
{
    int lineCount = 0;
    for (int r = rows - 1; r >= 0; r--)
    {
        bool isCompleteLine = true;
        for (int c = 0; c < cols && isCompleteLine; c++)
            isCompleteLine = grid[r][c] != 0;

        if (isCompleteLine)
            lineCount++;
    }

    return lineCount;
}
int getRoughness(char **grid, int rows, int cols)
{
    int prevHeight = 0, roughness = 0;
    for (int c = 0; c < cols; c++)
    {
        int height = 0;
        for (int r = 0; r < rows; r++)
        {
            if (grid[r][c])
            {
                height = rows - r;
                break;
            }
        }

        if (c > 0)
            roughness += abs(height - prevHeight);
        prevHeight = height;
    }
    return roughness;
}