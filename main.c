#include "raylib.h"

static const int screenWidth = 800;
static const int screenHeight = 800;


int main(void){

        InitWindow( screenWidth, screenHeight, "RayChess");  // Initialize window and OpenGL context
        SetTargetFPS(60);

        Texture2D spritesheet = LoadTexture("sprites.png");
        int tileSize = spritesheet.width / 8;
        int dest_Tile_Size = screenWidth / 8;
        int chessPieces = 8 * tileSize;

	while (!WindowShouldClose())
	{

		ClearBackground(RAYWHITE);

        Rectangle chessboard_s = {0.0f, 0.0f, (float) (8 * tileSize) , (float)(8 * tileSize)};

        Rectangle chessboard_d = {0.0f, 0.0f, (float) (8 * dest_Tile_Size) , (float)(8 * dest_Tile_Size)};

        DrawTexturePro(spritesheet, chessboard_s, chessboard_d, (Vector2){0,0}, 0.0f, WHITE ); 
        for (int row = 0; row < 2; row++){
            for(int col = 0; col < 6; col ++)
            {
                Rectangle sourcerec = {(float)(col * tileSize), (float)(chessPieces +(row * tileSize)), (float)tileSize, (float)tileSize};

                Rectangle destrec = {20 + (float)(col * tileSize), 20 + (row * tileSize), (float)tileSize, (float)tileSize };



                //DrawTexturePro(spritesheet, sourcerec, destrec, (Vector2){0,0}, 0.0f, WHITE ); 

            }
        }

		EndDrawing();
}
UnloadTexture(spritesheet);
CloseWindow();
return 0;

}
