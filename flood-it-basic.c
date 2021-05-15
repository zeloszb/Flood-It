#include "include/raylib.h"

// Some Defines
#define boxSize         25
#define fields          12
#define upperSpace      60

// Box struct
typedef struct box{
    Vector2 position;
    Color color;
} box;

// Define global variables
static const int screenWidth = 500;
static const int screenHeight = 500;

static int count;
static int max_count;
static int fields_painted;
static int counter;
static int spaceBorders;

static bool pressed = false;
static bool gameover = false;
static bool win = false;
static Vector2 mousePosition;
static Color currentColor = { 0 };

// -------------------------------------------
// All already painted fields are represented in as 1 in 'status[fields][fields]'and all unpainted are represented as 0
// -------------------------------------------
static box boxes[fields][fields];
static int status[fields][fields];

// Function Declaration
static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);

// Main part
int main(void)
{   
    // Initialization of the window
    InitWindow(screenWidth, screenHeight, "Flood-It");
    SetTargetFPS(60);

    // Initialization of all variables
    InitGame();

    // Main loop
    while (!WindowShouldClose())
    {   
        // Update and Draw Game
        UpdateGame();
        DrawGame();
        
    }

    // Close window and OpenGL context
    CloseWindow();

    return 0;
}

// Initialize game variables
void InitGame(void){

    count = 0;
    max_count = 30;
    fields_painted = 0;
    counter = 0;
    mousePosition = (Vector2){0.0f, 0.0f};
    spaceBorders = (screenWidth - fields * boxSize) / 2;

    // Array of all colors
    Color colors[6] = {RED, GREEN, BLUE, ORANGE, YELLOW, PINK};

    // Fill status with zeros
    for (int i = 0; i < fields; i++)
    {
        for (int j = 0; j < fields; j++)
        {
            boxes[i][j].position = (Vector2){spaceBorders + j * boxSize, upperSpace + i * boxSize};
            boxes[i][j].color = colors[GetRandomValue(0, 5)];
            status[i][j] = 0;
        }
    }
    
    // Set the start field and current color
    status[0][0] = 1;
    currentColor = boxes[0][0].color;
    
}

// Update Game
void UpdateGame(void){

    if (!gameover && !win){

        // Gameover when the player runs out of moves
        if (count > max_count)  gameover = true;

        // Win if all fields are painted
        if (fields_painted >= (fields * fields))  win = true;

        mousePosition = GetMousePosition();
        
        // Loop throught array and check for mouse collision
        for (int i = 0; i < fields; i++)
        {
            for (int j = 0; j < fields; j++)
            {   
                // Create rectangle and check collision
                Rectangle rec = (Rectangle){boxes[i][j].position.x, boxes[i][j].position.y, boxSize, boxSize};
                bool collision = CheckCollisionPointRec(mousePosition, rec);

                // Set current color to the color of the pressed field
                if (collision && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    pressed = true;
                    currentColor = boxes[i][j].color;
                    boxes[0][0].color = currentColor;
                    count += 1;
                }
            }
            
        }
        
        // Check if field was pressed
        if (pressed){

            // Go through the array
            for (int i = 0; i < fields; i++)
            {
                for (int j = 0; j < fields; j++)
                {   
                    // Looks left, right, top and bottom for the same color if field is painted
                    if (status[i][j] != 0){
                        counter += 1;
                        if (ColorToInt(boxes[i - 1][j].color) == ColorToInt(currentColor) && status[i - 1][j] == 0)  status[i - 1][j] = 1;
                        if (ColorToInt(boxes[i + 1][j].color) == ColorToInt(currentColor) && status[i + 1][j] == 0)  status[i + 1][j] = 1;
                        if (ColorToInt(boxes[i][j - 1].color) == ColorToInt(currentColor) && status[i][j - 1] == 0 && (j - 1) > 0 )  status[i][j - 1] = 1;
                        if (ColorToInt(boxes[i][j + 1].color) == ColorToInt(currentColor) && status[i][j + 1] == 0)  status[i][j + 1] = 1;
                    }
                }
            }

            // Set and reset Variables
            pressed = false;
            fields_painted = counter;
            counter = 0;
        }
    }

    // Wait for restart
    else{
        if (IsKeyPressed(KEY_ENTER)){
            InitGame();
            gameover = false;
            win = false;
        }
    }
}

// Draw Game
void DrawGame(void){

    ClearBackground(RAYWHITE);

    BeginDrawing();

        if (!gameover && !win){

            // Draw Rectangles
            for (int i = 0; i < fields; i++)
            {
                for (int j = 0; j < fields; j++)
                {   
                    if (status[i][j] == 1) DrawRectangleV(boxes[i][j].position, (Vector2){boxSize, boxSize}, currentColor);
                    else DrawRectangleV(boxes[i][j].position, (Vector2){boxSize, boxSize}, boxes[i][j].color);
                }
            }

            // Draw Mouse Position and a frame
            DrawCircleV(mousePosition, 3, MAROON);
            DrawRectangleLinesEx( (Rectangle){spaceBorders-5, upperSpace-5, (fields * boxSize) + 10, (fields * boxSize) + 10}, 5, BLACK);

            // Draw stats
            DrawText("Flood-It", GetScreenWidth()/2 - MeasureText("Flood-It", 25)/2, 15, 30, BLACK);
            DrawText(FormatText("TURNS REMAIN:\n         %d/%d", count, max_count), spaceBorders, 400, 19, BLACK);
            DrawText(FormatText("FIELDS FILLED:\n         %d/%d", fields_painted, fields * fields), 265, 400, 19, BLACK);
        }

        else{
            if (win) DrawText("You won! Press [ENTER] to restart", GetScreenWidth()/2 - MeasureText("You won! Press [ENTER] to restart", 20)/2, GetScreenHeight()/2 - 50, 20, BLACK);
            else DrawText("Game over! Press [ENTER] to restart",  GetScreenWidth()/2 - MeasureText("Game over! Press [ENTER] to restartt", 20)/2, GetScreenHeight()/2 - 50, 20, BLACK);
        }
    EndDrawing();
}
