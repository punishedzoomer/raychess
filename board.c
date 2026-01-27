#include "raylib.h"
#include <stdlib.h> // Required for malloc and free

// --- DEFINITIONS ---
typedef enum { BISHOP=0, KING, KNIGHT, PAWN, QUEEN, ROOK } PieceType;
typedef enum { SIDE_BLACK=0, SIDE_WHITE=1 } PieceColor;

// THE DYNAMIC NODE
typedef struct PieceNode {
    int x;                  // Grid X (0-7)
    int y;                  // Grid Y (0-7)
    PieceType type;
    PieceColor color;
    struct PieceNode* next; // Pointer to the next piece in the chain
} PieceNode;

// --- HELPER FUNCTION: Create a new piece and link it ---
// This function allocates memory for a new piece and adds it to the START of the list
void AddPiece(PieceNode** head, int x, int y, PieceType type, PieceColor color) {
    // 1. Allocate memory dynamically (This is the "dynamic" part!)
    PieceNode* newPiece = (PieceNode*)malloc(sizeof(PieceNode));
    
    // 2. Set the data
    newPiece->x = x;
    newPiece->y = y;
    newPiece->type = type;
    newPiece->color = color;
    
    // 3. Link it: Point this new piece to the current head
    newPiece->next = *head;
    
    // 4. Update the head to point to this new piece
    *head = newPiece;
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Raychess");

    Texture2D spriteSheet = LoadTexture("sprites.png");
    
    // Math constants
    int srcTileSize = spriteSheet.width / 8;
    int destTileSize = screenWidth / 8;
    int srcPiecesStartY = 8 * srcTileSize;

    // --- SETUP: DYNAMIC LINKED LIST ---
    // We start with an empty list (NULL pointer)
    PieceNode* pieceList = NULL;

    // Helper array for the back row types
    PieceType backRow[] = { ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };

    // Initialize the pieces dynamically
    for (int i = 0; i < 8; i++) {
        // Add Black Pawns (Row 1)
        AddPiece(&pieceList, i, 1, PAWN, SIDE_BLACK);
        // Add Black Back Row (Row 0)
        AddPiece(&pieceList, i, 0, backRow[i], SIDE_BLACK);

        // Add White Pawns (Row 6)
        AddPiece(&pieceList, i, 6, PAWN, SIDE_WHITE);
        // Add White Back Row (Row 7)
        AddPiece(&pieceList, i, 7, backRow[i], SIDE_WHITE);
    }

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw Board Background (Same as before)
        Rectangle srcBoard = {0, 0, (float)srcTileSize*8, (float)srcTileSize*8};
        Rectangle destBoard = {0, 0, (float)screenWidth, (float)screenHeight};
        DrawTexturePro(spriteSheet, srcBoard, destBoard, (Vector2){0,0}, 0.0f, WHITE);

        // --- RENDER LOOP: TRAVERSE THE LIST ---
        // Instead of looping 0..7, we follow the pointers
        
        PieceNode* current = pieceList; // Start at the head
        
        while (current != NULL) {
            
            // 1. CALCULATE SOURCE (Using current->type and current->color)
            int margin = 2;
            float srcX = (current->type * srcTileSize) + margin;
            float srcY = srcPiecesStartY + (current->color * srcTileSize) + margin;

            Rectangle sourceRec = {
                srcX, srcY,
                (float)srcTileSize - (margin * 2),
                (float)srcTileSize - (margin * 2)
            };

            // 2. CALCULATE DESTINATION (Using current->x and current->y)
            Rectangle destRec = {
                (float)(current->x * destTileSize),
                (float)(current->y * destTileSize),
                (float)destTileSize,
                (float)destTileSize
            };

            DrawTexturePro(spriteSheet, sourceRec, destRec, (Vector2){0,0}, 0.0f, WHITE);

            // 3. MOVE TO NEXT PIECE
            current = current->next;
        }

        EndDrawing();
    }

    // --- CLEANUP ---
    // With malloc, you MUST free the memory when done
    PieceNode* current = pieceList;
    while (current != NULL) {
        PieceNode* temp = current;
        current = current->next;
        free(temp);
    }

    UnloadTexture(spriteSheet);
    CloseWindow();

    return 0;
}
