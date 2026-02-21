#include "ft_shmup.h"
#include <cstdlib>
#include <algorithm>
#include <cmath>

// ============================================================================
// NOTA: In C++ non possiamo avere variabili statiche globali nell'header
// quindi next_id è un membro privato della classe Game
// ============================================================================

// ENTITY MANAGER - Gestione base delle entità
// Aggiunge una nuova entità al gioco
// Verifica che non si superi il limite MAX_ENTITIES
void Game::addEntity(const Entity& e)
{
    if (entities.size() < MAX_ENTITIES) {
        Entity new_entity = e;
        new_entity.id = next_id++;      // Assegna ID univoco
        new_entity.active = true;        // Attiva l'entità
        entities.push_back(new_entity);
    }
}
// Rimuove un'entità all'indice specificato
// Utilizza la tecnica "swap & pop" per efficienza (O(1) invece di O(n))
void Game::removeEntity(int index)
{
    if (index < 0 || index >= static_cast<int>(entities.size()))
        return;
    
    // Swap con l'ultimo elemento e rimuovi l'ultimo
    // Questo evita di dover shiftare tutti gli elementi successivi
    if (index < static_cast<int>(entities.size()) - 1) {
        std::swap(entities[index], entities.back());
    }
    entities.pop_back();
}
// MOVIMENTO ENTITÀ - Ogni tipo si muove secondo pattern diversi
// Muove tutti i proiettili in base al loro vettore (dx, dy)
void Game::moveBullets()
{
    for (size_t i = 0; i < entities.size(); i++)
    {
        Entity& e = entities[i];
        if (e.type == EntityType::PLAYER_BULLET || e.type == EntityType::ENEMY_BULLET)
        {
            e.x += e.dx;  // Movimento orizzontale
            e.y += e.dy;  // Movimento verticale
        }
    }
}
// Muove tutti i nemici con pattern specifici per tipo
void Game::moveEnemies()
{
    for (size_t i = 0; i < entities.size(); i++)
    {
        Entity& e = entities[i];
        
        switch (e.type) {
            case EntityType::BASIC_ENEMY:
                // BASIC: Scende dritto verso il basso
                // Comportamento semplice, prevedibile
                e.y += 1;
                break;
                
            case EntityType::SHOOTER_ENEMY:
                // SHOOTER: Come basic ma con AI di sparo avanzata
                // (vedi enemyShoot per il comportamento di sparo)
                e.y += 1;
                break;
                
            case EntityType::ZIGZAG_ENEMY:
                // ZIGZAG: Movimento a zig zag
                // Usa dx per movimento laterale, cambia direzione ogni 10 frame
                e.y += 1;
                e.x += e.dx;
                
                // Cambia direzione periodicamente per effetto zigzag
                // NOTA: static counter è condiviso tra tutti i nemici!
                // Per un controllo individuale, dovremmo usare una variabile membro
                static int zigzag_counter = 0;
                zigzag_counter++;
                if (zigzag_counter % 10 == 0) {
                    e.dx = (rand() % 3) - 1;  // -1, 0, o 1
                }
                
                // Limita ai bordi dello schermo
                if (e.x < 0) { e.x = 0; e.dx = 1; }
                if (e.x >= WIDTH) { e.x = WIDTH - 1; e.dx = -1; }
                break;
                
            case EntityType::TANK_ENEMY:
                // TANK: Più lento (33% velocità)
                // Corazzato, lento ma pericoloso
                if (rand() % 3 == 0) {  // 33% di probabilità di muoversi
                    e.y += 1;
                }
                break;
                
            default:
                // Altri tipi (scenery, ecc.) non gestiti qui
                break;
        }
        // Rimuovi se esce dallo schermo (verrà gestito da removeOffscreenEntities)
        if (e.y >= HEIGHT) {
            e.active = false;  // Segnala per la rimozione
        }
    }
}
// Muove gli elementi di scenario (rocce, stelle, ecc.)
void Game::moveScenery()
{
    for (size_t i = 0; i < entities.size(); i++)
    {
        Entity& e = entities[i];
        if (e.type == EntityType::SCENERY)
        {
            e.x += e.dx;  // Di solito dx = -1 (verso sinistra)
        }
    }
}
// RIMOZIONE - Pulisce le entità uscite dallo schermo
void Game::removeOffscreenEntities()
{
    std::vector<bool> to_remove(entities.size(), false);
    
    for (size_t i = 0; i < entities.size(); i++)
    {
        const Entity& e = entities[i];
        
        // Proiettili fuori schermo
        if ((e.type == EntityType::PLAYER_BULLET || e.type == EntityType::ENEMY_BULLET) &&
            (e.x < 0 || e.x >= WIDTH || e.y < 0 || e.y >= HEIGHT))
        {
            to_remove[i] = true;
        }
        // Nemici fuori schermo (in basso)
        else if ((e.type == EntityType::BASIC_ENEMY ||
                  e.type == EntityType::SHOOTER_ENEMY ||
                  e.type == EntityType::ZIGZAG_ENEMY ||
                  e.type == EntityType::TANK_ENEMY) && e.y >= HEIGHT)
        {
            to_remove[i] = true;
            // NOTA: Quando un nemico esce in basso, il giocatore NON perde vite
            // Potremmo aggiungere una penalità qui se volessimo
        }
        // Scenario fuori schermo (a sinistra)
        else if (e.type == EntityType::SCENERY && e.x < 0)
        {
            to_remove[i] = true;
        }
    }
    
    // Rimuovi dal fondo per non invalidare gli indici
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; i--)
    {
        if (to_remove[i])
            removeEntity(i);
    }
}
// SPAWN NEMICI - Genera nuovi nemici con pattern vari
void Game::spawnEnemies()
{
    // SPAWN CASUALE - 8% di probabilità per frame
    if (rand() % 100 < 3) // - Ridotto da 8% a 3%
    {
        int type = rand() % 4;  // 0-3 per i 4 tipi
        int x = rand() % WIDTH;
        
        switch (type) {
            case 0:  // BASIC_ENEMY
            {
                Entity enemy(
                    x, 0, EntityType::BASIC_ENEMY, 'V', 0, 1
                );
                addEntity(enemy);
                break;
            }
            case 1:  // SHOOTER_ENEMY
            {
                Entity enemy(
                    x, 0, EntityType::SHOOTER_ENEMY, 'W', 0, 1
                );
                addEntity(enemy);
                break;
            }
            case 2:  // ZIGZAG_ENEMY
            {
                Entity enemy(
                    x, 0, EntityType::ZIGZAG_ENEMY, 'Z', 
                    (rand() % 3) - 1,  // dx iniziale casuale (-1, 0, 1)
                    1
                );
                addEntity(enemy);
                break;
            }
            case 3:  // TANK_ENEMY
            {
                Entity enemy(
                    x, 0, EntityType::TANK_ENEMY, 'T', 0, 0  // dy=0 perché gestito nel movimento
                );
                addEntity(enemy);
                break;
            }
        }
    }
    // ONDE SPECIALI - Ogni 150 frame
    static int wave_counter = 0;
    wave_counter++;
    // ONDE SPECIALI - Ogni 250 frame invece di 150
    if (wave_counter % 250 == 0) {
        // Onda di 5 nemici in formazione orizzontale
        int start_x = rand() % (WIDTH - 10);
        for (int i = 0; i < 5; i++) {
            Entity enemy(
                start_x + i * 2,
                0,
                (i % 2 == 0) ? EntityType::BASIC_ENEMY : EntityType::SHOOTER_ENEMY,
                (i % 2 == 0) ? 'V' : 'W',
                0,
                1
            );
            addEntity(enemy);
        }
    }
}
// SPARO NEMICI - Ogni tipo ha pattern di sparo diverso
void Game::enemyShoot()
{
    for (size_t i = 0; i < entities.size(); i++)
    {
        if (!entities[i].is_active()) continue;
        
        Entity& e = entities[i];
        
        // Gestione cooldown individuale per nemico
        if (e.shoot_cooldown > 0) {
            e.shoot_cooldown--;
            continue;
        }
        // Probabilità base di sparo per tipo
        int shoot_probability = 0;
      
        switch (e.type) {
            case EntityType::BASIC_ENEMY:
                shoot_probability = 2;  // 2% - Nemico base spara poco
                break;
                
            case EntityType::SHOOTER_ENEMY:
                shoot_probability = 15; // 15% - Spara molto!
                break;
                
            case EntityType::ZIGZAG_ENEMY:
                shoot_probability = 5;  // 5% - Normale
                break;
                
            case EntityType::TANK_ENEMY:
                shoot_probability = 10; // 10% - Spara ma è lento
                break;
                
            default:
                continue;  // Non spara
        }
        
        // Controlla se spara (deve avere spazio sotto)
        if (e.y < HEIGHT - 1 && rand() % 100 < shoot_probability)
        {
            // Nemici normali: sparo singolo
            if (e.type == EntityType::BASIC_ENEMY || 
                e.type == EntityType::ZIGZAG_ENEMY ||
                e.type == EntityType::TANK_ENEMY) {
                
                Entity bullet(
                    e.x,
                    e.y + 1,
                    EntityType::ENEMY_BULLET,
                    '*',
                    0,
                    2  // Più veloce del nemico (dy=2)
                );
                addEntity(bullet);
                e.shoot_cooldown = 15;  // 15 frame di pausa
            }
            
            // SHOOTER_ENEMY: sparo TRIPLO (più pericoloso!)
            else if (e.type == EntityType::SHOOTER_ENEMY) {
                for (int offset = -1; offset <= 1; offset++) {
                    if (e.x + offset >= 0 && e.x + offset < WIDTH) {
                        Entity bullet(
                            e.x + offset,
                            e.y + 1,
                            EntityType::ENEMY_BULLET,
                            '*',
                            0,
                            2
                        );
                        addEntity(bullet);
                    }
                }
                e.shoot_cooldown = 30;  // Cooldown più lungo per la raffica
            }
        }
    }
}
// ============================================================================
// UPDATE PRINCIPALE - Coordina tutte le fasi
// ============================================================================
void Game::updateEntities()
{
    // FASE 1: Movimento - Tutto si muove
    moveBullets();     // Proiettili in base a dx, dy
    moveEnemies();     // Nemici con pattern specifici
    moveScenery();     // Scenario (rocce, stelle)
    // FASE 2: Rimozione - Entità fuori schermo
    removeOffscreenEntities();   
    // FASE 3: Spawn - Nuovi nemici
    spawnEnemies();
    // FASE 4: Sparo - Nemici sparano
    enemyShoot();
    // NOTA: Le collisioni sono gestite separatamente in collision.cpp
    // NOTA: Il player è gestito separatamente
}
// ============================================================================
// TABELLA RIASSUNTIVA NEMICI:
// ============================================================================
/*
| Tipo          | Carattere | Movimento                | Sparo    | Punti |
|---------------|-----------|--------------------------|----------|-------|
| BASIC_ENEMY   | 'V'       | Dritto verso il basso    | 2%       | 10    |
| SHOOTER_ENEMY | 'W'       | Dritto verso il basso    | 15% (x3) | 20    |
| ZIGZAG_ENEMY  | 'Z'       | Zig zag laterale         | 5%       | 15    |
| TANK_ENEMY    | 'T'       | Lento (33% velocità)     | 10%      | 30    |
*/
