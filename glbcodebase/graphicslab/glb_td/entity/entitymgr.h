//------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/26
// Brief: Manage all the entity in game
//------------------------------------------------------------
#ifndef ENTITY_ENTITYMGR_H_
#define ENTITY_ENTITYMGR_H_

#include "entity.h"

namespace entity {

class EntityMgr {
public:
    static void Initialize();
    static void Update(float dt);
    static void Destroy();

    static int32_t CreateEntity();
    static int32_t AddEntity(Entity* entity);
    static Entity* GetEntity(int32_t id);
    static void RemoveEntity(int32_t id);
};

};  // namespace entity

#endif  // ENTITY_ENTITYMGR_H_