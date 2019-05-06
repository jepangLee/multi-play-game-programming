#ifndef LINKING_CONTEXT_H
#define LINKING_CONTEXT_H
#include "../pch.h"
#include "../Object/GameObject.h"

class LinkingContext {
public:
	uint32_t GetNetworkId(GameObject* inGameObject) { 
		auto it = mGameObjectToNetworkIdMap.find(inGameObject);
		if (it != mGameObjectToNetworkIdMap.end())
			return it->second;
		else
			return 0;
	}
	GameObject* GetGameObject(uint32_t inNetworkId) { 
		auto it = mNetworkIdToGameObjectMap.find(inNetworkId);
		if (it != mNetworkIdToGameObjectMap.end())
			return it->second;
		else
			return 0;
	}
private:
	std::unordered_map<uint32_t, GameObject*>mNetworkIdToGameObjectMap;
	std::unordered_map<GameObject*, uint32_t>mGameObjectToNetworkIdMap;
};

#endif