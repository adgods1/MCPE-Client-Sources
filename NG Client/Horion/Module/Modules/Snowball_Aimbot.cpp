#include "Snowball_Aimbot.h"

#include "../../../Utils/Target.h"
#include "../../DrawUtils.h"

static std::vector<C_Entity*> targetList;

SnowballAimbot::SnowballAimbot() : IModule(0x0, Category::COMBAT, "Aimbot, but for snowballs") {
	registerBoolSetting("silent", &silent, silent);
	registerBoolSetting("predict", &predict, predict);
	registerBoolSetting("visualize", &visualize, visualize);
}

SnowballAimbot::~SnowballAimbot() {
}

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

const char* SnowballAimbot::getModuleName() {
	return ("SnowballAimbot");
}

static void findTargets(C_Entity* currentEntity, bool isRegularEntitie) {
	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 130) {
		targetList.push_back(currentEntity);
	}
}

void SnowballAimbot::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	targetList.clear();

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	if (localPlayer->getSelectedItemId() != 374)  // Snowball in hand?
		return;

	if (!(GameData::isRightClickDown() && GameData::canUseMoveKeys()))  // is aiming?
		return;

	g_Data.forEachEntity(findTargets);

	if (!targetList.empty()) {
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		vec3 origin = g_Data.getLocalPlayer()->eyePos0;  // TODO: sort list
		C_Entity* entity = targetList[0];
		vec3 pos = entity->aabb.centerPoint();
		if (predict) {
			vec3 velocity = entity->getPos()->sub(*entity->getPosOld());
			velocity.x *= origin.dist(pos) / 2.f;
			velocity.z *= origin.dist(pos) / 2.f;
			pos = pos.add(velocity);
		}
		if (visualize)
			DrawUtils::drawBox(pos.sub(0.5), pos.add(0.5), 0.3f, true);
		pos = pos.sub(origin);
		float yaw = (atan2f(pos.z, pos.x) * DEG_RAD) - 90;
		float len = pos.magnitudexz();
		constexpr float g = 0.002f;  // nukkit = 0.012, some servers need different values
		float tmp = 1 - g * (g * (len * len) + 2 * pos.y);
		float pitch = DEG_RAD * -atanf((1 - sqrtf(tmp)) / (g * len));

		if (silent) {
			angle = vec2(pitch, yaw);
			C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			p.pitch = angle.x;
			p.yaw = angle.y;
			p.headYaw = angle.y;
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
		} else {
			if (pitch < 120 && pitch > -120) {
				vec2 angles = vec2(pitch, yaw);

				vec2 appl = angles.sub(localPlayer->viewAngles).normAngles();
				appl.x = -appl.x;
				appl = appl.div(7);  // Smooth dat boi

				localPlayer->applyTurnDelta(&appl);
			}
		}
	}
}

void SnowballAimbot::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>() && silent) {
		if (!targetList.empty()) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
	}
}