#include "ft_shmup.h"
#include <cstdlib>  // per rand()

// Aggiorna lo scenario (rocce, scorrimento)
// Nota: il movimento delle rocce è gestito in updateEntities()
void Game::updateMap()
{
    scrolling_offset++;   // Incrementa l'offset (non usato qui)

    // Genera casualmente una roccia (SCENERY) con probabilità 1/150 per frame
    if (rand() % 150 == 0)
    {
        Entity obj(
            WIDTH - 1,           // x: appare sul bordo destro
            rand() % HEIGHT,      // y: posizione casuale
            EntityType::SCENERY,
            '#',                  // carattere per la roccia
            -1,                   // dx: si muove verso sinistra
            0                     // dy: nessun movimento verticale
        );
        addEntity(obj);
    }
    
    // Il movimento delle rocce e la rimozione quando escono dallo schermo
    // vengono gestiti automaticamente in updateEntities()
}
