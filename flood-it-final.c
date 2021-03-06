#include "include/raylib.h"

// Include Raygui
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_STATIC
#include "include/raygui.h"

// Some Defines
#define boxSize         25
#define upperSpace      125

// Box struct
typedef struct box{
    Vector2 position;
    Color color;
} box;

typedef struct colorCount{
    Color color;
    int amount;
} colorCount;


// Define global variables
static const int screenWidth = 550;
static const int screenHeight = 550;

static int fields;
static int new_fields;
static int fields_painted;
static int counter;
static int spaceBorders;
static int minmoves;
static int moves;

static int dropdownBoxActive1;
static int dropdownBoxActive2;

static char choices[128];
static int current_pallet_int;
static int new_pallet;
static char pallets[128];
static int pallet_length;

static bool dropdownEditMode1 = false;
static bool dropdownEditMode2 = false;
static bool pressed = false;
static bool gameover = false;
static bool win = false;

static Texture2D crone_single;
static Texture2D empty_crone;

static Vector2 mousePosition;
static Color currentColor = { 0 };

// -------------------------------------------
// All painted fields are represented in as 1 in 'status[fields][fields]'and all unpainted are represented as 0
// -------------------------------------------
static box boxes[128][128];
static int status[128][128];

// Function Declaration
static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static int calcMinMoves(int pallet_length, Color pallet[], box boxcopy[128][128]);
static int CalcResult(int minmoves, int moves);

// Main part
int main(void)
{   
    // Initialization of the window
    InitWindow(screenWidth, screenHeight, "Flood-It");
    SetTargetFPS(60);

    // Some pre-initalization
    fields = 12;
    new_fields = 4;
    current_pallet_int = 0;

    // Load Texture
    crone_single = LoadTexture("./resources/crone_single.png");
    empty_crone = LoadTexture("./resources/emty_crone.png");

    // Set status of dropdownboxes
    dropdownBoxActive1 = 0;
    dropdownBoxActive2 = 0;

    // Copy text
    strcat(choices, "4x4;6x6;8x8;10x10;12x12;14x14");
    strcat(pallets, "NORMAL;PEACH BLUE;OCEAN;MATT SPRING;CRYOLA;GREEN;FLICKR PINK;SIENNA;CREAM");

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

// Calculate the stars based on the min. possible moves
int CalcResult(int minmoves, int moves){
    if (moves <= minmoves)  return 5;
    if (moves <= minmoves + 2)  return 4;
    if (moves <= minmoves + 4)  return 3;
    if (moves <= minmoves + 6)  return 2;
    if (moves <= minmoves + 8)  return 1;
    if (moves <= minmoves + 12)  return 0;
    else  return 0;
}

// Initialize game variables
void InitGame(void){
    moves = 0;
    fields_painted = 0;
    mousePosition = (Vector2){0.0f, 0.0f};
    spaceBorders = (screenWidth - fields * boxSize) / 2;


    // Color pallets
    Color peach_blue[5] = {(Color){38, 70, 83, 255}, (Color){42, 157, 143, 255}, (Color){233, 196, 106, 255}, (Color){244, 162, 97, 255}, (Color){231, 111, 81, 255}}; // Preview: https://coolors.co/264653-2a9d8f-e9c46a-f4a261-e76f51
    Color ocean[5] = {(Color){5, 102, 141, 255}, (Color){2, 128, 144, 255}, (Color){0, 168, 150, 255}, (Color){2, 195, 154, 255}, (Color){240, 243, 189, 255}}; // Preview: https://coolors.co/05668d-028090-00a896-02c39a-f0f3bd
    Color matt_spring[5] = { (Color){91, 192, 235, 255}, (Color){253, 231, 76, 255}, (Color){155, 197, 61, 255}, (Color){229, 89, 52, 255}, (Color){250, 121, 33, 255}}; // Preview: https://coolors.co/5bc0eb-fde74c-9bc53d-e55934-fa7921
    Color colors[6] = {RED, GREEN, BLUE, ORANGE, YELLOW, PINK};
    Color green[6] = { (Color){0, 127, 95, 255}, (Color){85, 166, 48, 255}, (Color){170, 204, 0, 255}, (Color){212, 215, 0, 255}, (Color){255, 255, 63, 255}, (Color){43, 147, 72, 255}}; //Preview: https://coolors.co/007f5f-2b9348-55a630-80b918-aacc00-bfd200-d4d700-dddf00-eeef20-ffff3f
    Color cryola[6] = {(Color){217, 237, 146, 255}, (Color){153, 217, 140, 255}, (Color){82, 182, 154, 255}, (Color){52, 160, 164, 255}, (Color){26, 117, 159, 255}, (Color){24, 78, 119, 255}}; // Preview: https://coolors.co/d9ed92-b5e48c-99d98c-76c893-52b69a-34a0a4-168aad-1a759f-1e6091-184e77
    Color flickr_pink[7] = {(Color){247, 37, 133, 255}, (Color){114, 9, 183, 255}, (Color){67, 97, 238, 255}, (Color){72, 12, 168, 255}, (Color){63, 55, 201, 255}, (Color){72, 149, 239, 255}, (Color){76, 201, 240, 255}}; // Preview: https://coolors.co/f72585-b5179e-7209b7-560bad-480ca8-3a0ca3-3f37c9-4361ee-4895ef-4cc9f0
    Color sienna[8] = {(Color){55, 6, 23, 255}, (Color){106, 4, 15, 255}, (Color){157, 2, 8, 255}, (Color){208, 0, 0, 255}, (Color){220, 47, 2, 255}, (Color){232, 93, 4, 255}, (Color){244, 140, 6, 255}, (Color){255, 186, 8, 255}}; // Preview: https://coolors.co/03071e-370617-6a040f-9d0208-d00000-dc2f02-e85d04-f48c06-faa307-ffba08
    Color cream[9] = {(Color){255, 173, 173, 255}, (Color){255, 214, 165, 255}, (Color){253, 255, 182, 255}, (Color){202, 255, 191, 255}, (Color){155, 246, 255, 255}, (Color){160, 196, 255, 255}, (Color){189, 178, 255, 255}, (Color){255, 198, 255, 255}, (Color){255, 229, 217, 255}}; // Preview: https://coolors.co/ffadad-ffd6a5-fdffb6-caffbf-9bf6ff-a0c4ff-bdb2ff-ffc6ff-fffffc
    
    // Array of the colors
    Color *pallet[] = {colors, peach_blue, ocean, matt_spring, cryola, green, flickr_pink, sienna, cream};

    // Length
    int pallet_length_array[] = {
        sizeof(colors) / sizeof(colors[0]),
        sizeof(peach_blue) / sizeof(peach_blue[0]),
        sizeof(ocean) / sizeof(ocean[0]),
        sizeof(matt_spring) / sizeof(matt_spring[0]),
        sizeof(cryola) / sizeof(cryola[0]),
        sizeof(green) / sizeof(green[0]),
        sizeof(flickr_pink) / sizeof(flickr_pink[0]),
        sizeof(sienna) / sizeof(sienna[0]),
        sizeof(cream) / sizeof(cream[0]) 
    };

    // Fill status with zeros
    for (int i = 0; i < fields; i++)
    {
        for (int j = 0; j < fields; j++)
        {
            boxes[i][j].position = (Vector2){spaceBorders + j * boxSize, upperSpace + i * boxSize};
            int length = pallet_length_array[current_pallet_int];
            //printf("Length: %d\n", length);
            boxes[i][j].color = pallet[current_pallet_int][GetRandomValue(0, length - 1)];
            status[i][j] = 0;
        }
    }
    
    // Set the start field and current color
    status[0][0] = 1;
    currentColor = boxes[0][0].color;

    // Go throught the array again
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

    // Set and reset Variables, get the minimal moves
    fields_painted = counter;
    counter = 0;
    pallet_length = pallet_length_array[current_pallet_int];
    minmoves = calcMinMoves(pallet_length, pallet[current_pallet_int], boxes);
}

// Update Game
void UpdateGame(void){

    if (!gameover && !win){

        // Win if all fields are painted
        if (fields_painted >= (fields * fields)){
            win = true;
            
        }

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
                if (collision && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && dropdownEditMode1 != true && dropdownEditMode2 != true){
                    pressed = true;
                    currentColor = boxes[i][j].color;
                    boxes[0][0].color = currentColor;
                    
                }
            }
        }
        
        // Check if field was pressed
        if (pressed){
            moves++;
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
                        if (ColorToInt(boxes[i][j - 1].color) == ColorToInt(currentColor) && status[i][j - 1] == 0 && (j - 1) >= 0 )  status[i][j - 1] = 1;
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

        if (!win){

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

            // Draw stats and text
            DrawText("Flood-It", GetScreenWidth()/2 - MeasureText("Flood-It", 25)/2, 15, 30, BLACK);
            DrawText("Theme:", 95, 60, 20, BLACK);
            DrawText("Size:", 228, 60, 20, BLACK);
            DrawText("New Game:", 365, 60, 20, BLACK);

            DrawText(FormatText("Moves:\n   %d", moves), 140, (upperSpace + (fields * boxSize) + 20), 19, BLACK);
            DrawText(FormatText("FIELDS FILLED:\n       %d/%d", fields_painted, fields * fields), 285, (upperSpace + (fields * boxSize) + 20), 19, BLACK);

            // Theme selection
            if (GuiDropdownBox((Rectangle){95, 85, 110, 20}, pallets , &dropdownBoxActive2, dropdownEditMode2)){
                dropdownEditMode2 = !dropdownEditMode2;
                new_pallet = dropdownBoxActive2;
            }

            // Size selection
            if (GuiDropdownBox((Rectangle){ 228, 85, 110, 20}, choices, &dropdownBoxActive1, dropdownEditMode1)){
                dropdownEditMode1 = !dropdownEditMode1;

                if (dropdownBoxActive1 == 0)  new_fields = 4;
                if (dropdownBoxActive1 == 1)  new_fields = 6;
                if (dropdownBoxActive1 == 2)  new_fields = 8;
                if (dropdownBoxActive1 == 3)  new_fields = 10;
                if (dropdownBoxActive1 == 4)  new_fields = 12;
                if (dropdownBoxActive1 == 5)  new_fields = 14;
            }
            
            // Create new game
            if(GuiButton((Rectangle){ 365, 85, 110, 20}, "Create")){
                fields = new_fields;
                current_pallet_int = new_pallet;
                InitGame();
            }

        }

        else{

            DrawText("Press [ENTER] to restart", GetScreenWidth()/2 - MeasureText("Press [ENTER] to restart", 20)/2, GetScreenHeight()/2, 20, BLACK);
            int result = CalcResult(minmoves, moves);
            int resultBorders =  (screenWidth - (5 * 50)) / 2;
            
            for (int i = 0; i < 5; i++){
                if (i < result)  DrawTextureEx(crone_single, (Vector2){resultBorders + (i * 50), 185}, 0.0, 2.0, RAYWHITE);
                if (i >= result)  DrawTextureEx(empty_crone, (Vector2){resultBorders + (i * 50), 185}, 0.0, 2.0, RAYWHITE);
            }

            if (result == 5)  DrawText("Amazing!", GetScreenWidth()/2 - MeasureText("Amazing!", 40)/2, 125, 40, BLACK);
            if (result == 4)  DrawText("Good job!", GetScreenWidth()/2 - MeasureText("Good job!", 40)/2, 125, 40, BLACK);
            if (result == 3)  DrawText("Well done!", GetScreenWidth()/2 - MeasureText("Well done!", 40)/2, 125, 40, BLACK);
            if (result == 2)  DrawText("Not that good", GetScreenWidth()/2 - MeasureText("Not that good", 40)/2, 125, 40, BLACK);
            if (result == 1)  DrawText("Ouch - that was bad", GetScreenWidth()/2 - MeasureText("Ouch - that was bad", 40)/2, 125, 40, BLACK);
            if (result == 0)  DrawText("What did you do?", GetScreenWidth()/2 - MeasureText("What did you do?", 40)/2, 125, 40, BLACK);

        }
    EndDrawing();
}

// Function to calulate the minimum number of moves, based on the maximum amount of colors around the painted fields
// ******** Note: The algorythm isn't perfect, so there are some mistakes sometimes ******** 
int calcMinMoves(int pallet_length, Color pallet[], box boxcopy[128][128]){

    // Initalize a copy of the status
    int statuscopy[fields][fields];

    for (int i = 0; i < fields; i++)
    {
        for (int j = 0; j < fields; j++)
        {
            statuscopy[i][j] = 0; 
        }
    }
    statuscopy[0][0] = 1;
    
    // Create a counter for the amount of each color
    colorCount ccounter[pallet_length];

    // Fill in all colors of the pallet
    for (int t = 0; t < pallet_length; t++){
        ccounter[t].color.r = pallet[t].r;
        ccounter[t].color.g = pallet[t].g;
        ccounter[t].color.b = pallet[t].b;
        ccounter[t].color.a = pallet[t].a;
        ccounter[t].amount = 0;
    }

    // Define some variables (most of them are just copys)
    Color currentColorPreCalc = currentColor;
    bool done = false;
    int highest = 0;
    int position = 0;
    int countercopy = 0;
    int fields_painted_copy = 0;
    int moves = 0;

    // Go till the last move
    while (!done){

        // Loop through every field
        for (int i = 0; i < fields; i++)
        {
            for (int j = 0; j < fields; j++)
            {   
                // Looks left, right, top and bottom for the same color if field is painted
                if (statuscopy[i][j] != 0){
                        ;
                        // Loop through ccounter and increase the amount of the given color
                        for (int k=0; k < pallet_length; k++){
                            if (ColorToInt(boxcopy[i - 1][j].color) == ColorToInt(ccounter[k].color) && statuscopy[i - 1][j] == 0)  ccounter[k].amount += 1;
                            if (ColorToInt(boxcopy[i + 1][j].color) == ColorToInt(ccounter[k].color) && statuscopy[i + 1][j] == 0)  ccounter[k].amount += 1;
                            if (ColorToInt(boxcopy[i][j - 1].color) == ColorToInt(ccounter[k].color) && statuscopy[i][j - 1] == 0)  ccounter[k].amount += 1;
                            if (ColorToInt(boxcopy[i][j + 1].color) == ColorToInt(ccounter[k].color) && statuscopy[i][j + 1] == 0)  ccounter[k].amount += 1;
                        }
                }
            }
        }

        highest = 0;
        position = 0;

        // Look what colors are the most
        for (int u = 0; u < pallet_length; u++){
            if (ccounter[u].amount >= highest){
                highest = ccounter[u].amount;
                position = u;
            }
        }

        // Set the current color
        currentColorPreCalc = ccounter[position].color;
        
        // Update the status by the color choosen above (just like in `UpdateGame`)
        for (int i = 0; i < fields; i++)
        {
            for (int j = 0; j < fields; j++)
            {   
                // Looks left, right, top and bottom for the same color if field is painted
                if (statuscopy[i][j] != 0){
                    countercopy += 1;
                    if (ColorToInt(boxcopy[i - 1][j].color) == ColorToInt(currentColorPreCalc) && statuscopy[i - 1][j] == 0)  statuscopy[i - 1][j] = 1;
                    if (ColorToInt(boxcopy[i + 1][j].color) == ColorToInt(currentColorPreCalc) && statuscopy[i + 1][j] == 0)  statuscopy[i + 1][j] = 1;
                    if (ColorToInt(boxcopy[i][j - 1].color) == ColorToInt(currentColorPreCalc) && statuscopy[i][j - 1] == 0 && (j - 1) >= 0 )  statuscopy[i][j - 1] = 1;
                    if (ColorToInt(boxcopy[i][j + 1].color) == ColorToInt(currentColorPreCalc) && statuscopy[i][j + 1] == 0 && (j + 1) < fields)  statuscopy[i][j + 1] = 1;
                }
            }
        }

        // Set and reset Variables
        fields_painted_copy = countercopy;
        countercopy = 0;

        // Clear data
        for (int t = 0; t < pallet_length; t++){
            ccounter[t].amount = 0; 
        }

        int highest = 0;
        counter = 0;
        moves += 1;
        position = 0;

        // Check if the all fields are painted
        if (fields_painted_copy >= (fields * fields))  done = true;
    }

    return moves;
}
