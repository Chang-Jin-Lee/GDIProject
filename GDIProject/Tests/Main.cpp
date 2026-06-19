#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "../Games/Core/AutoMove.h"
#include "../Games/Core/GameCommand.h"
#include "../Games/Core/GameConfig.h"
#include "../Games/Core/NetworkProtocol.h"
#include "../Games/Core/StateSerializer.h"

namespace
{
int g_failed = 0;

void Require(bool condition, const char* message)
{
	if (!condition)
	{
		++g_failed;
		std::cerr << "[FAIL] " << message << '\n';
	}
}

void TestDefaultsContainScout()
{
	const GameConfig config = GameConfig::Defaults();
	const UnitStats warrior = config.GetUnit(EUnitType::Warrior);
	const UnitStats archer = config.GetUnit(EUnitType::Archer);
	const UnitStats scout = config.GetUnit(EUnitType::Scout);

	Require(warrior.AttackDamage == 20, "Warrior damage uses default config");
	Require(archer.AttackRange == 2, "Archer attack range uses default config");
	Require(scout.DisplayName == L"\uc815\ucc30\ubcd1", "Scout display name uses default config");
	Require(scout.ActionMaxCount == 5, "Scout action budget uses default config");
	Require(config.GetTile(ETileType::Mountain).MoveCost == 4, "Mountain move cost uses default config");
	Require(config.GameRules.PvpTurnSeconds == 30, "PvP timer default is 30 seconds");
	Require(config.NetworkRules.Port == 43117, "LAN default port is 43117");
}

void TestCommandRoundTrip()
{
	const GameCommand command = GameCommand::Move(3, EPlayerSlot::Player2, "P2_Archer_0", 4, 8);
	const std::string line = SerializeCommand(command);
	Require(line == "COMMAND|turn=3|slot=Player2|type=Move|unit=P2_Archer_0|row=4|col=8", "Move command serializes to the protocol");

	const CommandParseResult parsed = ParseCommand(line);
	Require(parsed.Success, "Move command parses successfully");
	Require(parsed.Command.Turn == 3, "Parsed command keeps turn");
	Require(parsed.Command.Slot == EPlayerSlot::Player2, "Parsed command keeps slot");
	Require(parsed.Command.Type == EGameCommandType::Move, "Parsed command keeps type");
	Require(parsed.Command.UnitId == "P2_Archer_0", "Parsed command keeps unit id");
	Require(parsed.Command.Row == 4 && parsed.Command.Col == 8, "Parsed command keeps destination");

	const CommandParseResult invalid = ParseCommand("COMMAND|turn=3|slot=Player2|type=Move|unit=P2_Archer_0|row=x|col=8");
	Require(!invalid.Success, "Invalid integer command is rejected");

	const GameCommand attack = GameCommand::Attack(4, EPlayerSlot::Player1, "P1_Warrior_0", "P2_Scout_0");
	const CommandParseResult parsedAttack = ParseCommand(SerializeCommand(attack));
	Require(parsedAttack.Success, "Attack command parses successfully");
	Require(parsedAttack.Command.Type == EGameCommandType::Attack, "Parsed attack keeps type");
	Require(parsedAttack.Command.TargetUnitId == "P2_Scout_0", "Parsed attack keeps target id");

	const GameCommand skip = GameCommand::Skip(5, EPlayerSlot::Player2, "P2_Scout_0");
	const CommandParseResult parsedSkip = ParseCommand(SerializeCommand(skip));
	Require(parsedSkip.Success, "Skip command parses successfully");
	Require(parsedSkip.Command.Type == EGameCommandType::Skip, "Parsed skip keeps type");
	Require(parsedSkip.Command.UnitId == "P2_Scout_0", "Parsed skip keeps unit id");
}

void TestAutoMoveChoosesWorstReachableTile()
{
	AutoMoveRequest request;
	request.UnitRow = 2;
	request.UnitCol = 2;
	request.RemainingAction = 4;
	request.NearestEnemyRow = 2;
	request.NearestEnemyCol = 3;
	request.TargetRow = 0;
	request.TargetCol = 0;
	request.Tiles = {
		{ TerrainTile{0, 0, ETileType::Plain, true, false, false}, TerrainTile{0, 1, ETileType::Plain, true, false, false}, TerrainTile{0, 2, ETileType::Plain, true, false, false}, TerrainTile{0, 3, ETileType::Plain, true, false, false} },
		{ TerrainTile{1, 0, ETileType::Plain, true, false, false}, TerrainTile{1, 1, ETileType::Hills, true, false, false}, TerrainTile{1, 2, ETileType::Plain, true, false, false}, TerrainTile{1, 3, ETileType::Plain, true, false, false} },
		{ TerrainTile{2, 0, ETileType::Mountain, true, false, false}, TerrainTile{2, 1, ETileType::Plain, true, false, false}, TerrainTile{2, 2, ETileType::Plain, true, false, false}, TerrainTile{2, 3, ETileType::Plain, true, true, false} },
		{ TerrainTile{3, 0, ETileType::Plain, true, false, false}, TerrainTile{3, 1, ETileType::Plain, true, false, false}, TerrainTile{3, 2, ETileType::Plain, true, false, false}, TerrainTile{3, 3, ETileType::Plain, true, false, false} },
	};

	const AutoMoveChoice choice = ChooseUnfavorableAutoMove(request, GameConfig::Defaults());
	Require(choice.HasMove, "Auto-move finds a legal move");
	Require(choice.Row == 3 && choice.Col == 0, "Auto-move chooses the highest unfavorable score");
}

void TestStateSerializationOrdersRecords()
{
	GameStateSnapshot snapshot;
	snapshot.Turn = 4;
	snapshot.ActiveSlot = EPlayerSlot::Player1;
	snapshot.TimerSeconds = 30;
	snapshot.Tiles.push_back(TerrainTile{1, 0, ETileType::Hills, true, false, false});
	snapshot.Tiles.push_back(TerrainTile{0, 1, ETileType::Plain, true, false, false});
	snapshot.Tiles.push_back(TerrainTile{0, 0, ETileType::Capital, true, false, true});
	snapshot.Units.push_back(UnitState{"P2_Archer_0", EUnitType::Archer, EPlayerSlot::Player2, 9, 13, 70, 3, true});
	snapshot.Units.push_back(UnitState{"P1_Warrior_0", EUnitType::Warrior, EPlayerSlot::Player1, 0, 1, 100, 3, true});

	const std::string line = SerializeState(snapshot);
	const std::string expected = "STATE|turn=4|active=Player1|timer=30|payload=T:0,0,Capital,1;T:0,1,Plain,1;T:1,0,Hills,1;U:P1_Warrior_0,Warrior,Player1,0,1,100,3,1;U:P2_Archer_0,Archer,Player2,9,13,70,3,1";
	Require(line == expected, "State serialization is deterministic");

	const StateParseResult parsed = ParseState(line);
	Require(parsed.Success, "State parses successfully");
	Require(parsed.Snapshot.Units.size() == 2, "Parsed state contains units");
	Require(parsed.Snapshot.Tiles.size() == 3, "Parsed state contains tiles");
}

void TestNetworkProtocolParsing()
{
	const NetworkLine hello = ParseNetworkLine("HELLO|version=1|role=client");
	Require(hello.Name == "HELLO", "HELLO line name parsed");
	Require(hello.Fields.at("role") == "client", "HELLO role parsed");

	const NetworkLine welcome = ParseNetworkLine("WELCOME|slot=Player2|seed=123456|mapHash=8F6A21C0");
	Require(welcome.Name == "WELCOME", "WELCOME line name parsed");
	Require(welcome.Fields.at("mapHash") == "8F6A21C0", "WELCOME map hash parsed");

	const NetworkLine bad = ParseNetworkLine("COMMAND|turn=3|broken");
	Require(!bad.Valid, "Malformed key-value field is rejected");
}
}

int main()
{
	TestDefaultsContainScout();
	TestCommandRoundTrip();
	TestAutoMoveChoosesWorstReachableTile();
	TestStateSerializationOrdersRecords();
	TestNetworkProtocolParsing();

	if (g_failed == 0)
	{
		std::cout << "All core tests passed.\n";
		return EXIT_SUCCESS;
	}

	std::cerr << g_failed << " core tests failed.\n";
	return EXIT_FAILURE;
}
