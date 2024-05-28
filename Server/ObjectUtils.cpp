#include "pch.h"
#include "ObjectUtils.h"
#include "Player.h"
#include "GameSession.h"

atomic<int64> ObjectUtils::s_idGenerator = 1;

shared_ptr<class Player> ObjectUtils::CreatePlayer(shared_ptr<GameSession> session)
{
	const int64 newId = s_idGenerator.fetch_add(1);

	shared_ptr<Player> player = make_shared<Player>();
	{
		player->objectInfo->set_object_id(newId);
		player->session = session;

		session->curPlayer.store(player);
	}

	return player;
}
