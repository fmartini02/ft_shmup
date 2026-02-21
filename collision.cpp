#include "ft_shmup.h"
#include <algorithm>

void Game::checkCollisions()
{
    std::vector<bool> to_remove(entities.size(), false);
    bool player_hit_this_frame = false;   // Evita che il giocatore venga colpito più volte nello stesso frame
    // ========== 1. PROIETTILI GIOCATORE vs NEMICI ==========
    for (size_t i = 0; i < entities.size(); ++i)
    {
        if (entities[i].type == EntityType::PLAYER_BULLET && entities[i].is_active())
        {
            for (size_t j = 0; j < entities.size(); ++j)
            {
                if (i == j || !entities[j].is_active()) continue;
                // CONTROLLA TUTTI I TIPI DI NEMICI
                if ((entities[j].type == EntityType::BASIC_ENEMY ||
                     entities[j].type == EntityType::SHOOTER_ENEMY ||
                     entities[j].type == EntityType::ZIGZAG_ENEMY ||
                     entities[j].type == EntityType::TANK_ENEMY) &&
                    entities[i].x == entities[j].x &&
                    entities[i].y == entities[j].y)
                {
                    to_remove[i] = true;          // Proiettile da rimuovere
                    if (!to_remove[j])            // Nemico non già segnato
                    {
                        to_remove[j] = true;
                        
                        // PUNTEGGIO DIVERSO IN BASE AL TIPO DI NEMICO
                        switch (entities[j].type) {
                            case EntityType::BASIC_ENEMY:
                                addScore(10);
                                break;
                            case EntityType::SHOOTER_ENEMY:
                                addScore(20);  // Più pericoloso = più punti
                                break;
                            case EntityType::ZIGZAG_ENEMY:
                                addScore(15);  // Difficile da colpire = punti medi
                                break;
                            case EntityType::TANK_ENEMY:
                                addScore(30);  // Corazzato = tanti punti
                                break;
                            default:
                                addScore(10);
                        }
                    }
                }
            }
        }
    }
    // ========== 2. PROIETTILI NEMICI vs GIOCATORE ==========
    for (size_t i = 0; i < entities.size() && !player_hit_this_frame; ++i)
    {
        if (entities[i].type == EntityType::ENEMY_BULLET && entities[i].is_active() &&
            entities[i].x == player.x && entities[i].y == player.y)
        {
            to_remove[i] = true;
            if (invincible_frames <= 0)   // Se non è invincibile
            {
                player_hit_this_frame = true;
                lives--;
                invincible_frames = 20;    // 20 frame di invulnerabilità
            }
        }
    }

    // ========== 3. NEMICI vs GIOCATORE ==========
    for (size_t i = 0; i < entities.size() && !player_hit_this_frame; ++i)
    {
        // CONTROLLA TUTTI I TIPI DI NEMICI
        if ((entities[i].type == EntityType::BASIC_ENEMY ||
             entities[i].type == EntityType::SHOOTER_ENEMY ||
             entities[i].type == EntityType::ZIGZAG_ENEMY ||
             entities[i].type == EntityType::TANK_ENEMY) &&
            entities[i].is_active() &&
            entities[i].x == player.x && entities[i].y == player.y)
        {
            to_remove[i] = true;
            if (invincible_frames <= 0)
            {
                player_hit_this_frame = true;
                lives--;
                invincible_frames = 20;
            }
        }
    }
    // ========== 4. RIMOZIONE ENTITÀ SEGNATE ==========
    removeMarkedEntities(to_remove);
}
void Game::removeMarkedEntities(const std::vector<bool>& to_remove)
{
    // Metodo: Rimozione con erase-remove idiom (C++ moderno)
    // NOTA: Questo metodo mantiene l'ordine delle entità superstiti
    auto new_end = std::remove_if(entities.begin(), entities.end(),
        [index = 0, &to_remove](const Entity&) mutable {
            return index < static_cast<int>(to_remove.size()) && to_remove[index++];
        });
    entities.erase(new_end, entities.end());
}
