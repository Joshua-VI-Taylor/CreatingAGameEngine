#include <DX3D/Game/World.h>
#include <DX3D/Entity/Entity.h>

World::World ( )
{

}

World::~World ( )
{
}

void World::update ( f32 deltaTime )
{
	for (auto e : m_entitiesToDestory)
	{
		m_entities[e->m_typeId].erase ( e );
	}
	m_entitiesToDestory.clear ( );

	for (auto&& [typeId, entities] : m_entities)
	{
		for (auto&& [ptr, entity] : entities) 
		{
			ptr->onUpdate ( deltaTime );
		}
	}
}

void World::createEntityInternal ( Entity* entity, size_t id )
{
	auto entityPtr = std::unique_ptr< Entity> ( entity );
	m_entities[id].emplace ( entity, std::move ( entityPtr ) );
	entity->m_typeId = id; 
	entity->m_world = this;
	entity->onCreate ( );
}

void World::removeEntity ( Entity* entity )
{
	m_entitiesToDestory.emplace ( entity );
}
