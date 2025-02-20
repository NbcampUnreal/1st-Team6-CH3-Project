#include "GameManager/GJGameInstance.h"

UGJGameInstance::UGJGameInstance()
{
	EnemyTotalKillCount = 0;
	TotalScore = 0;
	OneWaveScore = 0;
	TwoWaveScore = 0;
	ThreeWaveScore = 0;
	BossWaveScore = 0;
}

void UGJGameInstance::AddToScore(int amount)
{
	TotalScore += amount;
}

void UGJGameInstance::SaveOneWaveScore(int amount)
{
	OneWaveScore += amount;
}

void UGJGameInstance::SaveTwoWaveScore(int amount)
{
	TwoWaveScore += amount;
}

void UGJGameInstance::SaveThreeWaveScore(int amount)
{
	ThreeWaveScore += amount;
}

void UGJGameInstance::BossScore(int amount)
{
	BossWaveScore += amount;
}

void UGJGameInstance::AddToEnemyKill(int amount)
{
	EnemyTotalKillCount += amount;
}

int32 UGJGameInstance::GetTotalScore()
{
	return TotalScore;
}

int32 UGJGameInstance::GetOneWaveScore()
{
	return OneWaveScore;
}

int32 UGJGameInstance::GetTwoWaveScore()
{
	return TwoWaveScore;
}

int32 UGJGameInstance::GetThreeWaveScore()
{
	return ThreeWaveScore;
}

int32 UGJGameInstance::GetBossScore()
{
	return BossWaveScore;
}

int32 UGJGameInstance::GetEnemyTotalKillCount()
{
	return EnemyTotalKillCount;
}


