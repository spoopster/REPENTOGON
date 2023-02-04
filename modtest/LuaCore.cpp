#include <map>
#include <stdexcept>

#include <lua.hpp>

#include "LuaCore.h"

namespace lua {
	std::map<Metatables, void*> _metatables;
	std::map<std::string, Metatables> _metatable_idx_from_name;
	bool _metatable_idx_from_name_initialized = false;

	class MetatableAlreadyRegisteredException : public std::exception {
	public:
		MetatableAlreadyRegisteredException(Metatables metatable) : _metatable(metatable) {
			snprintf(_err, 256, "Metatable %d already registed\n", (int)_metatable);
		}

		const char* what() const override {
			return _err;
		}

	private:
		Metatables _metatable;
		char _err[256];
	};

	class UnregistedMetatableException : public std::exception {
	public:
		UnregistedMetatableException(Metatables metatable) : _metatable(metatable) {
			snprintf(_err, 256, "Attempt to get unregistered metatable %d\n", (int)_metatable);
		}

		const char* what() const override {
			return _err;
		}

	private:
		Metatables _metatable;
		char _err[256];
	};

	class UnknownMetatableNameException : public std::exception {
	public:
		UnknownMetatableNameException(std::string const& name) {
			_err = new char[256 + name.size()];
		}

		~UnknownMetatableNameException() {
			delete[] _err;
		}

		const char* what() const override {
			return _err;
		}

	private:
		char* _err;
	};

	void UnloadMetatables() {
		_metatables.clear();
	}

	void GetMetatable(lua_State* L, Metatables metatable) {
		auto iter = _metatables.find(metatable);

		if (iter == _metatables.end()) {
			throw UnregistedMetatableException(metatable);
		}

		lua_rawgetp(L, LUA_REGISTRYINDEX, _metatables[metatable]);
	}

	void RegisterMetatable(Metatables metatable, void* key) {
		if (_metatables.find(metatable) != _metatables.end()) {
			throw MetatableAlreadyRegisteredException(metatable);
		}

		_metatables[metatable] = key;
	}

	static void InitMetatableIdxFromName() {
		_metatable_idx_from_name["PillEffect"] = Metatables::PILL_EFFECT;
		_metatable_idx_from_name["Spawn"] = Metatables::SPAWN;
		_metatable_idx_from_name["EntityTear"] = Metatables::ENTITY_TEAR;
		_metatable_idx_from_name["HUD"] = Metatables::HUD;
		_metatable_idx_from_name["Seeds"] = Metatables::SEEDS;
		_metatable_idx_from_name["Room"] = Metatables::ROOM;
		_metatable_idx_from_name["ActiveItemDesc"] = Metatables::ACTIVE_ITEM_DESC;
		_metatable_idx_from_name["GridEntityDesc"] = Metatables::GRID_ENTITY_DESC;
		_metatable_idx_from_name["Entity"] = Metatables::ENTITY;
		_metatable_idx_from_name["EntityBomb"] = Metatables::ENTITY_BOMB;
		_metatable_idx_from_name["Config"] = Metatables::CONFIG;
		_metatable_idx_from_name["VectorList"] = Metatables::VECTOR_LIST;
		_metatable_idx_from_name["MusicManager"] = Metatables::MUSIC_MANAGER;
		_metatable_idx_from_name["EffectList"] = Metatables::EFFECT_LIST;
		_metatable_idx_from_name["ItemConfigList"] = Metatables::ITEM_CONFIG_LIST;
		_metatable_idx_from_name["EntityKnife"] = Metatables::ENTITY_KNIFE;
		_metatable_idx_from_name["Level"] = Metatables::LEVEL;
		_metatable_idx_from_name["PillConfigList"] = Metatables::PILL_CONFIG_LIST;
		_metatable_idx_from_name["RoomDescriptor"] = Metatables::ROOM_DESCRIPTOR;
		_metatable_idx_from_name["EntityRef"] = Metatables::ENTITY_REF;
		_metatable_idx_from_name["GridEntityPressurePlate"] = Metatables::GRID_ENTITY_PRESSURE_PLATE;
		_metatable_idx_from_name["TemporaryEffect"] = Metatables::TEMPORARY_EFFECT;
		_metatable_idx_from_name["EntityEffect"] = Metatables::ENTITY_EFFECT;
		_metatable_idx_from_name["RNG"] = Metatables::RNG;
		_metatable_idx_from_name["EntityPlayer"] = Metatables::ENTITY_PLAYER;
		_metatable_idx_from_name["Font"] = Metatables::FONT;
		_metatable_idx_from_name["EntityPickup"] = Metatables::ENTITY_PICKUP;
		_metatable_idx_from_name["Costume"] = Metatables::COSTUME;
		_metatable_idx_from_name["EntityList"] = Metatables::ENTITY_LIST;
		_metatable_idx_from_name["TemporaryEffects"] = Metatables::_TEMPORARY_EFFECTS;
		_metatable_idx_from_name["BitSet128"] = Metatables::BITSET_128;
		_metatable_idx_from_name["ArrayProxy_RoomDescriptor"] = Metatables::ARRAY_PROXY_ROOM_DESCRIPTOR;
		_metatable_idx_from_name["Game"] = Metatables::GAME;
		_metatable_idx_from_name["ProjectileParams"] = Metatables::PROJECTILE_PARAMS;
		_metatable_idx_from_name["GridEntityRock"] = Metatables::GRID_ENTITY_ROCK;
		_metatable_idx_from_name["EntityNPC"] = Metatables::ENTITY_NPC;
		_metatable_idx_from_name["EntityProjectile"] = Metatables::ENTITY_PROJECTILE;
		_metatable_idx_from_name["Color"] = Metatables::COLOR;
		_metatable_idx_from_name["GridEntityTNT"] = Metatables::GRID_ENTITY_TNT;
		_metatable_idx_from_name["EntityFamiliar"] = Metatables::ENTITY_FAMILIAR;
		_metatable_idx_from_name["KColor"] = Metatables::KCOLOR;
		_metatable_idx_from_name["CostumeConfigList"] = Metatables::COSTUME_CONFIG_LIST;
		_metatable_idx_from_name["RoomConfigEntries"] = Metatables::ROOM_CONFIG_ENTRIES;
		_metatable_idx_from_name["QueueItemData"] = Metatables::QUEUE_ITEM_DATA;
		_metatable_idx_from_name["intValues"] = Metatables::INT_VALUES;
		_metatable_idx_from_name["Entry"] = Metatables::ENTRY;
		_metatable_idx_from_name["ItemPool"] = Metatables::ITEM_POOL;
		_metatable_idx_from_name["Item"] = Metatables::ITEM;
		_metatable_idx_from_name["GridEntitySpikes"] = Metatables::GRID_ENTITY_SPIKES;
		_metatable_idx_from_name["GridEntity"] = Metatables::GRID_ENTITY;
		_metatable_idx_from_name["RoomConfigSpawns"] = Metatables::ROOM_CONFIG_SPAWNS;
		_metatable_idx_from_name["RoomConfig_Room"] = Metatables::ROOM_CONFIG_ROOM;
		_metatable_idx_from_name["EntityPtr"] = Metatables::ENTITY_PTR;
		_metatable_idx_from_name["PosVel"] = Metatables::POS_VEL;
		_metatable_idx_from_name["PathFinder"] = Metatables::PATHFINDER;
		_metatable_idx_from_name["TearParams"] = Metatables::TEAR_PARAMS;
		_metatable_idx_from_name["GridEntityPoop"] = Metatables::GRID_ENTITY_POOP;
		_metatable_idx_from_name["Card"] = Metatables::CARD;
		_metatable_idx_from_name["GridEntityPit"] = Metatables::GRID_ENTITY_PIT;
		_metatable_idx_from_name["Sprite"] = Metatables::SPRITE;
		_metatable_idx_from_name["GridEntityDoor"] = Metatables::GRID_ENTITY_DOOR;
		_metatable_idx_from_name["EntityLaser"] = Metatables::ENTITY_LASER;
		_metatable_idx_from_name["SFXManager"] = Metatables::SFX_MANAGER;
		_metatable_idx_from_name["CardConfigList"] = Metatables::CARD_CONFIG_LIST;
		_metatable_idx_from_name["Vector"] = Metatables::VECTOR;

		_metatable_idx_from_name["const PillEffect"] = Metatables::CONST_PILL_EFFECT;
		_metatable_idx_from_name["const Spawn"] = Metatables::CONST_SPAWN;
		_metatable_idx_from_name["const EntityTear"] = Metatables::CONST_ENTITY_TEAR;
		_metatable_idx_from_name["const HUD"] = Metatables::CONST_HUD;
		_metatable_idx_from_name["const Seeds"] = Metatables::CONST_SEEDS;
		_metatable_idx_from_name["const Room"] = Metatables::CONST_ROOM;
		_metatable_idx_from_name["const ActiveItemDesc"] = Metatables::CONST_ACTIVE_ITEM_DESC;
		_metatable_idx_from_name["const GridEntityDesc"] = Metatables::CONST_GRID_ENTITY_DESC;
		_metatable_idx_from_name["const Entity"] = Metatables::CONST_ENTITY;
		_metatable_idx_from_name["const EntityBomb"] = Metatables::CONST_ENTITY_BOMB;
		_metatable_idx_from_name["const Config"] = Metatables::CONST_CONFIG;
		_metatable_idx_from_name["const VectorList"] = Metatables::CONST_VECTOR_LIST;
		_metatable_idx_from_name["const MusicManager"] = Metatables::CONST_MUSIC_MANAGER;
		_metatable_idx_from_name["const EffectList"] = Metatables::CONST_EFFECT_LIST;
		_metatable_idx_from_name["const ItemConfigList"] = Metatables::CONST_ITEM_CONFIG_LIST;
		_metatable_idx_from_name["const EntityKnife"] = Metatables::CONST_ENTITY_KNIFE;
		_metatable_idx_from_name["const Level"] = Metatables::CONST_LEVEL;
		_metatable_idx_from_name["const PillConfigList"] = Metatables::CONST_PILL_CONFIG_LIST;
		_metatable_idx_from_name["const RoomDescriptor"] = Metatables::CONST_ROOM_DESCRIPTOR;
		_metatable_idx_from_name["const EntityRef"] = Metatables::CONST_ENTITY_REF;
		_metatable_idx_from_name["const GridEntityPressurePlate"] = Metatables::CONST_GRID_ENTITY_PRESSURE_PLATE;
		_metatable_idx_from_name["const TemporaryEffect"] = Metatables::CONST_TEMPORARY_EFFECT;
		_metatable_idx_from_name["const EntityEffect"] = Metatables::CONST_ENTITY_EFFECT;
		_metatable_idx_from_name["const RNG"] = Metatables::CONST_RNG;
		_metatable_idx_from_name["const EntityPlayer"] = Metatables::CONST_ENTITY_PLAYER;
		_metatable_idx_from_name["const Font"] = Metatables::CONST_FONT;
		_metatable_idx_from_name["const EntityPickup"] = Metatables::CONST_ENTITY_PICKUP;
		_metatable_idx_from_name["const Costume"] = Metatables::CONST_COSTUME;
		_metatable_idx_from_name["const EntityList"] = Metatables::CONST_ENTITY_LIST;
		_metatable_idx_from_name["const TemporaryEffects"] = Metatables::_CONST_TEMPORARY_EFFECTS;
		_metatable_idx_from_name["const BitSet128"] = Metatables::CONST_BITSET_128;
		_metatable_idx_from_name["const ArrayProxy_RoomDescriptor"] = Metatables::CONST_ARRAY_PROXY_ROOM_DESCRIPTOR;
		_metatable_idx_from_name["const Game"] = Metatables::CONST_GAME;
		_metatable_idx_from_name["const ProjectileParams"] = Metatables::CONST_PROJECTILE_PARAMS;
		_metatable_idx_from_name["const GridEntityRock"] = Metatables::CONST_GRID_ENTITY_ROCK;
		_metatable_idx_from_name["const EntityNPC"] = Metatables::CONST_ENTITY_NPC;
		_metatable_idx_from_name["const EntityProjectile"] = Metatables::CONST_ENTITY_PROJECTILE;
		_metatable_idx_from_name["const Color"] = Metatables::CONST_COLOR;
		_metatable_idx_from_name["const GridEntityTNT"] = Metatables::CONST_GRID_ENTITY_TNT;
		_metatable_idx_from_name["const EntityFamiliar"] = Metatables::CONST_ENTITY_FAMILIAR;
		_metatable_idx_from_name["const KColor"] = Metatables::CONST_KCOLOR;
		_metatable_idx_from_name["const CostumeConfigList"] = Metatables::CONST_COSTUME_CONFIG_LIST;
		_metatable_idx_from_name["const RoomConfigEntries"] = Metatables::CONST_ROOM_CONFIG_ENTRIES;
		_metatable_idx_from_name["const QueueItemData"] = Metatables::CONST_QUEUE_ITEM_DATA;
		_metatable_idx_from_name["const intValues"] = Metatables::CONST_INT_VALUES;
		_metatable_idx_from_name["const Entry"] = Metatables::CONST_ENTRY;
		_metatable_idx_from_name["const ItemPool"] = Metatables::CONST_ITEM_POOL;
		_metatable_idx_from_name["const Item"] = Metatables::CONST_ITEM;
		_metatable_idx_from_name["const GridEntitySpikes"] = Metatables::CONST_GRID_ENTITY_SPIKES;
		_metatable_idx_from_name["const GridEntity"] = Metatables::CONST_GRID_ENTITY;
		_metatable_idx_from_name["const RoomConfigSpawns"] = Metatables::CONST_ROOM_CONFIG_SPAWNS;
		_metatable_idx_from_name["const RoomConfig_Room"] = Metatables::CONST_ROOM_CONFIG_ROOM;
		_metatable_idx_from_name["const EntityPtr"] = Metatables::CONST_ENTITY_PTR;
		_metatable_idx_from_name["const PosVel"] = Metatables::CONST_POS_VEL;
		_metatable_idx_from_name["const PathFinder"] = Metatables::CONST_PATHFINDER;
		_metatable_idx_from_name["const TearParams"] = Metatables::CONST_TEAR_PARAMS;
		_metatable_idx_from_name["const GridEntityPoop"] = Metatables::CONST_GRID_ENTITY_POOP;
		_metatable_idx_from_name["const Card"] = Metatables::CONST_CARD;
		_metatable_idx_from_name["const GridEntityPit"] = Metatables::CONST_GRID_ENTITY_PIT;
		_metatable_idx_from_name["const Sprite"] = Metatables::CONST_SPRITE;
		_metatable_idx_from_name["const GridEntityDoor"] = Metatables::CONST_GRID_ENTITY_DOOR;
		_metatable_idx_from_name["const EntityLaser"] = Metatables::CONST_ENTITY_LASER;
		_metatable_idx_from_name["const SFXManager"] = Metatables::CONST_SFX_MANAGER;
		_metatable_idx_from_name["const CardConfigList"] = Metatables::CONST_CARD_CONFIG_LIST;
		_metatable_idx_from_name["const Vector"] = Metatables::CONST_VECTOR;
	}

	Metatables GetMetatableIdxFromName(std::string const& name) {
		if (!_metatable_idx_from_name_initialized) {
			InitMetatableIdxFromName();
			_metatable_idx_from_name_initialized = true;
		}

		auto iter = _metatable_idx_from_name.find(name);
		if (iter == _metatable_idx_from_name.end()) {
			throw UnknownMetatableNameException(name);
		}

		return _metatable_idx_from_name[name];
	}
}