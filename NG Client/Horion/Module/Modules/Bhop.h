#pragma once
#include "..\ModuleManager.h"
#include "Module.h"

class Bhop : public IModule {
public:
	float speed = 0.50f;
	bool hive = false;
	int timer = 20;
	int speedIndexThingyForHive = 125;
	double epicHiveSpeedArrayThingy[126] = {
		0.600095,
		0.569273,
		0.541226,
		0.515702,
		0.492476,
		0.471340,
		0.452106,
		0.434603,
		0.418676,
		0.404182,
		0.390992,
		0.378990,
		0.368067,
		0.358128,
		0.349083,
		0.340853,
		0.333363,
		0.326547,
		0.320345,
		0.314700,
		0.309564,
		0.304890,
		0.300637,
		0.296766,
		0.293244,
		0.290039,
		0.287122,
		0.284468,
		0.282053,
		0.279855,
		0.277855,
		0.276035,
		0.274378,
		0.272871,
		0.271499,
		0.270251,
		0.269116,
		0.268082,
		0.267141,
		0.266285,
		0.265507,
		0.264798,
		0.264153,
		0.263566,
		0.263032,
		0.262546,
		0.262103,
		0.261701,
		0.261335,
		0.261001,
		0.260698,
		0.260422,
		0.260171,
		0.259942,
		0.259734,
		0.259545,
		0.259373,
		0.259216,
		0.259073,
		0.258944,
		0.258826,
		0.258718,
		0.258620,
		0.258531,
		0.258450,
		0.258376,
		0.258309,
		0.258248,
		0.258193,
		0.258142,
		0.258096,
		0.258054,
		0.258016,
		0.257982,
		0.257950,
		0.257921,
		0.257895,
		0.257872,
		0.257850,
		0.257830,
		0.257812,
		0.257796,
		0.257781,
		0.257768,
		0.257755,
		0.257744,
		0.257734,
		0.257725,
		0.257716,
		0.257709,
		0.257702,
		0.257695,
		0.257690,
		0.257684,
		0.257680,
		0.257675,
		0.257671,
		0.257668,
		0.257664,
		0.257661,
		0.257659,
		0.257656,
		0.257654,
		0.257652,
		0.257650,
		0.257648,
		0.257647,
		0.257645,
		0.257644,
		0.257643,
		0.257642,
		0.257641,
		0.257640,
		0.257639,
		0.257639,
		0.257638,
		0.257637,
		0.257637,
		0.257636,
		0.257636,
		0.257635,
		0.257635,
		0.257635,
		0.257634,
		0.257634,
		0.257634};
	C_MoveInputHandler* yes;
	C_MoveInputHandler cachedInput;
	Bhop() : IModule(0x0, Category::MOVEMENT, "Hop around like a bunny!") {
		registerBoolSetting("Hive", &hive, hive);
		registerFloatSetting("Speed", &speed, speed, 0.10f, 1.50f);
		registerIntSetting("Timer", &timer, timer, 20, 50);
	}
	~Bhop(){};
	std::vector<C_MovePlayerPacket*> MovePlayerPacketHolder;
	inline std::vector<C_MovePlayerPacket*>* getMovePlayerPacketHolder() { return &MovePlayerPacketHolder; };
	// Inherited via IModule
	virtual const char* getModuleName() override { return ("Bhop"); }
	virtual void onMove(C_MoveInputHandler* input) override;
	virtual void onTick(C_GameMode* gm) override {
		g_Data.getClientInstance()->minecraft->setTimerSpeed(static_cast<float>(timer));
	}
	//virtual void onSendPacket(C_Packet* packet) override;
	virtual void onDisable() override { g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f); }
	virtual void onEnable() override { speedIndexThingyForHive = 125; }
};