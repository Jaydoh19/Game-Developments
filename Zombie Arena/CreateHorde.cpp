#include "ZombieArena.h"
#include "Zombie.h"

// Distance function
float distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

Zombie* createHorde(int numZombies, IntRect arena) {
    Zombie* zombies = new Zombie[numZombies];
    std::vector<Vector2f> usedPositions;

    int maxY = arena.height - 20;
    int minY = arena.top + 20;
    int maxX = arena.width - 20;
    int minX = arena.left + 20;

    for (int i = 0; i < numZombies; i++) {
        float x, y;
        bool positionOK;

        // Find a spawn location that isn't too close to others
        do {
            positionOK = true;
            int side = rand() % 4;

            switch (side) {
            case 0: x = minX; y = rand() % (maxY - minY) + minY; break;
            case 1: x = maxX; y = rand() % (maxY - minY) + minY; break;
            case 2: x = rand() % (maxX - minX) + minX; y = minY; break;
            case 3: x = rand() % (maxX - minX) + minX; y = maxY; break;
            }

            for (const auto& pos : usedPositions) {
                if (distance(x, y, pos.x, pos.y) < 100) {
                    positionOK = false;
                    break;
                }
            }
        } while (!positionOK);

        usedPositions.push_back(Vector2f(x, y));

        int type = rand() % 3;
        zombies[i].spawn(x, y, type, i);
    }

    return zombies;
}